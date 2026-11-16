// RingPad iOS shell: a full-screen Metal layer driven by a display link,
// the reusable DCPad Dreamcast touch controls and ••• menu (port/dcpad),
// physical controller input, and verified import of the owner's disc. Flycast's Vulkan renderer
// draws into the layer through MoltenVK; the recompiled game runs on Flycast's
// emulation thread.
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <AVFoundation/AVFoundation.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#import "SPGameLibrary.h"
#import "SPDiagnostics.h"
#import "SPPortMenu.h"
#import "DCPadControllerInput.h"
#import "DCPadOverlayView.h"

#include "GameCheats.h"
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

static_assert(ringpad::kFlycastA == DC_BTN_A && ringpad::kFlycastB == DC_BTN_B, "Flycast face bits");
static_assert(ringpad::kFlycastX == DC_BTN_X && ringpad::kFlycastY == DC_BTN_Y, "Flycast face bits");
static_assert(ringpad::kFlycastStart == DC_BTN_START, "Flycast Start bit");
static_assert(ringpad::kFlycastUp == DC_DPAD_UP && ringpad::kFlycastDown == DC_DPAD_DOWN, "Flycast D-pad bits");
static_assert(ringpad::kFlycastLeft == DC_DPAD_LEFT && ringpad::kFlycastRight == DC_DPAD_RIGHT, "Flycast D-pad bits");

