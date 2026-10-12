// See SPPortMenu.h.
#import "SPPortMenu.h"

#include "types.h"
#include "emulator.h"
#include "cfg/option.h"
#include "oslib/oslib.h"

#include <string>
#include <vector>

bool SPCaptureLastFrameRGB(std::vector<u8>& rgb, int& width, int& height);   // SPFrameCapture.cpp

static constexpr int kSlots = 3;

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
	[host setPaused:NO reason:@"state"];
	[host portMenuShowMessage:ok ? [NSString stringWithFormat:@"Saved to Slot %d", slot] : @"Couldn't save the state"];
}

- (void)loadSlot:(int)slot
{
	UIViewController<SPPortMenuHost> *host = _host;
	[host setPaused:YES reason:@"state"];
	dc_loadstate(slot);
	[host setPaused:NO reason:@"state"];
	[host portMenuShowMessage:[NSString stringWithFormat:@"Loaded Slot %d", slot]];
}

- (UIMenu *)displayMenu
{
	UIAction *fps = [UIAction actionWithTitle:@"Show FPS" image:[UIImage systemImageNamed:@"speedometer"]
			identifier:nil handler:^(UIAction *) {
		config::ShowFPS = !config::ShowFPS;
		SaveSettings();
	}];
	fps.state = config::ShowFPS ? UIMenuElementStateOn : UIMenuElementStateOff;
	struct Res { int lines; NSString *title; };
	const Res res[] = {{480, @"Original (480p)"}, {960, @"2× (960p)"}, {1440, @"3× (1440p)"}};
	NSMutableArray *resolutions = [NSMutableArray array];
	for (const Res& r : res) {
		const int lines = r.lines;
		UIAction *a = [UIAction actionWithTitle:r.title image:nil identifier:nil handler:^(UIAction *) {
			config::RenderResolution = lines;
			SaveSettings();
		}];
		a.state = config::RenderResolution == lines ? UIMenuElementStateOn : UIMenuElementStateOff;
		[resolutions addObject:a];
	}
	UIMenu *resolution = [UIMenu menuWithTitle:@"Resolution" image:[UIImage systemImageNamed:@"square.resize.up"]
			identifier:nil options:UIMenuOptionsSingleSelection children:resolutions];
	return [UIMenu menuWithTitle:@"Display" image:[UIImage systemImageNamed:@"display"] identifier:nil options:0
			children:@[resolution, fps]];
}

- (UIMenu *)soundMenu
{
	struct Level { int volume; NSString *title; NSString *image; };
	const Level levels[] = {{100, @"Full", @"speaker.wave.3"}, {50, @"Half", @"speaker.wave.1"}, {0, @"Off", @"speaker.slash"}};
	NSMutableArray *items = [NSMutableArray array];
	for (const Level& l : levels) {
		const int volume = l.volume;
		UIAction *a = [UIAction actionWithTitle:l.title image:[UIImage systemImageNamed:l.image] identifier:nil handler:^(UIAction *) {
			config::AudioVolume.set(volume);
			config::AudioVolume.calcDbPower();
			SaveSettings();
		}];
		a.state = config::AudioVolume == volume ? UIMenuElementStateOn : UIMenuElementStateOff;
		[items addObject:a];
	}
	return [UIMenu menuWithTitle:@"Sound" image:[UIImage systemImageNamed:@"speaker.wave.2"] identifier:nil
			options:UIMenuOptionsSingleSelection children:items];
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

@end
