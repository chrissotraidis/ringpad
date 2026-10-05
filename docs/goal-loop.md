# SonicPad goal-based loop

Operating loop for the private, gated Apple port of **Sonic Adventure** using SARecomp/Katana-derived code. Written **5 October 2026**. Requirements and source findings live in [`prd.md`](prd.md); read both before acting.

**Current reviewed state: source feasibility only; no Apple build or gameplay verified. The public SARecomp clone lacks required SDK/AOT inputs. G1 is unresolved until this machine proves otherwise.**

This is an execution contract, not a record of completed work. Work on Chris's Apple Silicon Mac. Default authorized terminal goal: **G10, private technical completion**, followed by an exact-artifact handoff for physical testing. G11 needs actual device interaction; G12 needs separate permission and publication approval. Do not fabricate either.

## 1. The goal stack

Work the **lowest unmet goal**. A goal is met only when its evidence exists under PRD Section 11. A regression reopens the lowest affected goal. Higher-goal preparation may proceed only when it is genuinely independent of a blocker; it never changes the blocked goal's status.

| Goal | Required outcome and evidence |
|---|---|
| **G0. Environment and protected state** | Native Apple Silicon toolchain/SDKs verified; root revision and unknown work preserved; SARecomp/SunPad/PaperPad references pinned; private authorization state, ignored inputs, dependency lock and repository-safety checks established. |
| **G1. Complete source and game inputs** | Exact PAL v1.003 GDI/three-track identities pass. A complete matching Katana runtime/title-source/headers/providers/media closure is present, or a reproducible generation route proves it. Every private input is declared. A README, hash manifest or Windows `.lib` is not source closure. |
| **G2. Native ARM64 core** | Correct-platform runtime/AOT build compiles and links for macOS; arithmetic/ABI checks pass; module/continuation/auxiliary-code inventory exists; linked execution strategy has no hidden host JIT. Unexecuted gameplay paths remain unverified, not silently covered. |
| **G3. Apple graphics and media contract** | Selected backend passes bounded macOS graphics/media tests with documented features/limits. A single-iPad-Simulator graphics probe checks the key Apple portability assumptions. No geometry-feature fiction, incorrect transparency shortcut or imported x86 media binary. |
| **G4. macOS title with input/audio** | Normal Apple app initializes the core, renderer, content and audio; title renders and accepts real input. Logs identify mode, source/ABI/backend, input owner and any fault. |
| **G5. macOS first-play loop** | Normal new game/Sonic opening, adventure field, Emerald Coast completion, natural save, clean exit and cold reload succeed with input, rendering and required audio. Confirm the supported build's actual route. **First hard playability gate.** |
| **G6. iPad Simulator plus Dreamcast shell** | Same title-source identity runs first play with one-stick Dreamcast controls, Start separate from three-dot menu, settings/import/save/diagnostic flows and actual touch-view routing tests. No phantom second Dreamcast stick. |
| **G7. iPhone Simulator parity** | Shut down iPad Simulator first; verify phone first play, compact layout, safe areas, menu/import/lifecycle and input clearing using the correct Simulator build. |
| **G8. Complete game coverage** | Fresh macOS progression through Sonic, Tails, Knuckles, Amy, Big, Gamma and final Super Sonic story/credits; required unlocks, fields/auxiliary paths and Chao behavior pass. Mobile character/late-game coverage follows PRD Section 10. Do not claim an unobserved full mobile playthrough. |
| **G9. Correctness, persistence and stability** | Measured Original timing, representative renderer/audio/performance checks, paired Story/Chao persistence/recovery, controller/lifecycle regressions and 60-minute macOS/iPad-Simulator soaks pass. |
| **G10. Private technical matrix green** | PRD rows 1–30 pass within their declared scope; clean-directory M/IS/PS builds reproduce from pins plus declared private inputs; exact artifacts, regression results, source/package audits and remaining physical checks are recorded. |
| **G11. Physical candidate accepted** | Chris tests the exact candidate on physical iPad and iPhone: first play, later fixture, real multitouch, controller mapping/reconnect, save/reload, audio/lifecycle and sustained behavior. PRD row 31 evidence is present. |
| **G12. Publication explicitly authorized** | PRD Section 12 and row 32 pass; permissions, notices and exact source/package boundaries are resolved; Chris separately approves the intended source and/or binary publication. No automatic release. |