// Flycast's fault handler chains here for faults it does not own. Log where
// the fault happened (symbol and image offset) before the process traps, so
// crash reports and the diagnostic log name the faulting function.
static void reportFault(int sig, siginfo_t *info, void *context)
{
	const ucontext_t *uc = static_cast<const ucontext_t *>(context);
	const uintptr_t pc = uc->uc_mcontext->__ss.__pc;
	const uintptr_t lr = uc->uc_mcontext->__ss.__lr;
	Dl_info where{}, caller{};
	dladdr(reinterpret_cast<void *>(pc), &where);
	dladdr(reinterpret_cast<void *>(lr), &caller);
	char line[512];
	const int n = snprintf(line, sizeof line, "RINGPAD_FAULT signal=%d addr=%p pc=%s+0x%lx lr=%s+0x%lx\n", sig, info->si_addr,
			where.dli_sname ?: "?", (unsigned long)(pc - (uintptr_t)(where.dli_saddr ?: where.dli_fbase)),
			caller.dli_sname ?: "?", (unsigned long)(lr - (uintptr_t)(caller.dli_saddr ?: caller.dli_fbase)));
	// Straight to the log file: the stderr copier thread may not run again.
	if (SPDiagnosticsFD() >= 0 && n > 0)
		write(SPDiagnosticsFD(), line, (size_t)MIN(n, (int)sizeof line - 1));
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
	// RingPad's own overlay replaces Flycast's virtual gamepad.
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

@interface SPGameViewController : UIViewController <UIDocumentPickerDelegate, DCPadOverlayDelegate, SPPortMenuHost>
@property (nonatomic, strong) CADisplayLink *displayLink;
@property (nonatomic, strong) DCPadOverlayView *overlay;
@property (nonatomic, strong) SPPortMenu *portMenu;
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
@property (nonatomic, strong) UILabel *fpsLabel;
@property (nonatomic) BOOL renderStarted;
@property (nonatomic) BOOL scriptedInput;
@end

static NSString *const kShowFPSKey = @"RingPad.showFPS";
double RingPad_GuestSeconds();   // sp_aot_executor.cpp

@implementation SPGameViewController
{
	ringpad::FlycastPressHold _hold;
	// Input seen since launch, for the UI tests' debug label.
	uint32_t _seenPressed;
	int _minX, _maxX, _minY, _maxY;
	unsigned _maxLT, _maxRT;
	// Presented frames: the FPS counter, the log's render line and the
	// check that the game moves again after a resume.
	uint64_t _frames;
	CFTimeInterval _lastFrame;
	CFTimeInterval _fpsStart;
	uint64_t _fpsFrames;
	CFTimeInterval _windowStart;
	uint64_t _windowFrames;
	int _windowHitches;
	double _windowWorst;
	CFTimeInterval _resumedAt;
	uint64_t _framesAtResume;
	double _guestAtResume;
	BOOL _stallLogged;
}

- (void)loadView
{
	SPMetalView *view = [[SPMetalView alloc] initWithFrame:UIScreen.mainScreen.bounds];
	view.backgroundColor = UIColor.blackColor;
	self.view = view;

	self.overlay = [[DCPadOverlayView alloc] initWithFrame:view.bounds storageNamespace:@"SonicAdventure"];
	self.overlay.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.overlay.menuTitle = @"RingPad";
	self.overlay.delegate = self;
	[view addSubview:self.overlay];
	self.pauseReasons = [NSMutableSet set];
	self.portMenu = [[SPPortMenu alloc] initWithHost:self];
	[self buildFPSLabel];

	if (std::getenv("RINGPAD_INPUT_DEBUG") != nullptr) {
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

// Top-left counter in the controls' glass style.
- (void)buildFPSLabel
{
	UILabel *label = [[UILabel alloc] init];
	label.accessibilityIdentifier = @"sp.fps";
	label.font = [UIFont monospacedDigitSystemFontOfSize:15 weight:UIFontWeightSemibold];
	label.textColor = UIColor.whiteColor;
	label.textAlignment = NSTextAlignmentCenter;
	label.backgroundColor = [UIColor colorWithWhite:0.05 alpha:0.55];
	label.layer.cornerRadius = 15;
	label.layer.borderWidth = 1;
	label.layer.borderColor = [UIColor colorWithWhite:1 alpha:0.22].CGColor;
	label.layer.masksToBounds = YES;
	label.text = @"-- FPS";
	label.hidden = ![NSUserDefaults.standardUserDefaults boolForKey:kShowFPSKey];
	label.translatesAutoresizingMaskIntoConstraints = NO;
	[self.view addSubview:label];
	[NSLayoutConstraint activateConstraints:@[
		[label.topAnchor constraintEqualToAnchor:self.view.safeAreaLayoutGuide.topAnchor constant:12],
		[label.leadingAnchor constraintEqualToAnchor:self.view.safeAreaLayoutGuide.leadingAnchor constant:12],
		[label.heightAnchor constraintEqualToConstant:30],
		[label.widthAnchor constraintEqualToConstant:84],
	]];
	self.fpsLabel = label;
}

- (BOOL)portMenuFPSVisible
{
	return !self.fpsLabel.hidden;
}

- (void)portMenuSetFPSVisible:(BOOL)visible
{
	[NSUserDefaults.standardUserDefaults setBool:visible forKey:kShowFPSKey];
	self.fpsLabel.hidden = !visible;
}

- (void)buildImportView
{
	UIView *panel = [[UIView alloc] initWithFrame:self.view.bounds];
	panel.backgroundColor = [UIColor colorWithRed:0.02 green:0.08 blue:0.28 alpha:1];
	panel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	panel.hidden = YES;

	UILabel *title = [[UILabel alloc] init];
	title.text = @"RingPad";
	title.font = [UIFont systemFontOfSize:40 weight:UIFontWeightHeavy];
	title.textColor = UIColor.whiteColor;

	UILabel *body = [[UILabel alloc] init];
	body.text = @"Choose the folder that holds your Sonic Adventure PAL v1.003 disc: one .gdi file and its three tracks. RingPad checks every track before installing it.";
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
	// RingPad draws its own FPS counter at the top left.
	config::ShowFPS = false;
	[self applyGraphicsDefaultsOnce];
	[SPPortMenu applyWidescreenPreference];
	[self restoreCheats];
	vgamepad::hide();
	EventManager::listen(Event::Resume, hideFlycastGamepad);
	self.scriptedInput = std::getenv("RINGPAD_INPUT_SCRIPT") != nullptr;

	AVAudioSession *session = AVAudioSession.sharedInstance;
	[session setCategory:AVAudioSessionCategoryPlayback error:nil];
	[session setActive:YES error:nil];

	self.controllerInput = [[DCPadControllerInput alloc] initWithNamespace:@"SonicAdventure"];
	__weak SPGameViewController *weakSelf = self;
	self.controllerInput.connectionChanged = ^(BOOL connected) {
		SPLog(@"controller %@", connected ? @"connected" : @"disconnected");
		[weakSelf.overlay setControllerConnected:connected];
	};
	[self.controllerInput start];

	if (const char *game = std::getenv("RINGPAD_GAME")) {
		SPLog(@"game: from RINGPAD_GAME");
		settings.content.path = game;
	} else if (const char *folder = std::getenv("RINGPAD_IMPORT")) {
		// A relative path is inside the app's Documents, where a device copy
		// (devicectl, Finder file sharing) puts the disc folder.
		NSString *path = [NSString stringWithUTF8String:folder];
		if (!path.isAbsolutePath)
			path = [@(home.c_str()) stringByAppendingPathComponent:path];
		[self showImport:NO];
		[self importFromURL:[NSURL fileURLWithPath:path isDirectory:YES]];
	} else if (NSString *installed = std::getenv("RINGPAD_IGNORE_INSTALLED") ? nil : [SPGameLibrary installedGamePath]) {
		SPLog(@"game: installed disc");
		settings.content.path = installed.fileSystemRepresentation;
	} else {
		SPLog(@"game: none installed, showing import");
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
	SPLog(@"render start: %.0fx%.0f px, resolution %d lines, volume %d", layer.drawableSize.width, layer.drawableSize.height,
			(int)config::RenderResolution, (int)config::AudioVolume);
	initRenderApi((__bridge void *)layer);
	mainui_init();
	self.renderStarted = YES;

	self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
	self.displayLink.preferredFrameRateRange = CAFrameRateRangeMake(60, 60, 60);
	[self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];
	[self showMenuHintOnce];
	// Development and device testing: RINGPAD_IMPORT_VMU=<file>[,<file>] imports
	// a save (paths relative to Documents) once the game has started.
	if (const char *vmu = std::getenv("RINGPAD_IMPORT_VMU")) {
		NSMutableArray<NSURL *> *urls = [NSMutableArray array];
		NSString *documents = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
		for (NSString *part in [@(vmu) componentsSeparatedByString:@","])
			[urls addObject:[NSURL fileURLWithPath:part.isAbsolutePath ? part : [documents stringByAppendingPathComponent:part]]];
		dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
			[self.portMenu importVMUFromURLs:urls];
		});
	}
}

// One tip on the first play: where save states and settings live.
- (void)showMenuHintOnce
{
	static NSString *const kKey = @"RingPad.menuHintShown";
	if (settings.content.path.empty() || [NSUserDefaults.standardUserDefaults boolForKey:kKey])
		return;
	dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
		if (self.overlay.hidden || self.overlay.isModal || [NSUserDefaults.standardUserDefaults boolForKey:kKey])
			return;
		[NSUserDefaults.standardUserDefaults setBool:YES forKey:kKey];
		[self.overlay showMessage:@"Tap ••• for save states and settings" duration:4.5];
	});
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
	if (mainui_rend_frame()) {
		if (imguiDriver != nullptr)
			imguiDriver->present();
		[self countFrame];
	}
	[self updateFrameStats];
}

