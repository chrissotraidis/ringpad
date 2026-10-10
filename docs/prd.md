# SonicPad PRD: Sonic Adventure, native on Apple platforms

Status: **conditional implementation specification; source-access gate unresolved**.
Written: **5 October 2026**. Working name: **SonicPad** (`sonicpad`), not a reserved product name.
Audience: an autonomous coding agent operating on Chris's **Apple Silicon Mac**.
Companion: [`goal-loop.md`](goal-loop.md). Read both before acting.

**2026-10-05 research amendment:** Chris authorized bounded investigation of a
public-source alternative after Katana acquisition failed. Execute
[`RECOVERY-LOOP.md`](RECOVERY-LOOP.md) as an isolated feasibility workstream.
The production foundation and exact PAL input requirements below remain pending
an explicit, evidence-backed foundation decision; synthetic success does not
clear the original G1.

> **Decision: GO for a private source/bootstrap audit and bounded, source-independent shell work. HOLD on a complete SARecomp-based Apple build until G1 establishes access to the missing runtime and game-source inputs.**
>
> Sonic Adventure is a credible porting target, but the public SARecomp repository is **not a self-contained, portable source release**. This review does not establish an executable Apple port. Do not tell Chris that cloning the repository and adding touch controls is all that remains.

## 1. Objective and delivery order

Build SonicPad: a game-specific **native ARM64 iOS/iPadOS port of the Dreamcast Sonic Adventure**, with a macOS application as the bring-up and regression target. Preserve SARecomp's working game-specific logic and native runtime interfaces where available. Reuse Chris's Apple application-shell mechanisms rather than rebuilding their menus, layout editor, controller handoff and diagnostics from scratch.

Use **SARecomp/Katana-derived ahead-of-time game code**, compiled for each Apple target. Do not substitute Sonic Adventure DX, a Windows executable wrapper, streaming, Rosetta, a general Dreamcast emulator or the GameCube/N64 runtimes used by other Pad projects. A change of foundation is a separate decision, not an automatic fallback.

The owner supplies the exact supported game dump and any legitimately available development sources. The app does not download game content. Game-code generation/compilation happens on the Mac before installation, not on the iPhone or iPad.

Delivery order:

1. Establish reproducible sources, exact game identity and the unresolved permission/source boundaries.
2. Obtain or reproduce the complete portable Katana runtime and retained/generated title-source closure.
3. Compile the AOT/runtime core to ARM64; prove the Apple graphics/media contract with a small test harness.
4. Complete macOS first play, including save and cold reload.
5. Complete iPad Simulator gameplay and the Dreamcast shell, then iPhone Simulator parity, **one Simulator at a time**.
6. Complete the seven-story progression target, Chao persistence and the technical matrix.
7. Measure performance, exercise lifecycle/save recovery and reproduce the build from a fresh directory.
8. Obtain exact-artifact physical iPad/iPhone acceptance from Chris.
9. Resolve publication rights and obtain separate, explicit approval before any public source or binary release.

This is not a title-screen demonstration. A responsive menu, a linked library or an attractive control overlay is not evidence of a working Sonic port.

## 2. What “done” means

Each definition requires evidence under Section 11. None is satisfied by this planning review.

| ID | Acceptance definition |
|---|---|
| D1 | **Inputs and bootstrap:** exact PAL disc tracks verified; full source/dependency closure available; the chosen retained-source or regeneration route and every input hash are recorded. |
| D2 | **Native core:** the necessary generated title code, runtime, bindings and providers compile/link for ARM64 with the correct Apple platform ABI; module/continuation behavior is covered; no hidden host-code generation or missing executable path is papered over. |
| D3 | **Apple renderer/media:** an Apple backend passes source-derived graphics contract tests, including transparency, depth, HUD and movie presentation; required audio/media decoders build for the target; limits and remaining differences are explicit. |
| D4 | **macOS first play:** normal title/new-game flow, Sonic opening/boss, adventure-field navigation, Emerald Coast completion, natural progress save, clean exit and cold reload, with working input, video, music, voices and effects. Validate the exact transitions against the supported build. |
| D5 | **Mobile core and shell:** iPad Simulator, then iPhone Simulator, run the first-play loop using the same game-source identity; Dreamcast touch controls, three-dot menu, settings, import and diagnostics function. Simulator evidence is not physical-device evidence. |
| D6 | **Game coverage:** a fresh profile progresses through Sonic, Tails, Knuckles, Amy, Big, Gamma and the final Super Sonic story, including credits and return; required unlocks, story transitions and Chao flows remain correct. See the coverage policy in Section 10. |
| D7 | **Input and persistence:** complete Dreamcast controls, controller ownership, interruption clearing, Story/Chao paired backups, repeated saves, restore, profile isolation and in-place app updates pass the applicable tests. |
| D8 | **Timing and stability:** Original scene-dependent timing is measured and preserved; representative demanding scenes, audio continuity, memory behavior, repeated transitions and a 60-minute soak are documented. No unsupported “60 FPS everywhere” claim. |
| D9 | **Reproducibility:** technical matrix rows 1–30 pass for the declared targets; the private clean-machine procedure works from pinned sources plus explicitly declared private inputs, without undisclosed caches or manual edits. |
| D10 | **Physical candidate:** Chris tests the exact iPad/iPhone candidate, real multitouch, controller behavior, save/reload and sustained device behavior; hashes and hands-on results are recorded. |
| D11 | **Publication:** exact source/package audits, dependency/license review, author attribution and Chris's explicit source/binary release decisions are recorded. Technical success alone does not authorize publishing. |

Baseline non-goals: App Store/TestFlight submissions, commercial signing services, tvOS, Android, SA2, arbitrary Dreamcast games, DX content, other disc revisions, network multiplayer, online services, VMU handheld minigame execution, cheats, mods, 100% emblem completion or new rendering/gameplay enhancements. Existing optional upstream camera/timing/aspect features may be retained behind explicit settings; they are not the initial correctness baseline.

## 3. Feasibility audit: what is actually established

### 3.1 Audited source state

SARecomp revision: **`d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2`**, whose latest commit is dated 23 September 2026 UTC. The README identifies the player release as **`v0.50.0-beta.1`**, describes all seven stories as playable, and notes a development pause dated 24 September. Those are upstream statements, not gameplay independently reproduced here. [S01]

