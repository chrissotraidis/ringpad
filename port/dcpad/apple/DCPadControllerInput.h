// DCPad physical controller input: Apple GameController -> Dreamcast state.
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/* Feeds player 1's extended gamepad into DreamcastInputMixer as the
 * controller source. A second controller never takes player 1, and a
 * disconnect releases exactly that controller's input. Face buttons follow a
 * persisted A/B/X/Y mapping; the stick, D-pad, Start (Menu button) and
 * analog triggers (L2/R2, with L1/R1 as full presses) map directly. */
@interface DCPadControllerInput : NSObject
- (instancetype)initWithNamespace:(NSString *)storageNamespace NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;
/* Called on the main queue whenever player 1 gains or loses a controller. */
@property (nonatomic, copy, nullable) void (^connectionChanged)(BOOL connected);
@property (nonatomic, readonly) BOOL connected;
- (void)start;
- (void)presentMappingFrom:(UIViewController *)presenter;
@end

NS_ASSUME_NONNULL_END
