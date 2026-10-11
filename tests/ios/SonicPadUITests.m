// SonicPad UI tests: real UIKit touches against the iOS shell in the Simulator.
//
// Run with scripts/ios/run-ui-tests.sh. The owner's disc is passed through
// TEST_RUNNER_SONICPAD_GDI_DIR (folder) and TEST_RUNNER_SONICPAD_GDI (file).
#import <XCTest/XCTest.h>

@interface SonicPadUITests : XCTestCase
@end

@implementation SonicPadUITests

- (void)setUp
{
	self.continueAfterFailure = NO;
	XCUIDevice.sharedDevice.orientation = UIDeviceOrientationLandscapeLeft;
}

- (NSString *)env:(NSString *)name
{
	NSString *value = NSProcessInfo.processInfo.environment[name];
	XCTAssertNotNil(value, @"%@ must be set for this test", name);
	return value;
}

- (void)attachScreenshot:(XCUIApplication *)app name:(NSString *)name
{
	XCTAttachment *shot = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
	shot.name = name;
	shot.lifetime = XCTAttachmentLifetimeKeepAlways;
	[self addAttachment:shot];
}

- (XCUIApplication *)launchGameWithDebug
{
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"SONICPAD_GAME": [self env:@"SONICPAD_GDI"], @"SONICPAD_INPUT_DEBUG": @"1" };
	[app launch];
	XCTAssertTrue([app.otherElements[@"sp.control.A"] waitForExistenceWithTimeout:30] ||
			[app.buttons[@"sp.control.A"] waitForExistenceWithTimeout:5]);
	return app;
}

- (XCUIElement *)control:(NSString *)name in:(XCUIApplication *)app
{
	XCUIElement *element = app.buttons[[@"sp.control." stringByAppendingString:name]];
	if (!element.exists)
		element = app.otherElements[[@"sp.control." stringByAppendingString:name]];
	XCTAssertTrue(element.exists, @"control %@", name);
	return element;
}

- (NSDictionary<NSString *, NSNumber *> *)inputState:(XCUIApplication *)app
{
	NSString *text = app.staticTexts[@"sp.inputDebug"].label;
	NSMutableDictionary *out = [NSMutableDictionary dictionary];
	NSScanner *scanner = [NSScanner scannerWithString:text];
	unsigned kcode = 0, seen = 0;
	int minX = 0, maxX = 0, minY = 0, maxY = 0, lt = 0, rt = 0;
	if ([scanner scanString:@"kcode=" intoString:nil] && [scanner scanHexInt:&kcode]
			&& [scanner scanString:@"seen=" intoString:nil] && [scanner scanHexInt:&seen]
			&& [scanner scanString:@"x=" intoString:nil] && [scanner scanInt:&minX]
			&& [scanner scanString:@".." intoString:nil] && [scanner scanInt:&maxX]
			&& [scanner scanString:@"y=" intoString:nil] && [scanner scanInt:&minY]
			&& [scanner scanString:@".." intoString:nil] && [scanner scanInt:&maxY]
			&& [scanner scanString:@"lt=" intoString:nil] && [scanner scanInt:&lt]
			&& [scanner scanString:@"rt=" intoString:nil] && [scanner scanInt:&rt]) {
		out[@"kcode"] = @(kcode);
		out[@"seen"] = @(seen);
		out[@"minX"] = @(minX);
		out[@"maxX"] = @(maxX);
		out[@"minY"] = @(minY);
		out[@"maxY"] = @(maxY);
		out[@"lt"] = @(lt);
		out[@"rt"] = @(rt);
	}
	XCTAssertEqual(out.count, 8u, @"unparsed debug label: %@", text);
	return out;
}

// Flycast DreamcastKey bits (core/input/gamepad.h).
static const unsigned kB = 1u << 1, kA = 1u << 2, kStart = 1u << 3, kUp = 1u << 4, kRight = 1u << 7, kX = 1u << 10;

- (void)testFreshInstallShowsImportInLandscape
{
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"SONICPAD_IGNORE_INSTALLED": @"1" };
	[app launch];
	XCUIElement *choose = app.buttons[@"sp.import.choose"];
	XCTAssertTrue([choose waitForExistenceWithTimeout:20]);
	const CGRect window = app.windows.firstMatch.frame;
	XCTAssertGreaterThan(window.size.width, window.size.height, @"app window is landscape");
	XCTAssertFalse(app.buttons[@"sp.import.cancel"].exists, @"no cancel without an installed game");
	[self attachScreenshot:app name:@"import-screen"];

	[choose tap];
	XCUIElement *pickerCancel = app.buttons[@"Cancel"];
	XCTAssertTrue([pickerCancel waitForExistenceWithTimeout:20], @"folder picker appears");
	[self attachScreenshot:app name:@"folder-picker"];
	[pickerCancel tap];
	XCTAssertTrue([choose waitForExistenceWithTimeout:10]);
}

