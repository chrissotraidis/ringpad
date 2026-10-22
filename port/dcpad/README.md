# DCPad: Dreamcast touch controls for Apple ports

DCPad is the touch control kit SonicPad uses, written so the next Dreamcast
port can reuse it unchanged. It turns multi-touch and Apple game controllers
into one Dreamcast controller state, and gives every port the same **•••**
menu, touch settings and layout editor.

DCPad keeps SunPad's mechanisms (a persistent ••• button, a settings panel,
Move Controls editing and per-device layouts) and lays the controls out for
how the device is held:

- **Where thumbs rest.** In landscape a thumb pivots on the side edge: on a
  tablet held by its sides about 45 mm above the bottom, on a phone near the
  lower corner. `thumb_pivot()` encodes this, and the host test checks that
  every default control is 15–60 mm from it on six iPad and iPhone sizes
  (24–54 mm today). The stick, the most used control, sits where the thumb
  rests (under 45 mm).
- **The Dreamcast arrangement.** As on the real controller, the stick is high
  and the D-pad is below and inward of it. L and R sit at the top of each
  thumb's arc on the side edges, the face diamond is under the right thumb and
  Start just inward of it. The middle of the screen stays clear.
- **Anchored to the corners.** Defaults are measured in points from the bottom
  corners rather than as fractions of the screen, because thumbs do not grow
  with the screen: a 13-inch iPad keeps the same reach as an 11-inch one.

DCPad adds what a Dreamcast pad needs:

| Control | Behaviour |
|---|---|
| Analog stick | Floating by default: touch anywhere in the lower-left area and the stick centres under your thumb. The base stays where the thumb landed; past the rim the knob stops at the edge and the stick holds full tilt in that direction. Full tilt at 85% of the radius by default; **Stick travel** in settings sets it from 50% to 100%. A fixed mode is in settings. A direction arc on the rim shows where the stick points. |
| D-pad | One eight-way cross. Roll the thumb between directions without lifting; cardinals own 60° and diagonals 30°. Keeps tracking if the thumb drifts off the cross. Pressed arms light up inside the cross. |
| A / B / X / Y | Dreamcast diamond and colours (A red bottom, B blue right, X yellow left, Y green top). Slide between buttons without lifting. Resting between two neighbours presses both, but opposite buttons never chord. |
| L / R | Analog sliders. A light touch is already past the game's digital threshold; pressure grows toward the outer screen edge, and past the rim notches (72% of the length) is a full press with a haptic tap, a glow and hysteresis. |
| Start | Separate pill beside the right thumb, never part of the ••• menu. |
| ••• menu | The host adds its own items and DCPad adds Controls: Touch Control Settings, Move Controls and Controller Mapping. SonicPad's items come from `port/apple/ios/SPPortMenu`, which any port on the same Flycast foundation can reuse: a top row with Save State, Load State (three dated slots) and Reset; Display (render resolution, Show FPS); Sound (Full, Half, Off); Game Files (Import Game, Export VMU Save); Help (About, Share Diagnostic Log). `showMessage:` confirms actions such as "Saved to Slot 1". |

Each finger owns one control from touch-down to lift, so lifting one thumb
never releases another's input, and a second finger can't take over a stick,
D-pad or trigger that is already held. Haptics fire on new presses.

All controls share one look: dark glass with a thin light rim and a soft
shadow, so they read over both bright and dark scenes. Anything held turns
Dreamcast orange; face buttons keep the controller's colours. The settings
panel, the editor bar and the ••• button use the same palette.
After four seconds untouched the controls fade back (the **Dim when idle**
setting) and the next touch restores them.

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

Add the library to the app target. It needs only `port/input` for the shared
controller state (set `DCPAD_INPUT_DIR` if that folder lives elsewhere):

```cmake
add_subdirectory(port/dcpad)
target_link_libraries(my_app PRIVATE dcpad)
```

Then create the overlay and the controller input:

```objc
DCPadOverlayView *pad = [[DCPadOverlayView alloc] initWithFrame:view.bounds storageNamespace:@"MyGame"];
pad.menuTitle = @"MyPad";
pad.delegate = self;                 // menu items, pause on modal, controller mapping
[view addSubview:pad];
// To place DCPad's Controls submenu among your own sections, implement
// -dcpadOverlay:menuElementsWithControls: and return the whole menu.

DCPadControllerInput *controllers = [[DCPadControllerInput alloc] initWithNamespace:@"MyGame"];
controllers.connectionChanged = ^(BOOL connected) { [pad setControllerConnected:connected]; };
[controllers start];

// Once per frame:
SonicPadDreamcastState state = sonicpad::DreamcastInputMixer::shared().consumeMerged();
sonicpad::ActionSnapshot snapshot = sonicpad::adapt(state);   // deadzone, trigger bits
```

Pause the game in `dcpadOverlay:modalStateChanged:`. The overlay has already
released all touch input by then.

In the hosting view controller, defer the system's edge gestures so a thumb
that starts at the screen edge reaches the controls first, and hide the home
indicator:

```objc
- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures { return UIRectEdgeAll; }
- (BOOL)prefersHomeIndicatorAutoHidden { return YES; }
```

For screenshots and design review, `DCPAD_PREVIEW="stick=0.7,-0.5 dpad=up face=AB l=255 r=128 start"`
draws a fixed pressed state (use `dpad=upright` for a diagonal). SonicPad's
`tests/ios/DCPadCaptureTests.m` captures idle, pressed, menu, settings and
Move Controls screenshots with it on any Simulator.
