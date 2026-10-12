// SonicPad UI tests: real UIKit touches against the iOS shell in the Simulator.
//
// Run with scripts/ios/run-ui-tests.sh. The owner's disc is passed through
// TEST_RUNNER_SONICPAD_GDI_DIR (folder) and TEST_RUNNER_SONICPAD_GDI (file).
// Touch tests read Flycast's port-A values from the SONICPAD_INPUT_DEBUG label.
#import <XCTest/XCTest.h>

// Flycast DreamcastKey bits (core/input/gamepad.h).
static const unsigned kB = 1u << 1, kA = 1u << 2, kStart = 1u << 3, kUp = 1u << 4, kDown = 1u << 5,
		kLeft = 1u << 6, kRight = 1u << 7, kY = 1u << 9, kX = 1u << 10;

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

- (void)attachScreenshot:(NSString *)name
{
	XCTAttachment *shot = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
	shot.name = name;
	shot.lifetime = XCTAttachmentLifetimeKeepAlways;
	[self addAttachment:shot];
}

// Launch with the disc, the input debug label, and a fresh touch layout.
- (XCUIApplication *)launchGame
{
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"SONICPAD_GAME": [self env:@"SONICPAD_GDI"], @"SONICPAD_INPUT_DEBUG": @"1" };
	app.launchArguments = @[@"-DCPad.SonicAdventure.tablet", @"{}", @"-DCPad.SonicAdventure.phone", @"{}"];
	[app launch];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:30]);
	XCTAssertTrue([app.staticTexts[@"sp.inputDebug"] waitForExistenceWithTimeout:10]);
	return app;
}

- (XCUIElement *)control:(NSString *)name in:(XCUIApplication *)app
{
	NSString *ident = [@"dcpad." stringByAppendingString:name];
	XCUIElement *element = app.buttons[ident];
	if (!element.exists)
		element = app.otherElements[ident];
	XCTAssertTrue(element.exists, @"control %@", name);
	return element;
}

- (NSDictionary<NSString *, NSNumber *> *)inputState:(XCUIApplication *)app
{
	NSString *text = app.staticTexts[@"sp.inputDebug"].label;
	NSScanner *scanner = [NSScanner scannerWithString:text];
	unsigned kcode = 0, seen = 0;
	int minX = 0, maxX = 0, minY = 0, maxY = 0, lt = 0, rt = 0, run = 0;
	const BOOL ok = [scanner scanString:@"kcode=" intoString:nil] && [scanner scanHexInt:&kcode]
			&& [scanner scanString:@"seen=" intoString:nil] && [scanner scanHexInt:&seen]
			&& [scanner scanString:@"x=" intoString:nil] && [scanner scanInt:&minX]
			&& [scanner scanString:@".." intoString:nil] && [scanner scanInt:&maxX]
			&& [scanner scanString:@"y=" intoString:nil] && [scanner scanInt:&minY]
			&& [scanner scanString:@".." intoString:nil] && [scanner scanInt:&maxY]
			&& [scanner scanString:@"lt=" intoString:nil] && [scanner scanInt:&lt]
			&& [scanner scanString:@"rt=" intoString:nil] && [scanner scanInt:&rt]
			&& [scanner scanString:@"run=" intoString:nil] && [scanner scanInt:&run];
	XCTAssertTrue(ok, @"unparsed debug label: %@", text);
	return @{ @"kcode": @(kcode), @"seen": @(seen), @"minX": @(minX), @"maxX": @(maxX), @"minY": @(minY),
			@"maxY": @(maxY), @"lt": @(lt), @"rt": @(rt), @"run": @(run) };
}

- (unsigned)seen:(XCUIApplication *)app
{
	return [[self inputState:app][@"seen"] unsignedIntValue];
}

