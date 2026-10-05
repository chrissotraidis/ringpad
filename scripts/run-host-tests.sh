#!/bin/sh
# SonicPad host test runner.
#
# Builds and runs source-derived host tests for the game-independent input
# contract. It uses a self-contained Homebrew LLVM chain so it does not depend
# on the license-gated Apple linker; this is a HOST LOGIC test only and makes
# no claim about ARM64 Apple ABI, the guest boundary, or any target build.
#
# Usage: scripts/run-host-tests.sh
set -u

cd "$(dirname "$0")/.." || exit 2

CXX= ; [ -n "$CXX" ] || CXX=/opt/homebrew/opt/llvm/bin/clang++
LLD= ; [ -n "$LLD" ] || LLD=/opt/homebrew/bin/ld64.lld
CXXLIB= ; [ -n "$CXXLIB" ] || CXXLIB=/opt/homebrew/opt/llvm/lib/c++
SDK= ; [ -n "$SDK" ] || SDK=/Library/Developer/CommandLineTools/SDKs/MacOSX12.1.sdk

for tool in "$CXX" "$LLD"; do
    if [ ! -x "$tool" ]; then echo "MISSING: $tool" >&2; exit 3; fi
done
if [ ! -d "$SDK" ]; then
    echo "MISSING SDK: $SDK" >&2
    echo "(SonicPad host tests need an SDK that ld64.lld can parse.)" >&2
    exit 3
fi

out="docs/artifacts/host-tests"
mkdir -p "$out"
log="$out/host-tests.log"

{
    echo "host test run: $(date -u "+%Y-%m-%dT%H:%M:%SZ")"
    echo "compiler: $CXX"
    echo "linker:   $LLD"
    echo "sdk:      $SDK"
    echo
} | tee "$log"

# Each suite is a self-contained binary. Fail fast on the first failure.
run_suite() {
    name="$1"; shift
    bin="$out/$name"
    rm -f "$bin"
    "$CXX" -std=c++20 -Wall -Wextra -Werror -O2 -I port/input "$@" \
        -o "$bin" --ld-path="$LLD" -isysroot "$SDK" \
        -L "$CXXLIB" -Wl,-rpath,"$CXXLIB" > "$out/$name.build.out" 2>&1
    if [ $? -ne 0 ] || [ ! -x "$bin" ]; then
        cat "$out/$name.build.out" | tee -a "$log"
        echo "BUILD FAILED: $name" | tee -a "$log"
        return 4
    fi
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
exit "$rc"
