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
bin="$out/test_dreamcast_input"
log="$out/host-tests.log"

{
    echo "host test run: $(date -u "+%Y-%m-%dT%H:%M:%SZ")"
    echo "compiler: $CXX"
    echo "linker:   $LLD"
    echo "sdk:      $SDK"
    echo
} | tee "$log"

"$CXX" -std=c++20 -Wall -Wextra -Werror -O2 -I port/input \
    port/input/DreamcastInputAdapter.cpp tests/host/test_dreamcast_input.cpp \
    -o "$bin" --ld-path="$LLD" -isysroot "$SDK" \
    -L "$CXXLIB" -Wl,-rpath,"$CXXLIB" > "$out/build.out" 2>&1
rc=$?
cat "$out/build.out" | tee -a "$log"
if [ ! -x "$bin" ]; then echo "BUILD FAILED" | tee -a "$log"; exit 4; fi

"$bin" > "$out/run.out" 2>&1
rc=$?
cat "$out/run.out" | tee -a "$log"
echo "exit status: $rc" | tee -a "$log"
exit "$rc"
