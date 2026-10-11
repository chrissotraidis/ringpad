// SonicPad iOS shell: a full-screen Metal layer driven by a display link,
// SonicPad's single-stick touch overlay, a three-dot menu kept separate from
// Start, and verified import of the owner's disc. Flycast's Vulkan renderer
// draws into the layer through MoltenVK; the recompiled game runs on Flycast's
// emulation thread.
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <AVFoundation/AVFoundation.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#import "SPGameLibrary.h"
#import "SPTouchOverlayView.h"

#include "types.h"
#include "emulator.h"
#include "stdclass.h"
#include "cfg/option.h"
#include "input/gamepad.h"
#include "input/gamepad_device.h"
#include "log/LogManager.h"
#include "oslib/oslib.h"
#include "oslib/i18n.h"
#include "ui/gui.h"
#include "ui/mainui.h"
#include "ui/imgui_driver.h"
#include "ui/vgamepad.h"
#include "wsi/context.h"

#include "DreamcastInputMixer.h"
#include "FlycastPadBridge.h"

#include <cstdlib>
#include <string>

#include <dlfcn.h>
#include <signal.h>
#include <sys/ucontext.h>

static_assert(sonicpad::kFlycastA == DC_BTN_A && sonicpad::kFlycastB == DC_BTN_B, "Flycast face bits");
static_assert(sonicpad::kFlycastX == DC_BTN_X && sonicpad::kFlycastY == DC_BTN_Y, "Flycast face bits");
static_assert(sonicpad::kFlycastStart == DC_BTN_START, "Flycast Start bit");
static_assert(sonicpad::kFlycastUp == DC_DPAD_UP && sonicpad::kFlycastDown == DC_DPAD_DOWN, "Flycast D-pad bits");
static_assert(sonicpad::kFlycastLeft == DC_DPAD_LEFT && sonicpad::kFlycastRight == DC_DPAD_RIGHT, "Flycast D-pad bits");

// Flycast's fault handler chains here for faults it does not own. Log where
// the fault happened (symbol and image offset) before the process traps, so
// crash reports name the faulting function.
static void reportFault(int sig, siginfo_t *info, void *context)
{
	const ucontext_t *uc = static_cast<const ucontext_t *>(context);
	const uintptr_t pc = uc->uc_mcontext->__ss.__pc;
	const uintptr_t lr = uc->uc_mcontext->__ss.__lr;
	Dl_info where{}, caller{};
	dladdr(reinterpret_cast<void *>(pc), &where);
	dladdr(reinterpret_cast<void *>(lr), &caller);
	NSLog(@"SONICPAD_FAULT signal=%d addr=%p pc=%s+0x%lx lr=%s+0x%lx", sig, info->si_addr,
			where.dli_sname ?: "?", (unsigned long)(pc - (uintptr_t)(where.dli_saddr ?: where.dli_fbase)),
			caller.dli_sname ?: "?", (unsigned long)(lr - (uintptr_t)(caller.dli_saddr ?: caller.dli_fbase)));
	signal(sig, SIG_DFL);
	raise(sig);
}

static void installFaultReporter()
{
	struct sigaction act {};
	act.sa_sigaction = reportFault;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, nullptr);
	sigaction(SIGBUS, &act, nullptr);
}

static void hideFlycastGamepad(Event, void *)
{
	// SonicPad's own overlay replaces Flycast's virtual gamepad.
	vgamepad::hide();
}

@interface SPMetalView : UIView
@end

@implementation SPMetalView
+ (Class)layerClass
{
	return [CAMetalLayer class];
}
@end

@interface SPGameViewController : UIViewController <UIDocumentPickerDelegate>
@property (nonatomic, strong) CADisplayLink *displayLink;
@property (nonatomic, strong) SPTouchOverlayView *overlay;
@property (nonatomic, strong) UIButton *menuButton;
@property (nonatomic, strong) UIView *importView;
@property (nonatomic, strong) UILabel *importStatus;
@property (nonatomic, strong) UIProgressView *importProgress;
@property (nonatomic, strong) UIButton *importChoose;
@property (nonatomic, strong) UIButton *importCancel;
@property (nonatomic, strong) UILabel *inputDebug;
@property (nonatomic) BOOL renderStarted;
@property (nonatomic) BOOL resumeAfterMenu;
@property (nonatomic) BOOL scriptedInput;
@end