| Finding | Evidence | Consequence |
|---|---|---|
| A substantial PC game-specific port already exists. | README describes Windows, Linux, Steam Deck, seven stories, native rendering, configuration, camera and profile support. [S01] | Reuse this work rather than beginning an unrelated Dreamcast implementation. |
| Public clone alone is insufficient. | `docs/DEVELOPMENT.md` explicitly says the pinned SDK and generated AOT inputs are absent and a complete clean-machine bootstrap is not provided/tested. [S02] | **G1 is a hard dependency gate**, not ordinary CMake troubleshooting. |
| Windows build consumes prebuilt private inputs. | Root CMake requires Windows x64 clang-cl and `.local/baseline/r354/aot/katana_generated.lib`; `PinnedSdk.cmake` imports Windows SDK/runtime/media libraries. [S03, S04] | Those libraries cannot be treated as Apple ARM64 link inputs. |
| Linux has a source-oriented separation, but still relies on local material. | `SonicLinux.cmake` requires `.local/linux-sdk/src/runtime/native_port.cpp` and retained `.local/working-product/generated/code/*.cpp`. It enables AVX2/FMA for `fpu_simd.cpp`. [S05] | Linux's target decomposition is useful; it is not a ready ARM64/macOS implementation. |
| Rendering is not already Metal. | Current Vulkan device selection requires Vulkan 1.3 features and `geometryShader`; renderer documentation describes a large Type-2 transparency arena. [S06] | A feature/semantic audit is mandatory. MoltenVK is a candidate, not a drop-in promise. |
| Input is already separated from game actions. | `sonic_input.hpp` exposes snapshots, sampling, transformation, modal suppression and test input; bindings separate gameplay, camera and host settings actions. [S07] | Add an Apple/Dreamcast adapter at this boundary instead of synthesizing Windows input. |
| Story and Chao storage must stay coordinated. | README and upstream engineering notes require paired backup/restore of their separate data. [S01, S08] | A single-file “save copied successfully” test is insufficient. |
| The game-data identity is exact. | Installer documentation specifies PAL v1.003, MK-5100050, one GDI and all three tracks with hashes. [S09] | Do not start from a US disc or DX assets just because other Pad projects use US inputs. |

The old development recovery release was removed because it included original retail data. The upstream guide describes restoration from an existing local backup, **not a supported download bootstrap**. Do not search for reuploads or use a player installer as a substitute for portable source. [S02]

### 3.2 Verdict and limits

**Engineering inference:** with a complete, compatible runtime/title-source package, the exposed C++ title adapters and Linux source separation make a native Apple port plausible. Significant Apple work still remains: ARM64/FPU portability, graphics compatibility, native hosting, media, lifecycle and Dreamcast input.

**Not verified:** availability of all necessary Katana sources, regeneration reproducibility, Apple compilation, Apple renderer correctness, mobile performance, full-game behavior, or distribution permission. No game dump or Katana development package was supplied for this review; no Xcode build, game launch, gameplay or device test was performed.

Searches during the review did not establish a public, complete KatanaRecomp source repository matching this project's pinned SDK. That is a bounded retrieval result, not a claim that no private or subsequently released source exists. The execution Mac may contain inputs unavailable to this review; G1 must inspect what actually exists there.

**Do not confuse two different holds:** missing source is a technical bootstrap blocker; publication permission is a separate release gate. Clearing either does not automatically clear the other.

## 4. Environment, workspace and authority

Use a dedicated local `sonicpad/` integration workspace, not the donor repositories as writable project roots. Verify Apple Silicon/native execution, Xcode and SDK versions, C++20 support, CMake 3.25+, Ninja, Python 3, Git, `jq`, `rg`, `xcrun simctl`, Instruments and Metal tools. Record actual installed versions. Install only missing tooling needed by the chosen route; do not upgrade unrelated projects or the OS.

Choose and record deployment targets from the renderer/library requirements and build evidence. Donor minimum-OS badges are not Sonic compatibility guarantees. Check current UIKit scene startup requirements before reusing an older app delegate; SunPad itself documents a source-versus-published-build startup distinction. [S10]

Recommended layout:

```text
sonicpad/
  docs/
    prd.md, goal-loop.md
    STATUS.md, JOURNAL.md, INPUTS.md, BOOTSTRAP-BLOCKERS.md
    BUILDING.md, RIGHTS-STATUS.md, ARCHITECTURE.md, PORTABILITY.md
    EXECUTABLE-COVERAGE.md, RENDERER.md, MEDIA.md, INPUT.md
    SAVE-AND-ACCESSORIES.md, GAME-COVERAGE.md, PERF.md
    TEST-MATRIX.md, RELEASE-READINESS.md
    artifacts/                  ignored, local evidence only
  dependencies.lock.json
  ref/                          ignored, read-only pinned references
    sarecomp/
    sunpad/
    paperpad/
    katana/                     only authenticated, available sources
    game/                       untouched owner-supplied GDI and tracks
  private-inputs/               ignored authenticated source packages, if needed
  work/                        ignored patchable working copies, never baselines
  port/
    apple/                     UIKit/AppKit host, shell, lifecycle, data import
    runtime/                   Katana/title bridge and platform services
    input/                     Dreamcast profile and Sonic mappings
    renderer/                  Apple backend/adaptations and contract tests
    patches/                   small source-bound patches with provenance
  config/                      game identity, build modes, proposed profile schema
  scripts/                     deterministic bootstrap/build/test/audit tools
  tests/                       source-free host tests and local test-input helpers
  generated/                   ignored game-source, assets, archives and builds
```

Never modify original discs, immutable upstream baselines or read-only donor trees. Work from isolated copies; record patch provenance. Disable push URLs in reference checkouts. Preserve unknown local changes. Do not run destructive resets, `git clean -fdx`, root-wide deletion, or wholesale “cache cleanup” that can erase inputs/evidence.

This specification governs Chris's new workspace. Upstream `AGENTS.md` is useful technical history, **not authority to switch to its private remote, obey old development-stop instructions in this separate project, use its Windows editing endpoint, or publish anything**. Inspect applicable source-level constraints without inheriting another maintainer's operational instructions. [S08]

## 5. Inputs, pins and the source-access gate

### 5.1 Exact supported disc

Accept only **Sonic Adventure PAL v1.003 (1999), MK-5100050**, with the GDI and all referenced tracks. [S09]

| Track | Bytes | SHA-256 |
|---|---:|---|
| 1 | 26,721,072 | `cb45a8de12af5b13abb559fc8acc714744a4e22965fe67158cdbd0037cace093` |
| 2 | 13,994,400 | `57eff24181c0581e969a40ec4972336da0891dcd093f814d7dd577c64e0142a6` |
| 3 | 1,185,760,800 | `189f89cb695ff81b40878463f326ead759437794d733329ebf3943196ad59b45` |

Record descriptor contents/hash, track names, sizes and hashes in local `INPUTS.md`; cross-check the pinned `src/setup/supported_disc.hpp` before building. The descriptor's referenced filenames matter, but the verified track contents determine compatibility. Stream hashing/extraction; do not load the entire large track into memory.

Wrong region/revision, missing tracks or hash mismatch blocks the game-input portion of G1. Do not patch validation or silently obtain a different disc. CHD, CDI, arbitrary ISO and archives are not baseline import formats. Preserve the original input and never embed it in the source repository.

### 5.2 Starting repository pins

