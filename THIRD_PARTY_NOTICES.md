# Third-party notices

SonicPad is distributed under the GNU General Public License, version 3 (see
[LICENSE](LICENSE)), because it links Flycast (GPL-2.0-or-later) and adapts
SunPad (GPL-3.0-or-later).

| Component | Version | Licence | Use |
|---|---|---|---|
| [Flycast](https://github.com/flyinghead/flycast) | `ed953fcbfdd1b8a7521d2966b13cc700c7e2d233` | GPL-2.0-or-later | Dreamcast hardware layer, high-level BIOS and SH-4 interpreter. Fetched at build time, unmodified; SonicPad's hooks are applied to configured copies of individual source files (see `CMakeLists.txt`). Flycast's submodules carry their own licences. |
| [MoltenVK](https://github.com/KhronosGroup/MoltenVK) | v1.4.2 | Apache-2.0 | Vulkan on Metal, linked statically into the iOS app. Fetched at build time and checked against the SHA-256 in `dependencies.lock.json`. |
| [SunPad](https://github.com/chrissotraidis/sunpad) | `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b` | GPL-3.0-or-later | Touch-input and controller mechanisms adapted in `port/input` and `port/dcpad` (see [docs/INPUT.md](docs/INPUT.md)). |
| SARecomp | n/a | n/a | The guest pad-bit values in `port/input` were checked against its `sonic_input_bindings.hpp`. No SARecomp code is included. |

Sonic Adventure is © SEGA. SonicPad contains no game data. Each player builds
the app from their own disc; the recompiled code in that build is translated
from the player's disc and must not be shared.
