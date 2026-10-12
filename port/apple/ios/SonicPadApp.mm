// SonicPad iOS shell: a full-screen Metal layer driven by a display link,
// the reusable DCPad Dreamcast touch controls and ••• menu (port/dcpad),
// physical controller input, and verified import of the owner's disc. Flycast's Vulkan renderer
// draws into the layer through MoltenVK; the recompiled game runs on Flycast's
// emulation thread.
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <AVFoundation/AVFoundation.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#import "SPGameLibrary.h"
#import "DCPadControllerInput.h"
#import "DCPadOverlayView.h"

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

@interface SPGameViewController : UIViewController <UIDocumentPickerDelegate, DCPadOverlayDelegate>
@property (nonatomic, strong) CADisplayLink *displayLink;
@property (nonatomic, strong) DCPadOverlayView *overlay;
@property (nonatomic, strong) DCPadControllerInput *controllerInput;
@property (nonatomic, strong) NSMutableSet<NSString *> *pauseReasons;
@property (nonatomic) BOOL stoppedByShell;
- (void)setPaused:(BOOL)paused reason:(NSString *)reason;
- (void)clearInput;
@property (nonatomic, strong) UIView *importView;
@property (nonatomic, strong) UILabel *importStatus;
@property (nonatomic, strong) UIProgressView *importProgress;
@property (nonatomic, strong) UIButton *importChoose;
@property (nonatomic, strong) UIButton *importCancel;
@property (nonatomic, strong) UILabel *inputDebug;
@property (nonatomic) BOOL renderStarted;
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

	self.overlay = [[DCPadOverlayView alloc] initWithFrame:view.bounds storageNamespace:@"SonicAdventure"];
	self.overlay.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.overlay.menuTitle = @"SonicPad";
	self.overlay.delegate = self;
	[view addSubview:self.overlay];
	self.pauseReasons = [NSMutableSet set];

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

	self.controllerInput = [[DCPadControllerInput alloc] initWithNamespace:@"SonicAdventure"];
	__weak SPGameViewController *weakSelf = self;
	self.controllerInput.connectionChanged = ^(BOOL connected) {
		[weakSelf.overlay setControllerConnected:connected];
	};
	[self.controllerInput start];

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
		self.inputDebug.text = [NSString stringWithFormat:@"kcode=%08X seen=%08X x=%d..%d y=%d..%d lt=%u rt=%u run=%d",
				kcode[0], _seenPressed, _minX, _maxX, _minY, _maxY, _maxLT, _maxRT, emu.running() ? 1 : 0];
	}
}

- (void)clearInput
{
	[self.overlay clearInput];
	sonicpad::DreamcastInputMixer::shared().clearAll();
	_hold.clear();
}

// The game runs only while nothing needs it paused (menu, settings, layout
// editing, import, background). Every change clears held input.
- (void)setPaused:(BOOL)paused reason:(NSString *)reason
{
	[self clearInput];
	const BOOL wasPaused = self.pauseReasons.count > 0;
	if (paused)
		[self.pauseReasons addObject:reason];
	else
		[self.pauseReasons removeObject:reason];
	const BOOL nowPaused = self.pauseReasons.count > 0;
	if (nowPaused && !wasPaused && emu.running()) {
		emu.stop();
		self.stoppedByShell = YES;
	} else if (!nowPaused && wasPaused && self.stoppedByShell) {
		self.stoppedByShell = NO;
		emu.start();
	}
}

- (void)dcpadOverlay:(DCPadOverlayView *)overlay modalStateChanged:(BOOL)modal
{
	[self setPaused:modal reason:@"overlay"];
}

- (void)dcpadOverlayRequestsControllerMapping:(DCPadOverlayView *)overlay
{
	[self setPaused:YES reason:@"mapping"];
	[self.controllerInput presentMappingFrom:self];
	[self watchPresentedControllerReleaseReason:@"mapping"];
}

// Alerts chain into each other; resume once none is showing.
- (void)watchPresentedControllerReleaseReason:(NSString *)reason
{
	dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.4 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
		if (self.presentedViewController != nil)
			[self watchPresentedControllerReleaseReason:reason];
		else
			[self setPaused:NO reason:reason];
	});
}

