# Public release preparation

Updated **29 October 2026**. Intended release: **0.1.0 experimental preview**,
source and build recipe only. Repository visibility remains the maintainer's
decision. No public IPA, translated game code, disc content, saves or signing
material are release assets.

## Publication status

**Do not make the existing repository public yet.** The archived Katana route
contains reconstructed original routines and captured game-state values. It
is also present in Git history. The active app does not use this archive.
The source export excludes `archive/` through `.gitattributes`; that does not
remove it from commits, branches or pull-request references.

Preserve the existing repository privately and prepare a fresh repository from
the reviewed source export before changing visibility, or complete a separate
review and cleanup of every historical reference. Replacing a repository or
rewriting its published history requires the maintainer's explicit approval.
See [rights and distribution boundaries](RIGHTS-STATUS.md).

## Checks for this candidate

| Check | Result |
|---|---|
| README and player documentation | Updated to the current app and recipe-only scope |
| GitHub issues | No open issues on 29 October 2026 |
| PadMint recipe schema | Pass with PadMint main `b090c36` and Apple Python 3.9 |
| Host tests and built-in disc identity | Final run pending |
| Simulator UI, including Dreamcast Original persistence | Running |
| Current-tree source checks and negative controls | Final run pending |
| Fresh PadMint player build from clean checkout | Pending |
| Reviewed source export and recipe checksums | Pending |
| Whole-repository publication review | Blocked by archived research in history |
| Physical iPad or iPhone validation | Not requested for this preparation pass |

## Player-build gate

Use `scripts/player-check.sh` from **current PadMint main**, not a warm developer
build. It uses Apple's `/usr/bin/python3`, a fresh source clone and an empty
PadMint folder. With the player's supported PAL v1.003 GDI available locally:

```sh
# Run from a clean checkout of PadMint main, before tagging SonicPad.
scripts/player-check.sh sonicpad ios "/path/to/Sonic Adventure.gdi" "/path/to/sonicpad"
```

Keep its workspace private: it contains a personal IPA and generated game code.
Record both repository revisions, the exit status, package checks and measured
build-space use. A build failure blocks publication.

SonicPad is not currently in PadMint's catalog. Its `padmint.json` can be used
with a local checkout; adding a catalog entry requires separate approval.
After an approved catalog/release publication, repeat the player check without
the checkout argument to test the actual download route. Do not announce that
route before it passes.

## Release contents

The prepared attachments are `sonicpad-v0.1.0-padmint.json` and `SHA256SUMS`.
Source comes from the reviewed release tree. A private source export can be
prepared with `git archive --format=tar.gz --prefix=sonicpad-v0.1.0/ HEAD`;
inspect its file list and verify that it contains no `archive/`, private inputs,
generated code or binary app packages before distributing it.

The [release notes](releases/v0.1.0.md), [installation guide](INSTALL.md) and
[known limitations](KNOWN-ISSUES.md) define the preview's scope. Simulator and
player-build checks do not establish full-game or new physical-device
acceptance. Keep the historical device observations in [STATUS.md](STATUS.md)
separate from this preparation pass.