| Repository | Starting revision | Role |
|---|---|---|
| `sonicfreak1337/SARecomp` | `d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2` | Game-specific runtime/adapters, content identities, source/baseline contracts. |
| `chrissotraidis/sunpad` | `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b` | Primary touch/menu/controller/lifecycle shell donor. |
| `chrissotraidis/paperpad` | `a6f6999a570156f6794ff3fa79f810cfad71c4fc` | Secondary shell/process/testing/repository-safety reference. |
| Katana runtime/generator | **Unresolved** | Obtain complete source and exact identities; do not invent a repository URL or expand a short archive label into a guessed commit. |
| MoltenVK, if selected | Resolve and pin a tested distribution/source graph | Apple Vulkan-to-Metal candidate; inspected source reference in S12 is not an accepted build dependency. |
| SDL3, FFmpeg, shader tools, other libraries | Resolve exact source/version/build configuration | Rebuild appropriate Apple slices; upstream Windows DLLs/Linux `.so` files are not reusable Apple binaries. |

Create a new `dependencies.lock.json` containing full commit/archive SHA-256, source origin, patches, ABI identity, license status, compiler/SDK and target platform. Do not copy the SnapPad N64 dependency graph into SonicPad.

**Reference drift matters:** current PaperPad uses PaperBoat and separately retains PaperPad Original documentation. The uploaded SnapPad PRD refers to the earlier ReCut/RT64 PaperPad. Do not mix their build recipes or claim their runtime architectures are interchangeable. Use SunPad for the main UI extraction and PaperPad for identified game-neutral mechanisms. [S10, S11]

### 5.3 G1: acquire a complete source closure

Inventory and prove one of these routes:

**Route A — authenticated retained-source build.** A complete source-only development package supplies all retained title C++ partitions, dispatch/binding tables, generated headers, matching Katana runtime implementation, platform/graphics/media interfaces, title providers and configuration necessary to compile Apple targets. Record hashes and permission/provenance. This can support reproducible private builds without claiming the public repository bootstraps from a disc alone.

**Route B — reproducible generation.** Obtain the matching Katana generator/analyzer sources or usable, authenticated Mac host tooling plus the complete game configuration, patches and metadata. Reproduce the required generated source from the owner's exact disc and validate the output against the expected interfaces/coverage. A generic SH-4 translator that produces some C++ is not proof of SARecomp compatibility.

Required inventory, based on the current build graph [S02–S05, S13]:

| Required material | Why it is needed | G1 proof |
|---|---|---|
| Matching Katana AOT and native-port runtime source and headers | Implements the interfaces currently imported as private SDK libraries. | All translation units/includes identified; source files actually present; version/ABI recorded. |
| Retained title source or working generation route | Linux consumes `generated/code/unit-*.cpp` and `native-port-*-shard-*.cpp`; Windows imports an AOT archive. | Complete source list, generated includes, dispatch/registration/binding tables and manifest; not only `.lib`, `.exe` or installer payloads. |
| Generated SDK headers and build configuration | Header/schema mismatch can compile incorrectly or break continuation semantics. | Matching definitions, configuration and provenance tied to the runtime/title pack. |
| Title-specific providers and auxiliary executable paths | Root CMake contains a separate `MINICART.PRS` AOT generation path. | Reproducible included source/output route or a complete compatible retained auxiliary pack. |
| Native audio/movie/codec implementation | Linux media targets still read runtime source from the local SDK. | Full decoder/host integration source and pinned source dependencies. |
| Original exact disc and extraction pipeline | Runtime assets are installed from owner-supplied tracks. | All three hashes pass; extraction manifest and content checks are reproducible. |
| Source rights/provenance record | Visibility and third-party notices alone do not establish the whole combination's permissions. | Available licenses/permissions inventoried; unknowns explicit; no redistribution assumed. |

For every input mark `present-verified`, `present-unverified`, `missing` or `incompatible`. A hash-only manifest does not mean its files are available. A Windows SDK binary cannot be relabeled as an ARM64 library.

If materials are missing, produce `docs/BOOTSTRAP-BLOCKERS.md` with the exact path/interface, expected identity, why required, locations checked, acceptable replacement and the smallest request to Chris/upstream. Draft a request for a **source-only** package or generator/runtime access; do not send messages or open issues automatically. No build retry can create absent source.

### 5.4 Read-before-port map

Read the pinned SARecomp `README.md`, `docs/DEVELOPMENT.md`, `docs/INSTALLATION.md`, `AGENTS.md`, root CMake, `cmake/PinnedSdk.cmake`, `cmake/SonicLinux.cmake` and its included platform/audio/movie/graphics/title files. Then inspect:

- `src/sonic_input.hpp`, `src/sonic_input_bindings.hpp`, `src/sonic_input.cpp`, camera policy and configuration handling;
- `src/renderer/sonic_vulkan.cpp`, `src/renderer/pinned/`, the shader compilation tool and graphics contract tests;
- `src/native_title_adapter.cpp`, function/graphics/platform boundary metadata, provider and auxiliary AOT tooling;
- profile/save implementation, setup validation/extraction, media implementation and tests;
- current source defaults alongside dated reports. Historical reports do not override current code or prove Apple behavior.

For SunPad read `docs/CONTROLS.md`, architecture/build/dependency/testing/known-issue docs, and the implementations reached from `apple/ios/SunPadGameOverlay.mm`, `SunPadInputMixer`, `SunPadSettings` and `SunPadDiagnostics`. The overlay includes useful normalized placement, touch cancellation and pressure UI, but also Sunshine-specific FLUDD/detent semantics that must not transfer. [S10]

## 6. Phase 0: reproducibility and safe startup

G0 records machine/toolchain state, repository state, source pins, original-input locations, chosen save namespace and `RIGHTS-STATUS.md = private-only / publication not approved`. Set ignores and safety checks **before** generating game-derived files.

G1 checks Section 5 in full. No assumption in this document clears it. If the execution machine has the required private sources, verify and proceed; otherwise keep the dependency gate open and report it honestly.

Provide scripts with the following responsibilities. These are **required scripts to implement**, not commands claimed to exist upstream:

```text
scripts/check-prerequisites.sh
scripts/clone-sources.sh
scripts/verify-sources.sh
scripts/check-source-closure.sh
scripts/verify-game.py
scripts/prepare-game.sh
scripts/generate-or-verify-aot.sh
scripts/apply-patches.sh
scripts/build-macos-app.sh
scripts/build-ios-simulator.sh
scripts/build-ios-device.sh
scripts/run-session.sh
scripts/test-host.sh
scripts/test-renderer.sh
scripts/test-smoke.sh
scripts/capture-evidence.sh
scripts/check-repo-safety.sh
scripts/audit-apple-package.sh
scripts/package-private-ipa.sh
```

Each script must fail with an actionable cause, preserve unknown work, record effective inputs and respect target-specific caches. Cache keys include source/patch/ABI/SDK/target/mode identity. UI changes must not gratuitously regenerate all title code.

## 7. Phase 1: ARM64 core and executable coverage

### 7.1 Source-based Apple targets

Create dedicated Apple targets rather than deleting the Windows compiler guard and hoping its imported SDK works. The Linux decomposition is a starting map for shared runtime, title code, platform, graphics, audio and movie targets, not a command to import its x86-64 dependencies. [S03–S05]

