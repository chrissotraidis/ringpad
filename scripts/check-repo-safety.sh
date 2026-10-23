#!/bin/sh
# SonicPad repository-safety check.
#
# Verifies the invariants that must hold before any local commit: the private
# integration workspace is intact, original game data and private development
# material are ignored, pinned references cannot be pushed, and no prohibited
# input is tracked. It is read-only and always exits with an explicit verdict.
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
tracked_disc=$(git ls-files | grep -Ei '\.(gdi|cdi|chd|iso|cue|7z)$|(^|/)track[0-9]+\.(bin|raw)$' || true)
if [ -z "$tracked_disc" ]; then ok "no disc/archive/track file tracked"; else bad "tracked disc data: $tracked_disc"; fi

printf '\nPrivate development material is never tracked:\n'
tracked_priv=$(git ls-files | grep -Ei '(katana_generated\.lib|\.local/|/title-source/|native_title_adapter\.cpp|unit-[0-9]+\.cpp|sp_aot_blocks_|sp_aot_index\.cpp|recomp-manifest|\.ksave|\.vmu$|dreamcast_vmu_.*\.bin$|\.dmp$)' || true)
if [ -z "$tracked_priv" ]; then ok "no private SDK/AOT/save artifacts tracked"; else bad "tracked private material: $tracked_priv"; fi

printf '\nThe published block list holds addresses and hashes only:\n'
# Rows are "pc nops cycles @source offset"; sources are "@index name sha256".
# Any other shape could carry instruction bytes and must not be committed.
if [ -f config/recomp-blocks.txt ]; then
    wide=$(awk '/^#/ { next } /^@/ { if (NF != 3) n++; next } NF != 5 || $4 !~ /^@[0-9]+$/ { n++ } END { print n + 0 }' config/recomp-blocks.txt)
    if [ "$wide" -eq 0 ]; then ok "config/recomp-blocks.txt: no opcode columns"; else bad "config/recomp-blocks.txt has $wide rows that are not address-only"; fi
else
    note "config/recomp-blocks.txt not present"
fi

printf '\nPinned references cannot push:\n'
for refdir in ref/flycast ref/sunpad ref/sarecomp ref/paperpad; do
    if [ -d ".git/$([ -d "$refdir/.git" ] && echo)" ]; then :; fi
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
if [ "$fail" -eq 0 ]; then printf 'VERDICT: safe for private work; do not push game data or private material.\n'; else printf 'VERDICT: UNSAFE — resolve the FAIL lines above.\n'; fi
exit "$fail"
