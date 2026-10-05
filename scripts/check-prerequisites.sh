#!/bin/sh
# Read-only toolchain selection; outputs are private, isolated probe artifacts.
# Exit 0: macOS and full Xcode tools usable. Exit 2: an Apple lane is blocked.
set -eu
cd "$(dirname "$0")/.."

mkdir -p docs/artifacts
out=$(mktemp -d "$PWD/docs/artifacts/prerequisites.XXXXXX")
apple_dir=${SONICPAD_APPLE_DEVELOPER_DIR:-/Library/Developer/CommandLineTools}
xcode_dir=${SONICPAD_XCODE_DEVELOPER_DIR:-/Applications/Xcode.app/Contents/Developer}
cat > "$out/probe.mm" <<'EOF'
#import <Foundation/Foundation.h>
#include <cstdio>
int main() {
    @autoreleasepool {
        if (sizeof(void*) != 8 || ![NSProcessInfo processInfo]) return 1;
        std::puts("APPLE_NATIVE_PROBE arm64 pointer_bits=64 foundation=ready");
    }
    return 0;
}
EOF

blocked=0
if DEVELOPER_DIR="$apple_dir" /usr/bin/clang++ -std=c++20 -arch arm64 \
    -mmacosx-version-min=13.0 -framework Foundation "$out/probe.mm" \
    -o "$out/probe" > "$out/macos-build.log" 2>&1 && \
    "$out/probe" > "$out/macos-run.log" 2>&1; then
    cat "$out/macos-run.log"
    DEVELOPER_DIR="$apple_dir" /usr/bin/xcrun vtool -show-build "$out/probe" \
        > "$out/macos-platform.log"
    cat "$out/macos-platform.log"
    shasum -a 256 "$out/probe"
else
    cat "$out/macos-build.log"
    [ ! -f "$out/macos-run.log" ] || cat "$out/macos-run.log"
    echo "MACOS_TOOLCHAIN=blocked"
    blocked=1
fi

if DEVELOPER_DIR="$xcode_dir" /usr/bin/xcodebuild -checkFirstLaunchStatus \
    > "$out/xcode-status.log" 2>&1; then
    # Enumerate existing devices only; never boot or shut down a Simulator here.
    if DEVELOPER_DIR="$xcode_dir" /usr/bin/xcrun simctl list devices booted \
        > "$out/booted-simulators.log" 2>&1; then
        echo "XCODE_TOOLS=usable"
        cat "$out/booted-simulators.log"
    else
        cat "$out/booted-simulators.log"
        blocked=1
    fi
else
    cat "$out/xcode-status.log"
    echo "XCODE_TOOLS=blocked (macOS CLT lane assessed separately)"
    blocked=1
fi
echo "Evidence: $out"
[ "$blocked" -eq 0 ] || exit 2
