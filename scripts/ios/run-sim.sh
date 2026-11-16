#!/bin/sh
# Build RingPad for the iOS Simulator, install it on one Simulator and run a
# scripted route. Only one Simulator is booted at a time.
# Usage: scripts/ios/run-sim.sh <simulator UDID> <input script> <exit at guest s> <log file>
set -eu
cd "$(dirname "$0")/../.."
udid=$1; script=$2; exit_at=$3; log=$4
build=generated/build-iossim
app=$build/flycast/Release-iphonesimulator/RingPad.app
gdi=${RINGPAD_GDI:-$PWD/$(ls ref/*/*.gdi | head -1)}

# Configure every time (about 2 s once cached): the Xcode generator only sees
# newly generated recompiled sources when CMake runs again.
cmake -S . -B "$build" -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_BUILD_TYPE=Release -Wno-deprecated >/dev/null
cmake --build "$build" --config Release --target flycast -- -sdk iphonesimulator -arch arm64 -quiet

booted=$(xcrun simctl list devices booted | grep -c Booted || true)
if [ "$booted" -gt 0 ] && ! xcrun simctl list devices booted | grep -q "$udid"; then
    echo "Another Simulator is booted; shut it down first." >&2
    exit 2
fi
xcrun simctl boot "$udid" 2>/dev/null || true
xcrun simctl bootstatus "$udid" -b >/dev/null
# Show the Simulator window when the app is installed; the run itself is headless.
open -a Simulator --args -CurrentDeviceUDID "$udid" 2>/dev/null || true
xcrun simctl install "$udid" "$app"
SIMCTL_CHILD_RINGPAD_GAME="$gdi" \
SIMCTL_CHILD_RINGPAD_STATS=1 \
SIMCTL_CHILD_RINGPAD_INPUT_SCRIPT="$PWD/$script" \
SIMCTL_CHILD_RINGPAD_EXIT_AT="$exit_at" \
    xcrun simctl launch --terminate-running-process --stdout="$log.out" --stderr="$log" "$udid" com.chrissotraidis.ringpad
echo "Launched; follow $log for RINGPAD_STATS and the final RINGPAD_PERF line."