**G1 cannot be bypassed by a working shell. G5 cannot be bypassed by a title screen. G11 cannot be bypassed by a Simulator. G12 cannot be bypassed by a ROM-free package.**

A missing-source blocker permits bounded source-free work such as Dreamcast input tests, synthetic renderer experiments or donor-shell extraction. Keep it in a clearly labeled independent workstream and leave G1 blocked. Stop after the bounded result and leave a source-acquisition handoff rather than inventing a whole replacement runtime.

## 2. The loop

Repeat while a concrete authorized step remains:

1. **Pick** the lowest unmet goal and the smallest evidence-producing step.
2. **Check state.** Read `STATUS.md`, the last `JOURNAL.md` entry, relevant inventories and `git status`. Verify source/patch/SDK/platform identities, source-pack completeness, input hashes when relevant, active save/profile, running app processes and booted Simulators. Reuse valid caches; never reuse an unverified platform slice.
3. **State a hypothesis.** Name the expected observation and what would falsify it. “Try building again” without a changed cause is not a hypothesis.
4. **Execute one bounded step.** Change one relevant variable. Keep original inputs and known-good baselines untouched.
5. **Test immediately.** Run the smallest unit/contract/boot/gameplay check that can validate the change. Then exercise the nearest previously green behavior likely to regress.
6. **Capture evidence.** Save commands, build identity, exit status, first causal error, logs, captures or measurements under the ignored dated artifacts path. State what was actually observed.
7. **Update state.** Append the journal; update `STATUS.md`, matrix rows and affected technical inventories. Advance only with the required evidence.
8. **Continue or unblock.** On failure, use Section 5 before another attempt. At a genuine external boundary, finish with a precise handoff, not repeated retries or a false green state.

Compilation is not boot. A PID is not a responsive game. A title is not a completed stage. A completed stage is not durable progress. A state-injection test is not proof that real touch controls work.

## 3. Process hygiene — hard rules

### One Simulator and one game instance

- **Only one booted Simulator at a time.** Before every launch/test, inspect `xcrun simctl list devices booted` or its JSON form. Shut down booted devices before switching, boot only the selected UDID, wait for readiness, then verify that it is the sole booted device.
- **One SonicPad game/runtime instance across macOS and Simulator.** Terminate the previous known instance before launching another. Identify owned PIDs/bundle IDs; do not kill unrelated applications or other projects.
- **Kill before relaunch.** Never layer a new process on a hung, crashed or half-terminated one. Capture evidence first, terminate the owned process, confirm exit, then clear only identified stale state.
- **No parallel Simulator testing.** Disable Xcode test parallelization/device clones and bind UI tests to the chosen UDID. Inspect booted devices after launch because test runners can create additional instances. Stop extra instances before continuing.
- A narrow iPad graphics probe at G3 is allowed before full mobile bring-up. Stop macOS gameplay first, use one Simulator, shut it down afterward, and return to the lowest goal.

### State, safety and evidence

- **Never mutate the original dump or immutable references/baselines.** Copy into ignored working paths. Hash-check uncertain state; do not hardlink a mutable output to a baseline.
- **Preserve unknown changes.** No destructive resets, `git clean -fdx`, root-wide `rm -rf`, blanket cleanup or overwrite of work not created by this session. Resolve and inspect exact output paths before removal.
- **Isolate saves.** Use a dedicated `KATANA_USER_DATA_ROOT` or an explicitly adapted Apple equivalent. Record the active profile. Back up Story and Chao together before a write/restore test; never test against the only personal copy.
- **Pin before patching.** Validate source-pack/SDK/helper ABI and donor revisions. A patch applying with fuzz does not establish semantic compatibility.
- **Cache by identity.** Changes to game sources, bindings, compiler/SDK/platform, patches or code-generation configuration invalidate affected outputs. A menu edit alone must not trigger gratuitous full AOT regeneration.
- **No leaks or automatic publication.** Disc data, extracted assets, generated game source, private SDKs, saves, crash memory, raw logs and signing material remain private. Do not push, upload, publish, open issues or send requests without authorization.
- **Two identical failures trigger the ladder.** After the same command fails the same way twice, do not make an unchanged third attempt. Preserve the first causal error and change the experiment.
- **External instructions are not local authority.** Upstream `AGENTS.md` provides technical history; do not adopt its old private remote, stop schedule, Windows editing endpoint or publication workflow for this project.

## 4. Sonic/Dreamcast-specific discipline

