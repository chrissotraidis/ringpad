#!/bin/sh
# Run a scripted acceptance route on the macOS build and print its PERF line.
# Usage: scripts/recomp/run-route.sh <input script> <exit at guest seconds> <log file> [extra VAR=value ...]
# Example: scripts/recomp/run-route.sh tests/acceptance/first-play-chaos0.txt 200 /tmp/run.log SONICPAD_UNCAPPED=1
set -eu
cd "$(dirname "$0")/../.."
script=$1; exit_at=$2; log=$3; shift 3
gdi=${SONICPAD_GDI:-$(ls ref/*/*.gdi | head -1)}
app=generated/build-mac/flycast/Flycast.app/Contents/MacOS/Flycast
if pgrep -x Flycast >/dev/null; then echo "Another Flycast instance is running; quit it first." >&2; exit 2; fi
env SONICPAD_INPUT_SCRIPT="$PWD/$script" SONICPAD_EXIT_AT="$exit_at" SONICPAD_STATS=1 "$@" \
    "$app" -config config:rend.vsync=no "$PWD/$gdi" > "$log" 2>&1 || true
grep -E 'SONICPAD_(AOT|PERF)' "$log"
