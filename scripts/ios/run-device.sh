#!/bin/sh
# Build SonicPad for a physical iPad or iPhone, sign it with your development
# team and install it in place: the imported disc, saves and settings stay.
# Usage: SONICPAD_TEAM=<team ID> scripts/ios/run-device.sh <device UDID> [--launch]
# --launch starts the app and streams its console until you press Ctrl-C.
set -eu
cd "$(dirname "$0")/../.."
udid=$1; launch=${2:-}
team=${SONICPAD_TEAM:?set SONICPAD_TEAM to your Apple development team ID}
build=generated/build-ios
app=$build/flycast/Release-iphoneos/SonicPad.app

# Configure every time so newly generated recompiled sources are included.
cmake -S . -B "$build" -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_BUILD_TYPE=Release -Wno-deprecated >/dev/null
# Flycast's CMake turns signing off for its own iOS target; turn it back on.
xcodebuild build -project "$build/sonicpad.xcodeproj" -scheme flycast -configuration Release \
    -destination "id=$udid" -allowProvisioningUpdates \
    DEVELOPMENT_TEAM="$team" CODE_SIGN_STYLE=Automatic CODE_SIGNING_ALLOWED=YES \
    CODE_SIGN_IDENTITY="Apple Development" -quiet
codesign --verify --deep --strict "$app"
xcrun devicectl device install app --device "$udid" "$app"
if [ "$launch" = "--launch" ]; then
    xcrun devicectl device process launch --device "$udid" --terminate-existing --console com.chrissotraidis.sonicpad
fi
