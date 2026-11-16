// RingPad game library. See SPGameLibrary.h.
#import "SPGameLibrary.h"

#include "GameImport.h"

#include <string>
#include <vector>

static NSURL *GameDirectory(NSString *name)
{
	NSURL *support = [NSFileManager.defaultManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask].firstObject;
	return [[support URLByAppendingPathComponent:@"RingPad" isDirectory:YES] URLByAppendingPathComponent:name isDirectory:YES];
}

@implementation SPGameLibrary

+ (NSString *)installedGamePath
{
	NSURL *dir = GameDirectory(@"Game");
	NSData *data = [NSData dataWithContentsOfURL:[dir URLByAppendingPathComponent:@"verified.json"]];
	if (data == nil)
		return nil;
	NSDictionary *marker = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
	NSString *gdi = [marker isKindOfClass:NSDictionary.class] ? marker[@"gdi"] : nil;
	NSArray *tracks = marker[@"tracks"];
	if (![gdi isKindOfClass:NSString.class] || ![tracks isKindOfClass:NSArray.class])
		return nil;
	const ringpad::GameIdentity& identity = ringpad::supported_game();
	if (tracks.count != identity.tracks.size())
		return nil;
	for (NSUInteger i = 0; i < tracks.count; i++) {
		NSDictionary *attrs = [NSFileManager.defaultManager attributesOfItemAtPath:[dir URLByAppendingPathComponent:tracks[i]].path error:nil];
		if (attrs == nil || attrs.fileSize != identity.tracks[i].bytes)
			return nil;
	}
	return [dir URLByAppendingPathComponent:gdi].path;
}

+ (void)importFolder:(NSURL *)folder
		progress:(void (^)(double))progress
		completion:(void (^)(NSString *, NSString *))completion
{
	dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), ^{
		const BOOL scoped = [folder startAccessingSecurityScopedResource];
		CFAbsoluteTime lastReport = 0;
		const ringpad::GameCheck check = ringpad::verify_game(folder.path.fileSystemRepresentation, ringpad::supported_game(),
				[&lastReport, progress](uint64_t done, uint64_t total) {
					const CFAbsoluteTime now = CFAbsoluteTimeGetCurrent();
					if (now - lastReport < 0.1 && done < total)
						return;
					lastReport = now;
					const double fraction = total ? double(done) / double(total) : 0;
					// Copy the block into a local strong variable. A block that names
					// the lambda's by-reference capture would keep only a reference to
					// storage that is freed when this import block finishes.
					void (^report)(double) = progress;
					dispatch_async(dispatch_get_main_queue(), ^{ report(fraction); });
				});
		NSString *error = check.ok ? nil : [NSString stringWithUTF8String:check.error.c_str()];
		NSString *installed = nil;
		if (check.ok)
			installed = [self installCheck:check error:&error];
		if (scoped)
			[folder stopAccessingSecurityScopedResource];
		dispatch_async(dispatch_get_main_queue(), ^{ completion(installed, error); });
	});
}

+ (NSString *)installCheck:(const ringpad::GameCheck &)check error:(NSString **)error
{
	NSFileManager *fm = NSFileManager.defaultManager;
	NSURL *staging = GameDirectory(@"Game.importing");
	NSURL *final = GameDirectory(@"Game");
	[fm removeItemAtURL:staging error:nil];
	NSError *err = nil;
	if (![fm createDirectoryAtURL:staging withIntermediateDirectories:YES attributes:nil error:&err]) {
		*error = @"RingPad could not create its game folder.";
		return nil;
	}
	std::vector<std::string> sources = check.track_paths;
	sources.insert(sources.begin(), check.gdi_path);
	NSMutableArray *trackNames = [NSMutableArray array];
	for (size_t i = 0; i < sources.size(); i++) {
		NSURL *source = [NSURL fileURLWithPath:[NSString stringWithUTF8String:sources[i].c_str()]];
		// copyItemAtURL clones on APFS, so importing from the same volume costs no extra space.
		if (![fm copyItemAtURL:source toURL:[staging URLByAppendingPathComponent:source.lastPathComponent] error:&err]) {
			*error = @"Copying the disc into RingPad failed. Check free space and try again.";
			[fm removeItemAtURL:staging error:nil];
			return nil;
		}
		if (i > 0)
			[trackNames addObject:source.lastPathComponent];
	}
	NSString *gdiName = [NSString stringWithUTF8String:check.gdi_path.c_str()].lastPathComponent;
	NSDictionary *marker = @{
		@"release": [NSString stringWithUTF8String:ringpad::supported_game().release],
		@"gdi": gdiName,
		@"gdi_sha256": [NSString stringWithUTF8String:check.gdi_sha256.c_str()],
		@"tracks": trackNames,
		@"verified": [[NSISO8601DateFormatter new] stringFromDate:NSDate.date],
	};
	NSData *json = [NSJSONSerialization dataWithJSONObject:marker options:NSJSONWritingPrettyPrinted error:nil];
	[json writeToURL:[staging URLByAppendingPathComponent:@"verified.json"] atomically:YES];
	[staging setResourceValue:@YES forKey:NSURLIsExcludedFromBackupKey error:nil];
	[fm removeItemAtURL:final error:nil];
	if (![fm moveItemAtURL:staging toURL:final error:&err]) {
		*error = @"RingPad could not finish installing the disc.";
		return nil;
	}
	return [final URLByAppendingPathComponent:gdiName].path;
}

@end
