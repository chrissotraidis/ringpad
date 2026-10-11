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

## Not yet verified

- Audio quality, VMU saves, Emerald Coast, reload, and anything after Chaos 0.
- Performance on iOS. The generated code still calls Flycast's per-instruction
  handlers, so it is correct by construction but not yet fast.
- Differential tests for generated code (needed once opcodes are inlined).
- iOS and Simulator builds: the Xcode licence is not accepted on this Mac.

## Known limits of the current runtime

- Block lookup is a hash map plus a byte comparison per block.
- Interrupts are checked at block boundaries, as in Flycast's own JIT.
- New routes need one trace pass; until generated, their code runs on the
  interpreter (correct, slower).
- Flycast runs create `~/Library/Application Support/Flycast/` for settings
  and VMU data.

## Next

S1: replace the hash-map lookup with a page-indexed table and chain direct
branches, measure host CPU per guest frame before and after, and add
differential tests before inlining the hottest opcodes. In parallel, S2:
extend the acceptance script through Station Square into Emerald Coast.
