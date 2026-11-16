// RingPad game library: verified import and lookup of the owner's disc.
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface SPGameLibrary : NSObject
/* Path of the imported, verified .gdi, or nil when no game is installed. */
+ (nullable NSString *)installedGamePath;
/* Verify the folder against Sonic Adventure PAL v1.003, then copy the .gdi and
 * its tracks into Application Support. Progress (0-1) and completion are
 * delivered on the main queue; error is a user-facing sentence. */
+ (void)importFolder:(NSURL *)folder
		progress:(void (^)(double fraction))progress
		completion:(void (^)(NSString *_Nullable gdiPath, NSString *_Nullable error))completion;
@end

NS_ASSUME_NONNULL_END
