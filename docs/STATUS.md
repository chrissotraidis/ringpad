# SonicPad status

Updated **23 October 2026**. History before the static-recompilation decision
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
  - Widescreen was dropped at this point because Flycast's widescreen option shrank the picture inside SonicPad's view. It came back on iPhone on 22 October using Flycast's patch for this disc (see below).
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
- **Emerald Coast recompiled:** with the 130-emblem save on the card, Trial ▸
  Action Stage reaches Emerald Coast directly. A new acceptance script
  (`tests/acceptance/trial-emerald-coast.txt`) runs and jumps through the
  opening of the stage. Traced once, it went from 93.6% to 99.8% recompiled
  (manifest `44c8a6d568863459`, 35,095 blocks, self-test 209,624 trials
  passed). The Chaos 0 route lands on exactly the same frame as before and is
  99.999% recompiled. An iPad build with this code was signed at 14:22.

### Graphics direction (22 October 2026)

Flycast stays the Dreamcast hardware layer. Replacing its graphics with a
native Metal renderer was scoped and set aside: it would mean hooking the
game's Ninja library calls in the executor and rewriting the PowerVR2 look
(transparency sorting, fog, effects), months of work. It would not give
widescreen by itself either, because the game culls and places the HUD for
4:3 before anything is drawn. Visual improvements come from settings and
game patches on top of Flycast. See [PLAN.md](PLAN.md).

### Player builds from the player's own disc (23 October 2026)

- **Published block list:** every one of the 35,095 recompiled blocks is a byte
  range of a file on the disc: 25,168 in `1ST_READ.BIN` at its load address
  `8C010000`, the rest in the IP.BIN boot sector, `MOVIE.BIN`, or a
  PRS-compressed stage (`0C900000`) or event (`0CB80000`) module. Four are
  Flycast's own BIOS stubs. `config/recomp-blocks.txt` (757 KB) records the
  address, file and offset of each, with SHA-256 digests, and no game bytes.
  Rebuilt from the disc it gives a byte-identical manifest
  (`44c8a6d568863459`) in one second.
- **One-command player build:** `scripts/build-personal.py` checks the disc,
  fetches pinned dependencies, rebuilds the manifest, generates the code and
  packages an unsigned IPA. With Apple's Python 3.9 it built Flycast and
  SonicPad from scratch in 2 min 57 s on this Mac: a 14 MB IPA, arm64, iOS 16+,
  SDK 27.1, version 0.1.0 with build number and commit. PadMint's
  `check-manifest` accepts `padmint.json` and `padmint audit` passes the IPA.
- **PadMint player check:** `scripts/player-check.sh sonicpad ios <gdi> <checkout>`
  from PadMint main `a723ae0` (Apple Python 3.9, empty PadMint folder, fresh
  clone with no `ref/`, two compile jobs) passed for `81ff44c` (6 min 28 s)
  and for `1387621` (5 min 47 s): build completed, checkout check passed,
  scene startup `declared`, publication gate PASS. With partial clones the
  whole check used 1.5 GB, Flycast's Git history 254 MB.
- **Scene declared in Info.plist:** the app starts from `UISceneConfigurations`
  instead of a delegate callback, so PadMint can confirm startup. Checked on the
  iPad (landscape 2732x2048, full speed) and with all 14 UI tests on the iPad
  Pro 12.9-inch Simulator.
- **Build identity:** every iOS build now reports version, build number (commit
  count) and commit in About and the diagnostic log.
- **Physical iPad, build 0.1.0 (33, 81ff44c):** installed in place on the iPad
  Pro 12.9-inch (M2), iPadOS 27.0.1, after backing up the app's saves and
  settings; every file read back identical afterwards. Hands-off boot, title and
  attract demo at 1920p with 4x xBRZ and the Unlock All Characters cheat: 1.00x
  speed and 30 FPS in every 30-second window over six minutes, 0.37-0.44 s of
  SH-4 CPU per guest second, 100% recompiled until the demo enters a stage
  (99.1-99.8%), self-test 209,624 trials passed, no memory or thermal events.
