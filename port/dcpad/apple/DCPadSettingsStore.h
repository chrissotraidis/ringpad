// DCPad settings persistence (NSUserDefaults, per namespace and device class).
#import <Foundation/Foundation.h>

#include "DCPad.h"

NS_ASSUME_NONNULL_BEGIN

@interface DCPadSettingsStore : NSObject
- (instancetype)initWithNamespace:(NSString *)storageNamespace NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;
- (dcpad::Settings)settingsForPhone:(BOOL)phone;
- (void)saveSettings:(const dcpad::Settings &)settings phone:(BOOL)phone;
/* Positions and sizes return to defaults; opacity, haptics and modes stay. */
- (void)resetLayoutForPhone:(BOOL)phone;
@end

NS_ASSUME_NONNULL_END