@implementation SPGameViewController
{
	sonicpad::FlycastPressHold _hold;
	// Input seen since launch, for the UI tests' debug label.
	uint32_t _seenPressed;
	int _minX, _maxX, _minY, _maxY;
	unsigned _maxLT, _maxRT;
}

- (void)loadView
{
	SPMetalView *view = [[SPMetalView alloc] initWithFrame:UIScreen.mainScreen.bounds];
	view.backgroundColor = UIColor.blackColor;
	self.view = view;

	self.overlay = [[SPTouchOverlayView alloc] initWithFrame:view.bounds];
	self.overlay.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	[view addSubview:self.overlay];

	UIButtonConfiguration *menuConfig = [UIButtonConfiguration plainButtonConfiguration];
	menuConfig.image = [UIImage systemImageNamed:@"ellipsis.circle.fill"
			withConfiguration:[UIImageSymbolConfiguration configurationWithPointSize:30]];
	menuConfig.baseForegroundColor = [UIColor colorWithWhite:1 alpha:0.85];
	self.menuButton = [UIButton buttonWithConfiguration:menuConfig primaryAction:nil];
	self.menuButton.accessibilityIdentifier = @"sp.menu";
	self.menuButton.accessibilityLabel = @"Menu";
	[self.menuButton addTarget:self action:@selector(showMenu) forControlEvents:UIControlEventTouchUpInside];
	self.menuButton.translatesAutoresizingMaskIntoConstraints = NO;
	[view addSubview:self.menuButton];
	[NSLayoutConstraint activateConstraints:@[
		[self.menuButton.topAnchor constraintEqualToAnchor:view.safeAreaLayoutGuide.topAnchor constant:8],
		[self.menuButton.trailingAnchor constraintEqualToAnchor:view.safeAreaLayoutGuide.trailingAnchor constant:-8],
		[self.menuButton.widthAnchor constraintEqualToConstant:48],
		[self.menuButton.heightAnchor constraintEqualToConstant:48],
	]];

	if (std::getenv("SONICPAD_INPUT_DEBUG") != nullptr) {
		self.inputDebug = [[UILabel alloc] init];
		self.inputDebug.accessibilityIdentifier = @"sp.inputDebug";
		self.inputDebug.font = [UIFont monospacedSystemFontOfSize:12 weight:UIFontWeightRegular];
		self.inputDebug.textColor = UIColor.yellowColor;
		self.inputDebug.translatesAutoresizingMaskIntoConstraints = NO;
		[view addSubview:self.inputDebug];
		[NSLayoutConstraint activateConstraints:@[
			[self.inputDebug.bottomAnchor constraintEqualToAnchor:view.safeAreaLayoutGuide.bottomAnchor constant:-4],
			[self.inputDebug.centerXAnchor constraintEqualToAnchor:view.centerXAnchor],
		]];
	}
	[self buildImportView];
}

