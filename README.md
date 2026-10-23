# SonicPad

Sonic Adventure (Dreamcast, PAL v1.003) as a static recompilation for macOS,
iPadOS and iOS. The game's SH-4 code is translated ahead of time into C++ and
compiled for arm64, and Flycast supplies the Dreamcast hardware layer. No JIT.

**Status, 22 October 2026:** on macOS arm64 the recompiled build plays from
boot through the title and Sonic's opening into the Chaos 0 fight, with over
99.99% of executed instructions running as generated code and about 3.9x real-time
throughput on an M1. The same build reaches the fight in the iPad Simulator,
rendering through MoltenVK. The iOS app is landscape and uses DCPad, a reusable
Dreamcast touch control kit (`port/dcpad`), and imports the disc with a hash check.
On a 12.9-inch iPad Pro (M2) it runs at full speed with more than half a CPU core
spare, and keeps a shareable diagnostic log. With a 130-emblem save, Trial mode
reaches Emerald Coast, whose opening runs 99.8% recompiled. See [STATUS.md](docs/STATUS.md).

- [PRD](docs/prd.md): goal, foundation decision, definition of done
- [Plan](docs/PLAN.md): gates and work loop
- [Building](docs/BUILDING.md): setup, build, trace and generate
- [Input contract](docs/INPUT.md): single-stick Dreamcast touch layer
- [Archived Katana route](archive/katana-route/README.md)

Game data and anything generated from it stay out of Git.

## Build your own

SonicPad ships as source and a build recipe only. You build the app on your
Mac from your own disc; it contains code translated from that disc, so it is
for your devices only.

You need an Apple Silicon Mac with full Xcode (with the iOS platform) and
CMake 3.24 or newer, about 6 GB of free space, and your own **Sonic Adventure
PAL v1.003** disc (MK-5100050) as a GDI: the `.gdi` file with `track01.bin`,
`track02.raw` and `track03.bin` next to it. Other releases, CDI and CHD images
are not supported.

With [PadMint](https://github.com/chrissotraidis/padmint), choose SonicPad and
your `.gdi`. Or from a clone of this repository:

```sh
python3 scripts/build-personal.py --gdi "/path/to/Sonic Adventure.gdi" --output SonicPad.ipa
```

Install the unsigned IPA with your own signing tool. In the app, ••• ▸ Game
Files ▸ Import Game picks the same disc folder from Files. See
[BUILDING.md](docs/BUILDING.md#player-build-padmint) for what the build does.

SonicPad is experimental: it has been tested on one iPad Pro (M2), and stages
not yet traced run on Flycast's slower interpreter until a later release covers them.
Licence: GPL-3.0 ([LICENSE](LICENSE), [notices](THIRD_PARTY_NOTICES.md)).
