# SonicPad status

Updated **10 October 2026**. History before the static-recompilation decision
is archived in [archive/katana-route](../archive/katana-route/README.md).

## Verified

- Disc: the PAL v1.003 GDI in `ref/` passes all three track hashes and the
  descriptor hash (`scripts/verify-game.py`).
- Build: the root `CMakeLists.txt` builds Flycast `ed953fc` plus the SonicPad
  executor for macOS arm64 with the Command Line Tools; all JITs compiled out.
- Gameplay on recompiled code: `tests/acceptance/first-play-chaos0.txt` goes
  from boot through the title screen, file select, character select and
  Sonic's opening into the playable Chaos 0 fight. Sonic moves and jumps from
  scripted input, takes hits and loses rings, and the HUD updates.
- Coverage on that route, final build (manifest `5963a3b11ab2d2e9`, 31,024
  blocks, 226,923 instructions, 22 generated files): 28.23 billion SH-4
  instructions ran as generated C++ and 246,269 on the interpreter, over 99.99%.
  The run reached the in-fight Homing Attack hint, and all 14,264 handler
  checks passed at startup.
- Guest time tracked wall time during scripted runs on an M1, so the game ran
  at roughly full speed. This is an observation, not a measurement.

## S1 performance (10 October 2026)

Scripted Chaos 0 route, 200 guest seconds, run uncapped on an M1 with
`scripts/recomp/run-route.sh ... SONICPAD_UNCAPPED=1`. Speed is guest time per
wall time; CPU is SH-4 thread CPU time per guest second. That thread also runs
Flycast's hardware scheduling, so it is the bottleneck in every run.

| Step | Speed | SH-4 CPU per guest s |
|---|---:|---:|
| Baseline: hash-map lookup, one handler call per instruction | 2.50x | 0.396 s |
| Page-indexed lookup plus two-entry successor cache (99.7% chain hits) | 3.13x | 0.315 s |
| Inline 11,805 integer, load/store and branch opcode values | 3.42x | 0.289 s |
| Idle skipping for pure self-loops (13.8% of guest time) | 3.91x | 0.253 s |

At normal real-time pacing (audio on) the SH-4 thread used 0.344 s of CPU per
guest second, and the replayed route still reached the Chaos 0 fight. Every
inlined form is checked at startup against Flycast's handler on randomized
registers and memory (203,256 trials, no failures); a deliberately broken form
was caught and disabled AOT. The block count was identical across repeated
runs of the same build, so routes are deterministic.

Remaining cost, from a symbolized profile: about 40% of SH-4 thread samples are
the dispatch loop (byte check, chain check and indirect call per block), then
memory access functions, floating-point handlers, the AICA sound CPU and the
game's frame-wait loop, which calls a function each pass and so is not skipped.

## S3 iPad Simulator (11 October 2026)

SonicPad.app (com.chrissotraidis.sonicpad) builds for the iOS Simulator with
Xcode 27 and runs on a dedicated "SonicPad iPad Air 11 (M3)" Simulator on the
iOS 26.5 runtime. The scripted Chaos 0 route ran from boot through the memory
card prompt, file and character select and Sonic's opening into the playable
fight, with the HUD and Homing Attack hint on screen.

- Rendering: Flycast's iOS shell is GLKit-only, and OpenGL ES is unavailable in
  the Apple Silicon Simulator. SonicPad has its own minimal iOS shell
  (`port/apple/ios/SonicPadApp.mm`): UIScene lifecycle, a `CAMetalLayer`
  view and a display link driving Flycast's Vulkan renderer on MoltenVK 1.4.2,
  linked statically.
- Recompiled code ran over 99.99% of executed instructions (32,408 interpreted)
  and the startup self-test passed in the iOS build.
- Speed: 205 guest seconds took 246 wall seconds (0.83x real time) while the
  SH-4 thread used 0.336 s CPU per guest second. The Simulator runs on the
  Mac's CPU and GPU path, so this is not a device measurement; the limit here
  is presentation, not the recompiled code. Device timing is the next S3 step.
- Shell gaps for S4: the app lays out in portrait instead of landscape, and
  Flycast's ImGui virtual gamepad still draws over the game.

## S4 iOS shell (11 October 2026)

All four UI tests in `tests/ios/SonicPadUITests.m` pass on the iPad Air 11
(M3) Simulator (`scripts/ios/run-ui-tests.sh`). They drive real UIKit
touches through XCUITest, so this is input acceptance level 2 (view-event
routing), not physical multitouch.

- Landscape: the scene requests landscape and the app window is wider than
  tall in every test; the game fills the screen with small 4:3 side bars.
- Touch layer: SonicPad's overlay draws the `port/input` layout (one stick,
  D-pad, A/B/X/Y diamond, L/R, Start) inside the safe area, and Flycast's
  virtual gamepad is hidden. Taps on A, X, D-pad Up and Start reach Flycast's
  port A, B stays untouched, and every button is released afterwards.
  Dragging the stick right and up gives +31,887 X and -31,887 Y. Sliding the
  right shoulder gives a trigger value of 62,194, with the left trigger at 0.
- Bridge: `port/input/FlycastPadBridge` maps the adapter snapshot to
  Flycast's controller values. A tap stays held for at least three display
  ticks so the 50 Hz PAL game cannot miss it between samples.
- Menu: the three-dot button is separate from Start, never sends Start,
  pauses the game, and offers Resume and Import Game.
- Import: with no game installed, the app opens on an import screen with a
  folder picker. `port/game/GameImport` checks the layout, size and SHA-256 of
  all three tracks against the PAL v1.003 identity. It then copies the disc
  into Application Support (cloned on APFS) and boots it. A relaunch without
  any test hooks boots the installed copy. Host tests cover the rejections:
  wrong hash, size, layout, missing track and two .gdi files.
- Stability: one full UI run crashed during import. A progress callback held a
  C++ reference to storage freed when the background import finished. The
  block is now copied before dispatch, and two further full runs passed 8/8.
  Faults now log `SONICPAD_FAULT` with the faulting symbol before the trap.
- Host tests: 210/210 checks pass, including bridge and import tests and an
  identity check of the owner's real disc.

## Not yet verified

- Audio quality, VMU saves, Emerald Coast, reload, and anything after Chaos 0.
- Performance on iOS. Floating-point opcodes still call Flycast's handlers.
- Physical iPad and iPhone builds: they need a signing identity and Chris's
  device; nothing has run on device hardware yet.

## Known limits of the current runtime

- Every block is checked against guest memory bytes before it runs.
- Interrupts are checked at block boundaries, as in Flycast's own JIT.
- New routes need one trace pass; until generated, their code runs on the
  interpreter (correct, slower).
- Flycast runs create `~/Library/Application Support/Flycast/` for settings
  and VMU data.

## Next

S3 device: sign a device build and measure Emerald Coast on Chris's iPad.
S4 follow-ups: physical controller input through the mixer, settings and
diagnostics. S2: extend the acceptance script through Station Square into
Emerald Coast.
