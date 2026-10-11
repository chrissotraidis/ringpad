// SonicPad single-stick Dreamcast touch overlay.
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/* Draws the port/input touch layout inside the safe area and feeds per-touch
 * arbitration into the shared DreamcastInputMixer (touch source). */
@interface SPTouchOverlayView : UIView
/* Drop every touch, held value and queued edge (menus, lifecycle, import). */
- (void)clearInput;
@end

NS_ASSUME_NONNULL_END