- (void)openTouchSettings:(XCUIApplication *)app
{
	[app.buttons[@"dcpad.menu"] tap];
	XCTAssertTrue([app.buttons[@"Controls"] waitForExistenceWithTimeout:5]);
	[app.buttons[@"Controls"] tap];
	XCTAssertTrue([app.buttons[@"Touch Control Settings…"] waitForExistenceWithTimeout:5]);
	[app.buttons[@"Touch Control Settings…"] tap];
}

// Menu items with a subtitle expose "Title, subtitle" as their label.
- (XCUIElement *)menuItem:(NSString *)title in:(XCUIApplication *)app
{
	return [app.buttons matchingPredicate:[NSPredicate predicateWithFormat:@"label BEGINSWITH %@", title]].firstMatch;
}

- (void)dismissMenu:(XCUIApplication *)app
{
	[[app coordinateWithNormalizedOffset:CGVectorMake(0.45, 0.5)] tap];
	[NSThread sleepForTimeInterval:0.8];
}

#pragma mark - Import and lifecycle

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
	XCTAssertFalse(app.buttons[@"dcpad.menu"].isHittable, @"no game controls on the import screen");
	[self attachScreenshot:@"import-screen"];

	[choose tap];
	XCUIElement *pickerCancel = app.buttons[@"Cancel"];
	XCTAssertTrue([pickerCancel waitForExistenceWithTimeout:20], @"folder picker appears");
	[pickerCancel tap];
	XCTAssertTrue([choose waitForExistenceWithTimeout:10]);
}

- (void)testImportVerifiesDiscAndStartsGame
{
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"SONICPAD_IMPORT": [self env:@"SONICPAD_GDI_DIR"] };
	[app launch];
	XCUIElement *menu = app.buttons[@"dcpad.menu"];
	XCTAssertTrue([menu waitForExistenceWithTimeout:120], @"import finished and the game view is shown");
	XCTAssertTrue(menu.isHittable);
	XCTAssertFalse(app.buttons[@"sp.import.choose"].exists);
	[NSThread sleepForTimeInterval:10];
	[self attachScreenshot:@"after-import"];

	[app terminate];
	app.launchEnvironment = @{};
	[app launch];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:30], @"installed game boots on relaunch");
	XCTAssertFalse(app.buttons[@"sp.import.choose"].exists);
}

#pragma mark - Touch controls

- (void)testFaceButtonsAndStart
{
	XCUIApplication *app = [self launchGame];
	XCTAssertEqual([self seen:app], 0u, @"no input before any touch");
	[[self control:@"A" in:app] tap];
	[[self control:@"X" in:app] tap];
	[[self control:@"start" in:app] tap];
	unsigned seen = [self seen:app];
	XCTAssertTrue(seen & kA, @"A");
	XCTAssertTrue(seen & kX, @"X");
	XCTAssertTrue(seen & kStart, @"Start");
	XCTAssertFalse(seen & (kB | kY), @"B and Y untouched");
	XCTAssertEqual([[self inputState:app][@"kcode"] unsignedIntValue], 0xFFFFFFFFu, @"everything released");

	// Slide from A onto B without lifting: both register, as on a real pad.
	XCUIElement *a = [self control:@"A" in:app], *b = [self control:@"B" in:app];
	[[a coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)] pressForDuration:0.2
			thenDragToCoordinate:[b coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)]];
	seen = [self seen:app];
	XCTAssertTrue(seen & kB, @"sliding from A reaches B");
	XCTAssertFalse(seen & kY, @"the slide never touched Y");

	// Between A and B (the thumb's natural resting spot) presses both.
	const CGRect af = a.frame, bf = b.frame;
	XCUICoordinate *origin = [app coordinateWithNormalizedOffset:CGVectorMake(0, 0)];
	XCUICoordinate *between = [origin coordinateWithOffset:CGVectorMake((CGRectGetMidX(af) + CGRectGetMidX(bf)) / 2,
			(CGRectGetMidY(af) + CGRectGetMidY(bf)) / 2)];
	[between tap];
	XCTAssertTrue([self seen:app] & kY ? NO : YES, @"the A/B gap never fires Y");
}

