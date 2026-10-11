# SonicPad inputs

Updated **10 October 2026**. Private: only hashes are committed.

| Input | Identity | Status |
|---|---|---|
| Sonic Adventure PAL v1.003 (1999), MK-5100050 | `ref/Sonic Adventure v1.003 (1999)(Sega)(PAL)(M5)[!]/`; descriptor SHA-256 `d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e`; tracks per `config/game-identity.json` | **present-verified** 10 Oct |
| Dreamcast BIOS | Not used; Flycast's reios high-level BIOS boots the disc | not required |
| Flycast | `ref/flycast` at `ed953fcbfdd1b8a7521d2966b13cc700c7e2d233`, push disabled | **present-verified** |
| SunPad (shell donor) | `ref/sunpad` at `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b` | present |
| Block manifest and generated C++ | `generated/recomp-manifest.txt`, `generated/recomp/` | local, regenerated from traces |
| `ref/Sonic Adventure.cdi` | V1.004 JUE CDI | unused; not a supported input |

The SARecomp and PaperPad checkouts under `ref/` are optional references only.
