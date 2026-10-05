# SonicPad bootstrap blockers

Updated: **5 October 2026, Astra audit**. Private-only. This replaces the earlier
recommendation to acquire the complete retired recovery archive.

## G0: macOS toolchain reopened

The independently installed Apple Command Line Tools work when selected for
the command:

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools /usr/bin/clang++ --version
scripts/check-prerequisites.sh
scripts/run-host-tests.sh
```

Apple clang 21.0.0 compiled, linked and ran an ARM64 Objective-C++ Foundation
probe. Mach-O metadata: **MACOS**, minimum 13.0, SDK 27.0, Apple linker 27037.1.
All 142 Dreamcast checks also pass with this Apple toolchain. Global developer
selection and license settings were not changed.

Full Xcode 27.0 remains license-blocked: it requires EA2002; the machine records
EA1990 accepted for Xcode 26.6. Simulator/iOS tooling remains unverified.
Chris can clear this with `sudo xcodebuild -license accept`. This blocks the
full Xcode/mobile lane, **not all macOS compilation**. The prerequisite script
exits 2 while full Xcode is unavailable, even when its macOS probe passes.

## G1: exact source inventory

Public pin: `d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2`.
Retained-source identities below come from its public baseline manifests.
Their hashes establish source identity; Apple ABI compatibility is unverified.

| Required material | Actual state | Exact identity or missing interface |
|---|---|---|
| Katana runtime/compiler source | **missing** | `katana-source-178448be.zip`, 6,185,732 bytes, SHA-256 `85e5bd27952552f0957559cca82bde5b4024cd71e50e94650cd922f42430cd66`, or a compatible complete source checkout with declared provenance. |
| Runtime translation units | **missing** | Linux build reads `src/runtime/{block_abi,block_table,block_dispatch,block_guards,cache_control,code_invalidation,crash_report,exception,fpu,fpu_simd,guest_program_range,memory,native_bringup_dispatch,native_port,runtime}.cpp` plus all transitive sources/includes. |
| Runtime/SDK headers | **missing** | r354 `sdk/include/`: 177 files (175 .hpp + 2 .in), 1,516,512 bytes. Verify actual files against the public per-file hashes. |
| Generated SDK contracts | **missing** | `sdk/generated/include/`: 3 .hpp + identity materials JSON, 65,232 bytes; `katana/{abi_contract,build_contract,component_identity}.hpp` and matching build/ABI provenance. |
| Generated guest source/metadata | **missing** | r354 `product/generated/`: 1,476 files, 4,356,016,177 bytes; 1,460 .cpp, 2 .hpp, 10 .json plus manifest/build metadata. Includes `generated/code/unit-*.cpp`, `native-port-*-shard-*.cpp`, dispatch, `generated/include/`, and `.katana-generated-artifacts`. Obtain source/metadata or reproduce it with the matching generator. |
| Title/provider snapshot | **present-verified, partial** | 38/39 r354 files privately staged under `.local/source-only/r354/title-source/`, each size/hash verified. Exact r354 `CMakeLists.txt` (15,384 bytes) remains missing. Current public CMake exists at a different identity. |
| Mini Cart auxiliary source/generator | **missing dependency closure** | Public `generate_minicart_aot.cpp` requires `katana/io/{raw_binary_loader,input_provenance}.hpp`, `analysis/control_flow_analysis.hpp`, `ir/{lower,optimize}.hpp`, `codegen/{cpp_emitter,native_aot_profile}.hpp`, texture interfaces and their implementations. Supply complete generator/build configuration or compatible retained auxiliary source. |
| Katana audio/movie/codec integration | **missing** | `src/runtime/native_port_{audio,audio_execution_domain,audio_command_queue,telemetry,movie,ffmpeg_codec,codec,sound_bank}.cpp` and transitive source/headers. See LinuxAudio/LinuxMovie and audio preparation helpers. |
| FFmpeg implementation source | **present-verified archive, extracted source** | Public revision `5a03dfa0f607ee6156a59bdad3987cd3b858ee5d`, archive SHA-256 `5de5245447bafc6734be2e6151becfc389550863d6bcef31b1146a21a69ac49a`, privately acquired under `.local/media-source/` via `scripts/fetch-media-source.py`. Apple libraries and decoder tests remain not-run. |
| Exact supported disc | **missing** | Owner-supplied PAL v1.003 GDI and three tracks below. |
| Native-content assets | **absent; derive from owner's disc** | The 2,072-file `native-content/` set contains game assets. It is not source material to request from upstream. Extraction/install identity remains unverified. |

Five of the six title files that differed from r354 were recovered from public
Git history at `40cb8ca05dd849390cbcde31f4b00da0a4137511`:
`native_port_manifest.cpp`, `native_title_adapter.cpp`,
`sonic_private_scenario_launcher.cpp/.hpp`, and `refresh_native_provider.cpp`.
Together with 33 matching current files, this yields 38/39.
`scripts/recover-title-source.py` reproduces the snapshot, records provenance,
refuses to overwrite unknown work, and exits 2 for the CMake gap.
It does not prove compatibility with the missing guest/runtime source.

**Media correction:** r354 `sdk/ffmpeg/` has 144 headers, five Windows .lib
files and five DLLs. These 154 files are not FFmpeg implementation source.
The actual public source pin above comes from `tools/build-lean-ffmpeg.py`.
The Katana codec interfaces still need source.

## Supported game input

Target: Sonic Adventure **PAL v1.003 (1999), MK-5100050**. Keep the GDI and all
track filenames exactly as referenced by its descriptor; preserve the original.

| Track | LBA | Type / sector bytes | Bytes | SHA-256 |
|---|---:|---|---:|---|
| 1 | 0 | 4 / 2352 | 26,721,072 | `cb45a8de12af5b13abb559fc8acc714744a4e22965fe67158cdbd0037cace093` |
| 2 | 11511 | 0 / 2352 | 13,994,400 | `57eff24181c0581e969a40ec4972336da0891dcd093f814d7dd577c64e0142a6` |
| 3 | 45000 | 4 / 2352 | 1,185,760,800 | `189f89cb695ff81b40878463f326ead759437794d733329ebf3943196ad59b45` |

Canonical descriptor SHA-256:
`d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e`.
Installer acceptance depends on its parsed layout and track checks.

Present input: `ref/Sonic Adventure.cdi`, 757,339,066 bytes, SHA-256
`105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533`.
Earlier IP.BIN inspection identified MK-51000 V1.004/JUE. Converting its
container cannot reproduce the supported PAL track bytes. Do not bypass validation.

## Retired archive: historical identity, not the requested input

The manifest records `r354-development.zip.part001…012`, tag
`r354-baseline`, total 3,115,490,620 bytes, archive SHA-256
`07f28aa2052827c34f94fe20ad1c53b129405ec019f85486675186d9e7508ee9`.
Parts 001–011 are 268,435,456 bytes each; part012 is 162,700,604 bytes.
The baseline pack hash `3c831c4588e31ef428e785352c0660ac4d766e0dec485406029903a0a00c9fd2`
is a separate identity; neither identifies a newly assembled source-only package.

The recovery archive mixes source, Windows binaries and installed game assets.
Upstream DEVELOPMENT.md explains its removal and prohibits re-upload.
`restore-baseline.py` restores an existing owner-held backup; it does not
provide a source download or establish an Apple bootstrap.
Do not request or hunt the complete removed archive. A maintainer can instead
identify the source/metadata subset above from their local material.

## Completed checks and scope

Astra's scan completed to exit 0 across `~/GitHub`, `~/Documents`,
`~/Downloads`, `~/Desktop`, `~/.local`, and `/Volumes`:
**1,235,012 filenames**, no traversal errors. It excluded `.git`,
`node_modules` and `__pycache__`. No matching GDI, runtime header, Katana
source archive, archive part or named Katana library was found.
Evidence: `docs/artifacts/2026-10-05-astra/local-source-scan.json`.

Earlier root-wide searches returned unfinished sessions and suppressed errors;
their claim of an exhaustive machine-wide search is withdrawn. This completed
scan is bounded to the listed roots and filename criteria.

Available public Git history supplies the title files above but no runtime/SDK
archive. Earlier authenticated GitHub checks found header consumers without the
supplying repository. Those searches do not rule out private access. The player
installer is not a source closure.

## Source-only acquisition request (Chris sends manually)

> I'm building SonicPad privately against SARecomp
> d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2. Could you provide authenticated
> access to matching Katana runtime/compiler source, generated SDK contracts,
> retained guest C++/metadata and auxiliary Mini Cart source or a reproducible
> generator route?
>
> The known runtime archive is katana-source-178448be.zip, 6,185,732 bytes,
> SHA-256 85e5bd27952552f0957559cca82bde5b4024cd71e50e94650cd922f42430cd66.
> I recovered 38/39 r354 title-source files by hash from public Git history;
> the exact r354 CMakeLists.txt remains missing. Please include compatible
> build configuration, provider identities, audio/movie/codec integration
> and the generator configuration/patches. A complete package matching the
> current public revision is also acceptable with provenance and compatibility
> information.
>
> I acquired the pinned public FFmpeg source. I will supply my own verified
> PAL v1.003 disc and derive game assets locally. Please exclude installed
> assets, discs, saves, Windows binaries and the complete removed recovery
> archive. This is a request for private source access; no publication is planned.

Owner action: place the supported GDI and tracks in
`private-inputs/pal-v1003/`, and authenticated source material in
`private-inputs/` or provide its accessible repository/path.
No request has been sent.

G0: macOS CLT lane verified; full Xcode/mobile tools still blocked.
G1: **unresolved** pending actual runtime, guest source, matching headers/providers,
media integration, auxiliary code and exact disc. G2/gameplay remain not-run.
