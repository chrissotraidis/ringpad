// SonicPad single-stick Dreamcast touch overlay. See SPTouchOverlayView.h.
#import "SPTouchOverlayView.h"

#include "DreamcastInputMixer.h"
#include "DreamcastTouchLayout.h"

#include <cmath>
#include <map>

using namespace sonicpad;

namespace {

NSString *ControlName(TouchControl c)
{
	switch (c) {
		case TouchControl::Start: return @"Start";
		case TouchControl::A: return @"A";
		case TouchControl::B: return @"B";
		case TouchControl::X: return @"X";
		case TouchControl::Y: return @"Y";
		case TouchControl::LeftShoulder: return @"L";
		case TouchControl::RightShoulder: return @"R";
		case TouchControl::DpadUp: return @"Up";
		case TouchControl::DpadDown: return @"Down";
		case TouchControl::DpadLeft: return @"Left";
		case TouchControl::DpadRight: return @"Right";
		case TouchControl::Move: return @"Move";
		default: return @"";
	}
}

NSString *ControlGlyph(TouchControl c)
{
	switch (c) {
		case TouchControl::Start: return @"START";
		case TouchControl::DpadUp: return @"\u25B2";
		case TouchControl::DpadDown: return @"\u25BC";
		case TouchControl::DpadLeft: return @"\u25C0";
		case TouchControl::DpadRight: return @"\u25B6";
		case TouchControl::Move: return @"";
		default: return ControlName(c);
	}
}

unsigned ControlBit(TouchControl c)
{
	switch (c) {
		case TouchControl::Start: return SonicPadDC_Start;
		case TouchControl::A: return SonicPadDC_A;
		case TouchControl::B: return SonicPadDC_B;
		case TouchControl::X: return SonicPadDC_X;
		case TouchControl::Y: return SonicPadDC_Y;
		case TouchControl::DpadUp: return SonicPadDC_Up;
		case TouchControl::DpadDown: return SonicPadDC_Down;
		case TouchControl::DpadLeft: return SonicPadDC_Left;
		case TouchControl::DpadRight: return SonicPadDC_Right;
		default: return 0;
	}
}

} // namespace

@implementation SPTouchOverlayView
{
	TouchLayout _layout;
	DreamcastTouchArbiter _arbiter;
	std::map<void *, int> _touchIds;
	int _nextTouchId;
}

- (instancetype)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self) {
		self.backgroundColor = UIColor.clearColor;
		self.opaque = NO;
		self.multipleTouchEnabled = YES;
		self.contentMode = UIViewContentModeRedraw;
		_layout = default_touch_layout(UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPhone);
		_nextTouchId = 1;
	}
	return self;
}

- (CGRect)controlRect
{
	return UIEdgeInsetsInsetRect(self.bounds, self.safeAreaInsets);
}

- (CGRect)frameForPlacement:(const TouchPlacement &)p inRect:(CGRect)rect
{
	const CGFloat unit = MIN(rect.size.width, rect.size.height);
	const CGFloat r = p.radius * unit;
	const CGFloat cx = rect.origin.x + p.cx * rect.size.width;
	const CGFloat cy = rect.origin.y + p.cy * rect.size.height;
	return CGRectMake(cx - r, cy - r, 2 * r, 2 * r);
}

- (void)layoutSubviews
{
	[super layoutSubviews];
	const CGRect rect = [self controlRect];
	NSMutableArray *elements = [NSMutableArray array];
	for (const TouchPlacement& p : _layout) {
		UIAccessibilityElement *element = [[UIAccessibilityElement alloc] initWithAccessibilityContainer:self];
		element.accessibilityLabel = p.control == TouchControl::Move ? @"Movement stick" : ControlName(p.control);
		element.accessibilityIdentifier = [@"sp.control." stringByAppendingString:ControlName(p.control)];
		element.accessibilityTraits = UIAccessibilityTraitButton;
		element.accessibilityFrameInContainerSpace = [self frameForPlacement:p inRect:rect];
		[elements addObject:element];
	}
	self.accessibilityElements = elements;
	[self setNeedsDisplay];
}

