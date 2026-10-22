# Building SonicPad

Requires an Apple Silicon Mac, the Command Line Tools, CMake 3.24+, Ninja,
Python 3.11+ (for `verify-game.py`), Git and Homebrew `libpng`. Run commands
from the repository root.

## One-time setup

```sh
git clone --recursive https://github.com/flyinghead/flycast.git ref/flycast
git -C ref/flycast checkout ed953fcbfdd1b8a7521d2966b13cc700c7e2d233
git -C ref/flycast submodule update --init --recursive
git -C ref/flycast remote set-url --push origin DISABLED
python3 scripts/verify-game.py "ref/<disc folder>/<disc>.gdi"
```

## Build

```sh
export DEVELOPER_DIR=/Library/Developer/CommandLineTools
cmake -S . -B generated/build-mac -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C generated/build-mac
```

The app is `generated/build-mac/flycast/Flycast.app`. With no generated
blocks yet, everything runs on the interpreter.

## Trace, generate, rebuild

```sh
APP=generated/build-mac/flycast/Flycast.app/Contents/MacOS/Flycast
SONICPAD_TRACE=$PWD/generated/traces/run-01.txt \
SONICPAD_STATS=1 \
SONICPAD_INPUT_SCRIPT=$PWD/tests/acceptance/first-play-chaos0.txt \
  "$APP" "$PWD/ref/<disc folder>/<disc>.gdi"
python3 scripts/recomp/generate_blocks.py generated/traces/run-01.txt
ninja -C generated/build-mac
```

Quit the app normally so the trace is flushed; it is also written every few
seconds. The generator merges new traces into `generated/recomp-manifest.txt`
and rewrites `generated/recomp/`. Both are game-derived and stay ignored.

| Variable | Effect |
|---|---|
| `SONICPAD_TRACE=<file>` | Record blocks that ran on the interpreter. |
| `SONICPAD_STATS=1` | Print AOT coverage every 2 s and the top misses on exit. |
| `SONICPAD_INPUT_SCRIPT=<file>` | Replay pad input at guest times, one event per line: `<start s> <duration s> <what>`. See the script commands below. |
| `SONICPAD_SHOT_DIR=<dir>` | Where `SHOT:` screenshots go (default `generated/shots`). |
| `SONICPAD_STATE_LOAD=<file>` | Resume from a development state written by `STATE:`; the script continues from the saved time. |
| `SONICPAD_NO_AOT=1` | Ignore generated code, for interpreter comparison. |
| `SONICPAD_UNCAPPED=1` | Run without audio pacing, to measure throughput. |
| `SONICPAD_EXIT_AT=<s>` | Print a `SONICPAD_PERF` line at that guest time and exit. |
| `SONICPAD_NO_IDLE_SKIP=1` | Disable idle skipping. |
| `SONICPAD_SELFTEST_ONLY=1` | Run the inline-opcode self-test at boot and exit (status 1 on failure). |
| `SONICPAD_SKIP_SELFTEST=1` | Skip the startup self-test. |

Input script commands:

| `<what>` | Effect |
|---|---|
| `A`, `B`, `X`, `Y`, `START`, `UP`, `DOWN`, `LEFT`, `RIGHT` | Hold the button for the duration. |
| `LT`, `RT` | Hold the analog trigger fully. |
| `STICK:x,y` | Stick position, about ±32000; y grows downwards. |
| `ANGLE:deg[,pct]` | Stick by angle (0 = up, 90 = right) at a percentage of full tilt (default 100). |
| `SHOT:<name>` | With duration 0: save a PNG of the last frame as `<name>.png`. |
| `STATE:<file>` | With duration 0: write a development save state, for iterating a long route one segment at a time. |

Development states are for building routes only. Acceptance runs replay the
whole route from boot without loading one.

`scripts/recomp/run-route.sh <script> <exit s> <log> [VAR=value ...]` runs one
route with vsync off and prints the `SONICPAD_AOT` and `SONICPAD_PERF` lines.
`generate_blocks.py --no-inline` emits handler calls only, for comparisons.
For profiling with symbols, configure a second build directory with
`-DCMAKE_BUILD_TYPE=RelWithDebInfo`; Flycast strips Release binaries.

## iOS Simulator

Needs Xcode with its licence accepted and the MoltenVK v1.4.2 package
(`MoltenVK-all.tar`, SHA-256
`562a15a29bc358446a56a4091c5f7e08f604184187c1d34f712148b61ef17276`):

```sh
mkdir -p ref/moltenvk-all && cd ref/moltenvk-all
gh release download v1.4.2 -R KhronosGroup/MoltenVK -p MoltenVK-all.tar
tar xf MoltenVK-all.tar && cd ../..
xcrun simctl create "SonicPad iPad Air 11 (M3)" \
  com.apple.CoreSimulator.SimDeviceType.iPad-Air-11-inch-M3 \
  com.apple.CoreSimulator.SimRuntime.iOS-26-5
scripts/ios/run-sim.sh <udid> tests/acceptance/first-play-chaos0.txt 205 /tmp/sim.log
```

The script configures `generated/build-iossim` with the Xcode generator on
first use, builds, installs `com.chrissotraidis.sonicpad` and launches it with
the `SONICPAD_*` variables passed through `SIMCTL_CHILD_`. The app reads the
GDI directly from `ref/`; nothing is copied into the Simulator.

UI tests (real UIKit touches, the folder picker and the import flow):

```sh
scripts/ios/run-ui-tests.sh <udid>
```

Set `SONICPAD_REAL_GDI=ref/<disc folder>` when running
`scripts/run-host-tests.sh` to also verify the owner's disc on the host.
On iOS, `SONICPAD_IMPORT=<folder>` runs the import flow on launch (a relative
path is inside the app's Documents),
`SONICPAD_IGNORE_INSTALLED=1` shows the import screen, and
`SONICPAD_INPUT_DEBUG=1` shows the port-A values the tests read.

## iPad or iPhone

Generate the recompiled code first (trace, generate, rebuild above);
without it the device runs everything on the interpreter, at about half speed.
Then build, sign with your development team and install in place (the
imported disc, saves and settings stay):

```sh
SONICPAD_TEAM=<team ID> scripts/ios/run-device.sh <device UDID> [--launch]
```

To put the disc on the device without the Files picker, copy the folder into
the app's Documents and launch with the import variable:

```sh
for f in ref/<disc folder>/*; do
  xcrun devicectl device copy to --device <UDID> --domain-type appDataContainer \
    --domain-identifier com.chrissotraidis.sonicpad --source "$f" \
    --destination "Documents/SonicAdventure/$(basename "$f")"
done
xcrun devicectl device process launch --device <UDID> --terminate-existing \
  --environment-variables '{"SONICPAD_IMPORT":"SonicAdventure"}' com.chrissotraidis.sonicpad
```

The import hashes every track on the device and clones the files, so it uses
no extra space.

## Diagnostic log

The app keeps this session and the previous one in
`Library/Application Support/SonicPad/Logs`: everything the runtime writes to
stderr, app events, and a `SONICPAD_LIVE` line every 30 s with guest speed
(1.00x is full speed), SH-4 CPU per guest second and recompiled coverage.
Players share it from ••• ▸ Help ▸ Share Diagnostic Log;
`scripts/ios/pull-device-log.sh <UDID>` copies it into `generated/device-logs/`.

Host input tests for the touch layer: `scripts/run-host-tests.sh`.
