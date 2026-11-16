# RingPad Dreamcast input contract

Updated **29 October 2026**. This describes the current Flycast-based runtime.
The canonical bit values originated in the earlier SARecomp research, but
SARecomp is not a build dependency or the active runtime boundary.

## Controller profile

One Dreamcast controller: **one analog movement stick**, a D-pad, A/B/X/Y,
independent analog L/R, and Start. There is no second analog stick. Start
sends the game's Start button; the **•••** button opens the host menu and
never sends Start.

| Control | Touch input | Physical controller input |
|---|---|---|
| Movement | Floating or fixed stick; adjustable full-tilt travel | Left thumbstick |
| D-pad | Eight-way cross with rolling directions | D-pad |
| A / B / X / Y | A bottom, B right, X left, Y top; adjacent chords | Positional face buttons, remappable |
| L / R | Independent analog sliders | Independent analog triggers |
| Start | Separate pill | Menu/Start button |

## Current input pipeline

1. `port/dcpad/apple/DCPadOverlayView` and `DCPadControllerInput` publish touch
   and physical-controller states to `DreamcastInputMixer`.
2. `port/input/DreamcastInputState.h` defines the canonical state: signed
   8-bit movement axes, unsigned 8-bit triggers and button flags. Positive Y
   points up.
3. `DreamcastInputAdapter.cpp` applies a 12% radial movement deadzone,
   normalizes the axes/triggers, and produces an `ActionSnapshot`.
4. `FlycastPadBridge.cpp` maps that snapshot to Flycast port A. It converts
   the button mask, flips Y to Flycast's down-positive convention, and scales
   movement to signed 16-bit and triggers to unsigned 16-bit values.
5. `RingPadApp.mm` writes the result into Flycast's controller state. Its
   compile-time checks verify the bridge's constants against Flycast's
   `core/input/gamepad.h`.

The game therefore receives Flycast's Dreamcast controller protocol, not the
old SARecomp action snapshot. The canonical bits below are internal values;
they are not written directly into Flycast's button word.

| Action | Canonical bit | Flycast bit |
|---|---|---|
| Up / Down / Left / Right | 0 / 1 / 2 / 3 | 4 / 5 / 6 / 7 |
| Start | 4 | 3 |
| A / B / X / Y | 10 / 11 / 12 / 13 | 2 / 1 / 10 / 9 |
| L / R | Adapter derives bits 14 / 15 above raw value 64 | Independent analog values, not digital button bits |

`flycast_kcode()` converts pressed profile buttons to Flycast's active-low
word. No guest axis is synthesized from a modern right thumbstick.

## Ownership, timing and reset

The mixer combines touch and controller buttons, takes the strongest movement
axis and preserves independent L/R values. A rising-edge latch retains a tap
that begins and ends between host samples. `FlycastPressHold` then keeps a new
button press for at least three display ticks so it reaches the PAL game's
input sample. The latch is not a turbo input.

Each finger owns one control from touch-down to lift. Lifting one finger
must not release another finger's input, and another finger cannot steal a
held stick, D-pad or trigger. Controller-slot bookkeeping keeps player 1
stable across additional connections. Modal menus, layout editing, focus and
lifecycle transitions release held values and queued edges.

The floating stick stays centered where the touch began and clamps at the
rim. Its full-tilt travel defaults to 85% of the radius and is adjustable from
50% to 100%. DCPad's trigger sliders include a haptic full-press marker and
hysteresis; that feedback does not add a GameCube-style button to the
Dreamcast controller.

## Layout and provenance

[DCPad](../port/dcpad/README.md) owns the current geometry, touch arbitration,
Core Animation drawing, touch settings and layout editor. Controls anchor to
landscape corners in points, with safe-area handling and separate phone and
tablet layouts. Size, opacity, stick travel, haptics, idle dimming and
hide-with-controller settings persist per game.

The earlier `DreamcastTouchLayout` and `DreamcastTouchArbiter` remain as
host-tested contracts for the original layout; they do not draw the current
app's controls.

Input merging, face remapping, player ownership and the persistent menu were
adapted from SunPad's input and overlay mechanisms at the revision recorded
in [Third-party notices](../THIRD_PARTY_NOTICES.md). DCPad implements the
Dreamcast-specific geometry and controls.

## Validation levels

| Level | Evidence and scope |
|---|---|
| Host tests | `scripts/run-host-tests.sh` covers the adapter, mixer, controller mapping, legacy layout, DCPad and Flycast bridge. No game data or device is needed for those suites. |
| Simulator routing | `tests/ios/RingPadUITests.m` sends UIKit taps, drags and holds and inspects Flycast port-A values. Recorded checks cover buttons, axes, triggers, menu pause/resume, settings, layout persistence and import. See [STATUS.md](STATUS.md) for the latest run. |
| Physical acceptance | The exact build still needs recorded hands-on multitouch/controller acceptance. Prior device boot, attract-demo and scripted-route measurements do not establish control feel or input latency. |

Passing a host or Simulator check does not close physical acceptance.
