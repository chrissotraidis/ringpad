#!/bin/sh
# Copy RingPad's diagnostic logs (this session and the previous one) from a
# device into generated/device-logs/<time>/.
# Usage: scripts/ios/pull-device-log.sh <device UDID>
set -eu
cd "$(dirname "$0")/../.."
out=generated/device-logs/$(date +%Y%m%d-%H%M%S)
mkdir -p "$out"
for name in ringpad.log ringpad.previous.log; do
    xcrun devicectl device copy from --device "$1" --domain-type appDataContainer \
        --domain-identifier com.chrissotraidis.ringpad \
        --source "Library/Application Support/RingPad/Logs/$name" --destination "$out/$name" >/dev/null 2>&1 || true
done
ls -l "$out"