Build and identify separately:

- macOS ARM64;
- iOS Simulator ARM64;
- iOS device ARM64.

Use `file`, `lipo -archs`, `otool -L` and Mach-O build-version inspection as appropriate. **Both device and Simulator can say arm64 while targeting different platforms.** Link success against the wrong slice is not acceptance. Do not use Rosetta for a native-core claim.

### 7.2 Portability inventory

In `PORTABILITY.md`, enumerate and replace or isolate:

- AVX/SSE/FMA intrinsics and x86 build flags, including the Linux `fpu_simd.cpp` flags;
- Windows timing/threading/file/window/media/input services and Linux/X11-specific adaptations;
- platform-dependent widths, `long`, `wchar_t`, alignment, function-pointer/address assumptions and serialization;
- floating-point control/rounding, fused operations, denormals, NaNs, signed zero and guest-visible side effects;
- memory mapping, page granularity, executable-memory assumptions and instruction-cache invalidation;
- generated helper/registration ABI and callback/continuation lifetime semantics.

These are audit targets, not all confirmed bugs. Start SIMD replacements with a correct scalar/reference path and source-derived tests, then optimize. Preserve guest arithmetic and actual FPU contracts; do not enable broad fast-math or change fusion/rounding just to gain speed. Keep functional upstream safety/optimization paths unless a specific incompatibility is established. [S05, S08]

### 7.3 AOT and module boundaries

Create `EXECUTABLE-COVERAGE.md`: main/resident title code, dynamically loaded modules, compressed code paths, code-write invalidation, function entries, indirect calls and auxiliary paths including Mini Cart where required by the title.

For each executable unit record its verified input identity, load/decompression path, code/data boundaries, generated registration, lifetime/invalidation rule, expected callbacks/continuations and coverage scene. Do not copy N64 overlays, RSP, MIPS, FlashRAM or RT64 terminology into this Dreamcast runtime.

Preserve source-visible module guards, live callback order, guest RAM effects and continuation rules. An indirect target failure needs the module identity, guest PC, originating call and last load/unload/invalidation events. Do not replay completed prologues/delay-slot work or turn a local label into a global function to suppress a crash. [S08]

**No host CPU JIT on Apple targets.** No downloaded native game modules, runtime C compilation, writable-executable guest-code caches or `MAP_JIT` workaround. Build game code on the Mac. Metal/MoltenVK GPU shader compilation is a separate graphics operation, not permission to generate host CPU game code.

Do not assume Katana has no interpreter, or that a helper named `dynamic_interpreter` means a production JIT exists. Audit the actual linked/runtime path. A documented bounded, non-JIT interpreter path may be evaluated for missing guest behavior, with counters and regression coverage; do not silently convert the game into an interpreter-only emulator or hide missing AOT paths. Any departure from the intended core requires an architecture entry, not a renamed acceptance claim.

## 8. Phase 2: Apple graphics, hosting and first play

### 8.1 Renderer decision gate

Preserve the existing native graphics command contract before choosing its Apple implementation. The current Vulkan source requires `geometryShader` during device selection and enablement. That is a confirmed capability requirement, **not proof that every scene executes a geometry shader**. Inspect actual pipelines, SPIR-V capabilities and draw paths. The inspected MoltenVK stage-mapping source does not implement the geometry-stage mapping; its general Metal-backed Vulkan support does not establish compatibility with this renderer. [S06, S12]

Conduct one bounded feasibility experiment at the graphics boundary:

1. Inventory required Vulkan features, formats, resource limits, shader stages and synchronization behavior; separate gameplay requirements from optional debug features.
2. Query the actual Apple target and selected MoltenVK version. Record supported versus required features, including fragment storage/atomics, depth behavior and buffer limits. Version numbers alone do not pass this check.
3. Attempt a small command-stream contract harness on macOS. Evaluate MoltenVK only after an explicit compatibility plan exists. A guard may be relaxed only when its underlying feature is proven unused or replaced with equivalent behavior, with a regression test.
4. Where needed, implement narrowly scoped shader/primitive adaptations. If that route is structurally unsuitable, implement a native Metal backend behind the same graphics interface. Record the choice and why; do not spend repeated sessions trying an unchanged unsupported backend.
5. Prove the selected contract on iPad Simulator as an early graphics probe, then return to macOS first play. This narrow probe is permitted before G6; it is not evidence that G6 is met. Stop the macOS game and run only one Simulator.

A native Metal backend means translating the existing renderer's semantics, not transplanting RT64 or Dolphin or rewriting Sonic's game logic. Do not combine two incomplete rendering paths into an undocumented fallback.

`RENDERER.md` must cover texture/palette and mip handling, mesh upload, primitive topology, winding/culling, flat/smooth shading, clipping, depth and manual depth comparisons, fog, blending, Type-2 transparency/secondary accumulation, HUD, movies, resize, capture and in-flight resource lifetime. Port or adapt the existing synthetic renderer-contract tests where available; use synthetic resources that contain no retail assets.

The historical renderer document describes an approximately **512 MiB** Type-2 arena. Recheck the current allocation and actual peak use; do not allocate that amount on mobile merely because the desktop used it. Define a measured capacity policy and explicit overflow behavior, test dense transparent scenes, and never silently drop fragments or substitute incorrect sorting to pass a screenshot. A mobile memory budget is a measured engineering decision, not a desktop constant. [S06]

Record image comparisons for controlled synthetic scenes and game scenes, including water, overlapping transparent effects, fog, coplanar geometry, HUD/fades and movie aspect. Establish tolerances before comparisons; explain deviations instead of moving tolerances to conceal defects. Track synchronization validation and renderer error counters.

### 8.2 Native host and media

Implement an Apple host using the appropriate AppKit/UIKit lifecycle and main-thread ownership. Retain one owner each for the window/surface, game update, input sampling, audio output and save writes. Bridge C++ and Objective-C++ through a small interface; do not simulate Windows messages or retain the Linux X11 event loop in the shipping Apple app.

Use the selected Xcode SDK's supported scene/lifecycle setup and verify it through actual launch. SunPad's current README records an iOS/SDK startup correction, so copying an older app delegate is not evidence of launch compatibility. [S10]

Rebuild required audio/movie/codec libraries from compatible source for each Apple platform. The Linux movie path still consumes private SDK source and platform-specific FFmpeg binaries; neither its `.so` files nor Windows DLLs are Apple dependencies. Inventory the title's actual codec/container needs before selecting a native or FFmpeg-backed decoder path. Preserve movie playback, music, voices, effects, timing and return-to-game behavior. Blank movies, muted channels or skipped cutscenes do not pass. [S04, S13]

Create `MEDIA.md` with decoded asset classes, sample formats/rates, channel behavior, streaming buffers, movie timing, audio queue depth, underruns, interruptions and route-change behavior. Background the game safely rather than continuing unbounded simulation or writes after suspension. Save durability must not depend on receiving a final application-termination callback.