- (void)buildImportView
{
	UIView *panel = [[UIView alloc] initWithFrame:self.view.bounds];
	panel.backgroundColor = [UIColor colorWithRed:0.02 green:0.08 blue:0.28 alpha:1];
	panel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	panel.hidden = YES;

	UILabel *title = [[UILabel alloc] init];
	title.text = @"SonicPad";
	title.font = [UIFont systemFontOfSize:40 weight:UIFontWeightHeavy];
	title.textColor = UIColor.whiteColor;

	UILabel *body = [[UILabel alloc] init];
	body.text = @"Choose the folder that holds your Sonic Adventure PAL v1.003 disc: one .gdi file and its three tracks. SonicPad checks every track before installing it.";
	body.numberOfLines = 0;
	body.textAlignment = NSTextAlignmentCenter;
	body.textColor = [UIColor colorWithWhite:1 alpha:0.85];
	body.font = [UIFont preferredFontForTextStyle:UIFontTextStyleBody];

	UIButtonConfiguration *chooseConfig = [UIButtonConfiguration filledButtonConfiguration];
	chooseConfig.title = @"Choose Folder";
	chooseConfig.cornerStyle = UIButtonConfigurationCornerStyleCapsule;
	self.importChoose = [UIButton buttonWithConfiguration:chooseConfig primaryAction:nil];
	self.importChoose.accessibilityIdentifier = @"sp.import.choose";
	[self.importChoose addTarget:self action:@selector(chooseFolder) forControlEvents:UIControlEventTouchUpInside];

	UIButtonConfiguration *cancelConfig = [UIButtonConfiguration plainButtonConfiguration];
	cancelConfig.title = @"Cancel";
	cancelConfig.baseForegroundColor = UIColor.whiteColor;
	self.importCancel = [UIButton buttonWithConfiguration:cancelConfig primaryAction:nil];
	self.importCancel.accessibilityIdentifier = @"sp.import.cancel";
	[self.importCancel addTarget:self action:@selector(cancelImport) forControlEvents:UIControlEventTouchUpInside];

	self.importProgress = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
	self.importProgress.hidden = YES;

	self.importStatus = [[UILabel alloc] init];
	self.importStatus.accessibilityIdentifier = @"sp.import.status";
	self.importStatus.numberOfLines = 0;
	self.importStatus.textAlignment = NSTextAlignmentCenter;
	self.importStatus.textColor = [UIColor colorWithRed:1 green:0.85 blue:0.4 alpha:1];

	UIStackView *stack = [[UIStackView alloc] initWithArrangedSubviews:@[title, body, self.importChoose,
			self.importProgress, self.importStatus, self.importCancel]];
	stack.axis = UILayoutConstraintAxisVertical;
	stack.alignment = UIStackViewAlignmentCenter;
	stack.spacing = 18;
	stack.translatesAutoresizingMaskIntoConstraints = NO;
	[panel addSubview:stack];
	[NSLayoutConstraint activateConstraints:@[
		[stack.centerXAnchor constraintEqualToAnchor:panel.centerXAnchor],
		[stack.centerYAnchor constraintEqualToAnchor:panel.centerYAnchor],
		[stack.widthAnchor constraintLessThanOrEqualToConstant:560],
		[stack.widthAnchor constraintLessThanOrEqualToAnchor:panel.widthAnchor multiplier:0.85],
		[self.importProgress.widthAnchor constraintEqualToAnchor:stack.widthAnchor],
		[body.widthAnchor constraintEqualToAnchor:stack.widthAnchor],
		[self.importStatus.widthAnchor constraintEqualToAnchor:stack.widthAnchor],
	]];
	[self.view addSubview:panel];
	self.importView = panel;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	LogManager::Init();
	i18n::init();

	NSString *documents = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
	std::string home = std::string(documents.fileSystemRepresentation) + "/";
	set_user_config_dir(home);
	set_user_data_dir(home);

	installFaultReporter();
	os_InstallFaultHandler();
	flycast_init(0, nullptr);
	config::RendererType = RenderType::Vulkan;
	vgamepad::hide();
	EventManager::listen(Event::Resume, hideFlycastGamepad);
	self.scriptedInput = std::getenv("SONICPAD_INPUT_SCRIPT") != nullptr;

	AVAudioSession *session = AVAudioSession.sharedInstance;
	[session setCategory:AVAudioSessionCategoryPlayback error:nil];
	[session setActive:YES error:nil];

	if (const char *game = std::getenv("SONICPAD_GAME")) {
		settings.content.path = game;
	} else if (const char *folder = std::getenv("SONICPAD_IMPORT")) {
		[self showImport:NO];
		[self importFromURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:folder] isDirectory:YES]];
	} else if (NSString *installed = std::getenv("SONICPAD_IGNORE_INSTALLED") ? nil : [SPGameLibrary installedGamePath]) {
		settings.content.path = installed.fileSystemRepresentation;
	} else {
		[self showImport:NO];
	}
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
	return UIInterfaceOrientationMaskLandscape;
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	if (self.renderStarted)
		return;
	[self updateDrawableSize];
	CAMetalLayer *layer = (CAMetalLayer *)self.view.layer;
	settings.display.dpi = 160.f * layer.contentsScale;
	initRenderApi((__bridge void *)layer);
	mainui_init();
	self.renderStarted = YES;

	self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
	self.displayLink.preferredFrameRateRange = CAFrameRateRangeMake(60, 60, 60);
	[self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];
}

