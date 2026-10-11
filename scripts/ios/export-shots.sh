#!/bin/sh
# Export the named screenshot attachments of the newest UI-test result bundle.
# Usage: scripts/ios/export-shots.sh <output dir> [result bundle]
set -eu
cd "$(dirname "$0")/../.."
out=$1
bundle=${2:-$(ls -d generated/ui-tests-*.xcresult | tail -1)}
mkdir -p "$out"
# xcresulttool refuses to overwrite an existing manifest.
find "$out" -maxdepth 1 -name manifest.json -delete
xcrun xcresulttool export attachments --path "$bundle" --output-path "$out" >/dev/null
python3 - "$out" <<'EOF'
import json, shutil, sys, os
out = sys.argv[1]
for test in json.load(open(os.path.join(out, 'manifest.json'))):
    for a in test.get('attachments', []):
        name = a['suggestedHumanReadableName'].split('_0_')[0] + '.png'
        shutil.copy(os.path.join(out, a['exportedFileName']), os.path.join(out, name))
        print(name)
EOF
