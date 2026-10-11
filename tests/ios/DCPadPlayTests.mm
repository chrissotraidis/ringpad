// Plays the opening of Sonic Adventure with SonicPad's touch controls only, attaching
// screenshots for review. Not an assertion suite beyond basic progress checks.
#import <XCTest/XCTest.h>

@interface DCPadPlayTests : XCTestCase
@end

@implementation DCPadPlayTests

- (void)shot:(NSString *)name
{
	XCTAttachment *a = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
	a.name = name;
	a.lifetime = XCTAttachmentLifetimeKeepAlways;
	[self addAttachment:a];
}

- (XCUIElement *)control:(NSString *)name in:(XCUIApplication *)app
{
	NSString *ident = [@"dcpad." stringByAppendingString:name];
	XCUIElement *e = app.buttons[ident];
	return e.exists ? e : app.otherElements[ident];
}

- (void)testPlayOpeningWithTouch
{
	XCUIDevice.sharedDevice.orientation = UIDeviceOrientationLandscapeLeft;
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"SONICPAD_GAME": NSProcessInfo.processInfo.environment[@"SONICPAD_GDI"] ?: @"" };
	app.launchArguments = @[@"-DCPad.SonicAdventure.tablet", @"{}", @"-DCPad.SonicAdventure.phone", @"{}"];
	NSDate *start = [NSDate date];
	[app launch];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:30]);
	XCUIElement *startButton = [self control:@"start" in:app];
	XCUIElement *a = [self control:@"A" in:app];
	XCUIElement *stick = [self control:@"stick" in:app];
	auto waitUntil = ^(double seconds) {
		const double left = seconds - [[NSDate date] timeIntervalSinceDate:start];
		if (left > 0)
			[NSThread sleepForTimeInterval:left];
	};
	// Skip the logos and intro, open the title menu.
	for (double t = 8; t <= 26; t += 3) {
		waitUntil(t);
		[startButton tap];
	}
	[self shot:@"1-title-or-file-select"];
	// File select -> Sonic -> Game.
	for (double t = 29; t <= 41; t += 3) {
		waitUntil(t);
		[a tap];
	}
	[self shot:@"2-after-menus"];
	// Skip the opening cutscene.
	for (double t = 50; t <= 70; t += 4) {
		waitUntil(t);
		[startButton tap];
	}
	waitUntil(95);
	[self shot:@"3-gameplay"];
	// Run with the stick and jump with A.
	XCUICoordinate *c = [stick coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)];
	for (int i = 0; i < 4; i++) {
		const CGVector dirs[4] = {CGVectorMake(0.5, 0.0), CGVectorMake(1.0, 0.5), CGVectorMake(0.5, 1.0), CGVectorMake(0.0, 0.5)};
		[c pressForDuration:0.05 thenDragToCoordinate:[stick coordinateWithNormalizedOffset:dirs[i]]
				withVelocity:XCUIGestureVelocityFast thenHoldForDuration:2.0];
		[a tap];
	}
	[self shot:@"4-after-running"];
	[NSThread sleepForTimeInterval:2];
	[self shot:@"5-later"];
}

@end