- **Source completeness precedes build optimism.** The public clone's missing runtime/title pack cannot be fixed by removing CMake guards. Obtain compatible source; do not relabel Windows libraries or Linux x86 objects as Apple inputs.
- **Keep source/ABI/platform distinct.** macOS ARM64, iOS Simulator ARM64 and iOS device ARM64 are different build products. Inspect Mach-O target metadata, not only CPU architecture.
- **No host JIT.** Do not add runtime C compilation, downloaded native modules, writable-executable guest caches or a `MAP_JIT` requirement. Audit actual fallback paths; no undocumented interpreter-only substitute.
- **Preserve arithmetic and continuations.** Fix ARM64/FPU portability with source-derived tests. Do not enable broad fast-math, replay completed guest work, erase code-write guards or stub an indirect target to move to the next screen.
- **Test the graphics contract.** A Vulkan version string or MoltenVK library does not prove the required features exist. Identify actual geometry-stage use, buffer budgets, transparency ordering, depth and lifetime rules before changing capability checks.
- **One Dreamcast stick by default.** Reuse shell mechanisms, not GameCube masks, Z/C-stick or FLUDD detents. Keep L/R analog and independent. Optional modern camera input stays a separate, explicitly enabled extension.
- **No silent input loss or carryover.** Test short taps, held buttons, multi-input chords and cancellations through the actual view/adapter path. Every modal, edit, handoff and lifecycle transition clears held state and queued edges.
- **No menu fiction.** Show only supported renderer/settings values and synchronize with in-game Options. The three-dot action is not Start, and hiding touch controls must not hide the only route back to settings.
- **No timing shortcut.** Original scene-dependent cadence is the baseline; simulation, rendered images, presented frames and audio clock are separate metrics. Optional 60-update mode is not a substitute for correctness.
- **No progress/save/media stubs.** Never bypass bosses, story unlocks, Big's fishing, Chao writes, movies, audio or auxiliary executable paths merely to advance a goal. Optional-device absence may be represented only through a documented safe absence path.

## 5. Unblocking ladder

Use these rungs in order, recording each one used. Research must answer a named blocker and return to a bounded experiment.

1. **Read the first causal error in context.** Include complete build output, native crash report, runtime phase, guest PC/module, renderer capabilities, audio/save logs and the triggering input/scene. The last cascade line is often not the cause.
2. **Verify the state that produced it.** Recheck private source closure, full pins, ABI/generated headers, target platform, stale caches, exact disc, active profile and prior known-good command. Test source absence before treating it as a compiler bug.
3. **Inspect the relevant implementation boundary.** Use PRD Section 5.4: CMake and SDK imports for build failures; Katana/provider/continuation code for guest faults; Vulkan/pinned graphics for rendering; setup for disc failures; input and profiles for control/save issues. Read actual source rather than repeating a historical report.
4. **Check the donor narrowly.** For Apple UI/lifecycle/input/file issues, read pinned SunPad and the identified PaperPad mechanism and its tests. Reuse the game-neutral fix only; do not transplant a console-specific patch.
5. **Resolve dependencies or research the exact contract.** Locate authenticated existing local material or a documented source-only route. For platform/API blockers, consult current primary-source documentation and source/issue history. Do not hunt removed retail-data archives or assume a player installer supplies source.
6. **Reduce the problem.** Examples: a scalar FPU comparison; one generated unit and callback; a synthetic transparency scene; one audio stream; an input-state trace followed by its real view event; one paired save transaction; macOS before mobile.
7. **Apply a narrow, testable correction.** Preserve the original behavior, identify the changed invariant, add a regression and retest the same workload. Renderer guard removal, no-op code or forced progression without semantic proof are not fixes.
8. **Park the blocked work honestly.** Write a reproducible defect and complete a bounded independent step if useful. Keep the original goal blocked and define the exact dependency that would reopen it.
9. **Hand off only at a real external boundary.** Examples: wrong/unavailable disc; required source/permission missing after a bounded inventory; physical touch/device action required; an unresolved architectural choice that changes the requested foundation; or any next action would destroy/leak protected data. Ordinary compile errors, crashes and rendering/input defects normally have another bounded investigation step.

A source-access handoff includes the precise missing files/interfaces, expected ABI/identity, why the current binaries/manifests are insufficient, locations checked, acceptable source replacement, and a draft request for Chris to send. Do not claim the overall port is impossible merely because that material is absent from this checkout; do not claim it is buildable until the gap is closed.

