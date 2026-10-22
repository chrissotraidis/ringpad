// Standard ••• menu items for a Dreamcast port on SonicPad's foundation
// (recompiled code on Flycast's hardware layer): save states, display, sound
// and VMU save export. Nothing here is specific to Sonic Adventure, so the
// next port can hand the same items to its DCPad overlay.
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol SPPortMenuHost <NSObject>
/* Stop or resume the emulator for a named reason (reasons stack). */
- (void)setPaused:(BOOL)paused reason:(NSString *)reason;
/* Short confirmation shown over the game. */
- (void)portMenuShowMessage:(NSString *)message;
@optional
/* The host draws its own FPS counter; Graphics ▸ Show FPS toggles it. */
- (BOOL)portMenuFPSVisible;
- (void)portMenuSetFPSVisible:(BOOL)visible;
@end

@interface SPPortMenu : NSObject

- (instancetype)initWithHost:(UIViewController<SPPortMenuHost> *)host NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

/* Save State ▸ and Load State ▸ with three slots and their dates. */
- (NSArray<UIMenuElement *> *)saveStateElementsForGameLoaded:(BOOL)loaded;
/* Graphics ▸: render resolution, texture upscaling and Show FPS. */
- (UIMenu *)graphicsMenu;
/* Sound ▸: volume. */
- (UIMenu *)soundMenu;
/* Shares the game's VMU save file (A1) through the system share sheet. */
- (UIAction *)exportVMUActionForGameLoaded:(BOOL)loaded;

@end

NS_ASSUME_NONNULL_END
