#!/bin/sh
# Copy SonicPad's diagnostic logs (this session and the previous one) from a
# device into generated/device-logs/<time>/.
# Usage: scripts/ios/pull-device-log.sh <device UDID>
set -eu
cd "$(dirname "$0")/../.."
out=generated/device-logs/$(date +%Y%m%d-%H%M%S)
mkdir -p "$out"
for name in sonicpad.log sonicpad.previous.log; do
    xcrun devicectl device copy from --device "$1" --domain-type appDataContainer \
        --domain-identifier com.chrissotraidis.sonicpad \
        --source "Library/Application Support/SonicPad/Logs/$name" --destination "$out/$name" >/dev/null 2>&1 || true
done
ls -l "$out"
