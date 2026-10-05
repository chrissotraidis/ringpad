# SonicPad Dreamcast input contract

Updated: **5 October 2026**. Game-independent; no guest build required.

## Canonical profile

One Dreamcast controller: **one analog movement stick**, a D-pad, A/B/X/Y,
independent analog L/R, and Start. There is deliberately **no second analog
stick**. Optional modern-camera input stays a separate, explicitly enabled host
extension, never a phantom Dreamcast axis, and Original mode must receive no
look input.

## Where it sits

`port/input/DreamcastInputState.h` defines the shell-to-adapter boundary
(`SonicPadDreamcastState`) and the adapter output (`ActionSnapshot`). The shell
(Apple overlay, physical controller, keyboard) fills the canonical state;
`port/input/DreamcastInputAdapter.cpp` converts it once into the fields of
SARecomp's guest snapshot at the verified boundary
(`ref/sarecomp/src/sonic_input.hpp` → `Snapshot::pad`, plus pad bits 14/15 for
the analog triggers). The adapter is a pure function with no host I/O, so it is
testable without a device and without the guest runtime.

## Guest mask (verified against the retained boundary)

Copied from `ref/sarecomp/src/sonic_input_bindings.hpp`, not guessed:

| Action | Guest bit |
|---|---|
| D-pad Up / Down / Left / Right | 1 / 2 / 4 / 8 |
| Start | 1 << 4 |
| A / B / X / Y | 1 << 10 / 11 / 12 / 13 |
| L trigger (derived) | 1 << 14, set when raw > 64 |
| R trigger (derived) | 1 << 15, set when raw > 64 |

SARecomp's default pad mapping also mirrors D-pad onto the movement action and
maps W/S/A/D to movement, Space/B/X/Y to the face buttons, Enter to Start and
Q/E to the triggers (`default_bindings`). The Apple adapter supplies the
movement/face/Start/trigger values through the same snapshot, so the game keeps
its existing action mapping.

## Analog rules

- Movement is normalized `int8` -> `[-1, 1]` with a **radial** deadzone
  (default 12% of full radius). Radial, not per-axis, so diagonals stay at 45
  degrees and the center never leaves a stuck axis.
- L/R are independent, continuous `[0, 255]`; the adapter preserves the analog
  value **and** sets the guest trigger bit at the threshold. No GameCube
  digital-click detent; a value just over 64 counts as held.

## Input correctness

The `EdgeLatch` guarantees a press observed between game samples is delivered
**exactly once** (a short tap is neither dropped nor repeated as turbo). The
shell calls `EdgeLatch::press` when a touch begins or a physical button
transitions, folds pending edges in with `consume(held_mask)`, and must call
`clear()` on every modal, layout-edit, handoff, owner change, focus loss and
lifecycle transition so no queued edge survives.

Per-touch identity and ownership arbitration belong to the shell overlay
(reused from SunPad mechanisms, not re-invented). Lifting one finger must not
release another finger's button; a second controller must not steal P1; SDL and
GameController must not each feed the same physical event.

## Default touch layout (intent; not yet wired)

| Control | Touch default | Physical default |
|---|---|---|
| Movement stick | Large left-side stick, safe-area aware | Left thumbstick |
| D-pad | Compact cluster near the left thumb | D-pad |
| A / B / X / Y | Dreamcast diamond: A bottom, B right, X left, Y top | Positional face buttons |
| L / R | Left/right shoulder regions; slide supplies the analog value | Analog triggers |
| Start | Small separate pause/start control | Controller menu/start |
| Three-dot menu | Persistent native button, never hidden | Optional host binding |

Start is separate from the three-dot menu; the three-dot action is never Start.

## Acceptance levels

1. **State/adapter unit tests** — `tests/host/test_dreamcast_input.cpp`, run by
   `scripts/run-host-tests.sh`. Source-derived; currently 46/46 pass.
2. **Simulator view-event routing** — not run. `xcrun simctl io ... screenshot`
   captures an image and does not test touch.
3. **Physical multitouch/controller** — not run; reserved for the owner on the
   exact candidate.

Levels 2 and 3 are open; passing level 1 is not evidence of either.

