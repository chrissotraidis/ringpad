# SonicPad inputs

Updated **22 October 2026**. Private: only hashes are committed.

| Input | Identity | Status |
|---|---|---|
| Sonic Adventure PAL v1.003 (1999), MK-5100050 | `ref/Sonic Adventure v1.003 (1999)(Sega)(PAL)(M5)[!]/`; descriptor SHA-256 `d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e`; tracks per `config/game-identity.json` | **present-verified** 22 Oct |
| Dreamcast BIOS | Not used; Flycast's reios high-level BIOS boots the disc | not required |
| Flycast | `ref/flycast` at `ed953fcbfdd1b8a7521d2966b13cc700c7e2d233`, unmodified, push disabled | **present-verified** 22 Oct |
| MoltenVK | `ref/moltenvk-all`, v1.4.2, hash in `dependencies.lock.json` | present |
| SunPad (shell donor) | `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b`; the shell code it donated is already in `port/` | not checked out; not needed to build |
| Block manifest and generated C++ | `generated/recomp-manifest.txt`, `generated/recomp/` | local, regenerated from traces |

SARecomp is not an input. The Dreamcast pad bits in `port/input` were copied
from its `src/sonic_input_bindings.hpp` (see [INPUT.md](INPUT.md)); no
SARecomp checkout is needed to build.