- (int)idForTouch:(UITouch *)touch create:(BOOL)create
{
	void *key = (__bridge void *)touch;
	auto it = _touchIds.find(key);
	if (it != _touchIds.end())
		return it->second;
	if (!create)
		return 0;
	const int touchId = _nextTouchId++;
	_touchIds[key] = touchId;
	return touchId;
}

- (void)publish
{
	DreamcastInputMixer::shared().setState(_arbiter.state(), true);
	[self setNeedsDisplay];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
	const CGRect rect = [self controlRect];
	for (UITouch *touch in touches) {
		const CGPoint p = [touch locationInView:self];
		_arbiter.begin([self idForTouch:touch create:YES], p.x - rect.origin.x, p.y - rect.origin.y,
				_layout, rect.size.width, rect.size.height);
	}
	[self publish];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
	const CGRect rect = [self controlRect];
	for (UITouch *touch in touches) {
		const int touchId = [self idForTouch:touch create:NO];
		if (touchId == 0)
			continue;
		const CGPoint p = [touch locationInView:self];
		_arbiter.move(touchId, p.x - rect.origin.x, p.y - rect.origin.y, _layout, rect.size.width, rect.size.height);
	}
	[self publish];
}

- (void)finishTouches:(NSSet<UITouch *> *)touches
{
	for (UITouch *touch in touches) {
		const int touchId = [self idForTouch:touch create:NO];
		if (touchId != 0)
			_arbiter.end(touchId);
		_touchIds.erase((__bridge void *)touch);
	}
	[self publish];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
	[self finishTouches:touches];
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
	[self finishTouches:touches];
}

- (void)clearInput
{
	_arbiter.clear();
	_touchIds.clear();
	DreamcastInputMixer::shared().clear(true);
	[self setNeedsDisplay];
}

- (void)drawRect:(CGRect)dirty
{
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	const CGRect rect = [self controlRect];
	const SonicPadDreamcastState state = _arbiter.state();
	for (const TouchPlacement& p : _layout) {
		const CGRect frame = [self frameForPlacement:p inRect:rect];
		const CGFloat r = frame.size.width / 2;
		bool pressed = (state.buttons & ControlBit(p.control)) != 0;
		if (p.control == TouchControl::LeftShoulder)
			pressed = state.triggerL > 0;
		if (p.control == TouchControl::RightShoulder)
			pressed = state.triggerR > 0;
		CGContextSetFillColorWithColor(ctx, [UIColor colorWithWhite:1 alpha:pressed ? 0.38 : 0.12].CGColor);
		CGContextSetStrokeColorWithColor(ctx, [UIColor colorWithWhite:1 alpha:0.55].CGColor);
		CGContextSetLineWidth(ctx, 2);
		CGContextFillEllipseInRect(ctx, frame);
		CGContextStrokeEllipseInRect(ctx, CGRectInset(frame, 1, 1));
		if (p.control == TouchControl::Move) {
			const CGFloat knob = r * 0.42f;
			const CGFloat kx = CGRectGetMidX(frame) + (state.moveX / 127.0f) * (r - knob);
			const CGFloat ky = CGRectGetMidY(frame) - (state.moveY / 127.0f) * (r - knob);
			CGContextSetFillColorWithColor(ctx, [UIColor colorWithWhite:1 alpha:0.45].CGColor);
			CGContextFillEllipseInRect(ctx, CGRectMake(kx - knob, ky - knob, 2 * knob, 2 * knob));
			continue;
		}
		NSString *glyph = ControlGlyph(p.control);
		const CGFloat size = p.control == TouchControl::Start ? r * 0.42f : r * 0.75f;
		NSDictionary *attrs = @{
			NSFontAttributeName: [UIFont systemFontOfSize:size weight:UIFontWeightBold],
			NSForegroundColorAttributeName: [UIColor colorWithWhite:1 alpha:0.85],
		};
		const CGSize textSize = [glyph sizeWithAttributes:attrs];
		[glyph drawAtPoint:CGPointMake(CGRectGetMidX(frame) - textSize.width / 2, CGRectGetMidY(frame) - textSize.height / 2)
				withAttributes:attrs];
	}
}

@end
