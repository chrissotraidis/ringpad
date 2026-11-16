// RingPad UI tests: real UIKit touches against the iOS shell in the Simulator.
//
// Run with scripts/ios/run-ui-tests.sh. The owner's disc is passed through
// TEST_RUNNER_RINGPAD_GDI_DIR (folder) and TEST_RUNNER_RINGPAD_GDI (file).
// Touch tests read Flycast's port-A values from the RINGPAD_INPUT_DEBUG label.
#import <XCTest/XCTest.h>

// Flycast DreamcastKey bits (core/input/gamepad.h).
static const unsigned kB = 1u << 1, kA = 1u << 2, kStart = 1u << 3, kUp = 1u << 4, kDown = 1u << 5,
		kLeft = 1u << 6, kRight = 1u << 7, kY = 1u << 9, kX = 1u << 10;

@interface RingPadUITests : XCTestCase
@end

@implementation RingPadUITests

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
	app.launchEnvironment = @{ @"RINGPAD_GAME": [self env:@"RINGPAD_GDI"], @"RINGPAD_INPUT_DEBUG": @"1" };
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
	long long frames = 0;
	double guest = 0;
	const BOOL ok = [scanner scanString:@"kcode=" intoString:nil] && [scanner scanHexInt:&kcode]
			&& [scanner scanString:@"seen=" intoString:nil] && [scanner scanHexInt:&seen]
			&& [scanner scanString:@"x=" intoString:nil] && [scanner scanInt:&minX]
			&& [scanner scanString:@".." intoString:nil] && [scanner scanInt:&maxX]
			&& [scanner scanString:@"y=" intoString:nil] && [scanner scanInt:&minY]
			&& [scanner scanString:@".." intoString:nil] && [scanner scanInt:&maxY]
			&& [scanner scanString:@"lt=" intoString:nil] && [scanner scanInt:&lt]
			&& [scanner scanString:@"rt=" intoString:nil] && [scanner scanInt:&rt]
			&& [scanner scanString:@"run=" intoString:nil] && [scanner scanInt:&run]
			&& [scanner scanString:@"frames=" intoString:nil] && [scanner scanLongLong:&frames]
			&& [scanner scanString:@"guest=" intoString:nil] && [scanner scanDouble:&guest];
	XCTAssertTrue(ok, @"unparsed debug label: %@", text);
	return @{ @"guest": @(guest), @"frames": @(frames), @"kcode": @(kcode), @"seen": @(seen), @"minX": @(minX), @"maxX": @(maxX), @"minY": @(minY),
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

- (void)openHelpMenu:(XCUIApplication *)app
{
	[app.buttons[@"dcpad.menu"] tap];
	XCUIElement *help = app.buttons[@"Help"];
	XCTAssertTrue([help waitForExistenceWithTimeout:5]);
	if (!help.isHittable) {
		// Native UIMenu scrolls when the landscape phone cannot fit every row.
		[[[self menuItem:@"Cheats" in:app] coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)] pressForDuration:0.05
				thenDragToCoordinate:[[self menuItem:@"Controls" in:app] coordinateWithNormalizedOffset:CGVectorMake(0.5, 0.5)]];
	}
	XCTAssertTrue(help.isHittable, @"Help is reachable through the native menu");
	[help tap];
}

#pragma mark - Import and lifecycle

- (void)testFreshInstallShowsImportInLandscape
{
	XCUIApplication *app = [[XCUIApplication alloc] init];
	app.launchEnvironment = @{ @"RINGPAD_IGNORE_INSTALLED": @"1" };
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
	app.launchEnvironment = @{ @"RINGPAD_IMPORT": [self env:@"RINGPAD_GDI_DIR"] };
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

- (void)testHelpMenuIsReachable
{
	XCUIApplication *app = [self launchGame];
	[self openHelpMenu:app];
	XCTAssertTrue([[self menuItem:@"About This Build" in:app] waitForExistenceWithTimeout:5]);
	XCTAssertTrue([self menuItem:@"Share Diagnostic Log…" in:app].isHittable);
	[self attachScreenshot:@"help-reachable"];
	[self dismissMenu:app];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"closing Help resumes the game");
}