- (void)testImportVerifiesDiscAndStartsGame
{
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"SONICPAD_IMPORT": [self env:@"SONICPAD_GDI_DIR"] };
	[app launch];
	XCUIElement *menu = app.buttons[@"sp.menu"];
	XCTAssertTrue([menu waitForExistenceWithTimeout:120], @"import finished and the game view is shown");
	XCTAssertTrue(menu.isHittable);
	XCTAssertFalse(app.buttons[@"sp.import.choose"].exists);
	[NSThread sleepForTimeInterval:15];
	[self attachScreenshot:app name:@"after-import"];

	// Relaunch without any hook: the installed copy boots by itself.
	[app terminate];
	app.launchEnvironment = @{};
	[app launch];
	XCTAssertTrue([app.buttons[@"sp.menu"] waitForExistenceWithTimeout:30], @"installed game boots on relaunch");
	XCTAssertFalse(app.buttons[@"sp.import.choose"].exists);
}

- (void)testTouchControlsDriveFlycastInput
{
	XCUIApplication *app = [self launchGameWithDebug];
	const CGRect window = app.windows.firstMatch.frame;
	XCTAssertGreaterThan(window.size.width, window.size.height, @"app window is landscape");
	XCTAssertTrue([app.staticTexts[@"sp.inputDebug"] waitForExistenceWithTimeout:10]);
	NSDictionary *idle = [self inputState:app];
	XCTAssertEqual([idle[@"seen"] unsignedIntValue], 0u, @"no input before any touch");

	[[self control:@"A" in:app] tap];
	[[self control:@"X" in:app] tap];
	[[self control:@"Up" in:app] tap];
	[[self control:@"Start" in:app] tap];
	NSDictionary *buttons = [self inputState:app];
	const unsigned seen = [buttons[@"seen"] unsignedIntValue];
	XCTAssertTrue(seen & kA, @"A reached Flycast");
	XCTAssertTrue(seen & kX, @"X reached Flycast");
	XCTAssertTrue(seen & kUp, @"D-pad up reached Flycast");
	XCTAssertTrue(seen & kStart, @"Start reached Flycast");
	XCTAssertFalse(seen & kB, @"B was not touched");
	XCTAssertEqual([buttons[@"kcode"] unsignedIntValue], 0xFFFFFFFFu, @"every button released after the taps");

	XCUIElement *stick = [self control:@"Move" in:app];
	XCUICoordinate *center = [stick coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)];
	[center pressForDuration:0.3 thenDragToCoordinate:[stick coordinateWithNormalizedOffset:CGVectorMake(1.0, 0.5)]];
	[center pressForDuration:0.3 thenDragToCoordinate:[stick coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.0)]];
	NSDictionary *stickState = [self inputState:app];
	XCTAssertGreaterThan([stickState[@"maxX"] intValue], 20000, @"stick right reached Flycast");
	XCTAssertLessThan([stickState[@"minY"] intValue], -20000, @"stick up is negative Flycast Y");
	XCTAssertFalse([stickState[@"seen"] unsignedIntValue] & kRight, @"the stick never fires the D-pad");

	XCUIElement *shoulder = [self control:@"R" in:app];
	[[shoulder coordinateWithNormalizedOffset:CGVectorMake(0.95, 0.5)] pressForDuration:0.3];
	NSDictionary *triggers = [self inputState:app];
	XCTAssertGreaterThan([triggers[@"rt"] intValue], 50000, @"right shoulder slide reached Flycast");
	XCTAssertEqual([triggers[@"lt"] intValue], 0, @"left trigger untouched");
	[self attachScreenshot:app name:@"touch-overlay"];
}

- (void)testMenuIsSeparateFromStartAndPauses
{
	XCUIApplication *app = [self launchGameWithDebug];
	XCUIElement *menu = app.buttons[@"sp.menu"];
	XCTAssertTrue([menu waitForExistenceWithTimeout:10]);
	[menu tap];
	XCUIElement *import = app.buttons[@"Import Game…"];
	XCTAssertTrue([import waitForExistenceWithTimeout:10], @"menu offers import");
	[self attachScreenshot:app name:@"menu"];
	XCUIElement *resume = app.buttons[@"Resume"];
	if (resume.exists)
		[resume tap];
	else
		[[app coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)] tap];
	XCTAssertFalse([self.class bits:[self inputState:app] has:kStart], @"the menu never sends Start");

	[menu tap];
	XCTAssertTrue([import waitForExistenceWithTimeout:10]);
	[import tap];
	XCUIElement *cancel = app.buttons[@"sp.import.cancel"];
	XCTAssertTrue([cancel waitForExistenceWithTimeout:10], @"import can be cancelled while a game is installed");
	[cancel tap];
	XCTAssertTrue([menu waitForExistenceWithTimeout:10]);
	XCTAssertTrue(menu.isHittable, @"back in the game after cancelling import");
}

+ (BOOL)bits:(NSDictionary *)state has:(unsigned)bit
{
	return ([state[@"seen"] unsignedIntValue] & bit) != 0;
}

@end
