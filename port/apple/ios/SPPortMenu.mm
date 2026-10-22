// See SPPortMenu.h.
#import "SPPortMenu.h"
#import "SPDiagnostics.h"
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#include "VMUImage.h"
#include "types.h"
#include "emulator.h"
#include "cheats.h"
#include "cfg/option.h"
#include "oslib/oslib.h"

#include <string>
#include <vector>

bool SPCaptureLastFrameRGB(std::vector<u8>& rgb, int& width, int& height);   // SPFrameCapture.cpp
void SPResetTextureCache();                                                   // SPFrameCapture.cpp

static constexpr int kSlots = 3;
static NSString *const kWidescreenKey = @"SonicPad.widescreen";

@interface SPPortMenu () <UIDocumentPickerDelegate>
@end

// PNG of the last presented frame, 320 pixels wide, stored in the state file
// so the menu can show what each slot holds. Rendering runs on the main
// thread, so reading the frame here is safe while the game is paused.
static std::vector<u8> CaptureThumbnailPNG()
{
	std::vector<u8> rgb;
	int w = 320, h = 0;
	if (!SPCaptureLastFrameRGB(rgb, w, h))
		return {};
	CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
	CFDataRef data = CFDataCreate(nullptr, rgb.data(), CFIndex(size_t(w) * h * 3));
	CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
	CGImageRef image = CGImageCreate(w, h, 8, 24, size_t(w) * 3, space, kCGBitmapByteOrderDefault | kCGImageAlphaNone,
			provider, nullptr, false, kCGRenderingIntentDefault);
	NSData *png = image != nullptr ? UIImagePNGRepresentation([UIImage imageWithCGImage:image]) : nil;
	CGImageRelease(image);
	CGDataProviderRelease(provider);
	CFRelease(data);
	CGColorSpaceRelease(space);
	const u8 *bytes = static_cast<const u8 *>(png.bytes);
	return png != nil ? std::vector<u8>(bytes, bytes + png.length) : std::vector<u8>();
}