- (void)testDPadEightWay
{
	XCUIElement *dpad = nil;
	XCUIApplication *app = [self launchGame];
	dpad = [self control:@"dpad" in:app];
	[[dpad coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.12)] tap];
	XCTAssertTrue([self seen:app] & kUp, @"up");
	XCTAssertFalse([self seen:app] & (kLeft | kRight), @"pure up is not a diagonal");

	// Roll from right to down-right to down without lifting.
	[[dpad coordinateWithNormalizedOffset:CGVectorMake(0.88, 0.5)] pressForDuration:0.2
			thenDragToCoordinate:[dpad coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.88)]];
	const unsigned seen = [self seen:app];
	XCTAssertTrue(seen & kRight, @"right");
	XCTAssertTrue(seen & kDown, @"down after rolling the thumb");
	XCTAssertEqual([[self inputState:app][@"kcode"] unsignedIntValue], 0xFFFFFFFFu, @"D-pad released");
}

- (void)testAnalogStickFixedAndFloating
{
	XCUIApplication *app = [self launchGame];
	XCUIElement *stick = [self control:@"stick" in:app];
	XCUICoordinate *center = [stick coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)];
	// Hold at the end so the display link samples the final position.
	[center pressForDuration:0.2 thenDragToCoordinate:[stick coordinateWithNormalizedOffset:CGVectorMake(0.95, 0.5)]
			withVelocity:XCUIGestureVelocityDefault thenHoldForDuration:0.5];
	NSDictionary *s = [self inputState:app];
	XCTAssertGreaterThan([s[@"maxX"] intValue], 30000, @"full right from the stick");
	XCTAssertFalse([s[@"seen"] unsignedIntValue] & (kLeft | kRight | kUp | kDown), @"the stick never fires the D-pad");

	// Floating: start a touch in empty space above-right of the stick and push up.
	const CGRect f = stick.frame;
	XCUICoordinate *origin = [app coordinateWithNormalizedOffset:CGVectorMake(0, 0)];
	XCUICoordinate *start = [origin coordinateWithOffset:CGVectorMake(CGRectGetMaxX(f) + f.size.width * 0.3, CGRectGetMinY(f) + 10)];
	XCUICoordinate *end = [start coordinateWithOffset:CGVectorMake(0, -f.size.height * 0.45)];
	[start pressForDuration:0.2 thenDragToCoordinate:end withVelocity:XCUIGestureVelocityDefault thenHoldForDuration:0.5];
	s = [self inputState:app];
	XCTAssertLessThan([s[@"minY"] intValue], -25000, @"the floating stick pushed up");
	XCTAssertEqual([s[@"kcode"] unsignedIntValue], 0xFFFFFFFFu, @"nothing held afterwards");
}

- (void)testAnalogTriggers
{
	XCUIApplication *app = [self launchGame];
	XCUIElement *r = [self control:@"R" in:app];
	[[r coordinateWithNormalizedOffset:CGVectorMake(0.15, 0.5)] pressForDuration:0.4];
	NSDictionary *s = [self inputState:app];
	XCTAssertGreaterThan([s[@"rt"] intValue], 25000, @"a light touch on R is past the digital threshold");
	XCTAssertLessThan([s[@"rt"] intValue], 60000, @"a light touch is not a full press");
	[[r coordinateWithNormalizedOffset:CGVectorMake(0.95, 0.5)] pressForDuration:0.4];
	XCTAssertEqual([[self inputState:app][@"rt"] intValue], 65535, @"the outer edge is a full press");
	XCUIElement *l = [self control:@"L" in:app];
	[[l coordinateWithNormalizedOffset:CGVectorMake(0.05, 0.5)] pressForDuration:0.4];
	XCTAssertEqual([[self inputState:app][@"lt"] intValue], 65535, @"L full press at its outer (left) edge");
}

