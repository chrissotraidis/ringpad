# SonicPad status

Updated **22 October 2026**. History before the static-recompilation decision
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

## Touch controls: DCPad (11 October 2026)

The first S4 overlay (plain circles, fixed stick, four separate D-pad
circles) was replaced by **DCPad** (@@port/dcpad@@). It is a reusable Dreamcast
control kit built on SunPad's accepted layout and mechanisms, so the next
Dreamcast port can drop it in. See [port/dcpad/README.md](../port/dcpad/README.md).

- **Controls:**
  - floating analog stick whose base follows the thumb
  - eight-way rolling D-pad
  - colour-coded Dreamcast face diamond with slide and neighbour chords
  - analog L/R sliders with a haptic full-press detent
  - separate Start
  - Core Animation drawing with pressed states
- **••• menu:** a SunPad-style persistent menu. SonicPad adds Game (Import, Reset) and Display (Show FPS, About). DCPad adds Controls (Touch Control Settings, Move Controls, Controller Mapping). Opening any of these pauses the game and releases input, and closing them resumes it.
- **Settings:** opacity, size, floating stick, haptics and hide-with-controller. Move Controls drags each control group and resizes it. Layouts persist per game and per phone or tablet; Reset restores the defaults.
- **Physical controllers:** Apple game controllers feed the same mixer, with player-1 ownership and an A/B/X/Y remap (adapted from SunPad).
- **Tests:**
  - @@tests/host/test_dcpad.cpp@@ runs 311 layout and engine checks on six device classes: everything on screen, no overlaps, clear of the menu, minimum sizes, eight-way sectors, chords, trigger hysteresis, the floating stick and multi-finger ownership.
  - Eight XCUITest UI tests pass on both the iPad Air 11 and iPhone 16 Pro Simulators with real taps, drags and holds. They cover buttons, slides, the D-pad roll, fixed and floating stick, light and full triggers, menu pause and resume, settings, Move Controls persistence across relaunch, Reset, import and relaunch.
  - A touch-only UI test played from boot through the title, file and character select and the opening into Station Square on the iPad Simulator.
- Fixed while iterating: the text fonts, the menu size on a landscape phone (Controls is now a submenu) and the editor bar covering controls.

### Refinement pass (11 October 2026, evening)

- **Look:** every control now uses one style: dark glass, a thin light rim and a soft shadow, with Dreamcast orange for anything held. This replaces the solid orange trigger slabs, grey outlined D-pad and flat discs.
  - Face buttons show coloured rings and letters at rest and fill with their colour when pressed.
  - The D-pad is one cross whose pressed arms light up inside it (two for a diagonal).
  - The stick shows a direction arc on its rim, plus a dashed ring at its full-tilt point while held, and the thumb eases home on release.
  - L and R mark their full-press point, keep their label at the light-press end, and glow at full press.
  - The settings panel, the editor bar and the ••• button match the controls.
- **Feel:**
  - A **Stick travel** setting (full tilt at 50–100% of the stick radius, default 85%) lets short thumb movements reach full speed. It is latched per touch, so changing it never jolts a held stick.
  - Swipes that start at a screen edge now reach the game before iOS gestures such as Control Center or Home.
  - The settings panel fits a landscape phone without scrolling.
- **Reuse:** DCPad is now its own CMake library (`port/dcpad/CMakeLists.txt`), so another port needs one `add_subdirectory` and one `target_link_libraries` line.
- **Tests:** 314 host checks pass, including new stick travel checks. All 10 UI tests (the eight control tests, the capture and the touch play-through) pass on the iPad Air 11 and iPhone 16 Pro Simulators.

### Hold-based layout and port menu (12 October 2026)

- **Placement:** the default controls now follow a landscape grip and the Dreamcast controller.
  - The stick sits where the left thumb rests, with the D-pad below and inward of it as on the real pad.
  - L and R sit on the side edges at the top of each thumb's arc, the face diamond is under the right thumb, and Start is just inward of it rather than at the bottom centre.
  - Defaults are anchored to the bottom corners in points, so a 13-inch iPad keeps an 11-inch reach.
  - A new host test measures each control against a thumb pivot in millimetres on six device sizes. Every control is 24–52 mm away, inside the 15–60 mm comfortable band, and the stick is under 45 mm.
