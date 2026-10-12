// DCPad overlay. See DCPadOverlayView.h.
#import "DCPadOverlayView.h"
#import "DCPadSettingsStore.h"

#include "DCPad.h"
#include "DreamcastInputMixer.h"

#include <cmath>
#include <map>

using dcpad::Control;

namespace {

UIColor *RGBA(CGFloat r, CGFloat g, CGFloat b, CGFloat a) { return [UIColor colorWithRed:r green:g blue:b alpha:a]; }

struct FaceStyle { dcpad::Face face; NSString *name; UIColor *fill; UIColor *glyph; UIColor *pressedText; };

NSArray *FaceIds() { return @[@"A", @"B", @"X", @"Y"]; }

/* Dreamcast controller colours: A red, B blue, X yellow, Y green. 'fill' is
 * the pressed colour; 'glyph' is a lighter tint that reads on dark glass. */
FaceStyle FaceStyles(int i)
{
	switch (i) {
		case 0: return {dcpad::FaceA, @"A", RGBA(0.86, 0.19, 0.20, 1), RGBA(1.00, 0.42, 0.40, 1), UIColor.whiteColor};
		case 1: return {dcpad::FaceB, @"B", RGBA(0.18, 0.43, 0.88, 1), RGBA(0.45, 0.66, 1.00, 1), UIColor.whiteColor};
		case 2: return {dcpad::FaceX, @"X", RGBA(0.96, 0.77, 0.17, 1), RGBA(1.00, 0.85, 0.30, 1), RGBA(0.18, 0.14, 0.02, 1)};
		default: return {dcpad::FaceY, @"Y", RGBA(0.22, 0.67, 0.33, 1), RGBA(0.42, 0.86, 0.50, 1), UIColor.whiteColor};
	}
}

/* One visual language for every control: dark glass, a thin light rim and a
 * soft shadow so controls read on both bright and dark scenes. Active state
 * is always the Dreamcast swirl orange. */
UIColor *kAccent() { return RGBA(0.96, 0.45, 0.13, 1); }
UIColor *kSurface() { return [UIColor colorWithWhite:0.03 alpha:0.46]; }
UIColor *kSurfacePressed() { return [UIColor colorWithWhite:0.03 alpha:0.62]; }
UIColor *kRim() { return [UIColor colorWithWhite:1 alpha:0.36]; }
UIColor *kGlyph() { return [UIColor colorWithWhite:1 alpha:0.78]; }


CGRect CG(const dcpad::Rect& r) { return CGRectMake(r.x, r.y, r.w, r.h); }

CATextLayer *TextLayer(NSString *text, CGFloat size, UIColor *color, CGFloat scale)
{
	CATextLayer *t = [CATextLayer layer];
	t.string = text;
	// UIFont is toll-free bridged to CTFont; a font name would lose the
	// system font's weight and fall back to a serif face.
	t.font = (__bridge CFTypeRef)[UIFont systemFontOfSize:size weight:UIFontWeightHeavy];
	t.fontSize = size;
	t.foregroundColor = color.CGColor;
	t.alignmentMode = kCAAlignmentCenter;
	t.contentsScale = scale;
	return t;
}

void CenterText(CATextLayer *t, CGRect frame, CGFloat size)
{
	t.fontSize = size;
	const CGFloat h = size * 1.25;
	t.frame = CGRectMake(frame.origin.x, CGRectGetMidY(frame) - h / 2, frame.size.width, h);
}

constexpr CGFloat kArmFraction = 0.36;    /* D-pad arm width / cross size */
constexpr CGFloat kThumbFraction = 0.42;  /* stick thumb radius / base radius */
constexpr double kIdleDimDelay = 4.0;     /* seconds untouched before the controls fade back */
constexpr float kIdleDimFactor = 0.45f;   /* share of the normal opacity while dimmed */

UIBezierPath *CrossPath(CGRect f)
{
	const CGFloat arm = f.size.width * kArmFraction, r = arm * 0.28;
	UIBezierPath *v = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(CGRectGetMidX(f) - arm / 2, f.origin.y, arm, f.size.height) cornerRadius:r];
	UIBezierPath *h = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(f.origin.x, CGRectGetMidY(f) - arm / 2, f.size.width, arm) cornerRadius:r];
	[v appendPath:h];
	v.usesEvenOddFillRule = NO;
	return v;
}

/* Outer part of one arm (where its arrow sits). */
CGRect ArmRect(CGRect f, uint8_t dir)
{
	const CGFloat arm = f.size.width * kArmFraction, len = (f.size.width - arm) / 2;
	switch (dir) {
		case dcpad::DPadUp: return CGRectMake(CGRectGetMidX(f) - arm / 2, f.origin.y, arm, len);
		case dcpad::DPadDown: return CGRectMake(CGRectGetMidX(f) - arm / 2, CGRectGetMaxY(f) - len, arm, len);
		case dcpad::DPadLeft: return CGRectMake(f.origin.x, CGRectGetMidY(f) - arm / 2, len, arm);
		default: return CGRectMake(CGRectGetMaxX(f) - len, CGRectGetMidY(f) - arm / 2, len, arm);
	}
}

/* One arm from the centre hub to its end; clipped to the cross when drawn so
 * it keeps the rounded end. A diagonal lights its two arms. */
CGRect HalfRect(CGRect f, uint8_t dir)
{
	const CGFloat arm = f.size.width * kArmFraction, half = f.size.width / 2;
	switch (dir) {
		case dcpad::DPadUp: return CGRectMake(CGRectGetMidX(f) - arm / 2, f.origin.y, arm, half);
		case dcpad::DPadDown: return CGRectMake(CGRectGetMidX(f) - arm / 2, CGRectGetMidY(f), arm, half);
		case dcpad::DPadLeft: return CGRectMake(f.origin.x, CGRectGetMidY(f) - arm / 2, half, arm);
		default: return CGRectMake(CGRectGetMidX(f), CGRectGetMidY(f) - arm / 2, half, arm);
	}
}

UIBezierPath *ArrowPath(CGRect arm, uint8_t dir)
{
	const CGFloat s = MIN(arm.size.width, arm.size.height) * 0.22;
	const CGPoint c = CGPointMake(CGRectGetMidX(arm), CGRectGetMidY(arm));
	UIBezierPath *p = [UIBezierPath bezierPath];
	switch (dir) {
		case dcpad::DPadUp: [p moveToPoint:CGPointMake(c.x, c.y - s)]; [p addLineToPoint:CGPointMake(c.x - s, c.y + s * 0.6)]; [p addLineToPoint:CGPointMake(c.x + s, c.y + s * 0.6)]; break;
		case dcpad::DPadDown: [p moveToPoint:CGPointMake(c.x, c.y + s)]; [p addLineToPoint:CGPointMake(c.x - s, c.y - s * 0.6)]; [p addLineToPoint:CGPointMake(c.x + s, c.y - s * 0.6)]; break;
		case dcpad::DPadLeft: [p moveToPoint:CGPointMake(c.x - s, c.y)]; [p addLineToPoint:CGPointMake(c.x + s * 0.6, c.y - s)]; [p addLineToPoint:CGPointMake(c.x + s * 0.6, c.y + s)]; break;
		default: [p moveToPoint:CGPointMake(c.x + s, c.y)]; [p addLineToPoint:CGPointMake(c.x - s * 0.6, c.y - s)]; [p addLineToPoint:CGPointMake(c.x - s * 0.6, c.y + s)]; break;
	}
	[p closePath];
	return p;
}


const uint8_t kDirs[4] = {dcpad::DPadUp, dcpad::DPadDown, dcpad::DPadLeft, dcpad::DPadRight};

} // namespace

#pragma mark - Menu button

@interface DCPadMenuButton : UIButton
@property (nonatomic, copy, nullable) void (^menuWillOpen)(void);
@property (nonatomic, copy, nullable) void (^menuDidClose)(void);
@end

