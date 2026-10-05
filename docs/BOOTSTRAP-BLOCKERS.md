# SonicPad bootstrap blockers (G1)

Updated: **5 October 2026**. This is a **source-only acquisition request**. It
records exactly what G1 needs, what was checked, and the smallest request that
would reopen the gate. Nothing here authorizes publication or fetching game
content.

## Verdict

G1 (complete source and game inputs) is **blocked** by two independent
prerequisites. Neither can be produced by retrying a build. The public
SARecomp clone with the pinned commit is present and correct, and it is
insufficient by itself.

## Blocker A — supported game input is absent / incompatible

- **Interface:** the setup engine reads a GDI descriptor plus three referenced
  tracks and validates them in ref/sarecomp/src/setup/install_engine.cpp against
  the identities in ref/sarecomp/src/setup/supported_disc.hpp.
- **Expected identity:** Sonic Adventure **PAL v1.003 (1999), MK-5100050**, one
  `.gdi` with three tracks:
  - track 1 — 26,721,072 bytes — `cb45a8de12af5b13abb559fc8acc714744a4e22965fe67158cdbd0037cace093`
  - track 2 — 13,994,400 bytes — `57eff24181c0581e969a40ec4972336da0891dcd093f814d7dd577c64e0142a6`
  - track 3 — 1,185,760,800 bytes — `189f89cb695ff81b40878463f326ead759437794d733329ebf3943196ad59b45`
  The header also declares a descriptor hash,
  `required_gdi_sha256=d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e`.
  This is the hash of the `.gdi` descriptor for the canonical PAL v1.003 image,
  so a matching disc should reproduce it; the exact track bytes are the primary
  contract the installer enforces.
- **What is present instead:** `ref/Sonic Adventure.cdi` (722.3 MiB, SHA-256
  `105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533`), a
  DiscJuggler CDI container. It is **byte-identical** to
  `~/Documents/Systems/Sega Dreamcast/Sonic Adventure.cdi`, so the library copy
  is not a second candidate. Its embedded IP.BIN reports product **MK-51000**,
  version **V1.004**, region **JUE**, date **19990812**: a later, multi-region
  master, not PAL v1.003.
- **Why the present input cannot work:** setup requires an exact GDI layout and
  the three track hashes, and the only accepted data track is the PAL
  1.185 GB track. The CDI is 722 MiB total, so the PAL data track cannot even be
  contained. Extracting the CDI would also yield V1.004 content, not v1.003.
  Do not patch validation or substitute a US/DX/other disc.
- **Locations checked:** `ref/`, `~/Documents/Systems/Sega Dreamcast/`, the
  `Dreamcast 7z Dump/` folder, `~/Downloads`, `~/Documents/Codex/`, and a
  filesystem scan for `*.gdi` (none found anywhere).
- **Acceptable replacement:** the owner's own PAL v1.003 (1999) GDI with all
  three track files named exactly as the descriptor references, supplied by
  Chris as a private input (for example under `private-inputs/`). Any region or
  revision other than PAL v1.003 remains incompatible.

## Blocker B — private Katana/title source closure is absent

- **Interface:** the build consumes source and libraries that are not in the
  public repository. Concretely, 103 files under `ref/sarecomp/src/` include
  `katana/runtime/*` headers (`native_port.hpp`, `memory.hpp`, `fpu.hpp`,
  `gdi.hpp`, and so on) that exist nowhere on this machine:
  - Windows root CMake requires `.local/baseline/r354/aot/katana_generated.lib`
    and `.local/toolchain/katana_core.lib` / `katana_runtime_core.lib`
    (`CMakeLists.txt`, `cmake/PinnedSdk.cmake`).
  - Linux `cmake/SonicLinux.cmake` requires `.local/linux-sdk/src/runtime/*.cpp`
    (runtime, fpu, fpu_simd, memory, dispatch, invalidation, ...) and the
    retained guest source `.local/working-product/generated/code/unit-*.cpp` and
    `native-port-*-shard-*.cpp`.
  - Shared headers live under `.local/baseline/r354/sdk/include` and
    `sdk/generated/include`.
- **Expected identity:** the retained-source bundle recorded by
  `ref/sarecomp/baseline/r354.json` — schema `sonic-accepted-baseline-v1`,
  build **r354**, product version 0.49.9, upstream commit
  `178448bee4239048661c5e2362850b1c135b6ba1`, 3,951 files / ~10.85 GB, pack
  SHA-256 `3c831c4588e31ef428e785352c0660ac4d766e0dec485406029903a0a00c9fd2`,
  including a `katana-source-178448be.zip` (SHA-256 to be verified after
  acquisition) and `title-source/src/*.cpp/.hpp`. The r354 manifest enumerates
  the exact per-file hashes needed to prove closure.
