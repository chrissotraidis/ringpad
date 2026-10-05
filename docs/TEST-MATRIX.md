# SonicPad test matrix

Updated: **5 October 2026**. Status of PRD Section 10.2 rows 1–32 for the
current, unbuilt source state. Every row carries its observation type, because
a source inspection, a host unit test, a compiled target, a Simulator run and a
physical device are different kinds of evidence and are not interchangeable.

Observed evidence is bound to the root revision `caabe68` plus the dirty patch
identity recorded in [`JOURNAL.md`](JOURNAL.md). No Apple target has been built,
so every row that requires a compiled binary is blocked or not-run rather than
failed. Nothing here is a green technical matrix; the private technical bar
(rows 1–30) is far from complete.

## Status key

| Status | Meaning |
|---|---|
| **passed** | The row's pass condition was observed in its declared scope. |
| **partial** | A strict subset of the pass condition was observed; the rest is open. |
| **blocked** | An external prerequisite is missing; no local retry can satisfy it. |
| **failed** | The pass condition was attempted and not met. |
| **not-run** | Not attempted, or blocked upstream. |

## Matrix

| # | Test | Target | Status | Observation type | Evidence / notes |
|---:|---|---|---|---|---|
| 1 | Workspace safety/authorization | Repo | **partial** | source inspection | `./scripts/check-repo-safety.sh` → VERDICT safe: `ref/`, `private-inputs/`, `work/`, `generated/`, `out/`, `docs/artifacts/` ignored; no disc/save/private artifact tracked; all three references push-disabled; HEAD `caabe68` intact. Remaining G0 item: Apple toolchain unusable until the license is accepted. |
| 2 | Full source closure | Fresh workspace | **blocked** | source inspection | Public SARecomp present and correct but insufficient: the r354 Katana/runtime/title closure is absent (103 `src/*` files include `katana/runtime/*` headers that do not exist locally). See [`BOOTSTRAP-BLOCKERS.md`](BOOTSTRAP-BLOCKERS.md) Blocker B. |
| 3 | Exact disc/extraction | Host + import tests | **blocked** | source inspection | Only `ref/Sonic Adventure.cdi` (multi-region V1.004, `105b9a88…726b9533`) exists; the supported PAL v1.003 GDI plus three exact tracks is absent. No `.gdi` anywhere on the machine. Blocker A. |
| 4 | ARM64 and ABI | M, IS, PS builds | **not-run** | — | Requires a compiled Apple target; blocked by row 2 and by the Xcode license. |
| 5 | Executable coverage | Core tests + M | **not-run** | — | Needs the guest AOT/module set (row 2). |
| 6 | Graphics contract | M, IS, PS | **not-run** | — | Needs a built core (rows 2/4). |
| 7 | Media and audio | M, IS, PS | **not-run** | — | Media/codec source is part of the missing closure. |
| 8 | Mac first-play loop | M | **not-run** | — | Requires rows 2–4. |
| 9 | Tablet first-play loop | IS | **not-run** | — | Requires rows 2–4; iPad Simulator not usable until the license is accepted. |
| 10 | Phone first-play loop | PS | **not-run** | — | As row 9. |
| 11 | Sonic story | M; mobile samples | **not-run** | — | Requires rows 2–4. |
| 12 | Tails story | M; mobile samples | **not-run** | — | Requires rows 2–4. |
| 13 | Knuckles story | M; mobile samples | **not-run** | — | Requires rows 2–4. |
| 14 | Amy story | M; mobile samples | **not-run** | — | Requires rows 2–4. |
| 15 | Big story | M; mobile samples | **not-run** | — | Requires rows 2–4. |
| 16 | Gamma story | M; mobile samples | **not-run** | — | Requires rows 2–4. |
| 17 | Final Super Sonic story | M; mobile late fixture | **not-run** | — | Requires rows 2–4 and a normal unlock path. |
| 18 | Fields and auxiliary paths | M; affected mobile routes | **not-run** | — | Includes the Mini Cart auxiliary AOT path; part of the missing closure. |
| 19 | Chao operations | M, IS, PS | **not-run** | — | Requires rows 2–4. |
| 20 | Complete input-state contract | Host tests | **partial** | host unit test | `./scripts/run-host-tests.sh` → 46/46 (adapter) + 50/50 (shell) + 46/46 (touch) pass. State level covers the canonical one-stick profile, radial deadzone, analog L/R thresholds, edge latch, the touch/controller mixer, the narrow face mapping, player-slot ownership, chords, rapid distinct taps, slide-out recentering, modal clear, the touch layout (one stick, no C-stick/Z), hit priority, stick normalization and shoulder slide without a detent. Still open: real view-event routing, owner transitions through the Apple overlay and live shell clearing. See [`INPUT.md`](INPUT.md). |
| 21 | Actual touch/UI path | IS, PS | **not-run** | — | Needs a built Apple target and view-event routing; `xcrun simctl io screenshot` does not test touch. |
| 22 | Three-dot/settings parity | IS, PS | **not-run** | — | Needs the built shell. |
| 23 | Import/reimport failure safety | IS, PS | **not-run** | — | Needs the built shell; the setup engine's failure paths are source-visible only. |
| 24 | Story/Chao persistence | M, IS, PS | **not-run** | — | Needs rows 2–4. |
| 25 | Ownership and lifecycle | M, IS, PS | **not-run** | — | Needs the built shell. Real hardware checks remain row 31. |
| 26 | Timing/performance | M + diagnostic mobile runs | **not-run** | — | Needs a playable core. |
| 27 | Diagnostics/privacy | M, IS, PS | **not-run** | — | Needs the built shell. |
| 28 | Soak/repeated transitions | M + IS | **not-run** | — | Needs a playable core. |
| 29 | Regression suite | Host + highest-good targets | **partial** | host unit test | Three input host suites exist and pass (`test_dreamcast_input` 46/46, `test_dreamcast_shell` 50/50, `test_dreamcast_touch` 46/46); no end-to-end smoke is possible without a built target. |
| 30 | Clean-machine reproduction | New directory | **blocked** | source inspection | Cannot reproduce from pins plus declared private inputs while the private inputs are absent (rows 2/3). |
| 31 | Physical exact candidate | Physical iPad + iPhone | **not-run** | — | No candidate build exists. |
| 32 | Publication authorization | Source + package | **not-run** | — | Not requested; private-only. |