// Small rounded thumbnail for a menu row.
static UIImage *SlotThumbnail(int slot)
{
	std::vector<u8> png;
	dc_getStateScreenshot(slot, png);
	UIImage *full = png.empty() ? nil : [UIImage imageWithData:[NSData dataWithBytes:png.data() length:png.size()]];
	if (full == nil)
		return nil;
	const CGSize size = CGSizeMake(48, 36);
	UIGraphicsImageRenderer *r = [[UIGraphicsImageRenderer alloc] initWithSize:size];
	return [[r imageWithActions:^(UIGraphicsImageRendererContext *) {
		[[UIBezierPath bezierPathWithRoundedRect:CGRectMake(0, 0, size.width, size.height) cornerRadius:5] addClip];
		[full drawInRect:CGRectMake(0, 0, size.width, size.height)];
	}] imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
}

@implementation SPPortMenu
{
	__weak UIViewController<SPPortMenuHost> *_host;
}

- (instancetype)initWithHost:(UIViewController<SPPortMenuHost> *)host
{
	if ((self = [super init]))
		_host = host;
	return self;
}

- (NSString *)dateForSlot:(int)slot
{
	const time_t t = dc_getStateCreationDate(slot);
	if (t == 0)
		return nil;
	return [NSDateFormatter localizedStringFromDate:[NSDate dateWithTimeIntervalSince1970:t]
			dateStyle:NSDateFormatterShortStyle timeStyle:NSDateFormatterShortStyle];
}

- (NSArray<UIMenuElement *> *)saveStateElementsForGameLoaded:(BOOL)loaded
{
	__weak SPPortMenu *weakSelf = self;
	NSMutableArray *saves = [NSMutableArray array], *loads = [NSMutableArray array];
	for (int slot = 1; slot <= kSlots; slot++) {
		NSString *date = loaded ? [self dateForSlot:slot] : nil;
		NSString *title = [NSString stringWithFormat:@"Slot %d", slot];
		UIImage *thumb = date != nil ? SlotThumbnail(slot) : nil;
		UIImage *empty = [UIImage systemImageNamed:@"square.dashed"];
		UIAction *save = [UIAction actionWithTitle:title image:thumb ?: empty identifier:nil
				handler:^(UIAction *) { [weakSelf saveSlot:slot]; }];
		save.subtitle = date ?: @"Empty";
		UIAction *load = [UIAction actionWithTitle:title image:thumb ?: empty identifier:nil
				handler:^(UIAction *) { [weakSelf loadSlot:slot]; }];
		load.subtitle = date ?: @"Empty";
		if (!loaded)
			save.attributes = UIMenuElementAttributesDisabled;
		if (date == nil)
			load.attributes = UIMenuElementAttributesDisabled;
		[saves addObject:save];
		[loads addObject:load];
	}
	return @[
		[UIMenu menuWithTitle:@"Save State" image:[UIImage systemImageNamed:@"square.and.arrow.down"] identifier:nil options:0 children:saves],
		[UIMenu menuWithTitle:@"Load State" image:[UIImage systemImageNamed:@"clock.arrow.circlepath"] identifier:nil options:0 children:loads],
	];
}

// The menu may already have resumed the game when an action runs, so each
// action holds its own pause while it touches emulator state.
- (void)saveSlot:(int)slot
{
	UIViewController<SPPortMenuHost> *host = _host;
	[host setPaused:YES reason:@"state"];
	const std::vector<u8> png = CaptureThumbnailPNG();
	dc_savestate(slot, png.empty() ? nullptr : png.data(), u32(png.size()));
	const BOOL ok = dc_getStateCreationDate(slot) != 0;
	SPLog(@"save state slot %d ok=%d", slot, ok);
	[host setPaused:NO reason:@"state"];
	[host portMenuShowMessage:ok ? [NSString stringWithFormat:@"Saved to Slot %d", slot] : @"Couldn't save the state"];
}

- (void)loadSlot:(int)slot
{
	UIViewController<SPPortMenuHost> *host = _host;
	[host setPaused:YES reason:@"state"];
	SPLog(@"load state slot %d", slot);
	dc_loadstate(slot);
	[host setPaused:NO reason:@"state"];
	[host portMenuShowMessage:[NSString stringWithFormat:@"Loaded Slot %d", slot]];
}

// One single-choice submenu whose subtitle shows the current choice.
static UIMenu *ChoiceMenu(NSString *title, NSString *image, NSArray<NSString *> *names, const int *values, int count,
		int current, void (^choose)(int value, NSString *name))
{
	NSMutableArray *items = [NSMutableArray array];
	NSString *currentName = nil;
	for (int i = 0; i < count; i++) {
		const int value = values[i];
		NSString *name = names[i];
		UIAction *a = [UIAction actionWithTitle:name image:nil identifier:nil handler:^(UIAction *) { choose(value, name); }];
		a.state = current == value ? UIMenuElementStateOn : UIMenuElementStateOff;
		if (current == value)
			currentName = name;
		[items addObject:a];
	}
	UIMenu *menu = [UIMenu menuWithTitle:title image:[UIImage systemImageNamed:image] identifier:nil
			options:UIMenuOptionsSingleSelection children:items];
	menu.subtitle = currentName;
	return menu;
}

- (UIMenu *)graphicsMenu
{
	__weak UIViewController<SPPortMenuHost> *host = _host;
	// Internal render height. 4× (1920 lines) is about native on a 12.9-inch iPad.
	const int lines[] = {480, 960, 1440, 1920};
	UIMenu *resolution = ChoiceMenu(@"Resolution", @"square.resize.up",
			@[@"Original (480p)", @"2× (960p)", @"3× (1440p)", @"4× (1920p)"], lines, 4, config::RenderResolution,
			^(int value, NSString *name) {
		config::RenderResolution = value;
		SaveSettings();
		SPLog(@"setting resolution=%d", value);
		[host portMenuShowMessage:[NSString stringWithFormat:@"Resolution: %@", name]];
	});
	// xBRZ redraws each texture at a higher resolution with smoothed edges.
	const int factors[] = {1, 2, 4};
	UIMenu *textures = ChoiceMenu(@"Texture Upscaling", @"wand.and.stars", @[@"Off", @"2× (xBRZ)", @"4× (xBRZ)"],
			factors, 3, config::TextureUpscale, ^(int value, NSString *name) {
		config::TextureUpscale = value;
		SaveSettings();
		SPResetTextureCache();
		SPLog(@"setting texture_upscale=%d", value);
		[host portMenuShowMessage:[NSString stringWithFormat:@"Texture Upscaling: %@", name]];
	});
	NSMutableArray *children = [NSMutableArray arrayWithObjects:resolution, textures, nil];
	if (SPPortMenu.widescreenAvailable) {
		const BOOL wide = config::WidescreenGameHacks;
		UIAction *widescreen = [UIAction actionWithTitle:@"Widescreen (16:9)" image:[UIImage systemImageNamed:@"rectangle.ratio.16.to.9"]
				identifier:nil handler:^(UIAction *) { [self setWidescreen:!wide]; }];
		widescreen.subtitle = @"Wider view instead of side bars";
		widescreen.state = wide ? UIMenuElementStateOn : UIMenuElementStateOff;
		[children addObject:widescreen];
	}
	if ([host respondsToSelector:@selector(portMenuSetFPSVisible:)]) {
		const BOOL visible = [host portMenuFPSVisible];
		UIAction *fps = [UIAction actionWithTitle:@"Show FPS" image:[UIImage systemImageNamed:@"speedometer"]
				identifier:nil handler:^(UIAction *) {
			[host portMenuSetFPSVisible:!visible];
			SPLog(@"setting show_fps=%d", !visible);
		}];
		fps.state = visible ? UIMenuElementStateOn : UIMenuElementStateOff;
		[children addObject:fps];
	}
	return [UIMenu menuWithTitle:@"Graphics" image:[UIImage systemImageNamed:@"sparkles.tv"] identifier:nil options:0
			children:children];
}

#pragma mark Widescreen

+ (BOOL)widescreenAvailable
{
	return UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPhone;
}

// Flycast's built-in widescreen patch for this disc widens the game's camera,
// and the picture is stretched from 4:3 to 16:9 to match. On by default on
// phones; never on a tablet, whose 4:3 screen already fits the game.
+ (void)applyWidescreenPreference
{
	NSNumber *saved = [NSUserDefaults.standardUserDefaults objectForKey:kWidescreenKey];
	config::WidescreenGameHacks = self.widescreenAvailable && (saved == nil || saved.boolValue);
	// Saved to emu.cfg: Flycast reloads its settings from that file when the game loads.
	SaveSettings();
	SPLog(@"setting widescreen=%d", (int)config::WidescreenGameHacks);
}

- (void)setWidescreen:(BOOL)on
{
	UIViewController<SPPortMenuHost> *host = _host;
	[NSUserDefaults.standardUserDefaults setBool:on forKey:kWidescreenKey];
	config::WidescreenGameHacks = on;
	SaveSettings();
	SPLog(@"setting widescreen=%d", on);
	if (settings.content.path.empty())
		return;
	if (on) {
		// The patch can be applied to the running game.
		[host setPaused:YES reason:@"widescreen"];
		cheatManager.reset(settings.content.gameId);
		if (cheatManager.isWidescreen())
			config::ScreenStretching.override(134);   // 4:3 -> 16:9
		[host setPaused:NO reason:@"widescreen"];
		[host portMenuShowMessage:@"Widescreen on"];
		return;
	}
	// The patched camera stays in game memory until the game reloads.
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Turn Off Widescreen?"
			message:@"The game restarts to go back to 4:3. Progress since your last in-game save is lost."
			preferredStyle:UIAlertControllerStyleAlert];
	[alert addAction:[UIAlertAction actionWithTitle:@"Later" style:UIAlertActionStyleCancel handler:^(UIAlertAction *) {
		[host portMenuShowMessage:@"Widescreen turns off next launch"];
	}]];
	[alert addAction:[UIAlertAction actionWithTitle:@"Restart Now" style:UIAlertActionStyleDestructive handler:^(UIAlertAction *) {
		config::ScreenStretching.reset();
		[host portMenuRestartGame];
	}]];
	[host presentViewController:alert animated:YES completion:nil];
}

