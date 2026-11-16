# RingPad test matrix

Updated: **5 October 2026**. Status of PRD Section 10.2 rows 1–32 for the
current, unbuilt source state. Every row carries its observation type, because
a source inspection, a host unit test, a compiled target, a Simulator run and a
physical device are different kinds of evidence and are not interchangeable.

Latest evidence is bound to audit root `864ae6e` plus the implementation patch
identity recorded in [`JOURNAL.md`](JOURNAL.md). Native ARM64 macOS probe and
input-test binaries now compile and run with Apple CLT. No game/core target
has been built. Nothing here is a green technical matrix; the private technical bar
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
| 1 | Workspace safety/authorization | Repo | **partial** | source inspection + native macOS probe | Safety invariants, incomplete dependency lock and private rights/provenance document established. Apple CLT/Foundation ARM64 compile/link/run passes (MACOS, SDK 27.0). Full Xcode/mobile tools remain license-blocked. |
| 2 | Full source closure | Fresh workspace | **blocked** | actual input hash audit | Source audit verifies 38/39 title/provider files, but guest code, SDK headers/contracts and runtime archive remain absent. Actual FFmpeg tree verified. This is input-identity evidence, not full G1/build/ABI closure; see `BOOTSTRAP-BLOCKERS.md`. |
| 3 | Exact disc/extraction | Host + import tests | **blocked** | actual format/missing-input rejection | `verify-game.py` rejects the provided CDI and missing GDI with exit 2. Config cross-checked against pinned upstream track identities. Six-root scan found no GDI; machine-wide absence not established. No actual supported-track or extraction acceptance. |
| 4 | ARM64 and ABI | M, IS, PS builds | **not-run** | — | macOS host probe/input tests pass, but Katana arithmetic/ABI/core linking and mobile slices require row 2; full Xcode is also license-blocked. |
| 5 | Executable coverage | Core tests + M | **not-run** | — | Needs the guest AOT/module set (row 2). |
| 6 | Graphics contract | M, IS, PS | **not-run** | — | Needs a built core (rows 2/4). |
| 7 | Media and audio | M, IS, PS | **not-run** | source acquisition only | Exact FFmpeg source acquired and archive hash verified; Katana media interfaces remain missing. No Apple codec build or media playback tested. |
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
| 29 | Regression suite | Host + highest-good targets | **partial** | native macOS + Python host tests | Three input suites pass with Apple CLT (142 checks); LLVM comparison also passed previously. 16 bootstrap verifier regressions pass. Intentional compiler failure exits 4 and never runs a stale binary. Game end-to-end smoke remains unavailable. |
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
| Apple CLT ARM64 Foundation probe | **passed in probe scope** | native macOS compile/link/run | `scripts/check-prerequisites.sh`; Mach-O platform MACOS, minimum 13.0, SDK 27.0. Full G0/mobile tools remain open. |
| Exact r354 title/provider source recovery | **partial** | per-file source hash verification | `scripts/recover-title-source.py`: 38/39 exact files staged privately; CMake missing. Does not close row 2. |
| FFmpeg implementation source | **passed in acquisition scope** | public source archive hash verification | `scripts/fetch-media-source.py`; pin in `config/media-source.json`. No row 7 build/playback acceptance. |
| Retained-source identity audit | **blocked** | read-only file/archive hash checks | `scripts/check-source-closure.sh`: 38/39 title/provider, 0/1,474 guest-source/metadata, 0/177 headers, 0/4 contracts, runtime archive missing; 10,139 FFmpeg archive/materialized entries verified. Report deliberately does not mark G1 complete. |
| Source/disc verifier failure contracts | **passed in synthetic scope** | 16 host Python regressions | Missing/corrupt source, binary-only input roots, modified cached source, lock drift, wrong disc content/layout, missing tracks and directory escapes rejected. Synthetic successful track hashes do not establish actual PAL-disc acceptance. |

## Session close

- Owned game/test processes: none started this session.
- Simulator: none launched; stored device plists record shutdown. Live simctl query blocked by full Xcode license.
- Repository-safety check: last run green (see row 1).
- Regression status: all three input host suites green in Apple and LLVM lanes; the nearest
  previously green behavior is the same suites, which are the highest observed
  target.
- Next exact step: see the last [`JOURNAL.md`](JOURNAL.md) entry.
