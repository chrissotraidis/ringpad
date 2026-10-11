// DCPad physical controller input. See DCPadControllerInput.h.
#import "DCPadControllerInput.h"

#import <GameController/GameController.h>

#include "DreamcastControllerMapping.h"
#include "DreamcastControllerSlots.h"
#include "DreamcastInputMixer.h"

#include <cmath>
#include <cstdlib>
#include <vector>

using namespace sonicpad;

static uintptr_t InstanceID(GCController *controller)
{
	return (uintptr_t)(__bridge void *)controller;
}

@implementation DCPadControllerInput
{
	NSString *_key;
	DreamcastControllerSlots _slots;
	NSMutableDictionary<NSNumber *, GCController *> *_configured;
	BOOL _connected;
}

- (instancetype)initWithNamespace:(NSString *)storageNamespace
{
	if ((self = [super init])) {
		_key = [NSString stringWithFormat:@"DCPad.%@.controllerMapping", storageNamespace];
		_configured = [NSMutableDictionary dictionary];
	}
	return self;
}

- (void)dealloc
{
	[NSNotificationCenter.defaultCenter removeObserver:self];
}

- (BOOL)connected
{
	return _connected;
}

- (DreamcastControllerMapping)mapping
{
	NSArray *saved = [NSUserDefaults.standardUserDefaults arrayForKey:_key];
	DreamcastControllerMapping m = default_controller_mapping();
	if (saved.count == 4) {
		m.gameA = [saved[0] unsignedCharValue];
		m.gameB = [saved[1] unsignedCharValue];
		m.gameX = [saved[2] unsignedCharValue];
		m.gameY = [saved[3] unsignedCharValue];
	}
	return mapping_is_valid(m) ? m : default_controller_mapping();
}

- (void)setMapping:(DreamcastControllerMapping)m
{
	[NSUserDefaults.standardUserDefaults setObject:@[@(m.gameA), @(m.gameB), @(m.gameX), @(m.gameY)] forKey:_key];
}

- (void)start
{
	[NSNotificationCenter.defaultCenter addObserver:self selector:@selector(controllersChanged:)
			name:GCControllerDidConnectNotification object:nil];
	[NSNotificationCenter.defaultCenter addObserver:self selector:@selector(controllersChanged:)
			name:GCControllerDidDisconnectNotification object:nil];
	[self reconcile];
}

- (void)controllersChanged:(NSNotification *)note
{
	dispatch_async(dispatch_get_main_queue(), ^{ [self reconcile]; });
}

// The Simulator can report virtual controllers that would hide the touch
// controls during testing, so only real hardware counts there by default.
static BOOL UsableController(GCController *c)
{
#if TARGET_OS_SIMULATOR
	if (getenv("DCPAD_SIMULATOR_CONTROLLERS") == nullptr)
		return NO;
#endif
	return c.extendedGamepad != nil;
}

- (void)reconcile
{
	std::vector<uintptr_t> current;
	for (GCController *c in GCController.controllers)
		if (UsableController(c))
			current.push_back(InstanceID(c));
	const ControllerReconcileResult result = _slots.reconcile(current);
	for (const ControllerSlotChange& removed : result.removed) {
		GCController *stale = _configured[@(removed.instance)];
		stale.extendedGamepad.valueChangedHandler = nil;
		[_configured removeObjectForKey:@(removed.instance)];
		if (removed.slot == 0)
			DreamcastInputMixer::shared().clear(false);
	}
	for (GCController *c in GCController.controllers) {
		if (!UsableController(c))
			continue;
		const int slot = _slots.slot_for(InstanceID(c));
		if (slot < 0 || _configured[@(InstanceID(c))] == c)
			continue;
		_configured[@(InstanceID(c))] = c;
		c.playerIndex = (GCControllerPlayerIndex)slot;
		if (slot != 0)
			continue;
		__weak DCPadControllerInput *weakSelf = self;
		c.extendedGamepad.valueChangedHandler = ^(GCExtendedGamepad *pad, GCControllerElement *element) {
			dispatch_async(dispatch_get_main_queue(), ^{ [weakSelf publish:pad]; });
		};
		[self publish:c.extendedGamepad];
	}
	const BOOL connected = _slots.instance_at(0) != 0;
	if (connected != _connected) {
		_connected = connected;
		if (self.connectionChanged)
			self.connectionChanged(connected);
	}
}