@implementation DCPadMenuButton

- (void)contextMenuInteraction:(UIContextMenuInteraction *)interaction
		willDisplayMenuForConfiguration:(UIContextMenuConfiguration *)configuration
		animator:(id<UIContextMenuInteractionAnimating>)animator
{
	if ([UIButton instancesRespondToSelector:_cmd])
		[super contextMenuInteraction:interaction willDisplayMenuForConfiguration:configuration animator:animator];
	if (self.menuWillOpen)
		self.menuWillOpen();
}

- (void)contextMenuInteraction:(UIContextMenuInteraction *)interaction
		willEndForConfiguration:(UIContextMenuConfiguration *)configuration
		animator:(id<UIContextMenuInteractionAnimating>)animator
{
	if ([UIButton instancesRespondToSelector:_cmd])
		[super contextMenuInteraction:interaction willEndForConfiguration:configuration animator:animator];
	void (^closed)(void) = self.menuDidClose;
	if (closed == nil)
		return;
	if (animator != nil)
		[animator addCompletion:closed];
	else
		closed();
}

@end

#pragma mark - Overlay

@implementation DCPadOverlayView
{
	DCPadSettingsStore *_store;
	dcpad::Settings _settings;
	dcpad::Layout _layout;
	dcpad::TouchEngine _engine;
	std::map<void *, int> _touchIds;
	int _nextTouchId;
	BOOL _phone;
	BOOL _controllerConnected;
	BOOL _menuOpen;
	BOOL _editing;
	Control _selected;
	Control _dragging;
	CGPoint _dragOffset;
	BOOL _dragMoved;

	CALayer *_controls;
	CAShapeLayer *_stickBase, *_stickGate, *_stickArc, *_stickThumb, *_stickDish;
	CAShapeLayer *_dpadShape, *_dpadPressed, *_dpadMask, *_dpadCenter, *_dpadArrows;
	BOOL _stickWasActive;
	CALayer *_face[4];
	CAShapeLayer *_faceCircle[4];
	CAGradientLayer *_faceSheen[4];
	CATextLayer *_faceText[4];
	CAShapeLayer *_trigShape[2], *_trigMask[2], *_trigDetent[2];
	CAGradientLayer *_trigFill[2];
	CAGradientLayer *_stickShade;
	CATextLayer *_trigText[2];
	CAShapeLayer *_startShape;
	CATextLayer *_startText;
	CAShapeLayer *_editOutlines, *_editSelected;

	DCPadMenuButton *_menuButton;
	UIView *_panel;
	UISlider *_opacitySlider, *_sizeSlider, *_travelSlider;
	UISwitch *_floatingSwitch, *_hapticsSwitch, *_hideSwitch, *_dimSwitch;
	UIView *_editorBar;
	UILabel *_editorLabel;
	UISlider *_editorSize;
	UIImpactFeedbackGenerator *_haptic;
	UIStackView *_panelStack;
	CGFloat _panelContentHeight;
	dcpad::Visual _preview;
	BOOL _previewing;
	UILabel *_message;
	NSUInteger _messageToken;
	BOOL _messageVisible;
	BOOL _dimmed;
	NSUInteger _dimToken;
}

- (UIButton *)menuButton
{
	return _menuButton;
}

- (instancetype)initWithFrame:(CGRect)frame storageNamespace:(NSString *)storageNamespace
{
	if (!(self = [super initWithFrame:frame]))
		return nil;
	self.multipleTouchEnabled = YES;
	self.backgroundColor = UIColor.clearColor;
	_menuTitle = @"Menu";
	_store = [[DCPadSettingsStore alloc] initWithNamespace:storageNamespace];
	_phone = UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPhone;
	_settings = [_store settingsForPhone:_phone];
	_nextTouchId = 1;
	_haptic = [[UIImpactFeedbackGenerator alloc] initWithStyle:UIImpactFeedbackStyleLight];
	[self buildLayers];
	[self buildMenuButton];
	[self buildPanel];
	[self buildEditorBar];
	[self buildMessage];
	[self loadPreview];
	[self scheduleIdleDim];
	return self;
}

- (void)buildMessage
{
	_message = [UILabel new];
	_message.font = [UIFont systemFontOfSize:15 weight:UIFontWeightSemibold];
	_message.textColor = UIColor.whiteColor;
	_message.textAlignment = NSTextAlignmentCenter;
	_message.backgroundColor = kSurfacePressed();
	_message.layer.cornerRadius = 18;
	_message.layer.borderWidth = 1.5;
	_message.layer.borderColor = kRim().CGColor;
	_message.layer.masksToBounds = YES;
	_message.alpha = 0;
	_message.accessibilityIdentifier = @"dcpad.message";
	[self addSubview:_message];
}

- (void)showMessage:(NSString *)message
{
	[self showMessage:message duration:1.8];
}

- (void)showMessage:(NSString *)message duration:(NSTimeInterval)duration
{
	_message.text = message;
	const CGRect safe = [self safeRect];
	const CGFloat w = MIN(safe.size.width - 120, [_message sizeThatFits:CGSizeMake(CGFLOAT_MAX, 36)].width + 36);
	_message.frame = CGRectMake(CGRectGetMidX(safe) - w / 2, CGRectGetMinY(safe) + 14, w, 36);
	[self bringSubviewToFront:_message];
	const NSUInteger token = ++_messageToken;
	_messageVisible = YES;
	[self updateAccessibility];
	[UIView animateWithDuration:0.15 animations:^{ self->_message.alpha = 1; }];
	UIAccessibilityPostNotification(UIAccessibilityAnnouncementNotification, message);
	dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(duration * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
		if (token != self->_messageToken)
			return;
		[UIView animateWithDuration:0.35 animations:^{ self->_message.alpha = 0; } completion:^(BOOL) {
			if (token != self->_messageToken)
				return;
			self->_messageVisible = NO;
			[self updateAccessibility];
		}];
	});
}

// DCPAD_PREVIEW="stick=0.7,-0.5 dpad=up face=AB l=128 r=255 start" draws a
// fixed pressed state for screenshots and design review; input still works.
- (void)loadPreview
{
	const char *spec = getenv("DCPAD_PREVIEW");
	if (spec == nullptr)
		return;
	_previewing = YES;
	for (NSString *part in [[NSString stringWithUTF8String:spec] componentsSeparatedByString:@" "]) {
		NSArray *kv = [part componentsSeparatedByString:@"="];
		NSString *key = kv.firstObject, *value = kv.count > 1 ? kv[1] : @"";
		if ([key isEqualToString:@"stick"]) {
			NSArray *xy = [value componentsSeparatedByString:@","];
			_preview.stick_active = YES;
			_preview.stick_x = [xy.firstObject floatValue];
			_preview.stick_y = xy.count > 1 ? [xy[1] floatValue] : 0;
		} else if ([key isEqualToString:@"dpad"]) {
			if ([value containsString:@"up"]) _preview.dpad |= dcpad::DPadUp;
			if ([value containsString:@"down"]) _preview.dpad |= dcpad::DPadDown;
			if ([value containsString:@"left"]) _preview.dpad |= dcpad::DPadLeft;
			if ([value containsString:@"right"]) _preview.dpad |= dcpad::DPadRight;
		} else if ([key isEqualToString:@"face"]) {
			if ([value containsString:@"A"]) _preview.face |= dcpad::FaceA;
			if ([value containsString:@"B"]) _preview.face |= dcpad::FaceB;
			if ([value containsString:@"X"]) _preview.face |= dcpad::FaceX;
			if ([value containsString:@"Y"]) _preview.face |= dcpad::FaceY;
		} else if ([key isEqualToString:@"l"]) {
			_preview.l = uint8_t(value.intValue);
			_preview.l_full = _preview.l == 255;
		} else if ([key isEqualToString:@"r"]) {
			_preview.r = uint8_t(value.intValue);
			_preview.r_full = _preview.r == 255;
		} else if ([key isEqualToString:@"start"]) {
			_preview.start = true;
		}
	}
}

