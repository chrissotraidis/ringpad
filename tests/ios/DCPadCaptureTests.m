// Visual capture of the DCPad controls for layout review (not an assertion suite).
// Run with: scripts/ios/run-ui-tests.sh <udid> -only-testing:RingPadUITests/DCPadCaptureTests
#import <XCTest/XCTest.h>

@interface DCPadCaptureTests : XCTestCase
@end

@implementation DCPadCaptureTests

- (void)shot:(NSString *)name
{
	XCTAttachment *a = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
	a.name = name;
	a.lifetime = XCTAttachmentLifetimeKeepAlways;
	[self addAttachment:a];
}

- (void)testCaptureControls
{
	XCUIDevice.sharedDevice.orientation = UIDeviceOrientationLandscapeLeft;
	XCUIApplication *app = [[XCUIApplication alloc] init];
	// Default touch settings, whatever earlier tests saved.
	app.launchArguments = @[@"-DCPad.SonicAdventure.tablet", @"{}", @"-DCPad.SonicAdventure.phone", @"{}"];
	NSMutableDictionary *env = [@{ @"RINGPAD_INPUT_DEBUG": @"1" } mutableCopy];
	NSString *gdi = NSProcessInfo.processInfo.environment[@"RINGPAD_GDI"];
	if (gdi != nil)
		env[@"RINGPAD_GAME"] = gdi;
	NSString *preview = NSProcessInfo.processInfo.environment[@"DCPAD_PREVIEW"];
	if (preview.length > 0)
		env[@"DCPAD_PREVIEW"] = preview;
	app.launchEnvironment = env;
	[app launch];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:30]);
	[NSThread sleepForTimeInterval:12];
	[self shot:@"1-idle"];

	XCUIElement *stick = app.otherElements[@"dcpad.stick"].exists ? app.otherElements[@"dcpad.stick"] : app.buttons[@"dcpad.stick"];
	XCUIElement *a = app.otherElements[@"dcpad.A"].exists ? app.otherElements[@"dcpad.A"] : app.buttons[@"dcpad.A"];
	if (stick.exists && a.exists) {
		XCUICoordinate *center = [stick coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)];
		[center pressForDuration:0.1 thenDragToCoordinate:[stick coordinateWithNormalizedOffset:CGVectorMake(0.9, 0.2)]
				withVelocity:XCUIGestureVelocityDefault thenHoldForDuration:1.5];
	}
	[self shot:@"2-after-drag"];

	[app.buttons[@"dcpad.menu"] tap];
	[NSThread sleepForTimeInterval:1.0];
	[self shot:@"3-menu"];
	[app.buttons[@"Controls"] tap];
	[NSThread sleepForTimeInterval:1.0];
	[self shot:@"3b-menu-controls"];
	XCUIElement *settings = app.buttons[@"Touch Control Settings…"];
	if ([settings waitForExistenceWithTimeout:5]) {
		[settings tap];
		[NSThread sleepForTimeInterval:1.0];
		[self shot:@"4-settings"];
		XCUIElement *move = app.buttons[@"dcpad.settings.move"];
		if (move.exists) {
			[move tap];
			[NSThread sleepForTimeInterval:1.0];
			[self shot:@"5-move"];
			[app.buttons[@"dcpad.edit.done"] tap];
		}
	}
}

@end
