# SonicPad rights and distribution boundaries

Updated **29 October 2026** for experimental source and recipe release preparation.
The repository remains private until the maintainer changes its visibility.

## Current software and dependencies

SonicPad-owned source is distributed under the GNU General Public License,
version 3 ([LICENSE](../LICENSE)). These software permissions do not grant
rights to distribute Sonic Adventure or code translated from it.

| Material | License or origin | Distribution boundary |
|---|---|---|
| SonicPad app, input, build and recompilation tools | GPL-3.0 | Source and build recipe are the intended release. |
| Flycast `ed953fc` | GPL-2.0-or-later, per its file headers | Fetched at build time. SonicPad applies hooks to configured source copies; retain notices and meet the applicable source obligations when distributing GPL software. |
| SunPad `c77dd61` input mechanisms | GPL-3.0-or-later | Adapted as described in [INPUT.md](INPUT.md); attribution is retained in [third-party notices](../THIRD_PARTY_NOTICES.md). |
| MoltenVK v1.4.2 | Apache-2.0 | Fetched at build time and linked into personal iOS builds; retains its own license and notices. |
| Original SonicPad app icon | Project artwork | Generated icon with a ring, sky and sea; no character, logo or extracted game artwork. |

## Player-supplied game content

| Material | Contents | Boundary |
|---|---|---|
| Disc, extracted files, traces and runtime captures | Game bytes and private evidence | Ignored locally; never attach to a release, issue or pull request. |
| `config/recomp-blocks.txt` | Addresses, lengths, cycle counts, disc paths, offsets and truncated SHA-256 digests | Recipe metadata, without instruction bytes. Reconstructed locally from the player's disc. |
| `config/code-modules.txt` | Disc paths, load addresses and truncated SHA-256 digests | Discovery metadata, without module bytes. |
| Rebuilt manifests and generated C++ | Translated game instructions | Created locally in ignored build directories; excluded from distribution. |
| Personal app and IPA | Includes translated game code | Built for the player's own devices; not a public release asset. |
| VMU cards and save states | Player saves and potentially game data | Keep private and back up locally; do not attach to issues. |

Sonic Adventure and Dreamcast belong to SEGA. SonicPad is unofficial and is
not affiliated with or endorsed by SEGA. Players must supply their own legally
obtained supported disc; the project does not supply game downloads.

## Historical research archive

`archive/katana-route` is excluded from the current build. It retains earlier
research, including MIT-licensed DreamcastRecompiled patches with their
[license notice](../licenses/DreamcastRecompiled-MIT.txt),
test harnesses and behavioral models of original guest routines. Some tests
include captured constants and register values. These models and the reachable
Git history require a separate provenance review before the entire repository
is made public; absence of disc binaries alone does not settle that review.

`scripts/check-repo-safety.sh` checks ignored paths, prohibited tracked artifacts
and the structure of the current recipe metadata. It does not establish
copyright clearance or audit historical source content. Do not interpret its
passing result as approval to publish the historical archive.