- (BOOL)updateDrawableSize
{
	CAMetalLayer *layer = (CAMetalLayer *)self.view.layer;
	const CGFloat scale = self.view.window.screen.nativeScale ?: UIScreen.mainScreen.nativeScale;
	const CGSize size = CGSizeMake(self.view.bounds.size.width * scale, self.view.bounds.size.height * scale);
	layer.contentsScale = scale;
	if (CGSizeEqualToSize(layer.drawableSize, size) && settings.display.width == (int)size.width)
		return NO;
	layer.drawableSize = size;
	settings.display.width = (int)size.width;
	settings.display.height = (int)size.height;
	return YES;
}

- (void)viewDidLayoutSubviews
{
	[super viewDidLayoutSubviews];
	if ([self updateDrawableSize] && self.renderStarted && GraphicsContext::Instance() != nullptr)
		GraphicsContext::Instance()->resize();
}

- (void)renderFrame
{
	[self updateInput];
	if (mainui_rend_frame() && imguiDriver != nullptr)
		imguiDriver->present();
}

- (void)updateInput
{
	if (self.scriptedInput)
		return;   // the executor's input script owns port A
	const SonicPadDreamcastState merged = sonicpad::DreamcastInputMixer::shared().consumeMerged();
	const sonicpad::FlycastPadState pad = sonicpad::to_flycast_pad(sonicpad::adapt(merged));
	const uint32_t pressed = _hold.apply(pad.pressed);
	kcode[0] = sonicpad::flycast_kcode(pressed);
	joyx[0] = pad.joyx;
	joyy[0] = pad.joyy;
	lt[0] = pad.lt;
	rt[0] = pad.rt;
	if (self.inputDebug != nil) {
		_seenPressed |= pressed;
		_minX = MIN(_minX, (int)pad.joyx);
		_maxX = MAX(_maxX, (int)pad.joyx);
		_minY = MIN(_minY, (int)pad.joyy);
		_maxY = MAX(_maxY, (int)pad.joyy);
		_maxLT = MAX(_maxLT, (unsigned)pad.lt);
		_maxRT = MAX(_maxRT, (unsigned)pad.rt);
		self.inputDebug.text = [NSString stringWithFormat:@"kcode=%08X seen=%08X x=%d..%d y=%d..%d lt=%u rt=%u",
				kcode[0], _seenPressed, _minX, _maxX, _minY, _maxY, _maxLT, _maxRT];
	}
}

- (void)clearInput
{
	[self.overlay clearInput];
	sonicpad::DreamcastInputMixer::shared().clearAll();
	_hold.clear();
}

- (void)pauseGame
{
	[self clearInput];
	self.resumeAfterMenu = emu.running();
	if (self.resumeAfterMenu)
		emu.stop();
}

- (void)resumeGame
{
	[self clearInput];
	if (self.resumeAfterMenu && !emu.running())
		emu.start();
	self.resumeAfterMenu = NO;
}

- (void)showMenu
{
	[self pauseGame];
	UIAlertController *menu = [UIAlertController alertControllerWithTitle:@"SonicPad" message:nil
			preferredStyle:UIAlertControllerStyleActionSheet];
	[menu addAction:[UIAlertAction actionWithTitle:@"Resume" style:UIAlertActionStyleCancel
			handler:^(UIAlertAction *) { [self resumeGame]; }]];
	[menu addAction:[UIAlertAction actionWithTitle:@"Import Game…" style:UIAlertActionStyleDefault
			handler:^(UIAlertAction *) { [self showImport:YES]; }]];
	menu.popoverPresentationController.sourceView = self.menuButton;
	menu.popoverPresentationController.sourceRect = self.menuButton.bounds;
	[self presentViewController:menu animated:YES completion:nil];
}

- (void)showImport:(BOOL)cancellable
{
	[self clearInput];
	self.importStatus.text = nil;
	self.importProgress.hidden = YES;
	self.importChoose.enabled = YES;
	self.importCancel.hidden = !cancellable;
	self.importView.hidden = NO;
	self.overlay.hidden = YES;
	self.menuButton.hidden = YES;
}

- (void)hideImport
{
	self.importView.hidden = YES;
	self.overlay.hidden = NO;
	self.menuButton.hidden = NO;
}

- (void)cancelImport
{
	[self hideImport];
	[self resumeGame];
}