#pragma mark Layers

- (void)buildLayers
{
	const CGFloat scale = UIScreen.mainScreen.scale;
	_controls = [CALayer layer];
	[self.layer addSublayer:_controls];

	auto shape = [&](CALayer *parent) {
		CAShapeLayer *s = [CAShapeLayer layer];
		s.contentsScale = scale;
		[parent addSublayer:s];
		return s;
	};
	auto surface = [&](CAShapeLayer *s) {
		s.fillColor = kSurface().CGColor;
		s.strokeColor = kRim().CGColor;
		s.lineWidth = 1.5;
		s.shadowColor = UIColor.blackColor.CGColor;
		s.shadowOpacity = 0.28;
		s.shadowRadius = 6;
		s.shadowOffset = CGSizeMake(0, 2);
	};
	auto glow = [&](CAShapeLayer *s, UIColor *color) {
		s.shadowColor = color.CGColor;
		s.shadowOffset = CGSizeZero;
		s.shadowRadius = 8;
		s.shadowOpacity = 0;
	};

	_stickBase = shape(_controls);
	surface(_stickBase);
	_stickGate = shape(_controls);
	_stickGate.fillColor = UIColor.clearColor.CGColor;
	_stickGate.strokeColor = [UIColor colorWithWhite:1 alpha:0.16].CGColor;
	_stickGate.lineWidth = 1;
	_stickGate.lineDashPattern = @[@2, @6];
	_stickArc = shape(_controls);
	_stickArc.fillColor = UIColor.clearColor.CGColor;
	_stickArc.strokeColor = kAccent().CGColor;
	_stickArc.lineWidth = 4;
	_stickArc.lineCap = kCALineCapRound;
	glow(_stickArc, kAccent());
	_stickArc.shadowOpacity = 0.8;
	_stickThumb = shape(_controls);
	_stickThumb.strokeColor = [UIColor colorWithWhite:1 alpha:0.55].CGColor;
	_stickThumb.lineWidth = 1.5;
	_stickThumb.shadowColor = UIColor.blackColor.CGColor;
	_stickThumb.shadowOpacity = 0.45;
	_stickThumb.shadowRadius = 6;
	_stickThumb.shadowOffset = CGSizeMake(0, 3);
	// Shading toward the bottom gives the thumb a rounded, lit-from-above look.
	_stickShade = [CAGradientLayer layer];
	_stickShade.colors = @[(id)[UIColor colorWithWhite:0 alpha:0].CGColor, (id)[UIColor colorWithWhite:0 alpha:0.20].CGColor];
	_stickShade.startPoint = CGPointMake(0.5, 0.35);
	_stickShade.endPoint = CGPointMake(0.5, 1.0);
	_stickShade.masksToBounds = YES;
	[_stickThumb addSublayer:_stickShade];
	_stickDish = shape(_stickThumb);
	_stickDish.fillColor = [UIColor colorWithWhite:0 alpha:0.05].CGColor;
	_stickDish.strokeColor = [UIColor colorWithWhite:0 alpha:0.14].CGColor;
	_stickDish.lineWidth = 1.5;

	_dpadShape = shape(_controls);
	surface(_dpadShape);
	_dpadPressed = shape(_controls);
	_dpadPressed.fillColor = [kAccent() colorWithAlphaComponent:0.85].CGColor;
	_dpadMask = [CAShapeLayer layer];
	_dpadPressed.mask = _dpadMask;
	_dpadCenter = shape(_controls);
	_dpadCenter.fillColor = [UIColor colorWithWhite:0 alpha:0.18].CGColor;
	_dpadCenter.strokeColor = [UIColor colorWithWhite:1 alpha:0.14].CGColor;
	_dpadCenter.lineWidth = 1;
	_dpadArrows = shape(_controls);
	_dpadArrows.fillColor = kGlyph().CGColor;

	for (int i = 0; i < 4; i++) {
		const FaceStyle st = FaceStyles(i);
		_face[i] = [CALayer layer];
		[_controls addSublayer:_face[i]];
		_faceCircle[i] = shape(_face[i]);
		_faceCircle[i].fillColor = kSurface().CGColor;
		_faceCircle[i].strokeColor = st.glyph.CGColor;
		_faceCircle[i].lineWidth = 2.5;
		glow(_faceCircle[i], st.fill);
		_faceSheen[i] = [CAGradientLayer layer];
		_faceSheen[i].colors = @[(id)[UIColor colorWithWhite:1 alpha:0.20].CGColor, (id)[UIColor colorWithWhite:1 alpha:0].CGColor];
		_faceSheen[i].startPoint = CGPointMake(0.5, 0);
		_faceSheen[i].endPoint = CGPointMake(0.5, 0.6);
		_faceSheen[i].masksToBounds = YES;
		[_face[i] addSublayer:_faceSheen[i]];
		_faceText[i] = TextLayer(st.name, 30, st.glyph, scale);
		_faceText[i].shadowColor = UIColor.blackColor.CGColor;
		_faceText[i].shadowOpacity = 0.35;
		_faceText[i].shadowRadius = 2;
		_faceText[i].shadowOffset = CGSizeMake(0, 1);
		[_face[i] addSublayer:_faceText[i]];
	}

	for (int i = 0; i < 2; i++) {
		_trigShape[i] = shape(_controls);
		surface(_trigShape[i]);
		// The fill brightens toward the outer, full-press end.
		_trigFill[i] = [CAGradientLayer layer];
		UIColor *deep = RGBA(0.78, 0.30, 0.07, 0.85), *bright = RGBA(1.0, 0.62, 0.26, 0.95);
		_trigFill[i].colors = i == 0 ? @[(id)bright.CGColor, (id)deep.CGColor] : @[(id)deep.CGColor, (id)bright.CGColor];
		_trigFill[i].startPoint = CGPointMake(0, 0.5);
		_trigFill[i].endPoint = CGPointMake(1, 0.5);
		_trigFill[i].masksToBounds = YES;
		[_controls addSublayer:_trigFill[i]];
		_trigMask[i] = [CAShapeLayer layer];
		_trigFill[i].mask = _trigMask[i];
		_trigDetent[i] = shape(_controls);
		_trigDetent[i].strokeColor = [UIColor colorWithWhite:1 alpha:0.30].CGColor;
		_trigDetent[i].lineWidth = 1.5;
		_trigDetent[i].lineCap = kCALineCapRound;
		_trigText[i] = TextLayer(i == 0 ? @"L" : @"R", 22, kGlyph(), scale);
		[_controls addSublayer:_trigText[i]];
	}

	_startShape = shape(_controls);
	surface(_startShape);
	_startText = TextLayer(@"START", 14, kGlyph(), scale);
	_startText.font = (__bridge CFTypeRef)[UIFont systemFontOfSize:14 weight:UIFontWeightBold];
	[_controls addSublayer:_startText];

	_editOutlines = shape(self.layer);
	_editOutlines.fillColor = [RGBA(1.0, 0.80, 0.22, 1) colorWithAlphaComponent:0.10].CGColor;
	_editOutlines.strokeColor = RGBA(1.0, 0.80, 0.22, 1).CGColor;
	_editOutlines.lineWidth = 2.5;
	_editOutlines.lineDashPattern = @[@8, @5];
	_editSelected = shape(self.layer);
	_editSelected.fillColor = [RGBA(0.22, 0.75, 1.0, 1) colorWithAlphaComponent:0.14].CGColor;
	_editSelected.strokeColor = RGBA(0.22, 0.75, 1.0, 1).CGColor;
	_editSelected.lineWidth = 3.5;
}


#pragma mark Menu