### 8.3 Baseline timing and presentation

Start with **Original timing, Original camera, original 4:3 presentation**. Upstream distinguishes scene-dependent Original cadence from a Recompiled mode targeting 60 simulation updates; presentation frequency is a separate quantity. Do not assume PAL means a universal 50 Hz gameplay target or that a 60/120 Hz display establishes game speed. [S01, S08]

Measure simulation steps, newly rendered game images, presented frames and audio time separately. Compare equivalent scenes against the supported upstream build or a trustworthy owner-supplied reference, recording its identity. When a reference measurement is unavailable, mark the comparison unverified rather than inventing a cadence.

Preserve collision, input timing, scripts, cutscene timing, movement, audio pitch and save timing. Keep qualified upstream functional optimizations and safety guards unless a demonstrated Apple incompatibility requires a scoped change. Optional 60-update timing, wider aspect and modern camera settings must remain explicit and separately tested; do not introduce new interpolation or speed patches as a shortcut to the baseline.

### 8.4 macOS first-play gate

Run the normal game, not a mocked scene. The intended route is:

**Title → fresh profile/new game → Sonic opening and Chaos 0 encounter → adventure field → Emerald Coast → stage completion/return → natural save → clean exit → cold launch → load and verify progress.**

Confirm the exact ordering and save checkpoints against the supported PAL build. Adapt the test route to actual game behavior; do not patch progression to match this prose. Include movement, jumping/action input, camera triggers, pause/resume, dialogue/menu acceptance, music, voices, effects and transitions.

The save check must show meaningful game-visible progress after cold reload, not merely a file's existence or modification time. Keep an isolated test profile and back up Story and Chao data together before destructive save tests.

After this route passes, the build may be described as **first-play verified on macOS**, not complete on iPhone/iPad. Enable debug-only deterministic input replay at the native snapshot boundary for regression work; it must not bypass the behavior under test or ship as an unrestricted cheat/test interface.

## 9. Phase 3: the Dreamcast Apple shell

### 9.1 Donor boundary

Use pinned SunPad as the primary donor for the three-dot menu, normalized layout editing, touch cancellation, input latching, controller reconciliation, native sheets, diagnostics and game-data management patterns. Use PaperPad's identified game-neutral testing/safety mechanisms where useful. Copy only necessary, license-compatible components with provenance. [S10, S11]

Do not import Dolphin, ModernGekko, GameCube button masks, Sunshine patches, FLUDD pressure detents, Z, C-stick behavior, PaperBoat, N64Recomp, RT64 or game-specific save formats. A reusable shell is not a reusable console core. Keep new code under `port/apple/` and `port/input/`, with an adapter into the actual Katana input/platform API.

### 9.2 Canonical Dreamcast input profile

Create a small, game-independent `DreamcastInputState` and a Sonic-specific binding adapter. The profile has **one analog movement stick**, a D-pad, A/B/X/Y, independent analog L/R triggers and Start. The host menu is a separate action. Derive the actual guest masks, axis signs, centering and trigger scaling from the obtained SDK and title code; do not guess them from GameCube definitions. Upstream already distinguishes movement, face/trigger actions, optional look actions and host settings actions. [S07]

Recommended default layout and behavior:

| Control | Touch default | Physical-controller default | Required behavior |
|---|---|---|---|
| Movement stick | Large left-side stick, radial range/deadzone, safe-area aware | Left thumbstick | Neutral, cardinal, diagonal, slow movement and full movement; no stuck axis after cancellation. |
| D-pad | Compact four-direction cluster near the left thumb; always available | D-pad | Distinct from analog movement in the state contract; test menus and any title use. |
| A / B / X / Y | Dreamcast diamond: A bottom, B right, X left, Y top | Positional face buttons, with suitable platform glyphs | Preserve each original action, held state and edges; do not hardcode every character to Sonic's actions. |
| L / R | Reachable left/right shoulder regions; touch-and-slide pressure or accessible equivalent | Analog triggers | Independent continuous values from released to full; simple press can mean full. No GameCube digital-click detent. |
| Start | Small separate pause/start control | Controller menu/start | Game pause/start, distinct from the native three-dot menu. |
| Three-dot menu | Persistent, reachable native button even when gameplay overlay is hidden | Explicit optional host binding | Opens shell UI, clears gameplay input, and never silently substitutes for Start. |

The canonical host profile should use normalized movement axes and trigger values, then convert once at the verified guest adapter. The reusable Dreamcast profile must preserve analog trigger range even where Sonic treats a particular action as a threshold. Touch is not assumed to report pressure; slide position supplies the analog value.

**No second analog stick in the default Dreamcast layout.** Upstream's optional Recompiled camera may use a separately named camera-drag region or physical right stick, enabled only when that camera mode is selected. That extension is host camera input, not a newly invented Dreamcast controller axis. Original mode must not receive phantom look input. Preserve scripted camera ownership. [S01, S07]

### 9.3 Input correctness and ownership

Use a thread-safe state snapshot and a defined edge-latch/consumption rule so a short touch between game samples is observed once, not missed or repeated as turbo. Maintain per-touch identity; lifting one finger must not release another finger's button. Test movement plus jump/action, trigger plus movement, simultaneous buttons, rapid taps, slide-out/cancel and two-trigger interaction.

Set explicit input ownership: the selected physical P1 controller or touch/keyboard as appropriate. Reconcile connection/disconnection and foreground state; retain stable ownership instead of mixing stale values or letting a second controller steal P1. Do not let SDL and GameController independently feed the same physical event twice. Reuse donor mechanisms, not undocumented donor arbitration rules.

Opening any shell sheet, entering layout edit, changing input owner, losing focus, backgrounding, interrupting or disconnecting must clear all held gameplay buttons, analog values, pending edges and optional look deltas. Returning to play requires fresh presses; never replay inputs queued while a modal was open. Layout editing must not move the character.

Persist separate phone/tablet normalized layouts, control scale and opacity. Clamp restored controls into the current safe area, support reset, prevent invisible/unreachable controls and preserve a way to reopen the menu. Test both landscape orientations and supported size changes. Keep the A/B/X/Y controls distinguishable without relying on color alone.

Acceptance has three distinct levels: state/adapter unit tests, actual Simulator view-event routing tests, and physical hands-on multitouch/controller testing. A debug snapshot injection proves the first level only. XCUITest must target the single intended Simulator and must not create parallel cloned Simulators. Use `xcrun simctl io <UDID> screenshot` for screenshots, not as a fictional generic touch-injection command.

### 9.4 Three-dot menu and settings

Keep a compact native menu consistent with Chris's Pad apps:

- **Display and audio:** Original 4:3; only implemented/qualified resolution or render-scale options; effective dimensions; supported volume controls. Keep timing/camera/aspect experiments clearly separate.
- **Controls:** show/hide, auto-hide for physical P1, move/edit, scale/opacity, reset, mapping and an input test panel.
- **Game Data & Saves:** import/verify/reimport/remove game data; profile status; paired backup/import/restore with confirmations.
- **Support:** review/share diagnostic log, report a problem, About, source attribution and license notices.