- **Look:** face buttons have a soft top sheen and a letter shadow, the stick thumb is shaded, and the L/R fills brighten toward the full-press end.
- **••• menu:** the reusable `port/apple/ios/SPPortMenu` adds what players expect from a Dreamcast port.
  - A top row of icons: Save State and Load State, each with three slots showing when they were saved, plus Reset.
  - Display: render resolution (Original 480p, 2×, 3×) and Show FPS.
  - Sound: Full, Half or Off.
  - Game Files: Import Game and Export VMU Save, which backs up the in-game save through the share sheet.
  - About This Build, then DCPad's Controls.
  - DCPad shows a short confirmation such as "Saved to Slot 1".
  - Widescreen was tried and dropped: Flycast's widescreen mode shrinks the picture in SonicPad's host, and Sonic Adventure has no native widescreen.
- **Tests:** 377 DCPad host checks pass. The UI suite adds save/load state and live resolution change tests.

### Fourth pass (12 October 2026)

- **Save-state thumbnails:** each save stores a 320-pixel frame, and the Save State and Load State lists show it next to the slot's date. Empty slots show a dashed placeholder.
- **Dim when idle:** after four seconds with no finger down, the controls fade to under half their opacity so cutscenes and scenery read clearly. The next touch restores them at once. It is a setting, on by default, and never dims while a menu or the editor is open.
- **Look and reach:** L and R mark the full-press point with two rim notches instead of a line that looked like a text cursor. The tablet face diamond sits 15 pt further from the edge, so B is clear of the palm; reach stays 24–54 mm on all six device sizes.
- **First play:** a one-time tip, "Tap ••• for save states and settings".
- **Tests:** 377 DCPad host checks and all 12 UI tests pass on the iPad and iPhone Simulators.

## S3 physical iPad (22 October 2026)

SonicPad is signed with Chris's development team, installed and running on
his **iPad Pro 12.9-inch (6th generation, M2), iPadOS 27.0.1**.

- **Speed:** full speed from the first boot. Over eight minutes of title,
  intro and attract demo, every 30-second window read 0.99–1.00x, with the
  SH-4 thread using 0.38–0.44 s of CPU per guest second (more than half a
  core spare). No faults, memory warnings or thermal changes were logged.
- **Recompiled code:** manifest `4e3bc925e044b752` (29,889 blocks from two
  traces of the Chaos 0 route). The startup self-test passed on the device
  (201,376 trials). Coverage is 99.99% on the traced route; the attract demo
  reaches untraced stages and drops to 96–98% while still running at full speed.
- **Import:** the disc was copied into the app's Documents and imported with
  the normal flow (`SONICPAD_IMPORT`, every track hashed on the device). The
  copy is an APFS clone, so free space did not change. Reinstalls keep it.
- **Icon:** a gold ring with speed streaks over sky and sea (generated; no
  characters, logos or game artwork) in `port/apple/ios/Assets.xcassets`.
- **Diagnostic log:** `SPDiagnostics` keeps this session and the previous one
  in `Library/Application Support/SonicPad/Logs`, copying everything the
  runtime writes to stderr with timestamps. It adds lifecycle, pause, import,
  save-state, controller, memory and thermal events, a `SONICPAD_LIVE` speed
  line every 30 s, and a crash line written directly from the fault handler.
  ••• ▸ Help ▸ Share Diagnostic Log sends both sessions as one text file;
  `scripts/ios/pull-device-log.sh` copies them to the Mac.
- **Build fixes found on the way:**
  - Flycast's own `Images.xcassets` replaced SonicPad's icon; it is now excluded.
  - The catalog needed its `MACOSX_PACKAGE_LOCATION` set in Flycast's directory.
  - CoreGraphics and OpenGLES are linked explicitly; they had only been pulled in
    implicitly by Flycast's catalog.
  - The iOS scripts re-run CMake each time, because the Xcode generator did not
    pick up newly generated recompiled sources and silently built an
    interpreter-only app.
