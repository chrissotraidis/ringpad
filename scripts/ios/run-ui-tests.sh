#!/bin/sh
# Run the SonicPad iOS UI tests (real UIKit touches) on one Simulator.
# Usage: scripts/ios/run-ui-tests.sh <simulator UDID> [extra xcodebuild args, e.g. -only-testing:...]
set -eu
cd "$(dirname "$0")/../.."
udid=$1; shift
build=generated/build-iossim
gdi=${SONICPAD_GDI:-$PWD/$(ls ref/*/*.gdi | head -1)}
results=generated/ui-tests-$(date +%Y%m%d-%H%M%S).xcresult

if [ ! -d "$build" ]; then
    cmake -S . -B "$build" -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_BUILD_TYPE=Release
fi
if xcrun simctl list devices booted | grep Booted | grep -vq "$udid"; then
    echo "Another Simulator is booted; shut it down first." >&2
    exit 2
fi
TEST_RUNNER_SONICPAD_GDI="$gdi" \
TEST_RUNNER_DCPAD_PREVIEW="${DCPAD_PREVIEW:-}" \
TEST_RUNNER_SONICPAD_GDI_DIR="$(dirname "$gdi")" \
    xcodebuild test -project "$build/sonicpad.xcodeproj" -scheme flycast -configuration Release \
        -destination "id=$udid" -resultBundlePath "$results" "$@"
echo "Results: $results"