Do not copy SunPad/PaperPad's `1×–4×` choices into a renderer that cannot implement them. Upstream notes a 100% render-scale cap; inspect the current renderer contract and expose only truthful choices. A disabled or absent unsupported setting is preferable to a cosmetic control that does nothing. [S08]

Bridge shell controls and the existing in-game Options through **one authoritative configuration model**. Changes must round-trip and persist without two menus overriding one another. Honor restart requirements and display rollback. Distinguish resume/restart game from an iOS process-exit button; do not use `exit(0)` as the normal mobile lifecycle.

### 9.5 Multi-file GDI import

The imported unit is the **GDI and its three referenced tracks**, not a single ROM file. Provide a directory-picker or explicit multi-file selection workflow with access to every required file. Selecting a GDI alone must not be treated as permission to read inaccessible sibling files. Use scoped file access correctly and release it after copying/validation.

Stage into an ignored/private temporary location; parse the descriptor defensively; reject missing tracks, unsupported layouts, absolute/escaping paths and unexpected symlink escapes. Stream copies and SHA-256 verification without loading the large data track into RAM. Check required free space before copying/extraction, show progress and handle cancellation, cloud-provider errors, app interruption and disk-full failures.

Commit a verified content installation atomically or through a recoverable transaction. A failed reimport leaves the previous valid installation intact. Record an installed-content identity; verify it against the title's content manifest where available. Keep saves outside the data-installation transaction. Removing game data requires confirmation and must not remove Story/Chao progress. Never mutate the original dump or bypass a checksum failure. [S09]

### 9.6 Saves, VMU behavior and lifecycle

Preserve the runtime's actual Story/Chao/VMU representation rather than inventing a new binary format. Inventory required in-game VMU behaviors, missing-device responses and capacity/error handling from the source. VMU handheld minigame execution, external accessories and network functions remain absent unless deliberately added; their absence must not hang normal play.

Pair Story and Chao backup/restore as one consistent profile transaction, with version/identity metadata, validation, rollback and confirmation before replacement. Never restore a backup into a live writer. Test save creation, repeat writes, profile switching, save-point interruption, process termination, cold load, backup recovery and an in-place app update with the same identity. Do not claim uninstalling preserves data.

Test foreground/background, audio interruptions/routes, sleep/resume, controller reconnect, low-memory handling, surface recreation and pending native sheets. A writable test fixture must be a private copy, never the only personal save.

### 9.7 Diagnostics

Log boot stage, build/source identity, validated content identity, renderer/capabilities and mode, guest/module faults, input owner/clears, lifecycle transitions, save-transaction status, audio underruns, memory pressure and clean shutdown. Keep detailed local traces separate from the small user-facing diagnostic log.

Export only a user-reviewable, sanitized report: no disc bytes, extracted assets, game-source snippets, save contents, crash memory, private absolute paths, credentials or signing material. Screenshots and save attachments are never automatic. A report action may prepare a draft but must not submit it without the user's action.

## 10. Phase 4: acceptance matrix and game coverage

### 10.1 Coverage policy

The following are required tests, **not assertions that these behaviors have been run**. Build the exact level/boss/minigame/transition manifest from the supported source/content and refine the routes accordingly.

Complete one fresh-profile, normal-unlock seven-story golden path on macOS. Private late-game fixtures may accelerate regressions but cannot replace the fresh progression proof. For each mobile Simulator, complete first play and run representative gameplay for every playable character, final-story transitions, Chao/save operations and the input/lifecycle matrix. A mobile-specific core/renderer/ABI defect requires expanding the affected story coverage, up to full progression where needed. Report full-game completion on the target where it was actually observed; macOS completion alone is not a claim of a full physical-iPhone playthrough.

Use character-specific mechanics as checks: Sonic movement/actions; Tails flight; Knuckles movement/climb/glide and search; Amy hammer/actions; Big casting/fishing/reeling; Gamma targeting/actions; final-story behavior. Derive exact button actions from the supported game, rather than relabeling generic buttons from memory. Include camera usability while moving, not only movement in an empty field.

### 10.2 Matrix

`M` = native Apple Silicon macOS; `IS` = iPad Simulator; `PS` = iPhone Simulator. Mobile tests are sequential. Rows marked physical require Chris or an explicitly identified hands-on tester on the exact device build.

| # | Test | Target | Pass condition |
|---|---|---|---|
| 1 | Workspace safety/authorization | Repo | Private state, immutable inputs, disabled reference pushes, ignores and executable safety checks exist. |
| 2 | Full source closure | Fresh workspace | Runtime/title/headers/providers/media source pack or generation route verified; no missing private input hidden by cache. |
| 3 | Exact disc/extraction | Host + import tests | Three track sizes/hashes pass; supported layout accepted; reproducible extracted-content identity. |
| 4 | ARM64 and ABI | M, IS, PS builds | Correct Mach-O platform slices and native linking; FPU/arithmetic/ABI regression tests pass without x86 imports. |
| 5 | Executable coverage | Core tests + M | Module/dispatch/continuation/invalidation manifest complete; auxiliary executable paths accounted for; no unexplained lookup failures. |
| 6 | Graphics contract | M, IS, PS | Synthetic primitives, textures, depth, transparency, fog, HUD/movie and resource-lifetime tests pass; capabilities/limits explicit. |
| 7 | Media and audio | M, IS, PS | Required codecs, music, voice, effects and movies work; interruption and transition timing checked. |
| 8 | Mac first-play loop | M | Full Section 8.4 route, real input, natural save, clean exit and cold reload pass. |
| 9 | Tablet first-play loop | IS | Same game-source identity completes first play with touch-path checks and save/reload. |
| 10 | Phone first-play loop | PS | Same first-play route, compact usable controls and safe-area/UI behavior pass. |
| 11 | Sonic story | M; mobile samples | Fresh story progression, relevant bosses/stages/credits and return; mobile mechanic/transition sample. |
| 12 | Tails story | M; mobile samples | Fresh progression and flight/actions; camera/cutscene framing checked, including known wider-aspect risk. |
| 13 | Knuckles story | M; mobile samples | Fresh progression, search/actions, demanding scenes and returns. |
| 14 | Amy story | M; mobile samples | Fresh progression, hammer/effects, interaction and return. |
| 15 | Big story | M; mobile samples | Fresh progression and successful fishing with tested controls; no script/tension/action bypass. |
| 16 | Gamma story | M; mobile samples | Fresh progression, targeting/actions, effects/timing and return. |
| 17 | Final Super Sonic story | M; mobile late fixture | Normal unlock, final sequence, credits and post-game return; no debug unlock as sole proof. |
| 18 | Fields and auxiliary paths | M; affected mobile routes | Required inter-story/field transitions, supported trials/minigames and Mini Cart executable route covered. |
| 19 | Chao operations | M, IS, PS | Garden interaction and separate data persist across save/reload; profile/backups remain coordinated. |
| 20 | Complete input-state contract | Host tests | All inputs, analog ranges, edge latches, chord/short-tap behavior, mapping, owner changes and clearing pass. |
| 21 | Actual touch/UI path | IS, PS | View events reach the adapter correctly; move/jump/action/trigger combinations, cancel, edit/reset and modal suppression pass. Unsupported automated gestures remain explicitly open for physical testing. |
| 22 | Three-dot/settings parity | IS, PS | Every visible entry works; persisted settings round-trip with game Options; unsupported options absent; safe restart/rollback. |
| 23 | Import/reimport failure safety | IS, PS | Wrong/missing tracks, cancellation, invalid paths, cloud read failure and disk-full retain prior valid data and saves. |
| 24 | Story/Chao persistence | M, IS, PS | Fresh/repeated saves, paired backup/restore, profile isolation, interrupted transaction recovery and in-place update pass. |
| 25 | Ownership and lifecycle | M, IS, PS | Testable disconnect/reconnect, focus/modal/background/audio/surface transitions clear input and recover safely. Real hardware checks remain row 31. |
| 26 | Timing/performance | M + diagnostic mobile runs | Original cadence reference recorded; simulation/render/presentation/audio distinct; representative profiles and memory budget documented. |
| 27 | Diagnostics/privacy | M, IS, PS | Useful breadcrumbs; sanitized reviewable export; automated leakage checks pass. |
| 28 | Soak/repeated transitions | M + IS | At least 60 minutes each with repeated stage/field/menu/Chao/save transitions; no crash, unbounded memory growth, stuck input or sustained audio failure. |
| 29 | Regression suite | Host + highest-good targets | Focused source-derived tests and end-to-end smoke green against declared build identity. |
| 30 | Clean-machine reproduction | New directory | Scripts rebuild M/IS/PS from pins and declared private inputs; no undocumented cache/manual edit; dependency/platform audits pass. |
| 31 | Physical exact candidate | Physical iPad + iPhone | Chris verifies first play, later-game fixture, real multitouch, mapping/reconnect, audio/lifecycle, save/reload and sustained usability; exact build/device evidence recorded. |
| 32 | Publication authorization | Source + package | Section 12 completed; source and binary decisions separate; exact-artifact approval recorded. |