- (void)countFrame
{
	const CFTimeInterval now = CACurrentMediaTime();
	if (_lastFrame > 0 && self.pauseReasons.count == 0) {
		const double gap = (now - _lastFrame) * 1000;
		if (gap > 50)
			_windowHitches++;
		_windowWorst = MAX(_windowWorst, gap);
	}
	_lastFrame = now;
	_frames++;
	_fpsFrames++;
	_windowFrames++;
}

// Twice a second: the FPS counter. Every 30 s of play: one render line in
// the log. After a resume: a log line if no frame arrives within 2 s.
- (void)updateFrameStats
{
	const CFTimeInterval now = CACurrentMediaTime();
	const BOOL running = self.pauseReasons.count == 0 && emu.running();
	if (now - _fpsStart >= 0.5) {
		if (running && !self.fpsLabel.hidden)
			self.fpsLabel.text = [NSString stringWithFormat:@"%.0f FPS", _fpsFrames / (now - _fpsStart)];
		_fpsStart = now;
		_fpsFrames = 0;
	}
	if (!running) {
		[self resetFrameWindow];
		return;
	}
	if (now - _windowStart >= 30) {
		SPLog(@"render fps=%.1f hitches_over_50ms=%d worst_frame_ms=%.0f resolution=%d texture_upscale=%d",
				_windowFrames / (now - _windowStart), _windowHitches, _windowWorst,
				(int)config::RenderResolution, (int)config::TextureUpscale);
		[self resetFrameWindow];
	}
	// The game should have moved on within 2 s of a resume: new frames from
	// the renderer and new guest time from the emulator thread.
	if (_resumedAt > 0 && !_stallLogged && now - _resumedAt > 2) {
		const double guest = RingPad_GuestSeconds() - _guestAtResume;
		const uint64_t frames = _frames - _framesAtResume;
		if (guest < 0.5 || frames < 10)
			SPLog(@"stall: %.1f s after resume, guest advanced %.2f s, %llu frames (emu running=%d)",
					now - _resumedAt, guest, frames, emu.running() ? 1 : 0);
		_stallLogged = YES;
	}
}

