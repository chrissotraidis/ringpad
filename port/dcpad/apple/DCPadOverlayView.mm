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

struct FaceStyle { dcpad::Face face; NSString *name; UIColor *fill; UIColor *text; };

NSArray *FaceIds() { return @[@"A", @"B", @"X", @"Y"]; }

/* Dreamcast controller colours: A red, B blue, X yellow, Y green. */
FaceStyle FaceStyles(int i)
{
	switch (i) {
		case 0: return {dcpad::FaceA, @"A", RGBA(0.86, 0.19, 0.20, 1), UIColor.whiteColor};
		case 1: return {dcpad::FaceB, @"B", RGBA(0.18, 0.43, 0.88, 1), UIColor.whiteColor};
		case 2: return {dcpad::FaceX, @"X", RGBA(0.96, 0.77, 0.17, 1), RGBA(0.18, 0.14, 0.02, 1)};
		default: return {dcpad::FaceY, @"Y", RGBA(0.22, 0.67, 0.33, 1), UIColor.whiteColor};
	}
}

UIColor *const kAccent() { return RGBA(0.96, 0.45, 0.13, 1); }   /* Dreamcast swirl orange */

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

UIBezierPath *CrossPath(CGRect f)
{
	const CGFloat arm = f.size.width * 0.36, r = arm * 0.22;
	UIBezierPath *v = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(CGRectGetMidX(f) - arm / 2, f.origin.y, arm, f.size.height) cornerRadius:r];
	UIBezierPath *h = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(f.origin.x, CGRectGetMidY(f) - arm / 2, f.size.width, arm) cornerRadius:r];
	[v appendPath:h];
	v.usesEvenOddFillRule = NO;
	return v;
}

CGRect ArmRect(CGRect f, uint8_t dir)
{
	const CGFloat arm = f.size.width * 0.36, len = (f.size.width - arm) / 2;
	switch (dir) {
		case dcpad::DPadUp: return CGRectMake(CGRectGetMidX(f) - arm / 2, f.origin.y, arm, len);
		case dcpad::DPadDown: return CGRectMake(CGRectGetMidX(f) - arm / 2, CGRectGetMaxY(f) - len, arm, len);
		case dcpad::DPadLeft: return CGRectMake(f.origin.x, CGRectGetMidY(f) - arm / 2, len, arm);
		default: return CGRectMake(CGRectGetMaxX(f) - len, CGRectGetMidY(f) - arm / 2, len, arm);
	}
}

