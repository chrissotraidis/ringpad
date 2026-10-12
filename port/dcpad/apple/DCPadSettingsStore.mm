// DCPad settings persistence. See DCPadSettingsStore.h.
#import "DCPadSettingsStore.h"

static NSString *const kControlKeys[] = {@"stick", @"dpad", @"face", @"l", @"r", @"start"};

@implementation DCPadSettingsStore
{
	NSString *_namespace;
}

- (instancetype)initWithNamespace:(NSString *)storageNamespace
{
	if ((self = [super init]))
		_namespace = [storageNamespace copy];
	return self;
}

- (NSString *)keyForPhone:(BOOL)phone
{
	return [NSString stringWithFormat:@"DCPad.%@.%@", _namespace, phone ? @"phone" : @"tablet"];
}

- (dcpad::Settings)settingsForPhone:(BOOL)phone
{
	dcpad::Settings s;
	NSDictionary *d = [NSUserDefaults.standardUserDefaults dictionaryForKey:[self keyForPhone:phone]];
	if (d == nil)
		return s;
	if (NSNumber *v = d[@"opacity"]) s.opacity = std::clamp(v.floatValue, 0.25f, 1.0f);
	if (NSNumber *v = d[@"size"]) s.size = std::clamp(v.floatValue, 0.7f, 1.4f);
	if (NSNumber *v = d[@"stickTravel"]) s.stick_travel = std::clamp(v.floatValue, 0.5f, 1.0f);
	if (NSNumber *v = d[@"floating"]) s.floating_stick = v.boolValue;
	if (NSNumber *v = d[@"haptics"]) s.haptics = v.boolValue;
	if (NSNumber *v = d[@"hideWithController"]) s.hide_with_controller = v.boolValue;
	if (NSNumber *v = d[@"dimWhenIdle"]) s.dim_when_idle = v.boolValue;
	NSDictionary *overrides = d[@"controls"];
	for (int i = 0; i < dcpad::kControlCount; i++) {
		NSDictionary *o = overrides[kControlKeys[i]];
		if (![o isKindOfClass:NSDictionary.class])
			continue;
		dcpad::ControlOverride& c = s.overrides[i];
		if (o[@"x"] != nil && o[@"y"] != nil) {
			c.has_position = true;
			c.nx = std::clamp([o[@"x"] floatValue], 0.0f, 1.0f);
			c.ny = std::clamp([o[@"y"] floatValue], 0.0f, 1.0f);
		}
		if (o[@"scale"] != nil)
			c.scale = std::clamp([o[@"scale"] floatValue], 0.6f, 1.75f);
	}
	return s;
}

- (void)saveSettings:(const dcpad::Settings &)s phone:(BOOL)phone
{
	NSMutableDictionary *controls = [NSMutableDictionary dictionary];
	for (int i = 0; i < dcpad::kControlCount; i++) {
		const dcpad::ControlOverride& c = s.overrides[i];
		NSMutableDictionary *o = [NSMutableDictionary dictionary];
		if (c.has_position) {
			o[@"x"] = @(c.nx);
			o[@"y"] = @(c.ny);
		}
		if (c.scale != 1.0f)
			o[@"scale"] = @(c.scale);
		if (o.count > 0)
			controls[kControlKeys[i]] = o;
	}
	NSDictionary *d = @{
		@"opacity": @(s.opacity),
		@"size": @(s.size),
		@"stickTravel": @(s.stick_travel),
		@"floating": @(s.floating_stick),
		@"haptics": @(s.haptics),
		@"hideWithController": @(s.hide_with_controller),
		@"dimWhenIdle": @(s.dim_when_idle),
		@"controls": controls,
	};
	[NSUserDefaults.standardUserDefaults setObject:d forKey:[self keyForPhone:phone]];
}

- (void)resetLayoutForPhone:(BOOL)phone
{
	dcpad::Settings s = [self settingsForPhone:phone];
	s.size = 1.0f;
	s.overrides = {};
	[self saveSettings:s phone:phone];
}

@end