Rows 1–30 are the private technical completion bar, subject to their explicit target scope. Rows 31–32 cannot be inferred from them. A test that a Simulator cannot faithfully perform is identified as such and assigned to row 31, not silently marked passed. Builds changed after acceptance reopen the affected rows.

## 11. Evidence, journal and reporting

Maintain these documents as the project advances; this kit supplies only the PRD and loop, not fabricated reports:

| Document | Purpose |
|---|---|
| `STATUS.md` | Lowest unmet goal; row results; known-good commands/artifacts; next step; open blockers. |
| `JOURNAL.md` | Dated append-only goal, hypothesis, command/change, observation, evidence, interpretation and next action. |
| `INPUTS.md`, `BUILDING.md` | Verified disc/source-pack identities, chosen bootstrap route, exact setup/build commands and declared private prerequisites. |
| `BOOTSTRAP-BLOCKERS.md` | Required material missing/incompatible, identities, checks performed, acceptable source replacement and request draft. |
| `TEST-MATRIX.md` | PRD row status, actual target/test scope, exact artifact and evidence links. |
| `ARCHITECTURE.md`, `PORTABILITY.md` | Chosen core/backend/host boundary, source dependency closure, ARM64/ABI/FPU decisions and patches. |
| `EXECUTABLE-COVERAGE.md` | Module/code paths, dispatch/continuations, lifetime guards, unresolved faults and coverage. |
| `RENDERER.md`, `MEDIA.md` | Feature contracts, synthetic comparisons, capacities, audio/video behavior and open differences. |
| `INPUT.md`, `SAVE-AND-ACCESSORIES.md` | Dreamcast mapping/ownership, test levels, Story/Chao/VMU behavior and transaction recovery. |
| `GAME-COVERAGE.md`, `PERF.md` | Fresh progression/fixture routes; cadence/profiles, device limits, soak and memory history. |
| `RIGHTS-STATUS.md`, `RELEASE-READINESS.md` | Provenance, permission unknowns, exact candidate/audits, physical acceptance and explicit publication decisions. |
| `artifacts/` | Ignored local screenshots, captures, logs, test reports, profiles and hashes, organized by session/build. |

Every row records `not-run`, `blocked`, `failed` or `passed`, with the observation type separately identified: source inspection, unit test, compiled target, Simulator interaction or physical hands-on. Include date, target/UDID or device, OS/SDK, root revision and dirty patch hash, dependency lock, source-pack/content identity, build configuration, artifact SHA-256, settings, fixture/profile and exact command. Bind evidence to the actual artifact, not just the filename.

Save the first causal error and full context before cleanup. Logs should identify source-pack/ABI/backend identity so stale caches or mismatched code are visible. Debug probes must not alter game time, hide errors or auto-complete gameplay to create a passing capture.

A screenshot proves a frame, compilation proves a build, and a Simulator proves only what was observed there. **If it was not run and observed, it is not done.** Do not attach an old evidence set to a new binary without a justified impact analysis and required reruns. State any unperformed manual test plainly.

## 12. Public release, provenance and package boundary

This is an engineering authorization policy, not legal advice or a determination that private use itself resolves copyright questions.

No public source or binary release is authorized by this PRD. The source audit did not establish a general SARecomp/Katana license covering this derivative combination. Verify root, directory and file-level notices and available permissions at the actual pins. Third-party dependency licenses and public visibility are not a substitute for that check. SunPad identifies its integration as GPL-3.0-or-later, so copying its code requires a compatibility/provenance decision, not a new blanket permissive license. [S10, S14]

Prefer a separate private integration workspace with immutable references while permission questions remain open. Before publication, identify the authorized source topology, copied code, patches, required notices and obligations for every combined component. Retain SoNiCFReaK/SARecomp/KatanaRecomp and shell-donor attribution; preserve appropriate AI-assistance disclosure without claiming authorship of upstream work.

Audit source and packages for original discs/assets, generated game C++, private source packs, personal saves, raw crash memory, credentials, provisioning/signing material, local paths and reference/cache directories. Do not copy the removed development archive into a release. An audit can validate a chosen boundary; it cannot grant redistribution rights.

**A ROM-free app can still contain game-derived AOT machine code.** Public integration-source release, a game-code-free base app, and a playable binary/IPA have different content boundaries and need separate decisions. A future owner-local Mac generation workflow may be evaluated once generation is actually reproducible; this document does not claim existing PadMint support for SonicPad or authorize distributing generated game code.

A public candidate requires D1–D10, matrix rows 1–31, exact source/binary hashes, package/source audits, completed dependency notices and permission decisions, no unresolved critical crash/progression/save/privacy defect, and Chris's explicit final approval of the intended publication. Do not push branches/tags, publish builds, create releases or post game captures by momentum.

