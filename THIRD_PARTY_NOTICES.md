# Third-party notices

SonicPad is distributed under the GNU General Public License, version 3 (see
[LICENSE](LICENSE)), because it links Flycast (GPL-2.0-or-later) and adapts
SunPad (GPL-3.0-or-later).

| Component | Version | Licence | Use |
|---|---|---|---|
| [Flycast](https://github.com/flyinghead/flycast) | `ed953fcbfdd1b8a7521d2966b13cc700c7e2d233` | GPL-2.0-or-later | Dreamcast hardware layer, high-level BIOS and SH-4 interpreter. Fetched at build time into an unmodified checkout; SonicPad applies its runtime hooks to configured source copies (see `CMakeLists.txt`). The generic SH-4 inline forms in `scripts/recomp/generate_blocks.py` are adapted from Flycast's interpreter. Flycast's submodules carry their own licences. |
| [MoltenVK](https://github.com/KhronosGroup/MoltenVK) | v1.4.2 | Apache-2.0 | Vulkan on Metal, linked statically into the iOS app. Fetched at build time and checked against the SHA-256 in `dependencies.lock.json`. |
| [SunPad](https://github.com/chrissotraidis/sunpad) | `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b` | GPL-3.0-or-later | Touch-input and controller mechanisms adapted in `port/input` and `port/dcpad` (see [docs/INPUT.md](docs/INPUT.md)). |
| SARecomp | Reference only | No code adopted | The guest pad-bit values in `port/input` were checked against its `sonic_input_bindings.hpp`. The current app does not depend on SARecomp. |
| DreamcastRecompiled (archived research only) | `ba7df34a50b5a7c676a590bb8b50bd784328c9b9` | MIT | Historical patches in `archive/katana-route/patches`, with the [upstream copyright and license](licenses/DreamcastRecompiled-MIT.txt). These are not used by the current app or player build. |

Sonic Adventure and Dreamcast are trademarks of SEGA. SonicPad is an
unofficial project and is not affiliated with or endorsed by SEGA. The GPL
covers the software, not SEGA's game content. The release distribution is
source and a build recipe; each player supplies their own supported disc.
Personal builds contain translated game code and are not release assets.
See [rights and distribution boundaries](docs/RIGHTS-STATUS.md).
