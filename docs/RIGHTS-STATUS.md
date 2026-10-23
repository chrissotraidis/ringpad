# SonicPad rights status

Updated **23 October 2026**. Private repository; no public release approved.

| Material | Notice | Boundary |
|---|---|---|
| Flycast `ed953fc` | GPL-2.0-or-later (file headers and LICENSE) | Linked into the app. Any distributed binary or source must follow the GPL. |
| SunPad `c77dd61` | GPL-3.0-or-later | Input mechanisms adapted per INPUT.md; compatible with Flycast under GPL-3.0. |
| MoltenVK v1.4.2 | Apache-2.0 | Linked statically into iOS builds; compatible with GPL-3.0 distribution. |
| SonicPad sources (`port/`, `scripts/`, CMake) | Ours | Would be released under GPL-3.0 if published. |
| Disc, traces, block manifest, generated C++ | Derived from Sega's game | Private and ignored; never published. |
| `config/recomp-blocks.txt` | Ours: addresses, disc file names, offsets and SHA-256 digests | Committed. Holds no instruction bytes; players rebuild the manifest from their own disc. Chris decides whether it ships publicly with the source. |
| Personal IPA from `scripts/build-personal.py` | Contains code translated from the player's disc | Built on the player's Mac for their own devices; never published. |

Publication needs Chris's separate approval against exact artifacts.