- (void)buildMenuButton
{
	_menuButton = [DCPadMenuButton buttonWithType:UIButtonTypeCustom];
	UIImageSymbolConfiguration *symbol = [UIImageSymbolConfiguration configurationWithPointSize:19 weight:UIImageSymbolWeightBold];
	UIImage *ellipsis = [UIImage systemImageNamed:@"ellipsis" withConfiguration:symbol];
	UIButtonConfiguration *config = [UIButtonConfiguration plainButtonConfiguration];
	config.image = ellipsis;
	config.baseForegroundColor = UIColor.whiteColor;
	config.imageColorTransformer = ^UIColor *(UIColor *) { return UIColor.whiteColor; };
	config.contentInsets = NSDirectionalEdgeInsetsZero;
	UIBackgroundConfiguration *background = [UIBackgroundConfiguration clearConfiguration];
	background.backgroundColor = kSurface();
	background.cornerRadius = 22;
	background.strokeColor = kRim();
	background.strokeWidth = 1.5;
	config.background = background;
	_menuButton.automaticallyUpdatesConfiguration = NO;
	_menuButton.configuration = config;
	_menuButton.tintColor = UIColor.whiteColor;
	_menuButton.accessibilityLabel = @"Menu";
	_menuButton.accessibilityIdentifier = @"dcpad.menu";
	_menuButton.showsMenuAsPrimaryAction = YES;
	__weak DCPadOverlayView *weakSelf = self;
	_menuButton.menu = [UIMenu menuWithChildren:@[[UIDeferredMenuElement elementWithUncachedProvider:
			^(void (^completion)(NSArray<UIMenuElement *> *)) {
		completion([weakSelf menuElements]);
	}]]];
	_menuButton.menuWillOpen = ^{
		DCPadOverlayView *strong = weakSelf;
		if (strong == nil)
			return;
		strong->_menuOpen = YES;
		[strong modalChanged];
	};
	_menuButton.menuDidClose = ^{
		DCPadOverlayView *strong = weakSelf;
		if (strong == nil)
			return;
		strong->_menuOpen = NO;
		[strong modalChanged];
	};
	[self addSubview:_menuButton];
}

- (NSArray<UIMenuElement *> *)menuElements
{
	NSMutableArray<UIMenuElement *> *items = [NSMutableArray array];
	if ([self.delegate respondsToSelector:@selector(dcpadOverlayMenuElements:)])
		[items addObjectsFromArray:[self.delegate dcpadOverlayMenuElements:self]];
	__weak DCPadOverlayView *weakSelf = self;
	NSMutableArray<UIMenuElement *> *controls = [NSMutableArray array];
	[controls addObject:[UIAction actionWithTitle:@"Touch Control Settings…" image:[UIImage systemImageNamed:@"slider.horizontal.3"]
			identifier:nil handler:^(UIAction *) { [weakSelf showSettingsPanel]; }]];
	[controls addObject:[UIAction actionWithTitle:@"Move Controls" image:[UIImage systemImageNamed:@"hand.draw"]
			identifier:nil handler:^(UIAction *) { [weakSelf beginLayoutEditing]; }]];
	if ([self.delegate respondsToSelector:@selector(dcpadOverlayRequestsControllerMapping:)])
		[controls addObject:[UIAction actionWithTitle:@"Controller Mapping…" image:[UIImage systemImageNamed:@"gamecontroller"]
				identifier:nil handler:^(UIAction *) {
			DCPadOverlayView *strong = weakSelf;
			[strong.delegate dcpadOverlayRequestsControllerMapping:strong];
		}]];
	// A submenu keeps the top level short enough for a landscape phone.
	[items addObject:[UIMenu menuWithTitle:@"Controls" image:[UIImage systemImageNamed:@"gamecontroller.fill"]
			identifier:nil options:0 children:controls]];
	// The title header costs a row a landscape phone does not have.
	return @[[UIMenu menuWithTitle:_phone ? @"" : self.menuTitle image:nil identifier:nil options:UIMenuOptionsDisplayInline children:items]];
}

#pragma mark Settings panel

- (UIView *)rowWithTitle:(NSString *)title control:(UIView *)control
{
	UILabel *label = [UILabel new];
	label.text = title;
	label.textColor = UIColor.whiteColor;
	label.font = [UIFont systemFontOfSize:15 weight:UIFontWeightMedium];
	label.adjustsFontSizeToFitWidth = YES;
	label.minimumScaleFactor = 0.8;
	// One label column so sliders and switches line up.
	[label.widthAnchor constraintEqualToConstant:150].active = YES;
	[control setContentHuggingPriority:UILayoutPriorityDefaultLow forAxis:UILayoutConstraintAxisHorizontal];
	UIView *holder = control;
	if ([control isKindOfClass:UISwitch.class]) {
		// Keep switches at their natural size, left-aligned in the control column.
		UIStackView *wrap = [[UIStackView alloc] initWithArrangedSubviews:@[control, [UIView new]]];
		wrap.alignment = UIStackViewAlignmentCenter;
		holder = wrap;
	}
	UIStackView *row = [[UIStackView alloc] initWithArrangedSubviews:@[label, holder]];
	row.spacing = 14;
	row.alignment = UIStackViewAlignmentCenter;
	return row;
}

- (UIButton *)panelButton:(NSString *)title identifier:(NSString *)identifier action:(SEL)action destructive:(BOOL)destructive
{
	UIButtonConfiguration *config = [UIButtonConfiguration filledButtonConfiguration];
	config.title = title;
	config.baseBackgroundColor = destructive ? [UIColor colorWithWhite:0.22 alpha:1] : kAccent();
	config.baseForegroundColor = destructive ? RGBA(1, 0.55, 0.5, 1) : UIColor.whiteColor;
	config.cornerStyle = UIButtonConfigurationCornerStyleMedium;
	UIButton *button = [UIButton buttonWithConfiguration:config primaryAction:nil];
	button.accessibilityIdentifier = identifier;
	[button addTarget:self action:action forControlEvents:UIControlEventTouchUpInside];
	return button;
}