#pragma mark - Menu and settings

- (void)testMenuPausesAndOffersControls
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:3];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"game running");
	[app.buttons[@"dcpad.menu"] tap];
	XCTAssertTrue([app.buttons[@"Save State"] waitForExistenceWithTimeout:10]);
	for (NSString *item in @[@"Save State", @"Load State", @"Reset", @"Display", @"Sound", @"Game Files", @"About This Build"])
		XCTAssertTrue(app.buttons[item].isHittable, @"top-level item %@ fits on screen", item);
	XCTAssertTrue(app.buttons[@"Controls"].isHittable, @"Controls submenu fits on screen");
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 0, @"the menu pauses the game");
	[self attachScreenshot:@"menu"];
	[app.buttons[@"Controls"] tap];
	XCTAssertTrue([app.buttons[@"Touch Control Settings…"] waitForExistenceWithTimeout:5]);
	XCTAssertTrue(app.buttons[@"Move Controls"].isHittable);
	XCTAssertTrue(app.buttons[@"Controller Mapping…"].isHittable);
	[self attachScreenshot:@"menu-controls"];
	[self dismissMenu:app];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"closing the menu resumes");
	XCTAssertFalse([self seen:app] & kStart, @"the menu never sends Start");

	[app.buttons[@"dcpad.menu"] tap];
	[app.buttons[@"Game Files"] tap];
	XCTAssertTrue([[self menuItem:@"Import Game…" in:app] waitForExistenceWithTimeout:5]);
	XCTAssertTrue([self menuItem:@"Export VMU Save…" in:app].exists);
	[[self menuItem:@"Import Game…" in:app] tap];
	XCUIElement *cancel = app.buttons[@"sp.import.cancel"];
	XCTAssertTrue([cancel waitForExistenceWithTimeout:10]);
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 0, @"import pauses the game");
	[cancel tap];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:10]);
	[NSThread sleepForTimeInterval:1];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"cancelling import resumes");
}

// Save states from the ••• menu: save to slot 1, see its date under Load
// State, load it, and the game keeps running afterwards.
- (void)testSaveAndLoadState
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:6];
	[app.buttons[@"dcpad.menu"] tap];
	XCTAssertTrue([app.buttons[@"Save State"] waitForExistenceWithTimeout:10]);
	[self attachScreenshot:@"menu-top"];
	[app.buttons[@"Save State"] tap];
	XCUIElement *slot = [self menuItem:@"Slot 1" in:app];
	XCTAssertTrue([slot waitForExistenceWithTimeout:5]);
	[slot tap];
	[NSThread sleepForTimeInterval:0.5];
	[self attachScreenshot:@"saved"];   // shows the "Saved to Slot 1" confirmation
	[NSThread sleepForTimeInterval:2.5];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game resumes after saving");

	[app.buttons[@"dcpad.menu"] tap];
	[app.buttons[@"Load State"] tap];
	XCUIElement *load = [self menuItem:@"Slot 1" in:app];
	XCTAssertTrue([load waitForExistenceWithTimeout:5]);
	XCTAssertTrue(load.isEnabled, @"slot 1 can be loaded once saved");
	XCTAssertFalse([load.label containsString:@"Empty"], @"slot 1 shows when it was saved: %@", load.label);
	[self attachScreenshot:@"load-menu"];
	[load tap];
	[NSThread sleepForTimeInterval:0.5];
	[self attachScreenshot:@"loaded"];
	[NSThread sleepForTimeInterval:2.5];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game runs after loading");

	[app.buttons[@"dcpad.menu"] tap];
	[app.buttons[@"Display"] tap];
	XCTAssertTrue([app.buttons[@"Resolution"] waitForExistenceWithTimeout:5]);
	XCTAssertTrue(app.buttons[@"Show FPS"].exists);
	[self attachScreenshot:@"display-menu"];
	[self dismissMenu:app];
}

