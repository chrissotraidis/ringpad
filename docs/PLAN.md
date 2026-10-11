# SonicPad plan and work loop

Updated **10 October 2026**. Work the lowest open gate. A gate closes only with
the evidence named in its row; record results in [STATUS.md](STATUS.md).

| Gate | Outcome | Evidence | Status |
|---|---|---|---|
| S0 Foundation proof | PAL GDI boots on macOS arm64 with no JIT; recompiled code runs from title to gameplay | Screenshots and `SONICPAD_STATS` from a scripted run | **Done 10 Oct** (Chaos 0 fight, over 99.99% AOT) |
| S1 Recompiler hardening | Faster lookup and block chaining; inline common opcodes; idle skipping; measured before and after | Startup differential self-test passes; `SONICPAD_PERF` on the Chaos 0 route | **Done 10 Oct** (2.50x to 3.91x uncapped; see STATUS) |
| S1b Discovery | Static discovery from traced seeds; per-route coverage report | Fewer first-run misses on a new route | Open |
| S2 macOS first play | D3 in [prd.md](prd.md) | Scripted replay plus a human run; save and reload verified; audio heard | Open |
| S3 iOS performance | iOS build of core plus AOT blocks; one iPad Simulator; Chris measures Emerald Coast on device | Frame-rate log from the device build | Simulator part **done 11 Oct** (Chaos 0 reached); device measurement open |
| S4 SonicPad shell | Landscape layout, touch layer from `port/input`, three-dot menu, GDI import with hash check | Simulator UI tests and touch routing tests | **Done 11 Oct** (4 UI tests pass); settings, diagnostics and controller support open |
| S5 Coverage and stability | Seven stories, Chao, soak, renderer review | Trace manifests per story, scripted regressions | Open |
| S6 Device acceptance | Chris tests the exact build | Recorded hashes and notes | Open |
| S7 Publication | Separate approval | License and package audit | Not started |

## The loop

1. Pick the lowest open gate and one measurable question.
2. Build, then run the relevant `tests/acceptance/*.txt` input script with
   `SONICPAD_STATS=1` and `SONICPAD_TRACE` pointing at a new trace file.
3. Check the result: screenshots, `aot_pct`, top misses, errors.
4. Generate from the new trace, rebuild and rerun the same script. Coverage
   must not fall and the route must still reach the same point.
5. Record the measurement in STATUS.md in a few lines. Do not paste logs.

## Rules

- One game instance and one Simulator at a time.
- Never commit game data, traces, `generated/` or screenshots of play.
- Leave `ref/flycast` unmodified; hooks go through the root CMake.
- A route counts only when the recompiled build reaches it; `SONICPAD_NO_AOT`
  is for comparisons.
- Stop a line of work when it stops moving a gate. If a gate needs Chris
  (licence, device, approval), say exactly what and continue on other gates.
