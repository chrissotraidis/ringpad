# Building SonicPad

Requires an Apple Silicon Mac, the Command Line Tools, CMake 3.24+, Ninja,
Python 3, Git and Homebrew `libpng`. Run commands from the repository root.

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
| `SONICPAD_INPUT_SCRIPT=<file>` | Replay pad input at guest times: `<start s> <duration s> <A,B,X,Y,START,UP,DOWN,LEFT,RIGHT or STICK:x,y>`. |
| `SONICPAD_NO_AOT=1` | Ignore generated code, for interpreter comparison. |
| `SONICPAD_UNCAPPED=1` | Run without audio pacing, to measure throughput. |
| `SONICPAD_EXIT_AT=<s>` | Print a `SONICPAD_PERF` line at that guest time and exit. |
| `SONICPAD_NO_IDLE_SKIP=1` | Disable idle skipping. |
| `SONICPAD_SELFTEST_ONLY=1` | Run the inline-opcode self-test at boot and exit (status 1 on failure). |
| `SONICPAD_SKIP_SELFTEST=1` | Skip the startup self-test. |

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

Host input tests for the touch layer: `scripts/run-host-tests.sh`.
