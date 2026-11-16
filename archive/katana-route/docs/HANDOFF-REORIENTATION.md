# RingPad review and reorientation handoff

Written 10 October 2026 at Chris's request. **Review the project direction before
resuming development.** This document is the starting point; the long status and
recovery logs are historical evidence, not a queue the next bot must execute.

## Outcome and recommendation

There is no verified playable Apple port. The original SARecomp bootstrap gate
G1 remains unresolved. We produced a useful single-stick input layer, exact
source/input audits, and extensive isolated Dreamcast recovery experiments.
Those experiments did not establish normal title-to-gameplay flow, decoded
movie presentation, audible playback, saves, Simulator routing or device use.

**Recommendation: stop expanding the instruction-by-instruction recovery queue
until an independent reviewer decides whether it is a viable route to the PRD.**
Keep the input work and bootstrap tools. Preserve the recovery work for review;
do not mistake its size, entry counts or passed local predictions for product
completion. Do not replace the required native Apple product with an emulator
demo or change the production foundation implicitly.

Chris's latest instruction is to wrap up, push/merge the existing work, and leave
a handoff for another bot to review and reorient. It is not authorization to
continue the old experiment queue automatically or contact upstream maintainers.

## Critical change found during this handoff

**The entire local `work/` directory is absent on 10 October.** This includes the
experimental DreamcastRecompiled checkout, patched runtime/emitter candidates,
build products, review/disassembly inputs, and the prepared next-run script.
The prior instruction to free space and run
`work/dreamcast-recovery/run-original-startup-live-movie-video-payload.py`
is now stale. Do not attempt it until the missing dependencies are restored or
reconstructed and reverified. This handoff did not delete or recreate them.

`tests/recovery/`, `patches/recovery/`, `docs/artifacts/`, `ref/` and `.local/`
remain present. Three checkpoint files still have their recorded hashes, but
their referenced input sets are incomplete:

| Checkpoint | Recorded identities | Missing now |
|---|---:|---:|
| Stream-advance native checkpoint | 29,886 | 556 |
| Video-copy preparation checkpoint | 380 | 16 |
| AICA verifier preparation checkpoint | 25 | 1 |

All missing paths in these three checks are under `work/`. This is an existence
audit, not a fresh verification of every remaining byte. Reports survive as
historical evidence; the complete experiments are not currently reproducible.
Git does not restore ignored material. A bot on another machine will also need
the owner's local evidence/input set before it can inspect or replay those runs.

At handoff, free space was about 2.43 GB, below the 2,952,790,016-byte full-run
guard. Space is a secondary blocker; clearing it alone no longer makes the
prepared run executable. Recheck actual space before any work.

## What is established, and its limits

| Work | Evidence | What it does not establish |
|---|---|---|
| Single-stick Dreamcast input | `port/input/`; fresh ARM64 host run: 46 adapter + 50 shell + 46 touch checks pass | Apple view/event wiring, guest execution, real multitouch/controller behavior |
| Pinned SunPad mechanisms | `docs/INPUT.md`; mixer, face remap, ownership, per-touch arbitration | A complete native application shell |
| Bootstrap auditing | 16 synthetic verifier tests pass; exact source/disc manifests and acquisition request | Availability or compatibility of missing runtime/title sources |
| Public-source recovery | Source patches, generators, models and saved native reports | Production foundation approval or complete game coverage |
| Latest native stream advance | Historical 119-cycle CPU/RAM/TMU match, 1,501 prior captures unchanged; 2,639 MOVIE entries | Decoder, renderer, full ARM path, audio quality or gameplay |
| First video packet boundary | Separate post-run 2,645-cycle CPU/RAM/TMU audit, packet `0xE0`, 2,018 bytes, prefix `00 00 01 B3` | A frozen prediction of that continuation or a decoded frame |
| Next video-copy experiment | Three compiled bodies, 187 instructions, 32 new entries; predicted 3,319-cycle copy | Native execution: it never ran |
| AICA verifier | Two historical windows passed; 28 corrupt/unsupported cases rejected | Full ARM-register modeling, timer-overflow behavior or audible output |

