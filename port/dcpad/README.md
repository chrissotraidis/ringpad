# DCPad: Dreamcast touch controls for Apple ports

DCPad is the touch control kit SonicPad uses, written so the next Dreamcast
port can reuse it unchanged. It turns multi-touch and Apple game controllers
into one Dreamcast controller state, and gives every port the same **•••**
menu, touch settings and layout editor.

The design follows SunPad, whose layout was accepted on a physical iPad: a
large stick low on the left, the D-pad above it, the face buttons low on the
right, L and R as analog sliders across the top corners, a persistent •••
button, a settings panel, Move Controls editing and per-device layouts.
DCPad adds what a Dreamcast pad needs:

| Control | Behaviour |
|---|---|
| Analog stick | Floating by default: touch anywhere in the lower-left area and the stick centres under your thumb. The base follows the thumb past the rim, so changing direction is instant. Full tilt at 85% of the radius. A fixed mode is in settings. |
| D-pad | One eight-way cross. Roll the thumb between directions without lifting; cardinals own 60° and diagonals 30°. Keeps tracking if the thumb drifts off the cross. |
| A / B / X / Y | Dreamcast diamond and colours (A red bottom, B blue right, X yellow left, Y green top). Slide between buttons without lifting. Resting between two neighbours presses both, but opposite buttons never chord. |
| L / R | Analog sliders. A light touch is already past the game's digital threshold; pressure grows toward the outer screen edge, and the last quarter is a full press with a haptic tap and hysteresis. |
| Start | Separate pill at the bottom centre, never part of the ••• menu. |
| ••• menu | The host adds its own items (SonicPad adds Import Game, Reset Game, Show FPS and About). DCPad adds Controls: Touch Control Settings, Move Controls and Controller Mapping. |

Each finger owns one control from touch-down to lift, so lifting one thumb
never releases another's input, and a second finger can't take over a stick,
D-pad or trigger that is already held. Haptics fire on new presses.

## Layout

- **Core** (`core/DCPad.{h,cpp}`): layout, hit testing and the per-touch engine in
  plain C++ with no UIKit. Host-tested on six device classes by
  `tests/host/test_dcpad.cpp`.
- **Apple** (`apple/`):
  - `DCPadOverlayView` draws the controls with Core Animation, handles touches and owns the menu, the settings panel and the layout editor.
  - `DCPadSettingsStore` saves settings per game namespace and per phone or tablet.
  - `DCPadControllerInput` feeds Apple game controllers into the same state, with player-1 ownership and face-button remapping.

Both inputs publish to `port/input/DreamcastInputMixer`. A port then adapts the
merged state to its own runtime, as SonicPad does with
`port/input/FlycastPadBridge`.

## Using it in another port

```objc
DCPadOverlayView *pad = [[DCPadOverlayView alloc] initWithFrame:view.bounds storageNamespace:@"MyGame"];
pad.menuTitle = @"MyPad";
pad.delegate = self;                 // menu items, pause on modal, controller mapping
[view addSubview:pad];

DCPadControllerInput *controllers = [[DCPadControllerInput alloc] initWithNamespace:@"MyGame"];
controllers.connectionChanged = ^(BOOL connected) { [pad setControllerConnected:connected]; };
[controllers start];

// Once per frame:
SonicPadDreamcastState state = sonicpad::DreamcastInputMixer::shared().consumeMerged();
sonicpad::ActionSnapshot snapshot = sonicpad::adapt(state);   // deadzone, trigger bits
```

Pause the game in `dcpadOverlay:modalStateChanged:`. The overlay has already
released all touch input by then.

For screenshots and design review, `DCPAD_PREVIEW="stick=0.7,-0.5 dpad=up face=AB l=255 r=128 start"`
draws a fixed pressed state.