- (void)chooseFolder
{
	UIDocumentPickerViewController *picker = [[UIDocumentPickerViewController alloc]
			initForOpeningContentTypes:@[UTTypeFolder]];
	picker.delegate = self;
	picker.allowsMultipleSelection = NO;
	[self presentViewController:picker animated:YES completion:nil];
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls
{
	if (urls.firstObject != nil)
		[self importFromURL:urls.firstObject];
}

- (void)importFromURL:(NSURL *)folder
{
	self.importChoose.enabled = NO;
	self.importCancel.enabled = NO;
	self.importProgress.hidden = NO;
	self.importProgress.progress = 0;
	self.importStatus.text = @"Checking the disc…";
	NSLog(@"SONICPAD_IMPORT start %@", folder.path);
	[SPGameLibrary importFolder:folder
			progress:^(double fraction) {
				self.importProgress.progress = fraction;
			}
			completion:^(NSString *gdiPath, NSString *error) {
				self.importChoose.enabled = YES;
				self.importCancel.enabled = YES;
				self.importProgress.hidden = YES;
				if (gdiPath == nil) {
					NSLog(@"SONICPAD_IMPORT failed %@", error);
					self.importStatus.text = error;
					return;
				}
				NSLog(@"SONICPAD_IMPORT ok %@", gdiPath);
				[self hideImport];
				self.resumeAfterMenu = NO;
				[self startGame:gdiPath];
			}];
}

- (void)startGame:(NSString *)gdiPath
{
	settings.content.path = gdiPath.fileSystemRepresentation;
	if (self.renderStarted)
		gui_start_game(settings.content.path);
}

- (BOOL)prefersStatusBarHidden
{
	return YES;
}

- (BOOL)prefersHomeIndicatorAutoHidden
{
	return YES;
}

@end

@interface SPSceneDelegate : UIResponder <UIWindowSceneDelegate>
@property (nonatomic, strong) UIWindow *window;
@property (nonatomic) BOOL resumeOnForeground;
@end

@implementation SPSceneDelegate

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)options
{
	UIWindowScene *windowScene = (UIWindowScene *)scene;
	self.window = [[UIWindow alloc] initWithWindowScene:windowScene];
	self.window.rootViewController = [[SPGameViewController alloc] init];
	[self.window makeKeyAndVisible];
	UIWindowSceneGeometryPreferencesIOS *landscape = [[UIWindowSceneGeometryPreferencesIOS alloc]
			initWithInterfaceOrientations:UIInterfaceOrientationMaskLandscape];
	[windowScene requestGeometryUpdateWithPreferences:landscape errorHandler:^(NSError *error) {
		NSLog(@"SonicPad landscape request failed: %@", error);
	}];
}

- (void)sceneDidEnterBackground:(UIScene *)scene
{
	[(SPGameViewController *)self.window.rootViewController clearInput];
	self.resumeOnForeground = emu.running();
	if (self.resumeOnForeground)
		emu.stop();
}

- (void)sceneWillEnterForeground:(UIScene *)scene
{
	if (self.resumeOnForeground)
		emu.start();
	self.resumeOnForeground = NO;
}

@end

@interface SPAppDelegate : UIResponder <UIApplicationDelegate>
@end

@implementation SPAppDelegate

- (UISceneConfiguration *)application:(UIApplication *)application
		configurationForConnectingSceneSession:(UISceneSession *)session
		options:(UISceneConnectionOptions *)options
{
	UISceneConfiguration *config = [[UISceneConfiguration alloc] initWithName:@"Default" sessionRole:session.role];
	config.delegateClass = [SPSceneDelegate class];
	return config;
}

@end

// Flycast reaches Vulkan through this entry point; MoltenVK is linked statically.
extern "C" void *vkGetInstanceProcAddr(void *instance, const char *name);
extern "C" void *SonicPad_VkGetInstanceProcAddr()
{
	return reinterpret_cast<void *>(&vkGetInstanceProcAddr);
}

// Platform callbacks Flycast's UI expects from the iOS shell.
const char *getIosJitStatus()
{
	return "Not used: static recompilation";
}

namespace vgamepad
{
void setEditMode(bool)
{
	// SonicPad's own touch layer replaces Flycast's virtual gamepad editor.
}
}

int main(int argc, char *argv[])
{
	@autoreleasepool {
		return UIApplicationMain(argc, argv, nil, NSStringFromClass([SPAppDelegate class]));
	}
}