- (UIMenu *)soundMenu
{
	struct Level { int volume; NSString *title; NSString *image; };
	const Level levels[] = {{100, @"Full", @"speaker.wave.3"}, {50, @"Half", @"speaker.wave.1"}, {0, @"Off", @"speaker.slash"}};
	NSMutableArray *items = [NSMutableArray array];
	NSString *current = nil;
	for (const Level& l : levels) {
		const int volume = l.volume;
		UIAction *a = [UIAction actionWithTitle:l.title image:[UIImage systemImageNamed:l.image] identifier:nil handler:^(UIAction *) {
			config::AudioVolume.set(volume);
			config::AudioVolume.calcDbPower();
			SaveSettings();
			SPLog(@"setting volume=%d", volume);
		}];
		a.state = config::AudioVolume == volume ? UIMenuElementStateOn : UIMenuElementStateOff;
		if (config::AudioVolume == volume)
			current = l.title;
		[items addObject:a];
	}
	UIMenu *sound = [UIMenu menuWithTitle:@"Sound" image:[UIImage systemImageNamed:@"speaker.wave.2"] identifier:nil
			options:UIMenuOptionsSingleSelection children:items];
	sound.subtitle = current;
	return sound;
}

- (UIAction *)exportVMUActionForGameLoaded:(BOOL)loaded
{
	__weak SPPortMenu *weakSelf = self;
	UIAction *a = [UIAction actionWithTitle:@"Export VMU Save…" image:[UIImage systemImageNamed:@"square.and.arrow.up"]
			identifier:nil handler:^(UIAction *) { [weakSelf exportVMU]; }];
	a.subtitle = @"Back up your in-game save";
	if (!loaded)
		a.attributes = UIMenuElementAttributesDisabled;
	return a;
}