- (void)buildPanel
{
	_panel = [UIView new];
	_panel.backgroundColor = [UIColor colorWithWhite:0.04 alpha:0.95];
	_panel.layer.cornerRadius = 18;
	_panel.layer.borderWidth = 1;
	_panel.layer.borderColor = [UIColor colorWithWhite:1 alpha:0.2].CGColor;
	_panel.accessibilityIdentifier = @"dcpad.settings";
	_panel.hidden = YES;

	UILabel *title = [UILabel new];
	title.text = @"Touch Controls";
	title.textColor = UIColor.whiteColor;
	title.font = [UIFont systemFontOfSize:19 weight:UIFontWeightBold];
	UIButtonConfiguration *closeConfig = [UIButtonConfiguration grayButtonConfiguration];
	closeConfig.image = [UIImage systemImageNamed:@"xmark"];
	closeConfig.cornerStyle = UIButtonConfigurationCornerStyleCapsule;
	UIButton *close = [UIButton buttonWithConfiguration:closeConfig primaryAction:nil];
	close.accessibilityIdentifier = @"dcpad.settings.close";
	close.accessibilityLabel = @"Close";
	[close addTarget:self action:@selector(hideSettingsPanel) forControlEvents:UIControlEventTouchUpInside];
	UIStackView *header = [[UIStackView alloc] initWithArrangedSubviews:@[title, close]];
	header.alignment = UIStackViewAlignmentCenter;

	_opacitySlider = [UISlider new];
	_opacitySlider.minimumValue = 0.25;
	_opacitySlider.maximumValue = 1.0;
	_opacitySlider.accessibilityIdentifier = @"dcpad.settings.opacity";
	[_opacitySlider addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_sizeSlider = [UISlider new];
	_sizeSlider.minimumValue = 0.7;
	_sizeSlider.maximumValue = 1.4;
	_sizeSlider.accessibilityIdentifier = @"dcpad.settings.size";
	[_sizeSlider addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_travelSlider = [UISlider new];
	_travelSlider.minimumValue = 0.5;
	_travelSlider.maximumValue = 1.0;
	_travelSlider.accessibilityIdentifier = @"dcpad.settings.stickTravel";
	[_travelSlider addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_floatingSwitch = [UISwitch new];
	_floatingSwitch.accessibilityIdentifier = @"dcpad.settings.floating";
	[_floatingSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_hapticsSwitch = [UISwitch new];
	_hapticsSwitch.accessibilityIdentifier = @"dcpad.settings.haptics";
	[_hapticsSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_hideSwitch = [UISwitch new];
	_hideSwitch.accessibilityIdentifier = @"dcpad.settings.hideWithController";
	[_hideSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_dimSwitch = [UISwitch new];
	_dimSwitch.accessibilityIdentifier = @"dcpad.settings.dimWhenIdle";
	[_dimSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	for (UISlider *slider in @[_opacitySlider, _sizeSlider, _travelSlider])
		slider.minimumTrackTintColor = kAccent();
	for (UISwitch *toggle in @[_floatingSwitch, _hapticsSwitch, _hideSwitch, _dimSwitch])
		toggle.onTintColor = kAccent();

	UIButton *move = [self panelButton:@"Move Controls" identifier:@"dcpad.settings.move" action:@selector(beginLayoutEditing) destructive:NO];
	UIButton *reset = [self panelButton:@"Reset Layout" identifier:@"dcpad.settings.reset" action:@selector(confirmReset) destructive:YES];
	UIStackView *buttons = [[UIStackView alloc] initWithArrangedSubviews:@[move, reset]];
	buttons.spacing = 10;
	buttons.distribution = UIStackViewDistributionFillEqually;

	UIStackView *stack = [[UIStackView alloc] initWithArrangedSubviews:@[header,
		[self rowWithTitle:@"Opacity" control:_opacitySlider],
		[self rowWithTitle:@"Size" control:_sizeSlider],
		[self rowWithTitle:@"Stick travel" control:_travelSlider],
		[self rowWithTitle:@"Floating stick" control:_floatingSwitch],
		[self rowWithTitle:@"Dim when idle" control:_dimSwitch],
		[self rowWithTitle:@"Haptics" control:_hapticsSwitch],
		[self rowWithTitle:@"Hide with controller" control:_hideSwitch],
		buttons]];
	stack.axis = UILayoutConstraintAxisVertical;
	// Compact on a landscape phone so every row and both buttons fit unscrolled.
	stack.spacing = _phone ? 3 : 10;
	stack.translatesAutoresizingMaskIntoConstraints = NO;
	_panelStack = stack;
	UIScrollView *scroll = [UIScrollView new];
	scroll.translatesAutoresizingMaskIntoConstraints = NO;
	[scroll addSubview:stack];
	[_panel addSubview:scroll];
	[NSLayoutConstraint activateConstraints:@[
		[scroll.leadingAnchor constraintEqualToAnchor:_panel.leadingAnchor],
		[scroll.trailingAnchor constraintEqualToAnchor:_panel.trailingAnchor],
		[scroll.topAnchor constraintEqualToAnchor:_panel.topAnchor],
		[scroll.bottomAnchor constraintEqualToAnchor:_panel.bottomAnchor],
		[stack.leadingAnchor constraintEqualToAnchor:scroll.contentLayoutGuide.leadingAnchor constant:18],
		[stack.trailingAnchor constraintEqualToAnchor:scroll.contentLayoutGuide.trailingAnchor constant:-18],
		[stack.topAnchor constraintEqualToAnchor:scroll.contentLayoutGuide.topAnchor constant:_phone ? 8 : 14],
		[stack.bottomAnchor constraintEqualToAnchor:scroll.contentLayoutGuide.bottomAnchor constant:_phone ? -8 : -16],
		[stack.widthAnchor constraintEqualToAnchor:scroll.frameLayoutGuide.widthAnchor constant:-36],
		[close.widthAnchor constraintEqualToConstant:34],
		[close.heightAnchor constraintEqualToConstant:34],
		[buttons.heightAnchor constraintEqualToConstant:_phone ? 36 : 42],
	]];
	[self addSubview:_panel];
}

- (void)buildEditorBar
{
	_editorBar = [UIView new];
	_editorBar.backgroundColor = [UIColor colorWithWhite:0.04 alpha:0.95];
	_editorBar.layer.cornerRadius = 16;
	_editorBar.layer.borderWidth = 1.5;
	_editorBar.layer.borderColor = RGBA(1.0, 0.80, 0.22, 0.95).CGColor;
	_editorBar.hidden = YES;
	_editorLabel = [UILabel new];
	_editorLabel.textColor = UIColor.whiteColor;
	_editorLabel.font = [UIFont systemFontOfSize:15 weight:UIFontWeightSemibold];
	_editorLabel.adjustsFontSizeToFitWidth = YES;
	_editorLabel.minimumScaleFactor = 0.6;
	_editorLabel.numberOfLines = 2;
	_editorSize = [UISlider new];
	_editorSize.minimumValue = 0.6;
	_editorSize.maximumValue = 1.75;
	_editorSize.accessibilityIdentifier = @"dcpad.edit.size";
	_editorSize.minimumTrackTintColor = kAccent();
	[_editorSize addTarget:self action:@selector(editorSizeChanged:) forControlEvents:UIControlEventValueChanged];
	UIButtonConfiguration *doneConfig = [UIButtonConfiguration filledButtonConfiguration];
	doneConfig.title = @"Done";
	doneConfig.baseBackgroundColor = kAccent();
	doneConfig.cornerStyle = UIButtonConfigurationCornerStyleMedium;
	UIButton *done = [UIButton buttonWithConfiguration:doneConfig primaryAction:nil];
	done.accessibilityIdentifier = @"dcpad.edit.done";
	[done addTarget:self action:@selector(endLayoutEditing) forControlEvents:UIControlEventTouchUpInside];
	UIStackView *stack = [[UIStackView alloc] initWithArrangedSubviews:@[_editorLabel, _editorSize, done]];
	stack.spacing = 12;
	stack.alignment = UIStackViewAlignmentCenter;
	stack.translatesAutoresizingMaskIntoConstraints = NO;
	[_editorBar addSubview:stack];
	[_editorLabel setContentCompressionResistancePriority:UILayoutPriorityDefaultLow forAxis:UILayoutConstraintAxisHorizontal];
	[NSLayoutConstraint activateConstraints:@[
		[stack.leadingAnchor constraintEqualToAnchor:_editorBar.leadingAnchor constant:16],
		[stack.trailingAnchor constraintEqualToAnchor:_editorBar.trailingAnchor constant:-10],
		[stack.topAnchor constraintEqualToAnchor:_editorBar.topAnchor constant:8],
		[stack.bottomAnchor constraintEqualToAnchor:_editorBar.bottomAnchor constant:-8],
		[_editorSize.widthAnchor constraintGreaterThanOrEqualToConstant:_phone ? 110 : 160],
		[_editorLabel.widthAnchor constraintGreaterThanOrEqualToConstant:_phone ? 110 : 150],
		[done.widthAnchor constraintEqualToConstant:76],
	]];
	[self addSubview:_editorBar];
}

#pragma mark Layout

- (CGRect)safeRect
{
	return UIEdgeInsetsInsetRect(self.bounds, self.safeAreaInsets);
}

- (void)layoutSubviews
{
	[super layoutSubviews];
	const CGRect safe = [self safeRect];
	_layout = dcpad::resolve_layout({float(safe.origin.x), float(safe.origin.y), float(safe.size.width), float(safe.size.height)}, _phone, _settings);
	_menuButton.frame = CGRectMake(CGRectGetMaxX(safe) - 44 - 10, CGRectGetMinY(safe) + 10, 44, 44);
	_panelContentHeight = [_panelStack systemLayoutSizeFittingSize:CGSizeMake(MIN(380, safe.size.width - 32) - 36, 0)
			withHorizontalFittingPriority:UILayoutPriorityRequired verticalFittingPriority:UILayoutPriorityFittingSizeLevel].height + (_phone ? 16 : 30);
	const CGFloat panelW = MIN(380, safe.size.width - 32);
	const CGFloat panelH = MIN(_panelContentHeight > 0 ? _panelContentHeight : 360, safe.size.height - (_phone ? 16 : 40));
	_panel.frame = CGRectMake(CGRectGetMidX(safe) - panelW / 2, CGRectGetMidY(safe) - panelH / 2, panelW, panelH);
	// The middle band is the one area no default control uses, so the editor
	// bar never covers a control the player wants to drag.
	const CGFloat barW = MIN(560, safe.size.width * 0.5);
	_editorBar.frame = CGRectMake(CGRectGetMidX(safe) - barW / 2, CGRectGetMidY(safe) - 29, barW, 58);
	[self layoutControls];
	[self render];
	[self updateAccessibility];
}

- (void)layoutControls
{
	[CATransaction begin];
	[CATransaction setDisableActions:YES];
	_controls.frame = self.bounds;
	auto circle = [](CGFloat r) { return [UIBezierPath bezierPathWithOvalInRect:CGRectMake(-r, -r, 2 * r, 2 * r)].CGPath; };
	const CGFloat r = _layout.stick_radius;
	_stickBase.path = circle(r);
	_stickBase.shadowPath = _stickBase.path;
	_stickGate.path = circle(r * std::clamp(_settings.stick_travel, 0.5f, 1.0f));
	const CGFloat t = r * kThumbFraction;
	_stickThumb.path = circle(t);
	_stickThumb.shadowPath = _stickThumb.path;
	_stickShade.frame = CGRectMake(-t, -t, 2 * t, 2 * t);
	_stickShade.cornerRadius = t;
	_stickDish.path = circle(t * 0.62);

	const CGRect dpad = CG(_layout.frame(Control::DPad));
	UIBezierPath *cross = CrossPath(dpad);
	_dpadShape.path = cross.CGPath;
	_dpadShape.shadowPath = cross.CGPath;
	_dpadMask.path = cross.CGPath;
	const CGFloat hub = dpad.size.width * kArmFraction * 0.26;
	_dpadCenter.path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(CGRectGetMidX(dpad) - hub, CGRectGetMidY(dpad) - hub, 2 * hub, 2 * hub)].CGPath;
	UIBezierPath *arrows = [UIBezierPath bezierPath];
	for (int i = 0; i < 4; i++)
		[arrows appendPath:ArrowPath(ArmRect(dpad, kDirs[i]), kDirs[i])];
	_dpadArrows.path = arrows.CGPath;

	const CGFloat fr = _layout.face_radius;
	const dcpad::Face faces[4] = {dcpad::FaceA, dcpad::FaceB, dcpad::FaceX, dcpad::FaceY};
	for (int i = 0; i < 4; i++) {
		float cx, cy;
		_layout.face_center(faces[i], &cx, &cy);
		_face[i].bounds = CGRectMake(0, 0, 2 * fr, 2 * fr);
		_face[i].position = CGPointMake(cx, cy);
		_faceCircle[i].path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(1.25, 1.25, 2 * fr - 2.5, 2 * fr - 2.5)].CGPath;
		_faceCircle[i].shadowPath = _faceCircle[i].path;
		_faceSheen[i].frame = CGRectInset(_face[i].bounds, 2.5, 2.5);
		_faceSheen[i].cornerRadius = fr - 2.5;
		CenterText(_faceText[i], _face[i].bounds, fr * 0.92);
	}

	const Control trig[2] = {Control::LTrigger, Control::RTrigger};
	for (int i = 0; i < 2; i++) {
		const CGRect f = CG(_layout.frame(trig[i]));
		UIBezierPath *pill = [UIBezierPath bezierPathWithRoundedRect:f cornerRadius:f.size.height / 2];
		_trigShape[i].path = pill.CGPath;
		_trigShape[i].shadowPath = pill.CGPath;
		_trigFill[i].frame = f;
		_trigFill[i].cornerRadius = f.size.height / 2;
		// Full-press point; both sliders grow toward the screen edge.
		const CGFloat x = i == 0 ? CGRectGetMaxX(f) - f.size.width * dcpad::kTriggerFullAt : f.origin.x + f.size.width * dcpad::kTriggerFullAt;
		UIBezierPath *tick = [UIBezierPath bezierPath];
		// Two notches on the rim mark the full-press point without crossing the label area.
		[tick moveToPoint:CGPointMake(x, f.origin.y + 1)];
		[tick addLineToPoint:CGPointMake(x, f.origin.y + f.size.height * 0.24)];
		[tick moveToPoint:CGPointMake(x, CGRectGetMaxY(f) - f.size.height * 0.24)];
		[tick addLineToPoint:CGPointMake(x, CGRectGetMaxY(f) - 1)];
		_trigDetent[i].path = tick.CGPath;
		// The label sits on the inner, light-press end so the fill never hides it.
		const CGFloat labelW = f.size.height;
		const CGRect label = i == 0 ? CGRectMake(CGRectGetMaxX(f) - labelW - f.size.height * 0.1, f.origin.y, labelW, f.size.height)
		                             : CGRectMake(f.origin.x + f.size.height * 0.1, f.origin.y, labelW, f.size.height);
		CenterText(_trigText[i], label, f.size.height * 0.42);
	}
	const CGRect start = CG(_layout.frame(Control::Start));
	UIBezierPath *startPill = [UIBezierPath bezierPathWithRoundedRect:start cornerRadius:start.size.height / 2];
	_startShape.path = startPill.CGPath;
	_startShape.shadowPath = startPill.CGPath;
	CenterText(_startText, start, start.size.height * 0.34);
	[CATransaction commit];
}

- (BOOL)controlsHidden
{
	return _controllerConnected && _settings.hide_with_controller && !_editing;
}

- (void)render
{
	const dcpad::Visual v = _previewing ? _preview : _engine.visual(_layout);
	[CATransaction begin];
	[CATransaction setDisableActions:YES];
	const float resting = _settings.opacity * (_dimmed ? kIdleDimFactor : 1.0f);
	_controls.opacity = [self controlsHidden] ? 0 : (_editing ? 1.0 : resting);

	// Stick: base, travel guide, a direction arc on the rim and the thumb.
	const CGPoint base = _previewing ? CGPointMake(_layout.frame(Control::Stick).midX(), _layout.frame(Control::Stick).midY())
	                                 : CGPointMake(v.stick_base_x, v.stick_base_y);
	const CGFloat r = _layout.stick_radius;
	_stickBase.position = base;
	_stickGate.position = base;
	_stickArc.position = base;
	const CGFloat travel = r * (1 - kThumbFraction) - 2;
	const CGPoint thumb = CGPointMake(base.x + v.stick_x * travel, base.y + v.stick_y * travel);
	if (_stickWasActive && !v.stick_active) {
		// Spring the thumb home on release instead of snapping.
		[CATransaction begin];
		[CATransaction setDisableActions:NO];
		[CATransaction setAnimationDuration:0.14];
		[CATransaction setAnimationTimingFunction:[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut]];
		_stickThumb.position = thumb;
		[CATransaction commit];
	} else {
		_stickThumb.position = thumb;
	}
	_stickWasActive = v.stick_active;
	const CGFloat idle = _settings.floating_stick && !v.stick_active ? 0.6 : 1.0;
	_stickBase.opacity = idle;
	_stickGate.opacity = v.stick_active ? 1.0 : 0.0;
	_stickBase.strokeColor = (v.stick_active ? [kAccent() colorWithAlphaComponent:0.75] : kRim()).CGColor;
	_stickBase.fillColor = (v.stick_active ? kSurfacePressed() : kSurface()).CGColor;
	_stickThumb.fillColor = [UIColor colorWithWhite:v.stick_active ? 0.96 : 0.80 alpha:v.stick_active ? 0.96 : 0.85].CGColor;
	const CGFloat magnitude = std::min(1.0f, std::hypot(v.stick_x, v.stick_y));
	if (v.stick_active && magnitude > 0.08) {
		const CGFloat angle = std::atan2(v.stick_y, v.stick_x), span = 0.22 + 0.30 * magnitude;
		_stickArc.path = [UIBezierPath bezierPathWithArcCenter:CGPointZero radius:r - 1 startAngle:angle - span endAngle:angle + span clockwise:YES].CGPath;
		_stickArc.opacity = 0.35 + 0.65 * magnitude;
	} else {
		_stickArc.opacity = 0;
	}

	// D-pad: the pressed arms light up inside the cross (two for a diagonal).
	const CGRect dpad = CG(_layout.frame(Control::DPad));
	UIBezierPath *pressed = [UIBezierPath bezierPath];
	for (int i = 0; i < 4; i++)
		if (v.dpad & kDirs[i])
			[pressed appendPath:[UIBezierPath bezierPathWithRect:HalfRect(dpad, kDirs[i])]];
	_dpadPressed.path = pressed.CGPath;
	_dpadShape.fillColor = (v.dpad ? kSurfacePressed() : kSurface()).CGColor;

	const uint8_t faceBits[4] = {dcpad::FaceA, dcpad::FaceB, dcpad::FaceX, dcpad::FaceY};
	for (int i = 0; i < 4; i++) {
		const BOOL down = (v.face & faceBits[i]) != 0;
		const FaceStyle st = FaceStyles(i);
		_face[i].transform = down ? CATransform3DMakeScale(0.92, 0.92, 1) : CATransform3DIdentity;
		_faceCircle[i].fillColor = (down ? st.fill : kSurface()).CGColor;
		_faceCircle[i].strokeColor = (down ? [UIColor colorWithWhite:1 alpha:0.9] : st.glyph).CGColor;
		_faceCircle[i].shadowOpacity = down ? 0.9 : 0;
		_faceSheen[i].opacity = down ? 0.6 : 1.0;
		_faceText[i].foregroundColor = (down ? st.pressedText : st.glyph).CGColor;
	}

	const Control trig[2] = {Control::LTrigger, Control::RTrigger};
	const uint8_t values[2] = {v.l, v.r};
	const bool fulls[2] = {v.l_full, v.r_full};
	for (int i = 0; i < 2; i++) {
		const CGRect f = CG(_layout.frame(trig[i]));
		CGRect fill = f;
		fill.size.width = f.size.width * (values[i] / 255.0);
		if (i == 0)
			fill.origin.x = CGRectGetMaxX(f) - fill.size.width;   /* L fills toward the left edge */
		_trigMask[i].path = [UIBezierPath bezierPathWithRect:CGRectOffset(fill, -f.origin.x, -f.origin.y)].CGPath;
		_trigFill[i].opacity = fulls[i] ? 1.0 : 0.72;
		_trigShape[i].strokeColor = (fulls[i] ? kAccent() : kRim()).CGColor;
		_trigShape[i].lineWidth = fulls[i] ? 2.5 : 1.5;
		_trigShape[i].shadowColor = (fulls[i] ? kAccent() : UIColor.blackColor).CGColor;
		_trigShape[i].shadowOpacity = fulls[i] ? 0.8 : 0.28;
		_trigShape[i].shadowOffset = fulls[i] ? CGSizeZero : CGSizeMake(0, 2);
		_trigDetent[i].strokeColor = [UIColor colorWithWhite:1 alpha:values[i] > 0 ? 0.75 : 0.30].CGColor;
		_trigText[i].foregroundColor = (values[i] > 0 ? UIColor.whiteColor : kGlyph()).CGColor;
	}
	_startShape.fillColor = (v.start ? [UIColor colorWithWhite:0.95 alpha:0.95] : kSurface()).CGColor;
	_startText.foregroundColor = (v.start ? UIColor.blackColor : kGlyph()).CGColor;

	if (_editing) {
		UIBezierPath *all = [UIBezierPath bezierPath], *sel = [UIBezierPath bezierPath];
		const Control groups[] = {Control::Stick, Control::DPad, Control::Face, Control::LTrigger, Control::RTrigger, Control::Start};
		for (Control c : groups) {
			const CGRect f = CGRectInset(CG(_layout.frame(c)), -6, -6);
			UIBezierPath *p = [UIBezierPath bezierPathWithRoundedRect:f cornerRadius:14];
			[(c == _selected ? sel : all) appendPath:p];
		}
		_editOutlines.path = all.CGPath;
		_editSelected.path = sel.CGPath;
	}
	_editOutlines.hidden = !_editing;
	_editSelected.hidden = !_editing;
	[CATransaction commit];
}


- (void)updateAccessibility
{
	NSMutableArray *elements = [NSMutableArray array];
	auto add = [&](NSString *ident, NSString *label, CGRect frame) {
		UIAccessibilityElement *e = [[UIAccessibilityElement alloc] initWithAccessibilityContainer:self];
		e.accessibilityIdentifier = ident;
		e.accessibilityLabel = label;
		e.accessibilityTraits = UIAccessibilityTraitButton;
		e.accessibilityFrameInContainerSpace = frame;
		[elements addObject:e];
	};
	if (![self controlsHidden]) {
		add(@"dcpad.stick", @"Analog stick", CG(_layout.frame(Control::Stick)));
		add(@"dcpad.dpad", @"D-pad", CG(_layout.frame(Control::DPad)));
		const dcpad::Face faces[4] = {dcpad::FaceA, dcpad::FaceB, dcpad::FaceX, dcpad::FaceY};
		for (int i = 0; i < 4; i++) {
			float cx, cy;
			_layout.face_center(faces[i], &cx, &cy);
			const CGFloat r = _layout.face_radius;
			add([@"dcpad." stringByAppendingString:FaceIds()[i]], FaceIds()[i], CGRectMake(cx - r, cy - r, 2 * r, 2 * r));
		}
		add(@"dcpad.L", @"L trigger", CG(_layout.frame(Control::LTrigger)));
		add(@"dcpad.R", @"R trigger", CG(_layout.frame(Control::RTrigger)));
		add(@"dcpad.start", @"Start", CG(_layout.frame(Control::Start)));
	}
	[elements addObject:_menuButton];
	if (_messageVisible)
		[elements addObject:_message];
	if (!_panel.hidden)
		[elements addObject:_panel];
	if (!_editorBar.hidden)
		[elements addObject:_editorBar];
	self.accessibilityElements = elements;
}

#pragma mark Touches

- (int)touchId:(UITouch *)touch create:(BOOL)create
{
	void *key = (__bridge void *)touch;
	auto it = _touchIds.find(key);
	if (it != _touchIds.end())
		return it->second;
	if (!create)
		return 0;
	_touchIds[key] = _nextTouchId;
	return _nextTouchId++;
}

- (void)publish
{
	sonicpad::DreamcastInputMixer::shared().setState(_engine.state(), true);
	if (_settings.haptics && _engine.take_press_events() > 0)
		[_haptic impactOccurred];
	else
		_engine.take_press_events();
	if (_engine.active_count() > 0)
		_dimmed = NO;
	else
		[self scheduleIdleDim];
	[self render];
}

// After a few seconds with no finger down the controls fade back so cutscenes
// and scenery read clearly; the next touch restores them at once.
- (void)scheduleIdleDim
{
	const NSUInteger token = ++_dimToken;
	if (!_settings.dim_when_idle)
		return;
	dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(kIdleDimDelay * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
		if (token != self->_dimToken || self->_engine.active_count() > 0 || self.isModal || self->_dimmed || self->_previewing)
			return;
		self->_dimmed = YES;
		[CATransaction begin];
		[CATransaction setAnimationDuration:0.6];
		self->_controls.opacity = [self controlsHidden] ? 0 : self->_settings.opacity * kIdleDimFactor;
		[CATransaction commit];
	});
}

- (BOOL)inputBlocked
{
	return _menuOpen || !_panel.hidden || [self controlsHidden];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
	if (_editing) {
		[self editTouchBegan:touches.anyObject];
		return;
	}
	if (!_panel.hidden) {
		// A tap outside the settings panel closes it instead of reaching the game.
		[self hideSettingsPanel];
		return;
	}
	if ([self inputBlocked])
		return;
	_dimmed = NO;
	[_haptic prepare];
	for (UITouch *touch in touches) {
		const CGPoint p = [touch locationInView:self];
		_engine.begin([self touchId:touch create:YES], p.x, p.y, _layout, _settings);
	}
	[self publish];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
	if (_editing) {
		[self editTouchMoved:touches.anyObject];
		return;
	}
	for (UITouch *touch in touches) {
		const int ident = [self touchId:touch create:NO];
		if (ident == 0)
			continue;
		const CGPoint p = [touch locationInView:self];
		_engine.move(ident, p.x, p.y, _layout, _settings);
	}
	[self publish];
}

- (void)finishTouches:(NSSet<UITouch *> *)touches
{
	if (_editing) {
		[self editTouchEnded];
		return;
	}
	for (UITouch *touch in touches) {
		const int ident = [self touchId:touch create:NO];
		if (ident != 0)
			_engine.end(ident);
		_touchIds.erase((__bridge void *)touch);
	}
	[self publish];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { [self finishTouches:touches]; }
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event { [self finishTouches:touches]; }

- (void)clearInput
{
	_engine.clear();
	_touchIds.clear();
	sonicpad::DreamcastInputMixer::shared().clear(true);
	_dimmed = NO;
	[self scheduleIdleDim];
	[self render];
}

- (void)setControllerConnected:(BOOL)connected
{
	if (_controllerConnected == connected)
		return;
	_controllerConnected = connected;
	if ([self controlsHidden])
		[self clearInput];
	[self render];
	[self updateAccessibility];
}

#pragma mark Modal state

- (BOOL)isModal
{
	return _menuOpen || !_panel.hidden || _editing;
}

- (void)modalChanged
{
	[self clearInput];
	if ([self.delegate respondsToSelector:@selector(dcpadOverlay:modalStateChanged:)])
		[self.delegate dcpadOverlay:self modalStateChanged:self.isModal];
	[self updateAccessibility];
}

- (void)showSettingsPanel
{
	if (_editing)
		[self endLayoutEditing];
	_opacitySlider.value = _settings.opacity;
	_sizeSlider.value = _settings.size;
	_travelSlider.value = _settings.stick_travel;
	_floatingSwitch.on = _settings.floating_stick;
	_hapticsSwitch.on = _settings.haptics;
	_hideSwitch.on = _settings.hide_with_controller;
	_dimSwitch.on = _settings.dim_when_idle;
	_panel.hidden = NO;
	[self bringSubviewToFront:_panel];
	[self modalChanged];
}

- (void)hideSettingsPanel
{
	_panel.hidden = YES;
	[self modalChanged];
}

- (void)settingsChanged:(id)sender
{
	_settings.opacity = _opacitySlider.value;
	_settings.size = _sizeSlider.value;
	_settings.stick_travel = _travelSlider.value;
	_settings.floating_stick = _floatingSwitch.on;
	_settings.haptics = _hapticsSwitch.on;
	_settings.hide_with_controller = _hideSwitch.on;
	_settings.dim_when_idle = _dimSwitch.on;
	[_store saveSettings:_settings phone:_phone];
	[self setNeedsLayout];
}

- (void)confirmReset
{
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Reset Touch Layout?"
			message:@"Every control returns to its default position and size on this device." preferredStyle:UIAlertControllerStyleAlert];
	[alert addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];
	__weak DCPadOverlayView *weakSelf = self;
	[alert addAction:[UIAlertAction actionWithTitle:@"Reset" style:UIAlertActionStyleDestructive handler:^(UIAlertAction *) {
		[weakSelf resetLayout];
	}]];
	[self.window.rootViewController presentViewController:alert animated:YES completion:nil];
}

- (void)resetLayout
{
	[_store resetLayoutForPhone:_phone];
	_settings = [_store settingsForPhone:_phone];
	_sizeSlider.value = _settings.size;
	[self setNeedsLayout];
}

#pragma mark Layout editing

- (NSString *)nameForControl:(Control)c
{
	switch (c) {
		case Control::Stick: return @"Analog stick";
		case Control::DPad: return @"D-pad";
		case Control::Face: return @"Face buttons";
		case Control::LTrigger: return @"L trigger";
		case Control::RTrigger: return @"R trigger";
		case Control::Start: return @"Start";
		default: return @"";
	}
}

- (void)beginLayoutEditing
{
	_panel.hidden = YES;
	_editing = YES;
	_selected = Control::None;
	_editorBar.hidden = NO;
	_editorSize.enabled = NO;
	_editorSize.hidden = YES;   // appears once a control is selected
	_editorLabel.text = @"Drag a control,\nthen size it here";
	[self bringSubviewToFront:_editorBar];
	[self modalChanged];
	[self render];
}

- (void)endLayoutEditing
{
	_editing = NO;
	_editorBar.hidden = YES;
	_selected = Control::None;
	_dragging = Control::None;
	[_store saveSettings:_settings phone:_phone];
	[self modalChanged];
	[self render];
}

- (Control)editableControlAt:(CGPoint)p
{
	const Control groups[] = {Control::Start, Control::LTrigger, Control::RTrigger, Control::DPad, Control::Face, Control::Stick};
	for (Control c : groups)
		if (CGRectContainsPoint(CGRectInset(CG(_layout.frame(c)), -12, -12), p))
			return c;
	return Control::None;
}

- (void)editTouchBegan:(UITouch *)touch
{
	const CGPoint p = [touch locationInView:self];
	_dragging = [self editableControlAt:p];
	_dragMoved = NO;
	if (_dragging == Control::None)
		return;
	const dcpad::Rect& f = _layout.frame(_dragging);
	_dragOffset = CGPointMake(p.x - f.midX(), p.y - f.midY());
}

- (void)editTouchMoved:(UITouch *)touch
{
	if (_dragging == Control::None)
		return;
	const CGPoint p = [touch locationInView:self];
	const CGRect safe = [self safeRect];
	if (safe.size.width <= 0 || safe.size.height <= 0)
		return;
	dcpad::ControlOverride& o = _settings.over(_dragging);
	o.has_position = true;
	o.nx = std::clamp(float((p.x - _dragOffset.x - safe.origin.x) / safe.size.width), 0.0f, 1.0f);
	o.ny = std::clamp(float((p.y - _dragOffset.y - safe.origin.y) / safe.size.height), 0.0f, 1.0f);
	_dragMoved = YES;
	_selected = _dragging;
	[self setNeedsLayout];
	[self layoutIfNeeded];
}

- (void)editTouchEnded
{
	if (_dragging != Control::None) {
		_selected = _dragging;
		_editorSize.enabled = YES;
		_editorSize.hidden = NO;
		_editorSize.value = _settings.over(_selected).scale;
		_editorLabel.text = [NSString stringWithFormat:@"%@ size", [self nameForControl:_selected]];
		[_store saveSettings:_settings phone:_phone];
	}
	_dragging = Control::None;
	[self render];
}

- (void)editorSizeChanged:(UISlider *)slider
{
	if (_selected == Control::None)
		return;
	_settings.over(_selected).scale = slider.value;
	[_store saveSettings:_settings phone:_phone];
	[self setNeedsLayout];
}

@end
