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

Host input tests for the touch layer: `scripts/run-host-tests.sh`.