- (void)exportVMU
{
	UIViewController<SPPortMenuHost> *host = _host;
	const std::string path = hostfs::getVmuPath("A1", false);
	NSString *file = [NSString stringWithUTF8String:path.c_str()];
	if (path.empty() || ![NSFileManager.defaultManager fileExistsAtPath:file]) {
		[host portMenuShowMessage:@"No VMU save yet"];
		return;
	}
	// Share a copy so the live file is never held open by another app.
	NSURL *copy = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:file.lastPathComponent]];
	[NSFileManager.defaultManager removeItemAtURL:copy error:nil];
	[NSFileManager.defaultManager copyItemAtURL:[NSURL fileURLWithPath:file] toURL:copy error:nil];
	[host setPaused:YES reason:@"export"];
	UIActivityViewController *share = [[UIActivityViewController alloc] initWithActivityItems:@[copy] applicationActivities:nil];
	share.popoverPresentationController.sourceView = host.view;
	share.popoverPresentationController.sourceRect = CGRectMake(CGRectGetMaxX(host.view.bounds) - 60, 40, 1, 1);
	share.completionWithItemsHandler = ^(UIActivityType, BOOL, NSArray *, NSError *) {
		[host setPaused:NO reason:@"export"];
	};
	[host presentViewController:share animated:YES completion:nil];
}

#pragma mark VMU import

- (UIAction *)importVMUActionForGameLoaded:(BOOL)loaded
{
	__weak SPPortMenu *weakSelf = self;
	UIAction *a = [UIAction actionWithTitle:@"Import VMU Save…" image:[UIImage systemImageNamed:@"square.and.arrow.down.on.square"]
			identifier:nil handler:^(UIAction *) { [weakSelf chooseVMU]; }];
	a.subtitle = @"A .VMS save with its .VMI, or a whole card";
	if (!loaded)
		a.attributes = UIMenuElementAttributesDisabled;
	return a;
}

