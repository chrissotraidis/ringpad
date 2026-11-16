# RingPad status

Updated **16 November 2026**. RingPad is an experimental source and build-recipe
release: players build a personal app from their own disc on an Apple Silicon
Mac. Open work is in [Follow-ups](FOLLOW-UPS.md); player-facing limits are in
[Known issues](KNOWN-ISSUES.md).

## What works

| Area | Evidence |
|---|---|
| Recompiled game code | SH-4 code is translated ahead of time to C++; no JIT. The tested set is 35,095 traced blocks plus 277,929 statically discovered blocks from `1ST_READ.BIN`, `MOVIE.BIN` and 204 code modules. Every inlined opcode form is checked against Flycast's handler at startup (313,696 trials, 0.6 s on iPad). |
| Coverage | Chaos 0 route 99.999% recompiled; Emerald Coast opening 99.8%; untraced Windy Valley 99.94%. Byte-identical screenshots with and without static blocks on all three routes. |
| Physical iPad | iPad Pro 12.9-inch (M2): full speed (0.99–1.00x) through title, attract demo and the Chaos 0 route, 0.37–0.44 s SH-4 CPU per guest second, 30 FPS at 1920p with 4× xBRZ. Load pauses of about 2.5 s are the game's own. |
| App shell | Landscape UIKit shell on Vulkan/MoltenVK, DCPad touch controls, controller support, ••• menu with save states, graphics presets (Dreamcast Original default), cheats, VMU import and export, disc import with hash checks, diagnostic log. |
| Player build | `scripts/build-personal.py` rebuilds both manifests from the player's disc and refuses any other result; the PadMint player check passes from a fresh clone. |
| Tests | 7 host suites (580+ checks) and 15 iOS UI tests with real UIKit touches. |

## Latest checks (16 November 2026, RingPad rename)

**0.1.1 candidate:** host tests (380 DCPad checks, including the new tablet
layout on three iPad and three iPhone sizes) and all 15 UI tests on the iPad
Air 11-inch (M4) Simulator pass. On the iPad Pro (M2), build 59 installed in
place, imported the disc and passed the startup self-test (313,696 trials, no
failures); frames captured through a cold launch show only RingPad's controls.
The external-display scene rule has not been tried with a physical display.

- Host tests, repository safety checks and `test_repo_safety.py` pass.
- All 15 UI tests pass on the iPad Air 11-inch (M4) Simulator.
- iPad Pro 12.9-inch (M2): signed `com.chrissotraidis.ringpad` build, disc
  import, Chaos 0 route to the fight at 0.99x, 99.999% recompiled, memory card
  created.
- PadMint player build from a fresh clone of `247e364` (PadMint main
  `f13bfe9`, Apple Python 3.9.6, empty PadMint folder): 8 min 33 s, 1.8 GiB,
  publication gate PASS, personal-only.
- After publishing: PadMint 0.6.7 lists RingPad. `scripts/player-check.sh`
  from PadMint `main` (`1246165`) passed the catalog check and built
  `RingPad-v0.1.0-ios-personal.ipa` from the published v0.1.0 release with
  Apple's Python and an empty PadMint folder.
- iPad Pro 12.9-inch (M2): build 52 of `main` installed in place, keeping the
  imported disc and saves.

Manifest IDs hash the trace header, which became `# ringpad-trace v1` in the
rename. The same blocks now read traced `d731f95362e4d44c` (was
`44c8a6d568863459`) and static `e3c89a61f5a109b2` (was `13d3333b81a8c42c`).
The new bundle identifier installs as a separate app, so earlier test installs
need the disc imported again.

## Decisions

- **Static recompilation over decompilation.** Translating the game's own
  machine code with Flycast as the hardware layer avoids a multi-year matching
  decompilation and needs no BIOS file.
- **Flycast renders.** A native Metal renderer hooking the game's Ninja calls
  was scoped and set aside: months of work, and it would not give widescreen
  because the game culls and places its HUD for 4:3. Visual changes go
  through Flycast settings and game patches.
- **Per Triangle sorting.** Flycast's Per Pixel renderer starts through
  MoltenVK but draws broken frames on iPad, so it is not offered. See the
  face-rendering notes in [Follow-ups](FOLLOW-UPS.md#sonics-face-head-polygons-over-the-eyes).
