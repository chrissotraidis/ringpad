# SonicPad status

Updated: **5 October 2026**. Private integration workspace; publication not approved.

Revalidated 2026-10-05 ("figure it out" turn): the two G1 prerequisites and the
Apple-toolchain license are unchanged, but the acquisition route is now **exact**
instead of vague. G1's missing source is one retired local archive, not a
repository: the 12-part `r354-development.zip.part001…012` set (tag
`r354-baseline`, concatenated 3,115,490,620 bytes, archive SHA-256
`07f28aa2052827c34f94fe20ad1c53b129405ec019f85486675186d9e7508ee9`) restorable by
`tools/restore-baseline.py` with **no download**; upstream `DEVELOPMENT.md`
states it "has been removed from GitHub… must not be uploaded again" and that a
clean-machine source bootstrap "is not yet provided." A full-filesystem sweep and
an authenticated `gh` code search found no other route: `sonicfreak1337` exposes
only the public SARecomp (one branch, one tag, player-installer release assets,
no Apple build files, no Katana source). `supported_disc.hpp` still accepts
exactly one release (PAL v1.003). `/usr/bin/clang++` fails with the license
message, so the gate blocks every Apple compile, not only `xcodebuild`/`simctl`.
Host suites stay 46/46 + 50/50 + 46/46 PASS.

Snapshot: this session's work is committed locally on `main` (never pushed).

Lowest unmet goal: **G1 — complete source and game inputs.** G0 is met except
for its toolchain-use step: the Apple SDKs are present but the license is not
accepted, so no Apple compile or link is possible yet.

Authorization: private-only; publication not approved. No push, release or
share is authorized.

G0: references pinned and frozen read-only (SARecomp `d5288fe`, SunPad
`c77dd61`, PaperPad `a6f6999`, all push-disabled); safety ignores and
`scripts/check-repo-safety.sh` established. Xcode 27.0 and the macOS/iOS/iOS
Simulator 27.0 SDKs are present, but the Apple toolchain is **license-blocked**,
so G0 is not fully met and no Apple target can build yet.

Source closure: **blocked**. Two independent, exact prerequisites are missing
(details in docs/BOOTSTRAP-BLOCKERS.md):

1. **Game input incompatible** — only a multi-region MK-51000 V1.004 CDI
   container is present. The supported target is PAL v1.003 MK-5100050 as a
   three-track GDI, and no PAL GDI/track file exists on the machine.
2. **Private Katana/title source closure missing** — the public SARecomp clone
   is insufficient by its own docs/DEVELOPMENT.md; the pinned r354 bundle
   (Katana source/runtime, generated AOT, title source) is not present locally.

Highest observed target and behavior: **host source-derived logic tests only.**
Three suites pass: the game-independent Dreamcast input adapter and edge latch
(46/46), the single-stick shell layer reused from pinned SunPad (mixer, narrow
face mapping, player slots; 50/50), and the touch layout + per-touch arbiter
reused from the pinned SunPad overlay (46/46). No Apple target has been
configured, compiled, linked or run.

Known-good build/run/test command: scripts/run-host-tests.sh (host logic; three
suites, uses the self-contained Homebrew LLVM chain, not the Apple toolchain).

Exact artifact/source identity: SARecomp pin
d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2 in ref/sarecomp (push disabled);
provided dump ref/Sonic Adventure.cdi SHA-256
105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533.

Matrix: rows 1, 20 and 29 **partial pass** (workspace safety + two input host
suites); rows 2, 3 and 30 **blocked**; all other rows **not-run**. See
docs/TEST-MATRIX.md. Journal: docs/JOURNAL.md.

Physical-device acceptance: not performed.

Open defect and unblocking rung reached: source-access boundary. Unblocking
ladder rung 5 (resolve dependencies / research the exact contract) and rung 8
(park the blocked work honestly). No further local retry can create absent
source.

Next step: Chris supplies the two missing private prerequisites described in
docs/BOOTSTRAP-BLOCKERS.md — (a) the supported PAL v1.003 GDI with all three
tracks, and (b) the 12-part `r354-baseline` development archive (or the
equivalent source-only Katana/runtime/title closure, or authenticated generator
access). Placing the archive in `ref/sarecomp/.local/archives/` lets
`python3 ref/sarecomp/tools/restore-baseline.py` restore it with no download.
G0 also needs one-time acceptance of the Xcode license by Chris
(`sudo xcodebuild -license accept`) before any Apple compile or link is possible.