- (NSArray<UIMenuElement *> *)dcpadOverlayMenuElements:(DCPadOverlayView *)overlay
{
	__weak SPGameViewController *weakSelf = self;
	UIAction *importGame = [UIAction actionWithTitle:@"Import Game…" image:[UIImage systemImageNamed:@"square.and.arrow.down"]
			identifier:nil handler:^(UIAction *) { [weakSelf showImport:YES]; }];
	UIAction *reset = [UIAction actionWithTitle:@"Reset Game…" image:[UIImage systemImageNamed:@"arrow.counterclockwise"]
			identifier:nil handler:^(UIAction *) { [weakSelf confirmReset]; }];
	reset.attributes = settings.content.path.empty() ? UIMenuElementAttributesDisabled : 0;
	UIAction *fps = [UIAction actionWithTitle:@"Show FPS" image:[UIImage systemImageNamed:@"speedometer"]
			identifier:nil handler:^(UIAction *) {
		config::ShowFPS = !config::ShowFPS;
		SaveSettings();
	}];
	fps.state = config::ShowFPS ? UIMenuElementStateOn : UIMenuElementStateOff;
	UIAction *about = [UIAction actionWithTitle:@"About This Build" image:[UIImage systemImageNamed:@"info.circle"]
			identifier:nil handler:^(UIAction *) { [weakSelf showAbout]; }];
	return @[
		[UIMenu menuWithTitle:@"Game" image:nil identifier:nil options:UIMenuOptionsDisplayInline children:@[importGame, reset]],
		[UIMenu menuWithTitle:@"Display" image:[UIImage systemImageNamed:@"display"] identifier:nil options:0 children:@[fps, about]],
	];
}

- (void)confirmReset
{
	[self setPaused:YES reason:@"reset"];
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Reset Game?"
			message:@"The game restarts from the beginning. Progress since your last in-game save is lost." preferredStyle:UIAlertControllerStyleAlert];
	[alert addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *) {
		[self setPaused:NO reason:@"reset"];
	}]];
	[alert addAction:[UIAlertAction actionWithTitle:@"Reset" style:UIAlertActionStyleDestructive handler:^(UIAlertAction *) {
		[self.pauseReasons removeObject:@"reset"];
		self.stoppedByShell = NO;   // reloading starts the emulator itself
		if (!settings.content.path.empty())
			gui_start_game(settings.content.path);
	}]];
	[self presentViewController:alert animated:YES completion:nil];
}

- (void)showAbout
{
	extern const char sp_aot_manifest_id[];
	NSString *version = [NSBundle.mainBundle objectForInfoDictionaryKey:@"CFBundleShortVersionString"] ?: @"?";
	NSString *message = [NSString stringWithFormat:@"Version %@\nRecompiled code manifest %s\nRenderer: Vulkan on Metal (MoltenVK)\nGame: %@",
			version, sp_aot_manifest_id,
			settings.content.path.empty() ? @"not loaded" : @"Sonic Adventure PAL v1.003"];
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"SonicPad" message:message
			preferredStyle:UIAlertControllerStyleAlert];
	[alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
	[self presentViewController:alert animated:YES completion:nil];
}

- (void)showImport:(BOOL)cancellable
{
	[self setPaused:YES reason:@"import"];
	self.importStatus.text = nil;
	self.importProgress.hidden = YES;
	self.importChoose.enabled = YES;
	self.importCancel.hidden = !cancellable;
	self.importView.hidden = NO;
	self.overlay.hidden = YES;
}

- (void)hideImport
{
	self.importView.hidden = YES;
	self.overlay.hidden = NO;
}

- (void)cancelImport
{
	[self hideImport];
	[self setPaused:NO reason:@"import"];
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
				[self.pauseReasons removeObject:@"import"];
				self.stoppedByShell = NO;   // loading starts the emulator itself
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

// Thumbs live on the screen edges; a swipe that starts there should reach the
// game first instead of opening Control Center or going home.
- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures
{
	return UIRectEdgeAll;
}

@end

@interface SPSceneDelegate : UIResponder <UIWindowSceneDelegate>
@property (nonatomic, strong) UIWindow *window;
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
	[(SPGameViewController *)self.window.rootViewController setPaused:YES reason:@"background"];
}

- (void)sceneWillEnterForeground:(UIScene *)scene
{
	[(SPGameViewController *)self.window.rootViewController setPaused:NO reason:@"background"];
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