- **What is present instead:** the public clone plus only the JSON identity
  manifests under `ref/sarecomp/baseline/`. Those contain paths, sizes and
  hashes but **not the files**. No `.local/` tree, no `katana_generated.lib`, no
  Katana source zip, no generated `unit-v*.cpp` guest source and no
  `katana/runtime/*` headers exist on the machine; a filesystem scan for those
  names returned nothing. The clone is not empty, though: `src/` holds the
  Sonic-specific adapter/provider code, including `native_title_adapter.cpp`
  (43,841 lines), and 33 of the 39 manifest `title-source` files are present and
  byte-identical to r354. That code is unusable and unlinked because its
  `katana/runtime/*` dependency closure is gone, and 6 of the 39 title files
  (`native_title_adapter.cpp`, `native_port_manifest.cpp`,
  `sonic_private_scenario_launcher.cpp/.hpp`, `tools/refresh_native_provider.cpp`,
  `CMakeLists.txt`) differ from the accepted r354 revision.
- **Why the manifest is insufficient:** a hash manifest is not source closure,
  and a Windows static library (`.lib`) cannot be re-labeled as an Apple ARM64
  input. Upstream documents that a clean-machine source bootstrap is not
  provided.
- **Locations checked:** `ref/sarecomp/baseline/`, the whole home directory for
  `katana_generated.lib` / `native_port.cpp` / `unit-*.cpp` / `katana-source-*.zip`
  and for `.local/baseline/r354`, `.local/linux-sdk`, `.local/working-product`
  and any `title-source/` directory, all mounted volumes (`/Volumes`), and the
  public GitHub namespace (see Blocker C).
- **Acceptable replacement:** the owner's authenticated r354 development bundle
  or a complete source-only development package covering: the Katana AOT and
  native-port runtime **source and headers**; the retained/generated title
  source (dispatch/registration/binding tables, generated includes, manifest);
  the generated SDK headers and build configuration; the title providers and
  the Mini Cart auxiliary AOT path; and the audio/movie/codec source. Route B
  (reproducible generation) is acceptable only with the matching
  Katana generator/analyzer sources or Mac host tooling plus configuration and
  patches, validated against the expected interfaces.

## Blocker C — no public Katana source route

A bounded retrieval check found no public `KatanaRecomp`/Katana source
repository matching the pinned SDK (`sonicfreak1337/KatanaRecomp`, `.../Katana`,
`.../katana-recomp`, `SoNiCFReaK/KatanaRecomp`, `.../KatanaSDK` all resolve as
absent/private). This is a bounded result, not proof that no private or later
release exists. If Chris has authenticated access to such a repository, that
alone may close Blocker B.

## Environment blocker — Apple toolchain license

The Xcode/Command Line Tools license is not accepted on this Mac. Every Apple
compile or link fails with `You have not agreed to the Xcode license
agreements.` This blocks any ARM64/ABI claim (G0 toolchain verification and G2)
and can only be cleared by Chris:

```sh
sudo xcodebuild -license accept   # interactive; requires the admin account
```

Host logic tests do not need this and already run (see docs/JOURNAL.md), but no
Apple target build can proceed until the license is accepted.

## Smallest request to Chris

1. Supply the supported **PAL v1.003 (1999) GDI** plus its three track files,
   privately (target path `private-inputs/pal-v1003/`).
2. Supply the **source-only r354 bundle** (or another complete, compatible
   Katana/runtime/title source package, or authenticated generator access),
   privately. At minimum the closure must include: the `katana/runtime/*`
   headers (r354 `sdk/include`, 177 files, plus `sdk/generated/include`); the
   generated guest units (`product/generated/code`, 1,459 files) or the
   generator/analyzer source to regenerate them; the native-content set
   (`native-content`, 2,072 files); the retained product source; the FFmpeg
   build source (`sdk/ffmpeg`, 154 files); and the 6 title files that differ
   from the public clone. Prebuilt Windows `.lib` archives alone are **not**
   acceptable Apple inputs.
3. Accept the Xcode license once: `sudo xcodebuild -license accept`.

## Draft request (send manually; SonicPad does not message anyone)

> Hi — SonicPad (private Apple port of Sonic Adventure, using SARecomp/Katana)
is blocked on two source inputs and I'd like to request them, source-only:
>
> 1. Sonic Adventure **PAL v1.003 (1999)** as a GDI with all three tracks. I
>    only have a multi-region V1.004 CDI, which the setup expects to reject.
> 2. A **source-only development package** for the Katana runtime/generator and
>    the retained title source at the accepted r354 identity (pack SHA-256
>    `3c831c4588e31ef428e785352c0660ac4d766e0dec485406029903a0a00c9fd2`), or a
>    documented reproducible generation route with the matching generator
>    sources. I do not need prebuilt Windows binaries; I need the sources and
>    generated headers to build Apple targets.
>
> No game data, saves or prebuilt binaries need to be sent — only the source
> package and a pointer to where I can place the PAL GDI I own.

## What is NOT blocked

Game-independent shell and input work may continue (goal-loop Section 1
permits bounded source-free work). The Dreamcast input contract, adapter and
edge latch are implemented and tested at the state level. Renderer-contract
and Synthetic probes remain possible once the toolchain license is accepted.
G1 stays blocked regardless.