- (void)testMenuPausesAndOffersControls
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:3];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"game running");
	[app.buttons[@"dcpad.menu"] tap];
	XCTAssertTrue([app.buttons[@"Save State"] waitForExistenceWithTimeout:10]);
	for (NSString *item in @[@"Save State", @"Load State", @"Reset", @"Controls", @"Graphics", @"Sound", @"Cheats", @"Game Files"])
		XCTAssertTrue([self menuItem:item in:app].isHittable, @"top-level item %@ fits on screen", item);
	XCTAssertTrue(app.buttons[@"Help"].exists, @"Help is present; short landscape menus may need scrolling");
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
	XCTAssertTrue([self menuItem:@"Import VMU Save…" in:app].exists);
	[self dismissMenu:app];

	[self openHelpMenu:app];
	XCTAssertTrue([[self menuItem:@"About This Build" in:app] waitForExistenceWithTimeout:5]);
	XCTAssertTrue([self menuItem:@"Share Diagnostic Log…" in:app].exists);
	[self attachScreenshot:@"menu-help"];
	[[self menuItem:@"Share Diagnostic Log…" in:app] tap];
	[NSThread sleepForTimeInterval:2];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 0, @"the share sheet pauses the game");
	[self attachScreenshot:@"share-diagnostics"];
	[self dismissMenu:app];
	[NSThread sleepForTimeInterval:1];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"closing the share sheet resumes");

	[app.buttons[@"dcpad.menu"] tap];
	[app.buttons[@"Game Files"] tap];
	XCTAssertTrue([[self menuItem:@"Import Game…" in:app] waitForExistenceWithTimeout:5]);
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
	[[self menuItem:@"Graphics" in:app] tap];
	XCTAssertTrue([[self menuItem:@"Resolution" in:app] waitForExistenceWithTimeout:5]);
	XCTAssertTrue([self menuItem:@"Dreamcast Original" in:app].exists);
	XCTAssertTrue([self menuItem:@"Texture Upscaling" in:app].exists);
	XCTAssertTrue(app.buttons[@"Show FPS"].exists);
	[self attachScreenshot:@"graphics-menu"];
	[self dismissMenu:app];
}

// Resolution, texture upscaling and the FPS counter apply while the game runs.
- (void)testGraphicsOptionsApply
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:8];
	[self attachScreenshot:@"graphics-default"];
	void (^choose)(NSArray<NSString *> *) = ^(NSArray<NSString *> *path) {
		[app.buttons[@"dcpad.menu"] tap];
		for (NSString *item in path) {
			XCUIElement *e = [self menuItem:item in:app];
			XCTAssertTrue([e waitForExistenceWithTimeout:5], @"%@", item);
			[e tap];
		}
		[NSThread sleepForTimeInterval:2];
	};
	choose(@[@"Graphics", @"Resolution", @"4×"]);
	[self attachScreenshot:@"graphics-4x"];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game keeps running at 4x");
	choose(@[@"Graphics", @"Texture Upscaling", @"2×"]);
	[self attachScreenshot:@"graphics-xbrz"];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game keeps running with xBRZ");
	choose(@[@"Graphics", @"Show FPS"]);
	XCUIElement *fps = app.staticTexts[@"sp.fps"];
	XCTAssertTrue([fps waitForExistenceWithTimeout:3]);
	XCTAssertTrue([fps.label hasSuffix:@"FPS"]);
	XCTAssertLessThan(fps.frame.origin.x, app.frame.size.width * 0.2, @"FPS sits at the left");
	XCTAssertLessThan(fps.frame.origin.y, app.frame.size.height * 0.2, @"FPS sits at the top");
	[self attachScreenshot:@"graphics-fps"];
	choose(@[@"Graphics", @"Show FPS"]);
	XCTAssertFalse(app.staticTexts[@"sp.fps"].exists, @"Show FPS toggles off");
	choose(@[@"Graphics", @"Resolution", @"3×"]);
	choose(@[@"Graphics", @"Dreamcast Original"]);
	[self attachScreenshot:@"graphics-dreamcast"];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game keeps running at the original look");
	void (^checkOriginal)(void) = ^{
		[app.buttons[@"dcpad.menu"] tap];
		[[self menuItem:@"Graphics" in:app] tap];
		XCUIElement *resolution = [self menuItem:@"Resolution" in:app];
		XCTAssertTrue([resolution waitForExistenceWithTimeout:5]);
		XCTAssertTrue([resolution.label containsString:@"Original (480p)"], @"preset restores resolution: %@", resolution.label);
		XCUIElement *textures = [self menuItem:@"Texture Upscaling" in:app];
		XCTAssertTrue([textures.label containsString:@"Off"], @"preset restores original textures: %@", textures.label);
		[self dismissMenu:app];
	};
	checkOriginal();
	[app terminate];
	[app launch];
	XCTAssertTrue([app.buttons[@"dcpad.menu"] waitForExistenceWithTimeout:30]);
	checkOriginal();   // both graphics choices survive a cold app relaunch
}

