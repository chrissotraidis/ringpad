# SonicPad

Sonic Adventure (Dreamcast, PAL v1.003) as a static recompilation for macOS,
iPadOS and iOS. The game's SH-4 code is translated ahead of time into C++ and
compiled for arm64, and Flycast supplies the Dreamcast hardware layer. No JIT.

**Status, 10 October 2026:** on macOS arm64 the recompiled build plays from
boot through the title and Sonic's opening into the Chaos 0 fight, with over
99.99% of executed instructions running as generated code and about 3.9x real-time
throughput on an M1. The same build reaches the fight in the iPad Simulator,
rendering through MoltenVK. The iOS app is landscape and uses DCPad, a reusable
Dreamcast touch control kit (`port/dcpad`), and imports the disc with a hash check. Device timing is next. See [STATUS.md](docs/STATUS.md).

- [PRD](docs/prd.md): goal, foundation decision, definition of done
- [Plan](docs/PLAN.md): gates and work loop
- [Building](docs/BUILDING.md): setup, build, trace and generate
- [Input contract](docs/INPUT.md): single-stick Dreamcast touch layer
- [Archived Katana route](archive/katana-route/README.md)

Game data and anything generated from it stay out of Git.
