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

## Not yet verified

- Audio quality, VMU saves, Emerald Coast, reload, and anything after Chaos 0.
- Performance on iOS. Floating-point opcodes still call Flycast's handlers.
- iOS and Simulator builds: the Xcode licence on this Mac is recorded for
  Xcode 26.6, so Xcode 27.0 tools refuse to run until it is accepted again.

## Known limits of the current runtime

- Every block is checked against guest memory bytes before it runs.
- Interrupts are checked at block boundaries, as in Flycast's own JIT.
- New routes need one trace pass; until generated, their code runs on the
  interpreter (correct, slower).
- Flycast runs create `~/Library/Application Support/Flycast/` for settings
  and VMU data.

## Next

S3: build the core and generated blocks for the iPad Simulator once the
Xcode licence is accepted. S2: extend the acceptance script through Station
Square into Emerald Coast.