// Closing the menu must leave the game moving, every time.
// Cheats ▸ toggles a cheat, the menu shows it as on, and the game keeps running.
- (void)testCheatsToggle
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:6];
	[app.buttons[@"dcpad.menu"] tap];
	[[self menuItem:@"Cheats" in:app] tap];
	for (NSString *item in @[@"Max Rings", @"Infinite Lives", @"Unlock All Characters"])
		XCTAssertTrue([[self menuItem:item in:app] waitForExistenceWithTimeout:5], @"%@", item);
	[self attachScreenshot:@"cheats-menu"];
	[[self menuItem:@"Max Rings" in:app] tap];
	[NSThread sleepForTimeInterval:1.5];
	XCTAssertEqual([[self inputState:app][@"run"] intValue], 1, @"the game runs after turning a cheat on");
	[app.buttons[@"dcpad.menu"] tap];
	XCUIElement *cheats = [self menuItem:@"Cheats" in:app];
	XCTAssertTrue([cheats waitForExistenceWithTimeout:5]);
	XCTAssertTrue([cheats.label containsString:@"1 on"], @"Cheats shows how many are on: %@", cheats.label);
	[cheats tap];
	[[self menuItem:@"Max Rings" in:app] tap];   // back off, so other tests start clean
	[NSThread sleepForTimeInterval:1];
}

- (void)testGameKeepsMovingAfterMenu
{
	XCUIApplication *app = [self launchGame];
	[NSThread sleepForTimeInterval:8];
	for (int i = 0; i < 8; i++) {
		[app.buttons[@"dcpad.menu"] tap];
		XCTAssertTrue([app.buttons[@"Save State"] waitForExistenceWithTimeout:5]);
		[NSThread sleepForTimeInterval:0.3 * (i % 3)];
		[self dismissMenu:app];
		const long long before = [[self inputState:app][@"frames"] longLongValue];
		const double guestBefore = [[self inputState:app][@"guest"] doubleValue];
		[NSThread sleepForTimeInterval:1.0];
		NSDictionary *after = [self inputState:app];
		XCTAssertEqual([after[@"run"] intValue], 1, @"running after close %d", i);
		XCTAssertGreaterThan([after[@"frames"] longLongValue], before + 20, @"frames advance after close %d", i);
		XCTAssertGreaterThan([after[@"guest"] doubleValue], guestBefore + 0.5, @"guest time advances after close %d", i);
		XCTAssertFalse([self seen:app] & kStart, @"closing the menu never sends Start");
		if (i == 0)
			[self attachScreenshot:@"after-menu-close"];
	}
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