// Render resolution applies while the game runs, then returns to the default.
- (void)testDisplayOptionsApply
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:8];
	[self attachScreenshot:@"display-default"];
	void (^choose)(NSArray<NSString *> *) = ^(NSArray<NSString *> *path) {
		[app.buttons[@"dcpad.menu"] tap];
		for (NSString *item in path) {
			XCUIElement *e = [self menuItem:item in:app];
			XCTAssertTrue([e waitForExistenceWithTimeout:5], @"%@", item);
			[e tap];
		}
		[NSThread sleepForTimeInterval:2];
	};
	choose(@[@"Display", @"Resolution", @"2×"]);
	[self attachScreenshot:@"display-2x"];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game keeps running at 2x");
	choose(@[@"Display", @"Resolution", @"Original"]);
}

- (void)testSettingsAndMoveControlsPersist
{
	XCUIApplication *app = [self launchGame];
	XCUIElement *dpad = [self control:@"dpad" in:app];
	const CGRect before = dpad.frame;

	[self openTouchSettings:app];
	XCUIElement *floating = app.switches[@"dcpad.settings.floating"];
	XCTAssertTrue([floating waitForExistenceWithTimeout:5]);
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 0, @"settings pause the game");
	[floating tap];
	[app.sliders[@"dcpad.settings.size"] adjustToNormalizedSliderPosition:0.95];
	[self attachScreenshot:@"settings"];
	[app.buttons[@"dcpad.settings.move"] tap];

	XCTAssertTrue([app.buttons[@"dcpad.edit.done"] waitForExistenceWithTimeout:5], @"layout editor open");
	dpad = [self control:@"dpad" in:app];
	const CGRect grown = dpad.frame;
	XCTAssertGreaterThan(grown.size.width, before.size.width * 1.2, @"size slider grew the controls");
	[[dpad coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)] pressForDuration:0.3
			thenDragToCoordinate:[app coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.3)]];
	[self attachScreenshot:@"move-controls"];
	[app.buttons[@"dcpad.edit.done"] tap];
	const CGRect moved = [self control:@"dpad" in:app].frame;
	// Dragged from its centre, the D-pad lands where the finger stopped.
	XCTAssertEqualWithAccuracy(CGRectGetMidX(moved), CGRectGetWidth(app.frame) * 0.5, 30, @"D-pad follows the drag");
	XCTAssertGreaterThan(CGRectGetMidX(moved) - CGRectGetMidX(grown), 50, @"D-pad moved right");
	[NSThread sleepForTimeInterval:1];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"game resumes after editing");

	// Relaunch without resetting defaults: the layout and settings stay.
	[app terminate];
	app.launchArguments = @[];
	[app launch];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:30]);
	const CGRect after = [self control:@"dpad" in:app].frame;
	XCTAssertEqualWithAccuracy(CGRectGetMidX(after), CGRectGetMidX(moved), 3, @"moved D-pad persists");
	XCTAssertEqualWithAccuracy(after.size.width, moved.size.width, 3, @"size persists");
	[self openTouchSettings:app];
	XCTAssertTrue([app.switches[@"dcpad.settings.floating"] waitForExistenceWithTimeout:5]);
	XCTAssertEqualObjects(app.switches[@"dcpad.settings.floating"].value, @"0", @"floating stick setting persists");

	// Reset brings the defaults back.
	[app.buttons[@"dcpad.settings.reset"] tap];
	[app.alerts.buttons[@"Reset"] tap];
	[app.buttons[@"dcpad.settings.close"] tap];
	const CGRect reset = [self control:@"dpad" in:app].frame;
	XCTAssertEqualWithAccuracy(CGRectGetMidX(reset), CGRectGetMidX(before), 3, @"reset restores the default position");
	XCTAssertEqualWithAccuracy(reset.size.width, before.size.width, 3, @"reset restores the default size");
}

@end
