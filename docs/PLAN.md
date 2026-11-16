# RingPad plan and work loop

Updated **29 October 2026**. Work the lowest open gate. A gate closes only with
the evidence named in its row; record results in [STATUS.md](STATUS.md).

| Gate | Outcome | Evidence | Status |
|---|---|---|---|
| S0 Foundation proof | PAL GDI boots on macOS arm64 with no JIT; recompiled code runs from title to gameplay | Screenshots and `RINGPAD_STATS` from a scripted run | **Done 10 Oct** (Chaos 0 fight, over 99.99% AOT) |
| S1 Recompiler hardening | Faster lookup and block chaining; inline common opcodes; idle skipping; measured before and after | Startup differential self-test passes; `RINGPAD_PERF` on the Chaos 0 route | **Done 10 Oct** (2.50x to 3.91x uncapped; see STATUS) |
| S1b Discovery | Static discovery from traced seeds; per-route coverage report | Fewer first-run misses on a new route | **Done 23 Oct**: 243,215 static blocks from 1ST_READ.BIN and 204 code modules; untraced Windy Valley went from 98.34% to 99.94% recompiled on the Mac and the iPad |
| S2 macOS first play | D3 in [prd.md](prd.md) | Scripted replay plus a human run; save and reload verified; audio heard | Open. Emerald Coast's opening runs 99.8% recompiled through Trial mode with a 130-emblem save (22 Oct); the new-game route through Station Square, save, cold reload and audio check remain |
| S3 iOS performance | iOS build of core plus AOT blocks; one iPad Simulator; Chris measures Emerald Coast on device | Frame-rate log from the device build | Simulator **done 11 Oct**. Device: full speed on the iPad Pro (M2) through the intro and attract demo (22 Oct); Emerald Coast on device open |
| S4 RingPad shell | Landscape layout, DCPad touch layer and `port/input` bridge, three-dot menu, GDI import with hash check | Simulator UI tests and touch routing tests | **Done 11 Oct**. Since then: signed iPad build, diagnostic log, Graphics menu, cheats, VMU save import, iPhone widescreen (22 Oct) |
| S5 Coverage and stability | Seven stories, Chao, soak | Trace manifests per story, scripted regressions | Open |
| S6 Device acceptance | Chris tests the exact build | Recorded hashes and notes | Open |
| S7 Publication | Experimental source and recipe release | License/content/history audit; clean player build; exact public-source selection | Preparation authorized **29 Oct**, with no new hardware pass requested. Historical player checks passed, but publication must use the audited source set and satisfy [RELEASE-CHECKLIST.md](RELEASE-CHECKLIST.md). Repository visibility remains the owner's action; a public PadMint catalog entry and post-publication player check are separate. |

## Experimental release scope

The source/recipe release does not claim all seven stories or broad device
acceptance. S2, S5 and S6 stay open, with limits explained in
[KNOWN-ISSUES.md](KNOWN-ISSUES.md). Historical device evidence is retained;
release preparation on 29 October does not require or imply a new hardware
run. Do not publish personal IPAs, game-derived code or the historical
recovery archive. The release checklist owns the source and Git-history gate.

## The loop

1. Pick the lowest open gate and one measurable question.
2. Build, then run the relevant `tests/acceptance/*.txt` input script with
   `RINGPAD_STATS=1` and `RINGPAD_TRACE` pointing at a new trace file.
3. Check the result: screenshots, `aot_pct`, top misses, errors.
4. Generate from the new trace, rebuild and rerun the same script. Coverage
   must not fall and the route must still reach the same point.
5. Record the measurement in STATUS.md in a few lines. Do not paste logs.

## Rules

- One game instance and one Simulator at a time.
- Never commit game data, traces, `generated/` or screenshots of play.
- Leave `ref/flycast` unmodified; hooks go through the root CMake.
- A route counts only when the recompiled build reaches it; `RINGPAD_NO_AOT`
  is for comparisons.
- Stop a line of work when it stops moving a gate. If a gate needs Chris
  (licence, device, approval), say exactly what and continue on other gates.

## Decisions

- **Graphics (22 October 2026):** Flycast stays the Dreamcast hardware layer,
  rendering through Vulkan on MoltenVK. A native Metal renderer that hooks the
  game's Ninja calls is out of scope; it is months of work and does not by
  itself give widescreen. Improvements go through Flycast settings (resolution,
  xBRZ) and game patches, such as the iPhone widescreen patch.
