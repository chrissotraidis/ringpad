# Building RingPad

For a personal iPhone/iPad app, start with [Install and update](INSTALL.md).
That route needs an Apple Silicon Mac, full Xcode with the iOS platform,
CMake 3.24+, Git and Python 3.9+ (including Apple's `/usr/bin/python3`).
Ninja and Homebrew `libpng` are only needed for the macOS development lane.
Run commands from the repository root.

## Player build (PadMint recipe)

Players build their own app from their own disc, without private traces or
maintainer files:

```sh
/usr/bin/python3 scripts/build-personal.py \
  --gdi "/path/to/Sonic Adventure.gdi" --output RingPad.ipa
```

The script verifies the disc, fetches pinned Flycast and MoltenVK into `ref/`
if missing, reconstructs the traced manifest from `config/recomp-blocks.txt`,
discovers static blocks, generates C++ in `build/personal/`, and builds and
packages an unsigned IPA. It leaves `generated/` untouched and refuses to
replace an existing output. Add `--jobs 2` to limit parallel build work.

[`padmint.json`](../padmint.json) runs the same script. RingPad is not yet in
PadMint's public catalog; use the command above. The IPA and generated game
code are private outputs, excluded from the source release. See the
[release checklist](RELEASE-CHECKLIST.md) for current publication gates.

## macOS development build

This is the tracing and regression lane, not a packaged Mac player release.
It needs the Command Line Tools, CMake 3.24+, Ninja, Python 3.9+, Git and
Homebrew `libpng`.

### One-time setup

```sh
git clone --recursive https://github.com/flyinghead/flycast.git ref/flycast
git -C ref/flycast checkout ed953fcbfdd1b8a7521d2966b13cc700c7e2d233
git -C ref/flycast submodule update --init --recursive
git -C ref/flycast remote set-url --push origin DISABLED
python3 scripts/verify-game.py "ref/<disc folder>/<disc>.gdi"
```

### Build

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools cmake -S . -B generated/build-mac -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C generated/build-mac
```

The app is `generated/build-mac/flycast/Flycast.app`. With no generated
blocks yet, everything runs on the interpreter. To reconstruct the current
compiled set directly from your disc before rebuilding:

```sh
python3 scripts/recomp/disc_blocks.py import "/path/to/Sonic Adventure.gdi"
python3 scripts/recomp/discover_blocks.py run "/path/to/Sonic Adventure.gdi" \
  --traced generated/recomp-manifest.txt --out generated/static-manifest.txt
python3 scripts/recomp/generate_blocks.py generated/recomp-manifest.txt \
  --static generated/static-manifest.txt
ninja -C generated/build-mac
```

For a first developer build, this avoids collecting your own traces. The
following sections describe extending and checking coverage.

## Trace, generate, rebuild

```sh
APP=generated/build-mac/flycast/Flycast.app/Contents/MacOS/Flycast
RINGPAD_TRACE=$PWD/generated/traces/run-01.txt \
RINGPAD_STATS=1 \
RINGPAD_INPUT_SCRIPT=$PWD/tests/acceptance/first-play-chaos0.txt \
  "$APP" "$PWD/ref/<disc folder>/<disc>.gdi"
python3 scripts/recomp/generate_blocks.py generated/traces/run-01.txt
ninja -C generated/build-mac
```

Quit the app normally so the trace is flushed; it is also written every few
seconds. The generator merges new traces into `generated/recomp-manifest.txt`
and rewrites `generated/recomp/`. Both are game-derived and stay ignored.

## Static blocks: the rest of the game's code

Traces only cover what ran on a recorded route. The rest of the game's code is
in the same disc files: `1ST_READ.BIN`, the movie player and 204 PRS-compressed
code modules (stages, adventure fields, bosses, mini-games, Chao and events),
listed with their load addresses in `config/code-modules.txt`.
`scripts/recomp/discover_blocks.py` walks that code from the traced blocks, from
call targets in constant pools and from code pointers that land on a function
prologue, and decodes every block exactly as the executor's `decodeBlock` does:

```sh
python3 scripts/recomp/discover_blocks.py run "ref/<disc folder>/<disc>.gdi" \
  --traced generated/recomp-manifest.txt --out generated/static-manifest.txt
python3 scripts/recomp/generate_blocks.py generated/recomp-manifest.txt \
  --static generated/static-manifest.txt
ninja -C generated/build-mac
```

Static blocks that were not traced compile in a compact form: Flycast's
handlers for PC-relative instructions, addresses relative to the block's entry,
and one function per distinct instruction sequence (the 165 event modules
repeat most of their code). The executor counts their cycles at startup and
runs them under the same byte check as traced blocks, so a block decoded from
data never runs. `RINGPAD_NO_STATIC=1` leaves them to the interpreter for
comparisons. `discover_blocks.py modules <gdi>` rebuilds the module list (about a
minute) if the detection changes.

Then refresh the published block list, which players build from:

```sh
python3 scripts/recomp/disc_blocks.py export "ref/<disc folder>/<disc>.gdi"
```

`config/recomp-blocks.txt` records each block as an address, a file on the disc
(`1ST_READ.BIN`, the IP.BIN boot sector, a PRS-compressed stage, event or movie
module) and an offset, with a SHA-256 per source file and for the whole
manifest. It holds no instruction bytes, and `scripts/check-repo-safety.sh`
rejects any row that could. `disc_blocks.py import <gdi>` rebuilds the exact
manifest from a disc and stops if any source file or the manifest hash differs.
Export also runs static discovery and records its id (`# static`), so a
player's build stops if its static blocks differ from the tested ones.

| Variable | Effect |
|---|---|
| `RINGPAD_TRACE=<file>` | Record blocks that ran on the interpreter. |
| `RINGPAD_STATS=1` | Print AOT coverage every 2 s and the top misses on exit. |
| `RINGPAD_INPUT_SCRIPT=<file>` | Replay pad input at guest times, one event per line: `<start s> <duration s> <what>`. See the script commands below. |
| `RINGPAD_SHOT_DIR=<dir>` | Where `SHOT:` screenshots go (default `generated/shots`). |
| `RINGPAD_STATE_LOAD=<file>` | Resume from a development state written by `STATE:`; the script continues from the saved time. |
| `RINGPAD_NO_AOT=1` | Ignore generated code, for interpreter comparison. |
| `RINGPAD_UNCAPPED=1` | Run without audio pacing, to measure throughput. |
| `RINGPAD_EXIT_AT=<s>` | Print a `RINGPAD_PERF` line at that guest time and exit. |
| `RINGPAD_NO_IDLE_SKIP=1` | Disable idle skipping. |
| `RINGPAD_SELFTEST_ONLY=1` | Run the inline-opcode self-test at boot and exit (status 1 on failure). |
| `RINGPAD_SKIP_SELFTEST=1` | Skip the startup self-test. |
| `RINGPAD_NO_STATIC=1` | Leave statically discovered blocks to the interpreter. |
| `RINGPAD_CHEATS=<id,id>` | Turn on cheats at boot (`rings`, `lives`, `characters`). |

Input script commands:

| `<what>` | Effect |
|---|---|
| `A`, `B`, `X`, `Y`, `START`, `UP`, `DOWN`, `LEFT`, `RIGHT` | Hold the button for the duration. |
| `LT`, `RT` | Hold the analog trigger fully. |
| `STICK:x,y` | Stick position, about ±32000; y grows downwards. |
| `ANGLE:deg[,pct]` | Stick by angle (0 = up, 90 = right) at a percentage of full tilt (default 100). |
| `SHOT:<name>` | With duration 0: save a PNG of the last frame as `<name>.png`. |
| `STATE:<file>` | With duration 0: write a development save state, for iterating a long route one segment at a time. |
| `DUMP:<file>` | With duration 0: write the 16 MB of main RAM, for finding game variables. |

Development states are for building routes only. Acceptance runs replay the
whole route from boot without loading one.

`scripts/recomp/run-route.sh <script> <exit s> <log> [VAR=value ...]` runs one
route with vsync off and prints the `RINGPAD_AOT` and `RINGPAD_PERF` lines.
`generate_blocks.py --no-inline` emits handler calls only, for comparisons.
For profiling with symbols, configure a second build directory with
`-DCMAKE_BUILD_TYPE=RelWithDebInfo`; Flycast strips Release binaries.

## iOS Simulator

Needs Xcode with its licence accepted and the MoltenVK v1.4.2 package
(`MoltenVK-all.tar`, SHA-256
`562a15a29bc358446a56a4091c5f7e08f604184187c1d34f712148b61ef17276`):

```sh
mkdir -p ref/moltenvk-all
curl --fail --location --output ref/moltenvk-all/MoltenVK-all.tar \
  https://github.com/KhronosGroup/MoltenVK/releases/download/v1.4.2/MoltenVK-all.tar
printf '%s  %s\n' \
  562a15a29bc358446a56a4091c5f7e08f604184187c1d34f712148b61ef17276 \
  ref/moltenvk-all/MoltenVK-all.tar | shasum -a 256 -c -
tar xf ref/moltenvk-all/MoltenVK-all.tar -C ref/moltenvk-all
xcrun simctl list devices available
RINGPAD_GDI="/path/to/Sonic Adventure.gdi" \
  scripts/ios/run-sim.sh <udid> tests/acceptance/first-play-chaos0.txt 205 /tmp/sim.log
```

Use one available iPhone or iPad Simulator UDID from the listing, or create
a dedicated Simulator in Xcode. Do not shut down another task's Simulator.
Keep full Xcode selected for this lane; the earlier Command Line Tools setting
was scoped to the macOS configure command only.

The script configures `generated/build-iossim` with the Xcode generator on
every run, builds, installs `com.chrissotraidis.ringpad` and launches it with
the `RINGPAD_*` variables passed through `SIMCTL_CHILD_`. The app reads the
GDI directly from the selected Mac path; the route runner does not copy it
into the Simulator.

UI tests (real UIKit touches, the folder picker and the import flow):

```sh
RINGPAD_GDI="/path/to/Sonic Adventure.gdi" scripts/ios/run-ui-tests.sh <udid>
```

Set `RINGPAD_REAL_GDI=ref/<disc folder>` when running
`scripts/run-host-tests.sh` to also verify the owner's disc on the host.
On iOS, `RINGPAD_IMPORT=<folder>` runs the import flow on launch (a relative
path is inside the app's Documents),
`RINGPAD_IGNORE_INSTALLED=1` shows the import screen, and
`RINGPAD_INPUT_DEBUG=1` shows the port-A values the tests read.
`RINGPAD_IMPORT_VMU=<file>[,<file>]` imports a save without the picker
(paths relative to Documents), as Game Files ▸ Import VMU Save does.

## iPad or iPhone

Generate the recompiled code first (disc reconstruction above, or tracing);
without it the device runs everything on the interpreter, at about half speed.
Back up saves first. Then build, sign with your development team and install
in place using the same identity. Earlier in-place installs preserved the
imported disc, saves and settings; changing signing identity or uninstalling
can disconnect or remove them:

```sh
RINGPAD_TEAM=<team ID> scripts/ios/run-device.sh <device UDID> [--launch]
```

To put the disc on the device without the Files picker, copy the folder into
the app's Documents and launch with the import variable:

```sh
for f in "ref/<disc folder>"/*; do
  xcrun devicectl device copy to --device <UDID> --domain-type appDataContainer \
    --domain-identifier com.chrissotraidis.ringpad --source "$f" \
    --destination "Documents/SonicAdventure/$(basename "$f")"
done
xcrun devicectl device process launch --device <UDID> --terminate-existing \
  --environment-variables '{"RINGPAD_IMPORT":"SonicAdventure"}' com.chrissotraidis.ringpad
```

The import hashes every track on the device before copying it. Same-volume
APFS copies may share storage; do not assume zero additional space for an
external or cloud-backed folder.

## Diagnostic log

The app keeps this session and the previous one in
`Library/Application Support/RingPad/Logs`: everything the runtime writes to
stderr and app events (lifecycle, pauses with their reason, menu, settings,
save states, import, controllers, memory and thermal). Every 30 s of play it
adds two lines: `RINGPAD_LIVE` with guest speed (1.00x is full speed), SH-4
CPU per guest second and recompiled coverage, and `render` with presented
frames per second, frames slower than 50 ms, the worst frame and the graphics
settings. A `stall` line appears if the game has not moved 2 s after a resume.
Players share it from ••• ▸ Help ▸ Share Diagnostic Log;
`scripts/ios/pull-device-log.sh <UDID>` copies it into `generated/device-logs/`.

## Host checks

```sh
scripts/run-host-tests.sh
scripts/check-repo-safety.sh
```

The host runner covers the input adapter, mixer, legacy layout contract,
DCPad, Flycast bridge, game importer and VMU operations. The safety checker
covers repository content, not gameplay acceptance. The publication/history
check and clean player build are recorded in
[RELEASE-CHECKLIST.md](RELEASE-CHECKLIST.md).
