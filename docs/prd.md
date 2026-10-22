# SonicPad PRD v2: static recompilation of Sonic Adventure for Apple platforms

Status: **active**. Written **10 October 2026**. Supersedes the Katana/SARecomp
PRD, archived at [archive/katana-route/docs/prd-katana-v1.md](../archive/katana-route/docs/prd-katana-v1.md).
Companion documents: [PLAN.md](PLAN.md), [STATUS.md](STATUS.md), [BUILDING.md](BUILDING.md), [INPUT.md](INPUT.md).

## Goal

A native ARM64 macOS, iPadOS and iOS app that plays the Dreamcast **Sonic
Adventure PAL v1.003 (MK-5100050)** from the owner's own disc, with the
single-stick Dreamcast touch controls, controller support, saves and correct
media. macOS is the bring-up and regression target; physical iPad and iPhone
acceptance is the finish line.

## Foundation decision (10 October 2026)

SonicPad is a **static recompilation** that we build and own. It does not depend
on SARecomp, KatanaRecomp or any private source package.

| Layer | Implementation |
|---|---|
| Game code | SH-4 machine code from the owner's disc, translated ahead of time to C++ by `scripts/recomp/generate_blocks.py` and compiled for arm64 into the app. |
| Code discovery | Trace-guided: a Mac build records every block the game executes, including code loaded later from disc, and the generator compiles them. Static discovery from traced seeds is the next extension. |
| Runtime | `port/recomp/sp_aot_executor.cpp` runs compiled blocks keyed by guest address and checked against the guest bytes, so stage code loaded over old code never runs stale. |
| Dreamcast hardware | [Flycast](https://github.com/flyinghead/flycast) at a pinned commit supplies PowerVR2, AICA sound, GD-ROM, Maple/VMU and the high-level BIOS replacement. No BIOS file is needed. |
| Fallback and oracle | Flycast's SH-4 interpreter runs any block not yet compiled and is the reference for differential tests. It is portable C++ and allowed on iOS. |
| JIT | **None.** Every Flycast JIT is compiled out (`TARGET_NO_REC`). |
| App shell | SunPad-derived Apple shell plus `port/input` (one stick, independent analog L/R, Start separate from the three-dot menu). |

Why this route: a matching decompilation of a game this size is a multi-year
effort with no Dreamcast starting point, and the Katana route needs private
material that cannot be obtained. Translating the game's own machine code
avoids both problems. Using an existing hardware layer avoids the failure of
the archived recovery effort, which rebuilt Dreamcast hardware one function at
a time and never reached a title screen.

## Inputs

Only the PAL v1.003 GDI with three tracks is supported; hashes are in
`config/game-identity.json` and checked by `scripts/verify-game.py`. The app
never downloads game content. Traces, the block manifest and generated C++ are
derived from the game and stay ignored and private, like the disc itself.

## Definition of done

| ID | Done when |
|---|---|
| D1 | Disc identity verified; the build reproduces from pins plus the owner's disc and a recorded block manifest. |
| D2 | Recompiled code covers the acceptance routes (target at least 99.9% of executed instructions, misses listed), with differential tests for every inlined opcode form. |
| D3 | macOS first play: title, new game, Sonic opening, Chaos 0, Station Square, Emerald Coast completion, natural VMU save, clean exit and cold reload, with video, music, voices and effects. |
| D4 | iPad Simulator then iPhone Simulator, one at a time, run the first-play loop with the SonicPad shell and touch controls. |
| D5 | Measured frame rate on Chris's physical iPad/iPhone holds the original game's rate in Emerald Coast without JIT. |
| D6 | All seven stories, credits and Chao flows covered by traces, scripts and play; 60-minute soak. |
| D7 | Chris accepts the exact device build. |
| D8 | Publication is a separate decision. Flycast is GPL-2.0-or-later and SunPad is GPL-3.0-or-later, so released source would be GPL-3.0; game-derived files are never published. |

Non-goals: SADX content, other disc revisions, online features, mods, a general
Dreamcast emulator front end, App Store distribution, and a native renderer
replacing Flycast's graphics (decided 22 October; see [PLAN.md](PLAN.md)).
