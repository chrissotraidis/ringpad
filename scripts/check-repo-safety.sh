#!/bin/sh
# SonicPad repository-safety check.
#
# Checks tracked filenames and recipe metadata before a commit or release.
# Private inputs remain ignored and reference checkouts cannot push.
# This is a hygiene check, not a review of source provenance or Git history.
#
# Usage: scripts/check-repo-safety.sh
set -u

fail=0
note() { printf '  %s\n' "$1"; }
ok()   { printf '  [ ok ] %s\n' "$1"; }
bad()  { printf '  [FAIL] %s\n' "$1"; fail=1; }

cd "$(dirname "$0")/.." || exit 2
printf 'SonicPad repository-safety check (%s)\n' "$(pwd)"

printf '\nIgnored private inputs and evidence:\n'
for entry in ref/ private-inputs/ work/ generated/ out/ docs/artifacts/; do
    if git check-ignore -q "$entry" 2>/dev/null; then ok "$entry"; else bad "$entry is not ignored"; fi
done

printf '\nOriginal game data is never tracked:\n'
tracked_disc=$(git ls-files | grep -Ei '\.(gdi|cdi|chd|iso|cue|7z|zip|ipa)$|(^|/)track[0-9]+\.(bin|raw)$' || true)
if [ -z "$tracked_disc" ]; then ok "no disc/archive/track file tracked"; else bad "tracked disc data: $tracked_disc"; fi

printf '\nPrivate development material is never tracked:\n'
tracked_priv=$(git ls-files | grep -Ei '(katana_generated\.lib|(^|/)(ref|generated|private-inputs|work|build|out|\.local)/|docs/artifacts/|/title-source/|native_title_adapter\.cpp|unit-[0-9]+\.cpp|sp_aot_blocks_|sp_aot_index\.cpp|recomp-manifest|static-manifest|\.ksave|\.(vmu|vms|vmi|sav|p12|pfx|mobileprovision|key|pem|dmp)$|(^|/)\.env($|\.)|dreamcast_vmu_.*\.bin$)' || true)
if [ -z "$tracked_priv" ]; then ok "no private SDK/AOT/save artifacts tracked"; else bad "tracked private material: $tracked_priv"; fi

printf '\nThe published block list holds addresses and hashes only:\n'
# Rows are "pc nops cycles @source offset"; sources are "@index name sha256".
# Any other shape could carry instruction bytes and must not be committed.
if [ -f config/recomp-blocks.txt ]; then
    if python3 - <<'PYCODE'
from pathlib import Path
import re
import sys

source = re.compile(r'@[0-9]+ (?:[A-Z0-9_]+(?:/[A-Z0-9_]+)*\.[A-Z0-9]+ [0-9a-f]{16}|reios -)')
block = re.compile(r'[0-9A-F]{8} [1-9][0-9]* [0-9]+ @[0-9]+ [0-9A-F]+')
module = re.compile(r'SONICAD/[A-Z0-9_]+\.PRS [0-9A-F]{8} [0-9a-f]{16}')
failed = False
for name, patterns in [('config/recomp-blocks.txt', (source, block)),
                       ('config/code-modules.txt', (module,))]:
    for number, line in enumerate(Path(name).read_text().splitlines(), 1):
        if line.startswith('#'):
            continue
        if not any(pattern.fullmatch(line) for pattern in patterns):
            print('%s:%d: invalid metadata row (contents withheld)' % (name, number))
            failed = True
sys.exit(1 if failed else 0)
PYCODE
    then ok "block and module lists contain only metadata rows"; else bad "invalid block/module metadata"; fi
else
    note "config/recomp-blocks.txt not present"
fi

printf '\nPinned references cannot push:\n'
for refdir in ref/flycast ref/sunpad ref/sarecomp ref/paperpad; do
    if [ -e "$refdir/.git" ]; then
        url=$(git -C "$refdir" remote get-url --push origin 2>/dev/null || echo "")
        if [ "$url" = "DISABLED" ]; then ok "$refdir push disabled"; else bad "$refdir push URL is '$url'"; fi
    else
        note "$refdir not present (fine if not yet pinned)"
    fi
done

printf '\nKnown-good local commit tree is intact (unknown work is never reset):\n'
if git rev-parse --verify -q HEAD >/dev/null; then ok "HEAD = $(git rev-parse --short HEAD)"; else bad "no HEAD revision"; fi

printf '\n'
if [ "$fail" -eq 0 ]; then printf 'VERDICT: repository hygiene checks passed; review provenance and history before publication.\n'; else printf 'VERDICT: UNSAFE — resolve the FAIL lines above.\n'; fi
exit "$fail"
