# SonicPad PRD v2: static recompilation of Sonic Adventure for Apple platforms

Status: **active**. Written **10 October 2026**, updated **29 October 2026**.
Supersedes the historical Katana/SARecomp research route.
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
| Code discovery | Trace-guided blocks plus static discovery from disc modules, traced seeds, constant-pool call targets and code pointers. Players reconstruct the tested manifests from their own disc using the committed address/offset lists. |
| Runtime | `port/recomp/sp_aot_executor.cpp` runs compiled blocks keyed by guest address and checked against the guest bytes, so stage code loaded over old code never runs stale. |
| Dreamcast hardware | [Flycast](https://github.com/flyinghead/flycast) at a pinned commit supplies PowerVR2, AICA sound, GD-ROM, Maple/VMU and the high-level BIOS replacement. No BIOS file is needed. |
| Fallback and oracle | Flycast's SH-4 interpreter runs any block not yet compiled and is the reference for differential tests. It is portable C++ and allowed on iOS. |
| JIT | **None.** Every Flycast JIT is compiled out (`TARGET_NO_REC`). |
| App shell | SonicPad UIKit shell with DCPad touch controls and SunPad-derived input mechanisms. `port/input/FlycastPadBridge` maps one stick, independent analog L/R and Start to Flycast port A; the three-dot menu is separate. |

Why this route: a matching decompilation of a game this size is a multi-year
effort with no Dreamcast starting point, and the Katana route needs private
material that cannot be obtained. Translating the game's own machine code
avoids both problems. Using an existing hardware layer avoids the failure of
the archived recovery effort, which rebuilt Dreamcast hardware one function at
a time and never reached a title screen.

## Inputs

Only the PAL v1.003 GDI with three tracks is supported; hashes are in
`config/game-identity.json` and checked by `scripts/verify-game.py`. The app
never downloads game content. Traces, instruction-bearing block manifests
and generated C++ are derived from the game and stay ignored and private,
like the disc itself.
`config/recomp-blocks.txt` and `config/code-modules.txt` contain addresses,
file names, offsets and hashes, allowing players to reconstruct the compiled
set without a private trace.

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
| D8 | Source publication passes the [release checklist](RELEASE-CHECKLIST.md), including history/content audit and clean player build. SonicPad source is GPL-3.0; game-derived files and personal IPAs are excluded. The owner controls repository visibility. |

An experimental source/recipe release does not close D3–D7. Those remain the
full project acceptance goals and must be disclosed as open until measured.

Non-goals: SADX content, other disc revisions, online features, mods, a general
Dreamcast emulator front end, App Store distribution, and a native renderer
replacing Flycast's graphics (decided 22 October; see [PLAN.md](PLAN.md)).