- (void)publish:(GCExtendedGamepad *)pad
{
	GCController *owner = _configured[@(_slots.instance_at(0))];
	if (owner == nil || owner.extendedGamepad != pad)
		return;
	const DreamcastControllerMapping m = [self mapping];
	uint8_t physical = 0;
	if (pad.buttonA.isPressed) physical |= DCPhysical_A;
	if (pad.buttonB.isPressed) physical |= DCPhysical_B;
	if (pad.buttonX.isPressed) physical |= DCPhysical_X;
	if (pad.buttonY.isPressed) physical |= DCPhysical_Y;
	if (pad.leftShoulder.isPressed) physical |= DCPhysical_ShoulderL;
	if (pad.rightShoulder.isPressed) physical |= DCPhysical_ShoulderR;
	SonicPadDreamcastState s{};
	s.connected = 1;
	s.buttons = apply_controller_mapping(m, physical);
	if (pad.dpad.up.isPressed) s.buttons |= SonicPadDC_Up;
	if (pad.dpad.down.isPressed) s.buttons |= SonicPadDC_Down;
	if (pad.dpad.left.isPressed) s.buttons |= SonicPadDC_Left;
	if (pad.dpad.right.isPressed) s.buttons |= SonicPadDC_Right;
	if (pad.buttonMenu.isPressed) s.buttons |= SonicPadDC_Start;
	s.moveX = int8_t(std::lround(pad.leftThumbstick.xAxis.value * 127.0f));
	s.moveY = int8_t(std::lround(pad.leftThumbstick.yAxis.value * 127.0f));
	/* Shoulders act as full trigger presses unless mapped to a face button. */
	const uint8_t mapped = m.gameA | m.gameB | m.gameX | m.gameY;
	float l = pad.leftTrigger.value, r = pad.rightTrigger.value;
	if (pad.leftShoulder.isPressed && !(mapped & DCPhysical_ShoulderL)) l = 1.0f;
	if (pad.rightShoulder.isPressed && !(mapped & DCPhysical_ShoulderR)) r = 1.0f;
	s.triggerL = uint8_t(std::lround(l * 255.0f));
	s.triggerR = uint8_t(std::lround(r * 255.0f));
	DreamcastInputMixer::shared().setState(s, false);
}

- (void)presentMappingFrom:(UIViewController *)presenter
{
	const DreamcastControllerMapping m = [self mapping];
	GCController *owner = _configured[@(_slots.instance_at(0))];
	NSString *message = owner != nil
		? [NSString stringWithFormat:@"Connected: %@. Choose a Dreamcast button to change which controller button presses it. The stick, D-pad, triggers and Start stay fixed.", owner.vendorName ?: @"controller"]
		: @"No controller is connected. You can still review or reset the mapping.";
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Controller Mapping" message:message
			preferredStyle:UIAlertControllerStyleAlert];
	const struct { DreamcastFace face; const char *name; uint8_t current; } faces[] = {
		{DreamcastFace::A, "A", m.gameA}, {DreamcastFace::B, "B", m.gameB},
		{DreamcastFace::X, "X", m.gameX}, {DreamcastFace::Y, "Y", m.gameY},
	};
	__weak DCPadControllerInput *weakSelf = self;
	for (const auto& f : faces) {
		const DreamcastFace face = f.face;
		NSString *title = [NSString stringWithFormat:@"Dreamcast %s — %s", f.name, physical_button_name(f.current)];
		[alert addAction:[UIAlertAction actionWithTitle:title style:UIAlertActionStyleDefault handler:^(UIAlertAction *) {
			[weakSelf presentChoicesForFace:face name:[NSString stringWithUTF8String:f.name] from:presenter];
		}]];
	}
	[alert addAction:[UIAlertAction actionWithTitle:@"Reset to Default" style:UIAlertActionStyleDestructive handler:^(UIAlertAction *) {
		[weakSelf setMapping:default_controller_mapping()];
		DreamcastInputMixer::shared().clear(false);
		[weakSelf presentMappingFrom:presenter];
	}]];
	[alert addAction:[UIAlertAction actionWithTitle:@"Done" style:UIAlertActionStyleCancel handler:nil]];
	[presenter presentViewController:alert animated:YES completion:nil];
}

- (void)presentChoicesForFace:(DreamcastFace)face name:(NSString *)name from:(UIViewController *)presenter
{
	UIAlertController *alert = [UIAlertController alertControllerWithTitle:[NSString stringWithFormat:@"Dreamcast %@", name]
			message:@"Choose the controller button. If it is already used, the two buttons swap."
			preferredStyle:UIAlertControllerStyleAlert];
	const uint8_t choices[] = {DCPhysical_A, DCPhysical_B, DCPhysical_X, DCPhysical_Y, DCPhysical_ShoulderL, DCPhysical_ShoulderR};
	__weak DCPadControllerInput *weakSelf = self;
	for (uint8_t physical : choices) {
		[alert addAction:[UIAlertAction actionWithTitle:[NSString stringWithUTF8String:physical_button_name(physical)]
				style:UIAlertActionStyleDefault handler:^(UIAlertAction *) {
			DCPadControllerInput *strong = weakSelf;
			[strong setMapping:mapping_by_assigning([strong mapping], face, physical)];
			DreamcastInputMixer::shared().clear(false);
			[strong presentMappingFrom:presenter];
		}]];
	}
	[alert addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *) {
		[weakSelf presentMappingFrom:presenter];
	}]];
	[presenter presentViewController:alert animated:YES completion:nil];
}

@end