Earlier scene/geometry/image and floating-point experiments are described in the
historical logs. They were not rerun for this handoff and must not be presented
as normal game progression. A successful selected-function harness is a narrower
claim than an ordinary game boot.

## Original goal and unresolved G1

Read `docs/prd.md` for the product contract and the goal stack near the end of
`docs/goal-loop.md`. The intended product is a native ARM64 Apple Sonic Adventure
port with the pinned shell approach, correct gameplay/input/media/persistence,
and separate macOS, Simulator and physical-device acceptance. Keep that scope.

Public SARecomp at `d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2` is not a complete
portable source release. Windows binaries and public source manifests cannot
replace the missing material. Required source paths still missing:

| Group | Missing |
|---|---:|
| Retained/generated title source and metadata | 1,474 |
| SDK headers | 177 |
| Generated SDK contracts | 4 |
| Exact r354 title/provider CMake file | 1 |
| Katana source archive | 1 |
| **Total** | **1,657** |

Exact archive: `katana-source-178448be.zip`, 6,185,732 bytes, SHA-256
`85e5bd27952552f0957559cca82bde5b4024cd71e50e94650cd922f42430cd66`.
Thirty-eight of 39 title/provider files were recovered from public history.
The pinned public FFmpeg source is present; Apple decoder builds and matching
Katana audio/movie interfaces remain unproven/missing. Mini Cart and the
transitive include/provider/build closure also need verification.

The required owner input is PAL v1.003, MK-5100050, GDI plus three exact tracks.
`ref/Sonic Adventure.cdi` is V1.004/JUE and is an incompatible input for the
original production route. It was used only for the separately authorized
recovery feasibility work. Container conversion does not solve this mismatch.

`docs/BOOTSTRAP-BLOCKERS.md`, sections **G1: exact source inventory**, **Supported
game input**, and **Source-only acquisition request**, contain the exact inventory
and ready-to-send request. The request is unsent. Do not send it without Chris's
authorization. A compatible complete source checkout with provenance can be
evaluated; the archive filename is an identity, not proof of public availability.

The earlier search covered public history, releases, forks and several archive
indexes. The 9 October metadata refresh found no new source release; the Katana
repository returned 404. Those observations do not prove the source cannot exist
privately. Repeating the same search without a new lead is unlikely to help.

## Where the work went off course

This is the outgoing bot's assessment, not a finding the reviewer must accept:

- The recovery loop optimized for reaching the next missing function. There was
  no demonstrated bound on how many functions, devices or transitions remained
  before a usable product. Narrow progress continued without a reliable estimate
  of the end-to-end work.
- The source-acquisition route and an alternative runtime experiment became
  interleaved. Progress in the latter never resolved the former, but updates
  emphasized small successes more than the unchanged product-level gap.
- At handoff there are 1,020 recovery source/test files, roughly 29 MB, and more
  than 41,000 added lines across existing documents. Repeated checkpoint blocks
  obscured the few facts needed to make a project decision.
- Many test sources contain absolute local includes and depend on ignored
  captures, generated manifests and scripts. A green historical report is not
  a clean-checkout build. The absent `work/` directory exposes this weakness.
- Full independent device/ARM behavior remained open while selected CPU/RAM
  windows passed. Test counts and registered-entry counts cannot estimate
  gameplay completion or justify continued investment on their own.

Do not spend another unbounded run polishing this evidence system. First assess
whether the implementation path can credibly produce the requested game.

## Reviewer assignment and decision loop

1. Inspect the current checkout, original PRD, input implementation, source
   blockers and a small representative sample of recovery evidence. Verify
   claims from files rather than adopting this narrative as proof.