- (void)resetFrameWindow
{
	_windowStart = CACurrentMediaTime();
	_windowFrames = 0;
	_windowHitches = 0;
	_windowWorst = 0;
	_lastFrame = 0;
}

- (void)updateInput
{
	if (self.scriptedInput)
		return;   // the executor's input script owns port A
	const RingPadDreamcastState merged = ringpad::DreamcastInputMixer::shared().consumeMerged();
	const ringpad::FlycastPadState pad = ringpad::to_flycast_pad(ringpad::adapt(merged));
	const uint32_t pressed = _hold.apply(pad.pressed);
	kcode[0] = ringpad::flycast_kcode(pressed);
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
		self.inputDebug.text = [NSString stringWithFormat:@"kcode=%08X seen=%08X x=%d..%d y=%d..%d lt=%u rt=%u run=%d frames=%llu guest=%.1f",
				kcode[0], _seenPressed, _minX, _maxX, _minY, _maxY, _maxLT, _maxRT, emu.running() ? 1 : 0, _frames,
				RingPad_GuestSeconds()];
	}
}

- (void)clearInput
{
	[self.overlay clearInput];
	ringpad::DreamcastInputMixer::shared().clearAll();
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
		SPLog(@"pause (%@)", reason);
		emu.stop();
		self.stoppedByShell = YES;
	} else if (!nowPaused && wasPaused && self.stoppedByShell) {
		SPLog(@"resume (%@)", reason);
		self.stoppedByShell = NO;
		emu.start();
		_resumedAt = CACurrentMediaTime();
		_framesAtResume = _frames;
		_guestAtResume = RingPad_GuestSeconds();
		_stallLogged = NO;
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

// The ••• menu, top to bottom: the quick row (save, load, reset), then how
// the game plays (Controls, Graphics, Sound), then files and help.
- (NSArray<UIMenuElement *> *)dcpadOverlay:(DCPadOverlayView *)overlay menuElementsWithControls:(UIMenu *)controls
{
	__weak SPGameViewController *weakSelf = self;
	const BOOL loaded = !settings.content.path.empty();
	UIAction *importGame = [UIAction actionWithTitle:@"Import Game…" image:[UIImage systemImageNamed:@"square.and.arrow.down"]
			identifier:nil handler:^(UIAction *) { [weakSelf showImport:YES]; }];
	importGame.subtitle = @"Your Sonic Adventure disc folder";
	UIAction *reset = [UIAction actionWithTitle:@"Reset" image:[UIImage systemImageNamed:@"arrow.counterclockwise"]
			identifier:nil handler:^(UIAction *) { [weakSelf confirmReset]; }];
	reset.attributes = loaded ? 0 : UIMenuElementAttributesDisabled;
	UIAction *about = [UIAction actionWithTitle:@"About This Build" image:[UIImage systemImageNamed:@"info.circle"]
			identifier:nil handler:^(UIAction *) { [weakSelf showAbout]; }];
	UIAction *diagnostics = [UIAction actionWithTitle:@"Share Diagnostic Log…" image:[UIImage systemImageNamed:@"doc.text.magnifyingglass"]
			identifier:nil handler:^(UIAction *) { [weakSelf shareDiagnostics]; }];
	diagnostics.subtitle = @"This session and the last one";
	NSMutableArray *quick = [[self.portMenu saveStateElementsForGameLoaded:loaded] mutableCopy];
	[quick addObject:reset];
	const BOOL phone = UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPhone;
	UIMenu *quickRow = [UIMenu menuWithTitle:phone ? @"" : @"RingPad" image:nil identifier:nil
			options:UIMenuOptionsDisplayInline children:quick];
	quickRow.preferredElementSize = UIMenuElementSizeMedium;
	UIMenu *files = [UIMenu menuWithTitle:@"Game Files" image:[UIImage systemImageNamed:@"folder"] identifier:nil options:0
			children:@[importGame, [self.portMenu importVMUActionForGameLoaded:loaded],
				[self.portMenu exportVMUActionForGameLoaded:loaded]]];
	UIMenu *help = [UIMenu menuWithTitle:@"Help" image:[UIImage systemImageNamed:@"questionmark.circle"] identifier:nil options:0
			children:@[about, diagnostics]];
	return @[
		quickRow,
		[UIMenu menuWithTitle:@"" image:nil identifier:nil options:UIMenuOptionsDisplayInline
				children:@[controls, [self.portMenu graphicsMenu], [self.portMenu soundMenu], [self cheatsMenu]]],
		[UIMenu menuWithTitle:@"" image:nil identifier:nil options:UIMenuOptionsDisplayInline children:@[files, help]],
	];
}

#pragma mark Cheats

static NSString *const kCheatsKey = @"RingPad.cheats";

// Cheats ▸: each one is a toggle that stays on across launches.
- (UIMenu *)cheatsMenu
{
	NSMutableArray *items = [NSMutableArray array];
	int on = 0;
	for (size_t i = 0; i < ringpad::cheat_count(); i++) {
		const ringpad::CheatInfo& info = ringpad::cheat_info(i);
		const BOOL enabled = ringpad::cheat_enabled(i);
		on += enabled ? 1 : 0;
		NSString *title = @(info.title);
		UIAction *a = [UIAction actionWithTitle:title image:nil identifier:nil handler:^(UIAction *) {
			ringpad::set_cheat_enabled(i, !enabled);
			[self saveCheats];
			SPLog(@"cheat %s=%d", ringpad::cheat_info(i).id, !enabled);
			[self.overlay showMessage:[NSString stringWithFormat:@"%@ %@", title, enabled ? @"off" : @"on"]];
		}];
		a.subtitle = @(info.subtitle);
		a.state = enabled ? UIMenuElementStateOn : UIMenuElementStateOff;
		[items addObject:a];
	}
	UIMenu *menu = [UIMenu menuWithTitle:@"Cheats" image:[UIImage systemImageNamed:@"wand.and.rays"] identifier:nil
			options:0 children:items];
	menu.subtitle = on > 0 ? [NSString stringWithFormat:@"%d on", on] : nil;
	return menu;
}

- (void)saveCheats
{
	NSMutableArray *ids = [NSMutableArray array];
	for (size_t i = 0; i < ringpad::cheat_count(); i++)
		if (ringpad::cheat_enabled(i))
			[ids addObject:@(ringpad::cheat_info(i).id)];
	[NSUserDefaults.standardUserDefaults setObject:ids forKey:kCheatsKey];
}

- (void)restoreCheats
{
	ringpad::install_cheats();
	NSArray *ids = [NSUserDefaults.standardUserDefaults arrayForKey:kCheatsKey];
	if (ids.count > 0)
		ringpad::enable_cheats_from_list([ids componentsJoinedByString:@","].UTF8String);
	SPLog(@"cheats on: %@", ids.count > 0 ? [ids componentsJoinedByString:@","] : @"none");
}

// Restart the loaded game (Reset, memory card import, widescreen off).
- (void)portMenuRestartGame
{
	if (settings.content.path.empty())
		return;
	SPLog(@"restart game");
	[self.pauseReasons minusSet:[NSSet setWithArray:@[@"reset", @"vmu-import", @"widescreen"]]];
	self.stoppedByShell = NO;   // reloading starts the emulator itself
	gui_start_game(settings.content.path);
}

// First launch only: keep the original resolution and textures. Existing
// installs retain their choices; players can change them under Graphics.
- (void)applyGraphicsDefaultsOnce
{
	static NSString *const kKey = @"RingPad.graphicsDefaults.v1";
	if ([NSUserDefaults.standardUserDefaults boolForKey:kKey])
		return;
	[NSUserDefaults.standardUserDefaults setBool:YES forKey:kKey];
	// New installs start as the Dreamcast drew the game; sharper rendering is
	// a choice in ••• ▸ Graphics. Existing installs keep their settings.
	config::RenderResolution = 480;
	config::TextureUpscale = 1;
	SaveSettings();
	SPLog(@"graphics defaults: dreamcast original (resolution=480, texture_upscale=1)");
}

- (void)shareDiagnostics
{
	SPLog(@"diagnostic log shared");
	NSURL *report = SPDiagnosticsReportURL();
	if (report == nil) {
		[self.overlay showMessage:@"Couldn't read the log"];
		return;
	}
	[self setPaused:YES reason:@"diagnostics"];
	UIActivityViewController *share = [[UIActivityViewController alloc] initWithActivityItems:@[report] applicationActivities:nil];
	share.popoverPresentationController.sourceView = self.view;
	share.popoverPresentationController.sourceRect = CGRectMake(CGRectGetMaxX(self.view.bounds) - 60, 40, 1, 1);
	share.completionWithItemsHandler = ^(UIActivityType, BOOL, NSArray *, NSError *) {
		[self setPaused:NO reason:@"diagnostics"];
	};
	[self presentViewController:share animated:YES completion:nil];
}

- (void)portMenuShowMessage:(NSString *)message
{
	SPLog(@"%@", message);
	[self.overlay showMessage:message];
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
		[self portMenuRestartGame];
	}]];
	[self presentViewController:alert animated:YES completion:nil];
}

