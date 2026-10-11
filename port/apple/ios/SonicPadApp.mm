// SonicPad iOS shell: a full-screen Metal layer driven by a display link.
// Flycast's Vulkan renderer draws into the layer through MoltenVK; the
// recompiled game runs on Flycast's emulation thread.
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <AVFoundation/AVFoundation.h>

#include "types.h"
#include "emulator.h"
#include "stdclass.h"
#include "cfg/option.h"
#include "log/LogManager.h"
#include "oslib/oslib.h"
#include "oslib/i18n.h"
#include "ui/mainui.h"
#include "ui/imgui_driver.h"
#include "wsi/context.h"

#include <cstdlib>
#include <string>

@interface SPMetalView : UIView
@end

@implementation SPMetalView
+ (Class)layerClass
{
	return [CAMetalLayer class];
}
@end

@interface SPGameViewController : UIViewController
@property (nonatomic, strong) CADisplayLink *displayLink;
@end

@implementation SPGameViewController

- (void)loadView
{
	SPMetalView *view = [[SPMetalView alloc] initWithFrame:UIScreen.mainScreen.bounds];
	view.backgroundColor = UIColor.blackColor;
	view.multipleTouchEnabled = YES;
	self.view = view;
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

	os_InstallFaultHandler();
	flycast_init(0, nullptr);
	config::RendererType = RenderType::Vulkan;
	if (const char *game = std::getenv("SONICPAD_GAME"))
		settings.content.path = game;

	AVAudioSession *session = AVAudioSession.sharedInstance;
	[session setCategory:AVAudioSessionCategoryPlayback error:nil];
	[session setActive:YES error:nil];
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	if (self.displayLink != nil)
		return;
	CAMetalLayer *layer = (CAMetalLayer *)self.view.layer;
	const CGFloat scale = self.view.window.screen.nativeScale;
	layer.contentsScale = scale;
	layer.drawableSize = CGSizeMake(self.view.bounds.size.width * scale, self.view.bounds.size.height * scale);

	settings.display.width = (int)layer.drawableSize.width;
	settings.display.height = (int)layer.drawableSize.height;
	settings.display.dpi = 160.f * scale;
	initRenderApi((__bridge void *)layer);
	mainui_init();

	self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
	self.displayLink.preferredFrameRateRange = CAFrameRateRangeMake(60, 60, 60);
	[self.displayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];
}

- (void)renderFrame
{
	if (mainui_rend_frame() && imguiDriver != nullptr)
		imguiDriver->present();
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
}

- (void)sceneDidEnterBackground:(UIScene *)scene
{
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