- **Load pauses are the game's own:** at boot and when the demo enters a stage
  one frame takes about 2.5 s while guest time keeps running at 1.00x. The
  pause is the same with 2x xBRZ (2,499 and 2,556 ms) as with 4x (2,504 and
  2,551 ms), so it is the game loading, not texture upscaling. The `render`
  line's worst frame includes these pauses.

### Static blocks from the disc files (23 October 2026, night)

- **What runs compiled now:** `scripts/recomp/discover_blocks.py` finds code in
  `1ST_READ.BIN`, `MOVIE.BIN` and 204 PRS code modules (`config/code-modules.txt`:
  165 event and 3 Chao modules at `0CB80000`; 12 stages, 6 adventure fields,
  11 bosses, the Chao garden, Twinkle Circuit, Sky Chase and the sandboard at
  `0C900000`; three menu modules at `8C900000`). It walks from the traced blocks, constant-pool call
  targets and code pointers that land on a function prologue, decoding blocks
  as `decodeBlock` does. 277,929 blocks (1.97 million instructions) in 20 s;
  every traced block is found except 381 in IP.BIN, Flycast's BIOS stubs and
  the `AC` mirror, which stay traced. Without traces it finds 80.5% of the
  traced `1ST_READ.BIN` blocks.
- **Compact code:** the 243,215 static blocks that were not traced compile
  address-relative with one function per distinct instruction sequence (1.05
  million instructions; the event modules share most of their code). The iOS
  app grew from 30 MB to 75 MB; the iPad build compiled in 44 s on this Mac.
  The startup self-test now covers 16,948 inline forms (313,696 trials, 0.6 s
  on the iPad).
- **Same behavior:** on the Mac, Windy Valley (never traced), Emerald Coast with
  other inputs and the traced Chaos 0 route gave byte-identical screenshots
  with and without static blocks (23 frames). The iPad executed exactly the
  same number of blocks as the Mac on the Windy Valley route in both builds.
- **Coverage on an untraced stage:** Trial ▸ Sonic ▸ Windy Valley, 145 s of
  play. Recompiled share 98.34% -> 99.94%, interpreted instructions 319 million
  -> 11 million, on the Mac and on the iPad Pro (M2). On the iPad, SH-4 CPU
  per guest second in the stage windows fell from 0.41-0.43 to 0.40-0.41;
  both ran at 1.00x. SH-4 CPU on the traced Chaos 0 route is unchanged within
  run-to-run noise (0.215-0.232 s per guest second, uncapped).
- **Executor:** each block remembers the exact successor variant that ran, so
  modules sharing a load address do not walk the variant chain;
  `SONICPAD_NO_STATIC=1` turns static blocks off for comparisons. On iOS,
  relative `SONICPAD_INPUT_SCRIPT`, `SONICPAD_SHOT_DIR` and
  `SONICPAD_STATE_LOAD` paths are inside Documents, so routes run on a device.
- **Player builds:** `build-personal.py` runs discovery on the player's disc and
  stops unless it gives the tested set (`# static 13d3333b81a8c42c` in
  `config/recomp-blocks.txt`).

## Not yet verified

- Audio quality, reload, device speed in Emerald Coast, and stages past its opening.
- The new stick, cheats and save import on the physical iPad: they were checked in
  the Simulator and in scripted runs.
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
- Widescreen is Flycast's two-value patch for this disc plus a 4:3 to 16:9
  stretch. Players of the same patch report objects popping in at the right
  edge and a stretched HUD; not yet checked in SonicPad. No PAL v1.003 culling
  or HUD fix has been found; Dreamcast Image Builder's culling fix targets USA
  v1.005.

## Next

Chris plays on the iPad and reports how the controls feel, sharing the
diagnostic log after anything odd. Then: device speed in Emerald Coast, an
iPhone pass, and Bluetooth controller acceptance. S2: extend the acceptance
script through Station Square into Emerald Coast and trace it, so later stages
run as recompiled code. With a 130-emblem save, later stages can be reached
quickly; each new stage needs one trace pass before it runs as recompiled code.