## Independent workstream (G1-blocked bounded work)

When a goal is blocked by missing source, the loop permits clearly labeled
bounded work that does not change the blocked goal's status. The single-stick
Dreamcast input contract is that workstream. It contributes partial evidence to
rows 1, 20 and 29 and to no other row, and it does not advance G1.

| Item | Status | Observation type | Evidence |
|---|---|---|---|
| Canonical one-stick Dreamcast state + guest pad/axis adapter | **partial pass** | host unit test | `port/input/DreamcastInputState.h`, `DreamcastInputAdapter.cpp`, 46/46 via `scripts/run-host-tests.sh` |
| Edge latch (sub-sample tap delivered exactly once) | **partial pass** | host unit test | `port/input/DreamcastInputState.h` `EdgeLatch`; adapter suite |
| Touch/controller mixer (single stick, rising-edge latch, clearAll) | **partial pass** | host unit test | `port/input/DreamcastInputMixer.{h,cpp}`, from SunPad mixer; 50/50 shell suite |
| Narrow face-button mapping + player slots | **partial pass** | host unit test | `port/input/DreamcastControllerMapping.{h,cpp}`, `DreamcastControllerSlots.h`; shell suite |
| Touch layout (one stick) + per-touch arbitration | **partial pass** | host unit test | `port/input/DreamcastTouchLayout.{h,cpp}`, from SunPad overlay; 46/46 touch suite |
| Reference pins frozen read-only (SARecomp/SunPad/PaperPad) | **passed** | source inspection | `ref/sarecomp` `d5288fe`, `ref/sunpad` `c77dd61`, `ref/paperpad` `a6f6999`, all push DISABLED |
| Simulator view-event routing | **not-run** | — | needs a built Apple target + accepted license |
| Physical multitouch/controller | **not-run** | — | reserved for row 31 |

## Session close

- Owned game/test processes: none started this session.
- Booted Simulators: none.
- Repository-safety check: last run green (see row 1).
- Regression status: both input host suites re-run green; the nearest
  previously green behavior is the same suites, which are the highest observed
  target.
- Next exact step: see the last [`JOURNAL.md`](JOURNAL.md) entry.