- (void)showAbout
{
	extern const char sp_aot_manifest_id[];
	NSDictionary *info = NSBundle.mainBundle.infoDictionary;
	NSString *message = [NSString stringWithFormat:@"Version %@ (build %@, %@)\nRecompiled code manifest %s\nRenderer: Vulkan on Metal (MoltenVK)\nGame: %@\n\nUnofficial. Not affiliated with or endorsed by SEGA.",
			info[@"CFBundleShortVersionString"] ?: @"?", info[@"CFBundleVersion"] ?: @"?", info[@"RingPadCommit"] ?: @"?",
			sp_aot_manifest_id,
			settings.content.path.empty() ? @"not loaded" : @"Sonic Adventure PAL v1.003"];
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"RingPad" message:message
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
	SPLog(@"import start %@", folder.lastPathComponent);
	[SPGameLibrary importFolder:folder
			progress:^(double fraction) {
				self.importProgress.progress = fraction;
			}
			completion:^(NSString *gdiPath, NSString *error) {
				self.importChoose.enabled = YES;
				self.importCancel.enabled = YES;
				self.importProgress.hidden = YES;
				if (gdiPath == nil) {
					SPLog(@"import failed: %@", error);
					self.importStatus.text = error;
					return;
				}
				SPLog(@"import ok %@", gdiPath.lastPathComponent);
				[self hideImport];
				[self.pauseReasons removeObject:@"import"];
				self.stoppedByShell = NO;   // loading starts the emulator itself
				[self startGame:gdiPath];
				[self showMenuHintOnce];
			}];
}

