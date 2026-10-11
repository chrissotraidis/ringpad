// DCPad: reusable Dreamcast touch controls for iOS and iPadOS.
//
// A full-screen overlay that draws the DCPad layout, turns multi-touch into
// one Dreamcast controller state (published to DreamcastInputMixer as the
// touch source), and hosts the persistent ••• menu, the touch-control
// settings panel and Move Controls editing. Hosts add their own menu items
// through the delegate and pause the game while the overlay is modal.
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class DCPadOverlayView;

@protocol DCPadOverlayDelegate <NSObject>
@optional
/* Game-specific menu items, shown above the standard Controls section. */
- (NSArray<UIMenuElement *> *)dcpadOverlayMenuElements:(DCPadOverlayView *)overlay;
/* YES while the menu, settings panel or layout editor is showing. Hosts
 * pause the game and the overlay has already released all touch input. */
- (void)dcpadOverlay:(DCPadOverlayView *)overlay modalStateChanged:(BOOL)modal;
/* The user chose Controller Mapping… from the Controls section. */
- (void)dcpadOverlayRequestsControllerMapping:(DCPadOverlayView *)overlay;
@end

@interface DCPadOverlayView : UIView

/* storageNamespace keeps layouts of different games apart (for example the
 * game's product name). Layouts are stored separately for phone and tablet. */
- (instancetype)initWithFrame:(CGRect)frame storageNamespace:(NSString *)storageNamespace NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithFrame:(CGRect)frame NS_UNAVAILABLE;
- (instancetype)initWithCoder:(NSCoder *)coder NS_UNAVAILABLE;

@property (nonatomic, weak, nullable) id<DCPadOverlayDelegate> delegate;
@property (nonatomic, copy) NSString *menuTitle;
@property (nonatomic, readonly) UIButton *menuButton;
/* YES while the menu, settings panel or layout editor is showing. */
@property (nonatomic, readonly, getter=isModal) BOOL modal;

/* Release every touch and held value (lifecycle, import, controller handoff). */
- (void)clearInput;
/* A physical controller owns player 1; hides touch controls if the user's
 * "Hide with Controller" setting is on. */
- (void)setControllerConnected:(BOOL)connected;
- (void)showSettingsPanel;
- (void)beginLayoutEditing;

@end

NS_ASSUME_NONNULL_END
