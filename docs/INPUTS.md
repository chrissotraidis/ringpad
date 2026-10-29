# SonicPad build inputs

Updated **29 October 2026**. Game inputs and generated instruction data stay
private. The supported-disc identity is recorded as hashes; it is not game
content or a download source.

| Input | Identity | How it is obtained |
|---|---|---|
| Sonic Adventure PAL v1.003 (1999), MK-5100050 | GDI and three tracks matching [`config/game-identity.json`](../config/game-identity.json) | Supplied by the player; checked before compilation and on device import |
| Dreamcast BIOS | Not used | Flycast's high-level BIOS replacement boots the supported disc |
| Flycast | `ed953fcbfdd1b8a7521d2966b13cc700c7e2d233` | Builder fetches the pinned unmodified source and submodules into `ref/flycast` |
| MoltenVK | v1.4.2; archive SHA-256 in [`dependencies.lock.json`](../dependencies.lock.json) | Builder downloads and checks the pinned archive into `ref/moltenvk-all` |
| SunPad input/menu donor | `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b` | Adapted source is already in `port/`; a SunPad checkout is not needed |
| Address and module lists | [`config/recomp-blocks.txt`](../config/recomp-blocks.txt), [`config/code-modules.txt`](../config/code-modules.txt) | Included source metadata: addresses, disc paths, offsets and hashes, without instruction bytes |
| Instruction manifests and generated C++ | `build/personal/` for player builds; `generated/` for development | Reconstructed from the player's disc; ignored and excluded from public artifacts |

SARecomp is not an input. The canonical pad bits were checked against its
historical input definitions; the current [input bridge](INPUT.md) maps that
internal state to Flycast.

For required tools, setup and generation commands, see [Building](BUILDING.md).
For source-publication boundaries and history review, see
[Rights status](RIGHTS-STATUS.md) and [Release checklist](RELEASE-CHECKLIST.md).
