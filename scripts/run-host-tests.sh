#!/bin/sh
# SonicPad host test runner.
#
# Builds and runs source-derived host tests for the game-independent input
# contract. The default is the independently installed Apple Command Line
# Tools, selected for this process only. SONICPAD_HOST_TOOLCHAIN=llvm retains
# the Homebrew lane. These tests do not link Katana or test the guest ABI.
#
# Usage: scripts/run-host-tests.sh
set -u

cd "$(dirname "$0")/.." || exit 2

lane=${SONICPAD_HOST_TOOLCHAIN:-apple}
case "$lane" in
    apple)
        DEVELOPER_DIR=${SONICPAD_APPLE_DEVELOPER_DIR:-/Library/Developer/CommandLineTools}
        export DEVELOPER_DIR
        CXX=${CXX:-/usr/bin/clang++}
        SDK=${SDK:-$(/usr/bin/xcrun --sdk macosx --show-sdk-path)}
        ;;
    llvm)
        CXX=${CXX:-/opt/homebrew/opt/llvm/bin/clang++}
        LLD=${LLD:-/opt/homebrew/bin/ld64.lld}
        CXXLIB=${CXXLIB:-/opt/homebrew/opt/llvm/lib/c++}
        SDK=${SDK:-/Library/Developer/CommandLineTools/SDKs/MacOSX12.1.sdk}
        if [ ! -x "$LLD" ]; then echo "MISSING: $LLD" >&2; exit 3; fi
        ;;
    *) echo "Unknown host toolchain: $lane (use apple or llvm)" >&2; exit 2 ;;
esac

for tool in "$CXX"; do
    if [ ! -x "$tool" ]; then echo "MISSING: $tool" >&2; exit 3; fi
done
if [ ! -d "$SDK" ]; then
    echo "MISSING SDK: $SDK" >&2
    exit 3
fi

out="docs/artifacts/host-tests/$lane"
mkdir -p "$out"
log="$out/host-tests.log"

{
    echo "host test run: $(date -u "+%Y-%m-%dT%H:%M:%SZ")"
    echo "compiler: $CXX"
    echo "toolchain lane: $lane"
    "$CXX" --version
    echo "sdk:      $SDK"
    echo
} | tee "$log"

# Each suite is a self-contained binary. Fail fast on the first failure.
run_suite() {
    name="$1"; shift
    bin="$out/$name"
    # Compile to a separate file so a failed compile can never run an old binary.
    if [ "$lane" = apple ]; then
        "$CXX" -std=c++20 -Wall -Wextra -Werror -O2 -I port/input "$@" \
            -arch arm64 -mmacosx-version-min=13.0 -isysroot "$SDK" \
            -o "$bin.building" > "$out/$name.build.out" 2>&1
        build_rc=$?
    else
        "$CXX" -std=c++20 -Wall -Wextra -Werror -O2 -I port/input "$@" \
            -o "$bin.building" --ld-path="$LLD" -isysroot "$SDK" \
            -L "$CXXLIB" -Wl,-rpath,"$CXXLIB" > "$out/$name.build.out" 2>&1
        build_rc=$?
    fi
    if [ "$build_rc" -ne 0 ] || [ ! -x "$bin.building" ]; then
        cat "$out/$name.build.out" | tee -a "$log"
        echo "BUILD FAILED: $name" | tee -a "$log"
        return 4
    fi
    mv "$bin.building" "$bin" || return 4
    "$bin" > "$out/$name.run.out" 2>&1
    rc=$?
    cat "$out/$name.run.out" | tee -a "$log"
    echo "$name exit status: $rc" | tee -a "$log"
    return "$rc"
}

run_suite test_dreamcast_input \
    port/input/DreamcastInputAdapter.cpp tests/host/test_dreamcast_input.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

run_suite test_dreamcast_shell \
    port/input/DreamcastInputAdapter.cpp port/input/DreamcastInputMixer.cpp \
    port/input/DreamcastControllerMapping.cpp tests/host/test_dreamcast_shell.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

run_suite test_dreamcast_touch \
    port/input/DreamcastTouchLayout.cpp tests/host/test_dreamcast_touch.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

run_suite test_dcpad -I port/dcpad/core \
    port/dcpad/core/DCPad.cpp tests/host/test_dcpad.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

run_suite test_flycast_bridge \
    port/input/FlycastPadBridge.cpp port/input/DreamcastInputAdapter.cpp tests/host/test_flycast_bridge.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

run_suite test_game_import -I port/game \
    port/game/GameImport.cpp tests/host/test_game_import.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

run_suite test_vmu_image -I port/game \
    port/game/VMUImage.cpp tests/host/test_vmu_image.cpp
rc=$?
[ "$rc" -eq 0 ] || exit "$rc"

# The app's built-in disc identity must match config/game-identity.json.
python3 - <<'EOF' | tee -a "$log"
import json, re, sys
identity = json.load(open('config/game-identity.json'))['supported']
source = open('port/game/GameImport.cpp').read()
missing = [t['sha256'] for t in identity['tracks'] if t['sha256'] not in source or str(t['bytes']) not in source]
missing += [identity['gdi_sha256']] if identity['gdi_sha256'] not in source else []
print('game identity consistency:', 'PASS' if not missing else 'FAIL ' + ', '.join(missing))
sys.exit(1 if missing else 0)
EOF
rc=$?
exit "$rc"