- (void)startGame:(NSString *)gdiPath
{
	SPLog(@"start game");
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
		SPLog(@"landscape request failed: %@", error);
	}];
}

- (void)sceneDidEnterBackground:(UIScene *)scene
{
	SPLog(@"background");
	[(SPGameViewController *)self.window.rootViewController setPaused:YES reason:@"background"];
}

- (void)sceneWillEnterForeground:(UIScene *)scene
{
	SPLog(@"foreground");
	[(SPGameViewController *)self.window.rootViewController setPaused:NO reason:@"background"];
}

@end

// The scene and its SPSceneDelegate are declared in Info.plist.
@interface SPAppDelegate : UIResponder <UIApplicationDelegate>
@end

@implementation SPAppDelegate
@end

// Flycast reaches Vulkan through this entry point; MoltenVK is linked statically.
extern "C" void *vkGetInstanceProcAddr(void *instance, const char *name);
extern "C" void *RingPad_VkGetInstanceProcAddr()
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
	// RingPad's own touch layer replaces Flycast's virtual gamepad editor.
}
}

int main(int argc, char *argv[])
{
	@autoreleasepool {
		// MoltenVK lists every Vulkan extension at info level; keep warnings
		// and errors so the diagnostic log stays readable.
		setenv("MVK_CONFIG_LOG_LEVEL", "2", 0);
		// Development runs on a device: relative script, screenshot and state
		// paths are inside the app's Documents, where devicectl copies files.
		NSString *documents = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
		for (const char *name : {"RINGPAD_INPUT_SCRIPT", "RINGPAD_SHOT_DIR", "RINGPAD_STATE_LOAD"})
			if (const char *value = std::getenv(name); value != nullptr && value[0] != '/')
				setenv(name, [documents stringByAppendingPathComponent:@(value)].fileSystemRepresentation, 1);
		SPDiagnosticsStart();
		return UIApplicationMain(argc, argv, nil, NSStringFromClass([SPAppDelegate class]));
	}
}