- (void)chooseVMU
{
	UIViewController<SPPortMenuHost> *host = _host;
	[host setPaused:YES reason:@"vmu-import"];
	UIDocumentPickerViewController *picker = [[UIDocumentPickerViewController alloc]
			initForOpeningContentTypes:@[UTTypeData] asCopy:YES];
	picker.delegate = self;
	picker.allowsMultipleSelection = YES;   // a .VMS and its .VMI together
	[host presentViewController:picker animated:YES completion:nil];
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller
{
	[_host setPaused:NO reason:@"vmu-import"];
}

static std::vector<uint8_t> Bytes(NSData *data)
{
	const uint8_t *p = static_cast<const uint8_t *>(data.bytes);
	return std::vector<uint8_t>(p, p + data.length);
}

static NSData *ReadCurrentCard()
{
	return [NSData dataWithContentsOfFile:[NSString stringWithUTF8String:hostfs::getVmuPath("A1", true).c_str()]];
}

// Builds the new port A card from what was picked: a whole 128 KB card, or a
// .VMS save plus its .VMI, written into a copy of the current card so other
// games' saves stay. Returns nil and sets *problem when the pick is unusable.
static NSData *CardFromPick(NSArray<NSURL *> *urls, NSString **summary, NSString **problem)
{
	NSData *vms = nil, *vmi = nil, *card = nil;
	NSString *vmsName = nil;
	for (NSURL *url in urls) {
		NSData *data = [NSData dataWithContentsOfURL:url];
		NSString *ext = url.pathExtension.lowercaseString;
		if (data.length == sonicpad::kVMUImageBytes)
			card = data;
		else if ([ext isEqualToString:@"vmi"])
			vmi = data;
		else if ([ext isEqualToString:@"vms"]) {
			vms = data;
			vmsName = url.lastPathComponent;
		}
	}
	if (card != nil) {
		const std::string why = sonicpad::vmu_image_problem(Bytes(card));
		if (!why.empty()) {
			*problem = [NSString stringWithFormat:@"This file is 128 KB but %s.", why.c_str()];
			return nil;
		}
		*summary = @"the whole memory card";
		return card;
	}
	if (vms == nil || vmi == nil) {
		*problem = @"Choose a memory card image (a 128 KB .bin), or a save's .VMS file together with its .VMI file. "
				@"Select both in Files before tapping Open.";
		return nil;
	}
	sonicpad::VMUFile file = sonicpad::vmu_file_from_vmi(Bytes(vmi));
	if (file.name.empty()) {
		*problem = @"The .VMI file is not a VMU file description.";
		return nil;
	}
	file.data = Bytes(vms);
	NSData *current = ReadCurrentCard();
	std::vector<uint8_t> image = current != nil ? Bytes(current) : std::vector<uint8_t>();
	if (!sonicpad::vmu_image_problem(image).empty()) {
		*problem = @"SonicPad's memory card is not ready yet. Start the game once, then import the save.";
		return nil;
	}
	NSDateComponents *c = [NSCalendar.currentCalendar components:NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay
			| NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond | NSCalendarUnitWeekday fromDate:NSDate.date];
	auto bcd = [](long v) { return uint8_t(((v / 10) % 10) << 4 | (v % 10)); };
	const uint8_t when[8] = {bcd(c.year / 100), bcd(c.year % 100), bcd(c.month), bcd(c.day), bcd(c.hour), bcd(c.minute),
			bcd(c.second), uint8_t((c.weekday + 5) % 7)};
	const std::string why = sonicpad::vmu_write_file(image, file, when);
	if (!why.empty()) {
		*problem = [NSString stringWithFormat:@"The save could not be added: %s.", why.c_str()];
		return nil;
	}
	*summary = [NSString stringWithFormat:@"%@ (%s)", vmsName, file.name.c_str()];
	return [NSData dataWithBytes:image.data() length:image.size()];
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls
{
	[self importVMUFromURLs:urls confirm:YES];
}

- (void)importVMUFromURLs:(NSArray<NSURL *> *)urls
{
	[_host setPaused:YES reason:@"vmu-import"];
	[self importVMUFromURLs:urls confirm:NO];
}

- (void)importVMUFromURLs:(NSArray<NSURL *> *)urls confirm:(BOOL)confirm
{
	UIViewController<SPPortMenuHost> *host = _host;
	NSString *summary = nil, *problem = nil;
	NSData *data = CardFromPick(urls, &summary, &problem);
	NSString *names = [[urls valueForKey:@"lastPathComponent"] componentsJoinedByString:@", "];
	if (data == nil) {
		SPLog(@"vmu import rejected %@: %@", names, problem);
		UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Can't Import This Save" message:problem
				preferredStyle:UIAlertControllerStyleAlert];
		[alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleCancel handler:^(UIAlertAction *) {
			[host setPaused:NO reason:@"vmu-import"];
		}]];
		[host presentViewController:alert animated:YES completion:nil];
		return;
	}
	if (!confirm) {
		[self installVMU:data named:names];
		return;
	}
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Import This Save?"
			message:[NSString stringWithFormat:@"%@ goes onto the memory card in port A, replacing a Sonic Adventure save "
					@"already there, and the game restarts. Your current card is kept as a backup in SonicPad's Documents folder.",
					summary]
			preferredStyle:UIAlertControllerStyleAlert];
	[alert addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *) {
		[host setPaused:NO reason:@"vmu-import"];
	}]];
	[alert addAction:[UIAlertAction actionWithTitle:@"Replace and Restart" style:UIAlertActionStyleDestructive handler:^(UIAlertAction *) {
		[self installVMU:data named:names];
	}]];
	[host presentViewController:alert animated:YES completion:nil];
}