- **Simulator:** 11 of the 12 UI tests passed on a 12.9-inch iPad Pro Simulator in
  the full run; the menu test failed only because About moved into the new Help
  submenu, and the updated test (which now also opens and closes the share sheet)
  passes. All 584 host checks pass.

### Polish pass (22 October 2026, afternoon)

From Chris's first session on the iPad:

- **••• menu:** a SonicPad header over the quick row (Save State, Load State,
  Reset), then Controls, Graphics and Sound, then Game Files and Help. Sound,
  Resolution and Texture Upscaling show the current choice under their name.
  DCPad hands its Controls submenu to the host through a new optional delegate
  method, so a port can place it.
- **Graphics:** Resolution now goes to 4× (1920p, about native on a 12.9-inch
  iPad), and Texture Upscaling (xBRZ 2× or 4×) rebuilds the texture cache so it
  shows at once. New installs start at 4× on iPad and 2× on iPhone. On the M2
  iPad, 3× and then 4× with 2× xBRZ each held the game's 30 FPS at full speed,
  with no frame over 50 ms after boot.
- **Show FPS:** SonicPad's own counter in a glass pill at the top left;
  Flycast's on-screen counter at the bottom is off.
- **Pause after closing the menu:** on the iPad the game stayed paused after
  the ••• menu closed until the next tap. DCPad reported the close from the
  dismiss animator's completion, which on iPadOS 27 can wait for the next
  touch; his log showed each resume seconds after the close. DCPad now reports
  the close at once, and a touch on the controls while the menu is marked open
  clears it. The Simulator never showed the delay (XCUITest keeps events
  flowing); a new UI test still checks that frames and guest time advance
  after eight closes, and the app logs a `stall` line if the game has not
  moved 2 s after any resume. Confirmation on the device is Chris's next play.
- **Logging:** a `render` line every 30 s of play (FPS, frames over 50 ms,
  worst frame, graphics settings), plus menu, touch settings, layout, graphics,
  sound and save-state events.

### Cheats, saves, widescreen and the stick (22 October 2026, evening)

- **Analog stick:** a floating stick now stays where the thumb landed. Past
  the rim the knob stops at the edge and holds full tilt; before, the base
  followed the thumb with no limit and could slide across the screen.
- **Cheats:** Max Rings, Infinite Lives and Unlock All Characters, found and
  verified on this disc (the published PAL addresses are 0xC0 off). See
  [CHEATS.md](CHEATS.md).
- **Import VMU Save:** a `.VMS` save with its `.VMI`, or a whole card. A
  130-emblem save imported through the app showed every character and 100%
  clear on File 1.
- **Widescreen:** on by default on iPhone, using Flycast's patch for this disc;
  checked on an iPhone 17 Pro Simulator in the Chaos 0 fight. Not offered on iPad.

## Not yet verified

- Audio quality, Emerald Coast, reload, and anything after Chaos 0 on a new file.
- The new stick, cheats and save import on the physical iPad: they were checked in
  the Simulator and in scripted runs; the iPad build is installed for Chris to try.
- Hands-on touch play on the physical iPad. The controls were only tested
  with UI tests in the Simulator; the device runs above were the intro and
  attract demo without touches.
- iPhone hardware, and device speed in Emerald Coast and later stages.
- Render resolutions above 480p on the device.

## Known limits of the current runtime

- Every block is checked against guest memory bytes before it runs.
- Interrupts are checked at block boundaries, as in Flycast's own JIT.
- New routes need one trace pass; until generated, their code runs on the
  interpreter (correct, slower).
- Flycast runs create `~/Library/Application Support/Flycast/` for settings
  and VMU data.

## Next

Chris plays on the iPad and reports how the controls feel, sharing the
diagnostic log after anything odd. Then: device speed in Emerald Coast, an
iPhone pass, and Bluetooth controller acceptance. S2: extend the acceptance
script through Station Square into Emerald Coast and trace it, so later stages
run as recompiled code. With a 130-emblem save, later stages can be reached
quickly; each new stage needs one trace pass before it runs as recompiled code.