Approved descriptive substance, only when justified by the implemented architecture and release decision:

> SonicPad is an unofficial Apple-platform adaptation of Sonic Adventure Recompiled using ahead-of-time compiled Dreamcast game code and a native Apple application shell. Users must supply the supported original game data. Credit belongs to SEGA for the original game, SoNiCFReaK and the SARecomp/KatanaRecomp contributors for the upstream port, and the credited Apple-shell contributors. The project is not affiliated with or endorsed by SEGA.

Do not say “emulator-free,” “all devices supported,” “60 FPS everywhere” or “legal because no ROM is included.”

## 13. Risk register

| Risk | Standing at review | Required response |
|---|---|---|
| Missing Katana/title source closure | Confirmed public-checkout gap; local availability unknown | G1 inventory; authenticated source-only package or reproducible generation route; no fake bootstrap. |
| ARM64 runtime/FPU portability | x86 AVX2/FMA flags confirmed; semantic compatibility unproven | Scalar correctness baseline, source-derived arithmetic tests, separate platform slices and narrow optimization. |
| Vulkan feature mismatch | Geometry capability demanded in source; Apple compatibility unproven | Feature/pipeline audit, bounded MoltenVK test or native Metal command-contract backend. |
| Transparency memory and ordering | Large documented desktop arena; mobile budget unknown | Measure capacity and overflow; preserve correctness; test dense transparent scenes. |
| Hidden executable/auxiliary paths | Separate Mini Cart path visible; full closure unverified | Source/registration/module manifest and coverage; no silent executable stubs. |
| Multimedia dependency closure | Private SDK and platform-specific media libs visible | Obtain source, rebuild target codecs, verify movies and every audio class. |
| GameCube shell assumptions | Donor has two sticks, Z and FLUDD detents | One-stick Dreamcast profile; new adapter/mapping and analog-trigger tests. |
| Multi-file import and storage | GDI references three tracks, including a large data track | Scoped directory/multiselect access, streaming validation, transactional install and failure tests. |
| Story/Chao inconsistency | Paired-data requirement confirmed upstream | Quiesced paired backup/restore with recovery and game-visible reload. |
| Misleading performance claims | Upstream warns about slowdowns; Apple unmeasured | Separate simulation/presentation metrics; baseline timing and real-device profiles. |
| Reference drift and duplicate settings | Current PaperPad architecture differs; both native and game menus exist | Pin donors, extract mechanisms, one configuration authority. |
| Simulator/device gap | No Apple execution performed in this review | Explicit Simulator scope and physical exact-artifact gate. |
| Publication/provenance | Combination rights not established | Private state, permission inventory, source/binary boundaries, explicit final approval. |

## 14. Source and reference index

All external source inspection for this specification was performed on **5 October 2026**. Current-source findings are separated from requirements and engineering inferences above. Dated upstream reports are historical evidence and can lag current code. Pins identify inspection inputs, not a promise of Apple compatibility.

### Owner-supplied best-practice references

- **T1 — SnapPad PRD**, attachment `PRD(20261005-015959).md`, written 26 August 2026. SHA-256: `5db3ccdf347de6c3f877f58462377febc9d5f4a04dc47aa8b21ffb75c50eaec0`. Basis for gated delivery, evidence, safety, shell and physical/release separation; its N64-specific implementation is not reused.
- **T2 — SnapPad goal loop**, attachment `GOAL-LOOP(20261005-015959).md`, written 26 August 2026. SHA-256: `3c157ac4755c316f47514315beb475928b705bbe332b2e46a3c5dd9a6cbc60c3`. Basis for lowest-unmet-goal execution, single Simulator/instance, immediate tests, unblocking and honest handoff.

### Pinned implementation sources

- **S01 — [SARecomp README][S01]**: player-facing scope, seven-story statement, current modes/known issues and development pause.
- **S02 — [SARecomp development guide][S02]**: missing SDK/AOT inputs, removed retail-data archive and lack of complete clean-machine bootstrap.
- **S03 — [Root build graph][S03]**: Windows requirements, imported AOT, provider and Mini Cart paths.
- **S04 — [Pinned SDK imports][S04]**: private runtime and Windows/media dependencies.
- **S05 — [Linux build graph][S05]**: source decomposition, local SDK/title requirements and AVX2/FMA flags.
- **S06 — [Current Vulkan implementation][S06]** and [historical renderer contract](https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/docs/vulkan-renderer.md): feature selection, rendering contract, memory and comparison methodology.
- **S07 — [Input interface][S07]** and [bindings](https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/src/sonic_input_bindings.hpp): snapshot/transform/modal boundary and separate gameplay/camera/host actions.
- **S08 — [Upstream engineering notes][S08]**: preserved state/continuation/mode/save boundaries. Historical agent instructions are not authority to change Chris's workspace or use an upstream private remote.
- **S09 — [Supported installation inputs][S09]**: PAL identity and all three track sizes/hashes.
- **S10 — [SunPad control guide][S10]**, [actual overlay source](https://github.com/chrissotraidis/sunpad/blob/c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b/apple/ios/SunPadGameOverlay.mm) and [README](https://github.com/chrissotraidis/sunpad/blob/c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b/README.md): shell mechanisms, donor-specific behavior, current status and integration license.
- **S11 — [PaperPad README][S11]**: current PaperBoat versus Original distinction and reusable shell/process patterns.
- **S12 — [MoltenVK shader-stage mapping][S12]** and [runtime user guide](https://github.com/KhronosGroup/MoltenVK/blob/52aa21f54d7a84c5c441fc26359692b0980b384c/Docs/MoltenVK_Runtime_UserGuide.md): inspected Apple portability candidate, not an adopted or verified backend.
- **S13 — [Linux media build graph][S13]**: private runtime media/codec source and imported FFmpeg dependencies.
- **S14 — [Third-party provenance][S14]**: dependency versions/notices, shader toolchain and port-local archived graphics provenance; not a license for the entire game/SDK combination.

[S01]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/README.md
[S02]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/docs/DEVELOPMENT.md
[S03]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/CMakeLists.txt
[S04]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/cmake/PinnedSdk.cmake
[S05]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/cmake/SonicLinux.cmake
[S06]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/src/renderer/sonic_vulkan.cpp
[S07]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/src/sonic_input.hpp
[S08]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/AGENTS.md
[S09]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/docs/INSTALLATION.md
[S10]: https://github.com/chrissotraidis/sunpad/blob/c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b/docs/CONTROLS.md
[S11]: https://github.com/chrissotraidis/paperpad/blob/a6f6999a570156f6794ff3fa79f810cfad71c4fc/README.md
[S12]: https://github.com/KhronosGroup/MoltenVK/blob/52aa21f54d7a84c5c441fc26359692b0980b384c/MoltenVK/MoltenVK/Vulkan/mvk_datatypes.mm
[S13]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/cmake/LinuxMovie.cmake
[S14]: https://github.com/sonicfreak1337/SARecomp/blob/d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2/third_party/README.md