- (void)installVMU:(NSData *)data named:(NSString *)name
{
	UIViewController<SPPortMenuHost> *host = _host;
	NSFileManager *fm = NSFileManager.defaultManager;
	NSString *live = [NSString stringWithUTF8String:hostfs::getVmuPath("A1", true).c_str()];
	NSString *backup = nil;
	if ([fm fileExistsAtPath:live]) {
		NSDateFormatter *f = [[NSDateFormatter alloc] init];
		f.dateFormat = @"yyyyMMdd-HHmmss";
		backup = [[live stringByDeletingPathExtension] stringByAppendingFormat:@".backup-%@.bin", [f stringFromDate:NSDate.date]];
		if (![fm copyItemAtPath:live toPath:backup error:nil]) {
			SPLog(@"vmu import: backup failed, card unchanged");
			[host setPaused:NO reason:@"vmu-import"];
			[host portMenuShowMessage:@"Couldn't back up the current card"];
			return;
		}
	}
	// Write beside the card, then swap it in, so a failed write leaves the old card.
	NSString *staged = [live stringByAppendingString:@".importing"];
	NSError *error = nil;
	BOOL ok = [data writeToFile:staged options:NSDataWritingAtomic error:&error];
	if (ok && backup != nil)
		ok = [fm replaceItemAtURL:[NSURL fileURLWithPath:live] withItemAtURL:[NSURL fileURLWithPath:staged]
				backupItemName:nil options:0 resultingItemURL:nil error:&error];
	else if (ok)
		ok = [fm moveItemAtPath:staged toPath:live error:&error];
	if (!ok) {
		SPLog(@"vmu import failed: %@", error.localizedDescription);
		[fm removeItemAtPath:staged error:nil];
		[host setPaused:NO reason:@"vmu-import"];
		[host portMenuShowMessage:@"Couldn't import the memory card"];
		return;
	}
	SPLog(@"vmu import %@ installed, backup %@", name, backup.lastPathComponent ?: @"none");
	[host portMenuRestartGame];
	[host portMenuShowMessage:@"Memory card imported"];
}

@end