UIBezierPath *ArrowPath(CGRect arm, uint8_t dir)
{
	const CGFloat s = MIN(arm.size.width, arm.size.height) * 0.30;
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
	CAShapeLayer *_stickBase, *_stickRing, *_stickThumb;
	CAShapeLayer *_dpadShape, *_dpadArms[4], *_dpadArrows;
	CALayer *_face[4];
	CAShapeLayer *_faceCircle[4];
	CATextLayer *_faceText[4];
	CAShapeLayer *_trigShape[2], *_trigFill[2];
	CATextLayer *_trigText[2];
	CAShapeLayer *_startShape;
	CATextLayer *_startText;
	CAShapeLayer *_editOutlines, *_editSelected;

	DCPadMenuButton *_menuButton;
	UIView *_panel;
	UISlider *_opacitySlider, *_sizeSlider;
	UISwitch *_floatingSwitch, *_hapticsSwitch, *_hideSwitch;
	UIView *_editorBar;
	UILabel *_editorLabel;
	UISlider *_editorSize;
	UIImpactFeedbackGenerator *_haptic;
	UIStackView *_panelStack;
	CGFloat _panelContentHeight;
	dcpad::Visual _preview;
	BOOL _previewing;
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
	[self loadPreview];
	return self;
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
	_stickBase = shape(_controls);
	_stickBase.fillColor = [UIColor colorWithWhite:0.05 alpha:0.42].CGColor;
	_stickBase.strokeColor = [UIColor colorWithWhite:1 alpha:0.55].CGColor;
	_stickBase.lineWidth = 2.5;
	_stickRing = shape(_controls);
	_stickRing.fillColor = UIColor.clearColor.CGColor;
	_stickRing.strokeColor = [UIColor colorWithWhite:1 alpha:0.16].CGColor;
	_stickRing.lineWidth = 1.5;
	_stickThumb = shape(_controls);
	_stickThumb.fillColor = [UIColor colorWithWhite:0.78 alpha:0.78].CGColor;
	_stickThumb.strokeColor = [UIColor colorWithWhite:1 alpha:0.9].CGColor;
	_stickThumb.lineWidth = 2;
	_stickThumb.shadowColor = UIColor.blackColor.CGColor;
	_stickThumb.shadowOpacity = 0.35;
	_stickThumb.shadowRadius = 4;
	_stickThumb.shadowOffset = CGSizeMake(0, 2);

	_dpadShape = shape(_controls);
	_dpadShape.fillColor = [UIColor colorWithWhite:0.08 alpha:0.55].CGColor;
	_dpadShape.strokeColor = [UIColor colorWithWhite:1 alpha:0.55].CGColor;
	_dpadShape.lineWidth = 2.5;
	for (int i = 0; i < 4; i++) {
		_dpadArms[i] = shape(_controls);
		_dpadArms[i].fillColor = [kAccent() colorWithAlphaComponent:0.75].CGColor;
		_dpadArms[i].hidden = YES;
	}
	_dpadArrows = shape(_controls);
	_dpadArrows.fillColor = [UIColor colorWithWhite:1 alpha:0.85].CGColor;

	for (int i = 0; i < 4; i++) {
		const FaceStyle st = FaceStyles(i);
		_face[i] = [CALayer layer];
		[_controls addSublayer:_face[i]];
		_faceCircle[i] = shape(_face[i]);
		_faceCircle[i].fillColor = [st.fill colorWithAlphaComponent:0.82].CGColor;
		_faceCircle[i].strokeColor = [UIColor colorWithWhite:1 alpha:0.7].CGColor;
		_faceCircle[i].lineWidth = 2.5;
		_faceCircle[i].shadowColor = st.fill.CGColor;
		_faceCircle[i].shadowRadius = 10;
		_faceCircle[i].shadowOffset = CGSizeZero;
		_faceCircle[i].shadowOpacity = 0;
		_faceText[i] = TextLayer(st.name, 30, st.text, scale);
		[_face[i] addSublayer:_faceText[i]];
	}

	for (int i = 0; i < 2; i++) {
		_trigShape[i] = shape(_controls);
		_trigShape[i].fillColor = [UIColor colorWithWhite:0.08 alpha:0.55].CGColor;
		_trigShape[i].strokeColor = [UIColor colorWithWhite:1 alpha:0.55].CGColor;
		_trigShape[i].lineWidth = 2.5;
		_trigFill[i] = shape(_controls);
		_trigFill[i].fillColor = [kAccent() colorWithAlphaComponent:0.70].CGColor;
		_trigText[i] = TextLayer(i == 0 ? @"L" : @"R", 22, UIColor.whiteColor, scale);
		[_controls addSublayer:_trigText[i]];
	}

	_startShape = shape(_controls);
	_startShape.fillColor = [UIColor colorWithWhite:0.08 alpha:0.55].CGColor;
	_startShape.strokeColor = [UIColor colorWithWhite:1 alpha:0.55].CGColor;
	_startShape.lineWidth = 2;
	_startText = TextLayer(@"START", 14, UIColor.whiteColor, scale);
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
	background.backgroundColor = [UIColor colorWithWhite:0.06 alpha:0.72];
	background.cornerRadius = 22;
	background.strokeColor = [UIColor colorWithWhite:1 alpha:0.30];
	background.strokeWidth = 1;
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
	return @[[UIMenu menuWithTitle:self.menuTitle image:nil identifier:nil options:UIMenuOptionsDisplayInline children:items]];
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
	config.baseBackgroundColor = destructive ? [UIColor colorWithWhite:0.22 alpha:1] : RGBA(0.13, 0.47, 0.84, 1);
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
	_floatingSwitch = [UISwitch new];
	_floatingSwitch.accessibilityIdentifier = @"dcpad.settings.floating";
	[_floatingSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_hapticsSwitch = [UISwitch new];
	_hapticsSwitch.accessibilityIdentifier = @"dcpad.settings.haptics";
	[_hapticsSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];
	_hideSwitch = [UISwitch new];
	_hideSwitch.accessibilityIdentifier = @"dcpad.settings.hideWithController";
	[_hideSwitch addTarget:self action:@selector(settingsChanged:) forControlEvents:UIControlEventValueChanged];

	UIButton *move = [self panelButton:@"Move Controls" identifier:@"dcpad.settings.move" action:@selector(beginLayoutEditing) destructive:NO];
	UIButton *reset = [self panelButton:@"Reset Layout" identifier:@"dcpad.settings.reset" action:@selector(confirmReset) destructive:YES];
	UIStackView *buttons = [[UIStackView alloc] initWithArrangedSubviews:@[move, reset]];
	buttons.spacing = 10;
	buttons.distribution = UIStackViewDistributionFillEqually;

	UIStackView *stack = [[UIStackView alloc] initWithArrangedSubviews:@[header,
		[self rowWithTitle:@"Opacity" control:_opacitySlider],
		[self rowWithTitle:@"Size" control:_sizeSlider],
		[self rowWithTitle:@"Floating stick" control:_floatingSwitch],
		[self rowWithTitle:@"Haptics" control:_hapticsSwitch],
		[self rowWithTitle:@"Hide with controller" control:_hideSwitch],
		buttons]];
	stack.axis = UILayoutConstraintAxisVertical;
	stack.spacing = 10;
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
		[stack.topAnchor constraintEqualToAnchor:scroll.contentLayoutGuide.topAnchor constant:14],
		[stack.bottomAnchor constraintEqualToAnchor:scroll.contentLayoutGuide.bottomAnchor constant:-16],
		[stack.widthAnchor constraintEqualToAnchor:scroll.frameLayoutGuide.widthAnchor constant:-36],
		[close.widthAnchor constraintEqualToConstant:34],
		[close.heightAnchor constraintEqualToConstant:34],
		[buttons.heightAnchor constraintEqualToConstant:42],
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
	[_editorSize addTarget:self action:@selector(editorSizeChanged:) forControlEvents:UIControlEventValueChanged];
	UIButtonConfiguration *doneConfig = [UIButtonConfiguration filledButtonConfiguration];
	doneConfig.title = @"Done";
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
			withHorizontalFittingPriority:UILayoutPriorityRequired verticalFittingPriority:UILayoutPriorityFittingSizeLevel].height + 30;
	const CGFloat panelW = MIN(380, safe.size.width - 32);
	const CGFloat panelH = MIN(_panelContentHeight > 0 ? _panelContentHeight : 360, safe.size.height - 40);
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
	const CGFloat r = _layout.stick_radius;
	_stickBase.path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(-r, -r, 2 * r, 2 * r)].CGPath;
	const CGFloat ring = r * 0.62;
	_stickRing.path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(-ring, -ring, 2 * ring, 2 * ring)].CGPath;
	const CGFloat t = r * 0.44;
	_stickThumb.path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(-t, -t, 2 * t, 2 * t)].CGPath;

	const CGRect dpad = CG(_layout.frame(Control::DPad));
	_dpadShape.path = CrossPath(dpad).CGPath;
	UIBezierPath *arrows = [UIBezierPath bezierPath];
	for (int i = 0; i < 4; i++) {
		const CGRect arm = ArmRect(dpad, kDirs[i]);
		_dpadArms[i].path = [UIBezierPath bezierPathWithRoundedRect:CGRectInset(arm, 3, 3) cornerRadius:arm.size.width * 0.12].CGPath;
		[arrows appendPath:ArrowPath(arm, kDirs[i])];
	}
	_dpadArrows.path = arrows.CGPath;

	const CGFloat fr = _layout.face_radius;
	const dcpad::Face faces[4] = {dcpad::FaceA, dcpad::FaceB, dcpad::FaceX, dcpad::FaceY};
	for (int i = 0; i < 4; i++) {
		float cx, cy;
		_layout.face_center(faces[i], &cx, &cy);
		_face[i].bounds = CGRectMake(0, 0, 2 * fr, 2 * fr);
		_face[i].position = CGPointMake(cx, cy);
		_faceCircle[i].path = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(1.25, 1.25, 2 * fr - 2.5, 2 * fr - 2.5)].CGPath;
		CenterText(_faceText[i], _face[i].bounds, fr * 0.95);
	}

	const Control trig[2] = {Control::LTrigger, Control::RTrigger};
	for (int i = 0; i < 2; i++) {
		const CGRect f = CG(_layout.frame(trig[i]));
		_trigShape[i].path = [UIBezierPath bezierPathWithRoundedRect:f cornerRadius:f.size.height / 2].CGPath;
		CenterText(_trigText[i], f, f.size.height * 0.45);
	}
	const CGRect start = CG(_layout.frame(Control::Start));
	_startShape.path = [UIBezierPath bezierPathWithRoundedRect:start cornerRadius:start.size.height / 2].CGPath;
	CenterText(_startText, start, start.size.height * 0.36);
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
	_controls.opacity = [self controlsHidden] ? 0 : (_editing ? 1.0 : _settings.opacity);

	const CGPoint base = _previewing ? CGPointMake(_layout.frame(Control::Stick).midX(), _layout.frame(Control::Stick).midY())
	                                 : CGPointMake(v.stick_base_x, v.stick_base_y);
	_stickBase.position = base;
	_stickRing.position = base;
	const CGFloat travel = _layout.stick_radius * (1 - 0.44) - 2;
	_stickThumb.position = CGPointMake(base.x + v.stick_x * travel, base.y + v.stick_y * travel);
	const CGFloat idle = _settings.floating_stick && !v.stick_active ? 0.75 : 1.0;
	_stickBase.opacity = idle;
	_stickRing.opacity = idle;
	_stickThumb.fillColor = (v.stick_active ? [UIColor colorWithWhite:0.92 alpha:0.92] : [UIColor colorWithWhite:0.78 alpha:0.78]).CGColor;

	for (int i = 0; i < 4; i++)
		_dpadArms[i].hidden = (v.dpad & kDirs[i]) == 0;

	const uint8_t faceBits[4] = {dcpad::FaceA, dcpad::FaceB, dcpad::FaceX, dcpad::FaceY};
	for (int i = 0; i < 4; i++) {
		const BOOL down = (v.face & faceBits[i]) != 0;
		const FaceStyle st = FaceStyles(i);
		_face[i].transform = down ? CATransform3DMakeScale(0.9, 0.9, 1) : CATransform3DIdentity;
		_faceCircle[i].fillColor = [st.fill colorWithAlphaComponent:down ? 1.0 : 0.82].CGColor;
		_faceCircle[i].strokeColor = [UIColor colorWithWhite:1 alpha:down ? 1.0 : 0.7].CGColor;
		_faceCircle[i].shadowOpacity = down ? 0.9 : 0;
	}

	const Control trig[2] = {Control::LTrigger, Control::RTrigger};
	const uint8_t values[2] = {v.l, v.r};
	const bool fulls[2] = {v.l_full, v.r_full};
	for (int i = 0; i < 2; i++) {
		const CGRect f = CG(_layout.frame(trig[i]));
		const CGFloat frac = values[i] / 255.0;
		CGRect fill = f;
		fill.size.width = f.size.width * frac;
		if (i == 0)
			fill.origin.x = CGRectGetMaxX(f) - fill.size.width;   /* L fills toward the left edge */
		UIBezierPath *clip = [UIBezierPath bezierPathWithRoundedRect:f cornerRadius:f.size.height / 2];
		UIBezierPath *path = [UIBezierPath bezierPathWithRect:fill];
		CAShapeLayer *mask = [CAShapeLayer layer];
		mask.path = clip.CGPath;
		_trigFill[i].path = path.CGPath;
		_trigFill[i].mask = mask;
		_trigFill[i].fillColor = [kAccent() colorWithAlphaComponent:fulls[i] ? 0.95 : 0.6].CGColor;
		_trigShape[i].strokeColor = (fulls[i] ? kAccent() : [UIColor colorWithWhite:1 alpha:0.55]).CGColor;
		_trigShape[i].lineWidth = fulls[i] ? 3.5 : 2.5;
	}
	_startShape.fillColor = (v.start ? [UIColor colorWithWhite:0.95 alpha:0.95] : [UIColor colorWithWhite:0.08 alpha:0.55]).CGColor;
	_startText.foregroundColor = (v.start ? UIColor.blackColor : UIColor.whiteColor).CGColor;

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
	[self render];
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
	_floatingSwitch.on = _settings.floating_stick;
	_hapticsSwitch.on = _settings.haptics;
	_hideSwitch.on = _settings.hide_with_controller;
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
	_settings.floating_stick = _floatingSwitch.on;
	_settings.haptics = _hapticsSwitch.on;
	_settings.hide_with_controller = _hideSwitch.on;
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