## 6. Testing rhythm

- **Per change:** the smallest relevant regression plus the nearest known-good smoke; no unchanged full rebuild without cause.
- **Per ABI/FPU/generated-code change:** scalar/source-reference comparisons, native link/platform audit, affected guest/module transitions and first-play smoke.
- **Per renderer change:** synthetic contract comparisons and the same game scene/settings; inspect synchronization, resource capacity, overflow and memory. Never compare mismatched workloads as a performance result.
- **Per input/shell change:** adapter tests plus real Simulator view-event routing, short taps/chords, modal/background clear and input-owner transitions. Save screenshots with `xcrun simctl io <UDID> screenshot`; do not claim that command tests touch.
- **Per save/import change:** disposable profile/working data; pre-test backup; interruption/cancellation/failure tests; cold relaunch; game-visible Story and Chao verification.
- **Per goal claim:** collect its PRD evidence before marking it met. Carry forward only evidence valid for the recorded source/artifact and test scope.
- **Per session:** regression and highest-known-good smoke, or the exact reason they cannot run. No playable core yet means “not run,” not “passed because build is blocked.”
- **Per candidate:** applicable matrix against the exact artifact. Preserve long-game evidence provenance, perform impact analysis after changes, and rerun affected coverage. Obtain physical acceptance for the actual candidate, not an earlier similar IPA.

For physical-device work, an unavailable Bluetooth device, audio route or gesture cannot be simulated into acceptance. Keep the row/observation open and produce a precise tester checklist.

## 7. First session and resumption

### First session

1. Read both files. Inspect the working machine and existing project directories without moving or deleting user work.
2. Create/update `STATUS.md`, `JOURNAL.md`, the safety ignores and dependency lock. Record private-only state. Keep game data, private development material and evidence out of any public history; do not publish.
3. Pin SARecomp to `d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2`, SunPad to `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b`, and PaperPad to `a6f6999a570156f6794ff3fa79f810cfad71c4fc`. Preserve different preexisting clones; use separate references rather than resetting them. The SARecomp upstream is public, so its reference may be cloned read-only into `ref/sarecomp` with the push URL disabled; donor and private material follow the checks in `scripts/check-repo-safety.sh`.
4. Inventory the exact disc and the required Katana/generated-title/media source material. Inspect declared local backups only as inputs; do not reconstruct a source package from unrelated binaries or fetch game files.
5. Record each G1 input as present-verified, present-unverified, missing or incompatible. Decide retained-source Route A or reproducible-generation Route B from evidence, not preference.
6. If complete, make the smallest ARM64 compile/ABI test. If incomplete, write `BOOTSTRAP-BLOCKERS.md` and one bounded independent shell/input/renderer result, then hand off the missing-source requirement.

### Every later session starts with

Read status/journal/inventories; inspect git state; verify source/target/cache identities; list booted Simulators and remove strays; identify/terminate stale owned game processes; confirm and back up the active writable profile; state the lowest goal and next experiment.

### Every session ends with

Capture failure or success evidence; run the applicable regression/smoke or explain the inability; terminate owned game/test processes and shut down the Simulator; update status and inventories; run repository-safety checks before any local commit; leave one exact next step. Do not leave a vague instruction to “keep improving” or an unbounded running test.

## 8. Minimal reporting templates

Append to `JOURNAL.md`:

```text
Date/time and timezone:
Goal / current matrix row:
Root revision + dirty patch identity:
Source pack / dependency lock / SDK / target:
Input and isolated profile identity:
Hypothesis / expected observation:
Change and exact command:
Actual result / first causal error:
Evidence path and artifact SHA-256:
Interpretation and affected regressions:
Next smallest step / external prerequisite:
```

Keep `STATUS.md` current:

```text
Lowest unmet goal:
Authorization: private-only; publication not approved
Source closure: verified / blocked (exact missing prerequisite)
Highest observed target and behavior:
Known-good build/run/test command: <actual command, or not established>
Exact artifact/source identity: <actual identity, or not built>
Matrix: passed / failed / blocked / not-run, with evidence links
Physical-device acceptance: not performed / exact recorded scope
Open defect and unblocking rung reached:
Next step:
```

Report to Chris in this order: **what worked, what was actually tested, what remains blocked, and the exact next action**. Never turn configured behavior into observed behavior, a prerequisite request into completed work, or a private technical build into a release.