2. Write a short **keep / refocus / stop** assessment. Identify reusable work,
   misleading or weak claims, missing dependencies, and the shortest credible
   path to a normal macOS title/new-game/first-play milestone.
3. Compare two explicit routes: acquire compatible Katana/title sources for the
   original port, or seek Chris's evidence-backed approval for a different
   foundation. Explain the cost and unknowns of restoring the experimental core.
   Public alternatives should be evaluated as candidates, not silently adopted.
4. Propose at most one bounded feasibility experiment, with a resource/time
   budget, a user-visible acceptance result and a stop condition. A new entry
   count or another modeled function return is insufficient as the sole result.
5. Before implementation, state the decision and why it moves toward the PRD.
   If external material is still required, hand over the exact request rather
   than repeatedly trying an impossible build.

Do not create more parallel game/Simulator runs, alter the disc-validation
contract, bypass missing-target guards or relabel a prediction as execution.
Keep Start separate from the host menu and preserve one stick and independent
analog L/R. Physical-device checks remain owner acceptance.

## Evidence and navigation

Start with this file, `docs/prd.md`, `docs/INPUT.md` and the named blocker
sections. Use `docs/STATUS.md`, `docs/JOURNAL.md`, `docs/RECOVERY-LOOP.md` and
`docs/BUILDING.md` for targeted historical searches; do not read their chronology
as an instruction to continue the last queue.

Key local-only evidence under `docs/artifacts/recovery-aot/`:

- `20261009-live-movie-stream-advance-final-verification.json`
- `20261009-original-startup-live-movie-stream-advance/live-movie-stream-advance-checkpoint.json`
  SHA-256 `673b81137bd8cf15e0aef4f8420823f192bc6c56d1ff8096de9066cf45b8793c`
- `20261009-live-movie-video-payload-preparation/preparation-checkpoint.json`
  SHA-256 `2ccf85143c8018b116753a85e2c71699287794586e1066fc4ff7e7109d6f3880`
- `20261009-video-payload-aica-verifier-preparation/aica-verifier-checkpoint.json`
  SHA-256 `847981bacd71e423a11475870132ea876e8a579d487116522ca3bf7225a43dd1`

The complete dependency checks for these checkpoints currently fail because
referenced `work/` inputs are missing. The filenames/hashes above locate saved
records, not a claim that they can now be replayed.

Fresh handoff validation: 142 ARM64 input checks, 16 bootstrap-verifier tests,
and AST parsing of 72 Python files pass. A fresh source audit still reports
1,657 missing paths, 38/39 verified providers and 10,139/10,139 verified FFmpeg
entries; its expected exit code is 2. No full recovery build, native game,
Simulator or physical-device test was run. The host suite wrote to a fresh
`docs/artifacts/handoff-20261010/` directory to preserve previous evidence.

## Git and preservation boundary

The destination is the existing **private** `chrissotraidis/ringpad` repository.
Chris authorized pushing/merging the project work and this handoff on 10 October.
That does not authorize a public release or uploading the ignored game/source
inputs and captures. `ref/`, `.local/`, `private-inputs/`, `work/`, generated output
and `docs/artifacts/` stay local/ignored. Patches retain the upstream MIT notice.

The checkpoint commit preserves the existing diagnostic sources and historical
logs for review, including unsuccessful/superseded experiments; it is not a claim
that all recovery tests pass or that the snapshot is an application release.
Do not delete or reorganize that evidence before determining what the reviewer
needs. The original goal remains unresolved; finishing this handoff does not
complete the port.

Staging audit: no binary files, private-input/evidence paths or detected private
key/GitHub-token patterns in the staged snapshot. This is a private preservation
snapshot, not a comprehensive code/security review. Diff whitespace diagnostics
are confined to preserved patch-context blank lines and trailing blank lines in
`startup-movie-resident-rate-prefix.cpp` and its `-candidate.cpp` counterpart.
Those historical hash-bound files were left byte-for-byte intact; the remaining
staged files pass `git diff --check`.
