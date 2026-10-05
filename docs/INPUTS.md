# SonicPad verified inputs and private prerequisites

Updated: **5 October 2026**. Private-only. Each input is marked
**present-verified**, **present-unverified**, **missing** or **incompatible**.

## Supported game input (target)

| Track | Bytes | LBA | Type | SHA-256 | Status |
|---|---:|---:|---:|---|---|
| 1 | 26,721,072 | 0 | 4 | cb45a8de12af5b13abb559fc8acc714744a4e22965fe67158cdbd0037cace093 | **missing** |
| 2 | 13,994,400 | 11511 | 0 | 57eff24181c0581e969a40ec4972336da0891dcd093f814d7dd577c64e0142a6 | **missing** |
| 3 | 1,185,760,800 | 45000 | 4 | 189f89cb695ff81b40878463f326ead759437794d733329ebf3943196ad59b45 | **missing** |

Target: Sonic Adventure **PAL v1.003 (1999), MK-5100050**, supplied as a `.gdi`
with all three tracks (see config/game-identity.json). The supported-disc
header also declares `required_gdi_sha256`
`d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e` for the
descriptor; a matching PAL v1.003 `.gdi` should reproduce it.

## Provided dump (present, incompatible)

| Input | Value |
|---|---|
| Path | ref/Sonic Adventure.cdi |
| Bytes | 757,339,066 (722.3 MiB) |
| SHA-256 | 105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533 |
| Container | DiscJuggler CDI |
| Embedded IP.BIN | product MK-51000, version V1.004, region JUE, date 19990812 |
| Compared to | byte-identical to `~/Documents/Systems/Sega Dreamcast/Sonic Adventure.cdi` |
| Verdict | **incompatible** — multi-region V1.004 master, not the supported PAL v1.003 GDI; total size is below the required data track |

## Source closure (target)

| Required material | Expected identity | Status |
|---|---|---|
| SARecomp game-specific source | commit d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2 | **present-verified** (ref/sarecomp, push disabled); 33/39 title-source files match r354, 6 differ |
| Baseline/identity manifests | ref/sarecomp/baseline/*.json | **present-verified** |
| Katana AOT/native-port runtime source + headers | r354 bundle, `.local/baseline/r354/sdk`, `.local/linux-sdk` | **missing** |
| Retained/generated title source | `.local/working-product/generated/code/unit-*.cpp`, `native-port-*-shard-*.cpp`, `title-source/src/*` | **missing** |
| Generated AOT archive | `.local/baseline/r354/aot/katana_generated.lib` (Windows; not an Apple input) | **missing** |
| Private SDK libs | `.local/toolchain/katana_core.lib`, `katana_runtime_core.lib` (Windows) | **missing** / not Apple inputs |
| Katana source archive | r354 bundle `katana-source-178448be.zip` | **missing** |
| Media/codec sources | FFmpeg + runtime media source for Apple rebuild (r354 sdk/ffmpeg, 154 files / 111.31 MB) | **missing** |
| Katana `katana/runtime/*` headers | r354 `sdk/include` (177 files) + `sdk/generated/include` | **missing** — 103 `ref/sarecomp/src/*` files include them |
| Guest generated units | r354 `product/generated/code`, 1,459 files / 4,353.14 MB | **missing** |
| Retained guest product source | r354 `product` (3,263.19 MB of `katana_generated.lib` + 2,026.40 MB of other product source) | **missing** |
| Ready-made native content | r354 `native-content`, 2,072 files / 1,052.27 MB | **missing** |

Owner-authenticated private development package recorded by
`ref/sarecomp/baseline/r354.json`: build **r354**, version 0.49.9, upstream
commit `178448bee4239048661c5e2362850b1c135b6ba1`, 3,951 files / 10,853,522,212
bytes, pack SHA-256
`3c831c4588e31ef428e785352c0660ac4d766e0dec485406029903a0a00c9fd2`.

## Toolchain (G0)

| Tool | Version | Status |
|---|---|---|
| macOS | 27.0.1 (26A434), arm64 | present-verified |
| Xcode | 27.0 (27A266a); macOS 27.0, iPhoneOS 27.0, iPhoneSimulator 27.0 SDKs installed | present; **license-blocked** |
| Apple clang | 21.0.0 (CLT) | present; **license-blocked** |
| CLT SDKs | MacOSX 12.0, 12.1, 26, 26.5, 27.0, 27 | present-verified |
| Homebrew LLVM | 22.1.8 (+ ld64.lld) | present-verified (host logic only) |
| CMake | 4.4.0 | present-verified |
| Ninja | 1.13.2 | present-verified |
| Python | 3.14.6 | present-verified |
| git / jq / rg | 2.36.1 / 1.7.1 / 15.2.0 | present-verified |
| iOS Simulator runtimes | present in CoreSimulator | not re-verified this session — `xcrun simctl` is license-gated |

## Reference pins

| Repository | Pin | Local status |
|---|---|---|
| sonicfreak1337/SARecomp | d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2 | cloned `ref/sarecomp`, present-verified, push disabled |
| chrissotraidis/sunpad | c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b | resolved in `~/GitHub/sunpad` clone, present-verified |
| chrissotraidis/paperpad | a6f6999a570156f6794ff3fa79f810cfad71c4fc | resolved in `~/GitHub/paperpad` clone, present-verified |
