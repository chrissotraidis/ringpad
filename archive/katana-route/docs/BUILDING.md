# SonicPad current build and input checks

**10 October 2026 — review handoff supersedes the experiment queue.**
Chris requested wrap-up, private push/merge and an independent direction review.
Start with [HANDOFF-REORIENTATION.md](HANDOFF-REORIENTATION.md). The local `work/`
directory is now absent, including the prepared next-run script and experimental
runtime checkout. Historical checkpoints retain their recorded hashes but have
missing dependencies. Freeing disk space alone cannot resume the prior command.
Do not automatically extend the recovery queue; assess viability against the PRD.
The historical entries below are preserved, not current instructions.

**Current execution checkpoint, 7 October:** the six-callback binding loop,
interface wrapper and descriptor fields return in full startup:339state/247device
checks,331earlier captures and same-binary baselines preserved. Next execute ED38A0:
matching signature predicts10cycles; mismatch tails to missingED1AA2 in12cycles.
See [STATUS.md](STATUS.md) for exact identities, capacity guard and open G1 gates.
Older recovery notes below are historical.

**Previous bounded recovery — 7 October 2026: initializer tail returns in connected native tests, R3/C8 partial.**

Three original bodies are now verified beyond the last full startup stop:

- **ED8216:** 36 positive-prefix, 60 nonpositive-return and nine activation
  controls pass. Actual count8/buffer8EEE8000 reaches ED8246 in17cycles; zero and
  negative counts return in18. Its four saved words match the prior prediction.
- **ED8246:** 16 leaf, 48 connected and nine activation controls pass. It performs
  22 ordered word stores per record, with live globals/literals preserved under
  overlap. The eight-record loop performs704store bytes and returns ED8108 in
  722cycles; the retained caller reaches EDA7C4 at727. Leaf timing is80cycles.
- **EDA7C4:** 40 helper, 40 connected and nine activation controls pass. Its two
  retained copies transfer128bytes with two PREF calls;135cycles for a supplied
  pointer,138 for the aligned fallback. The ED8110 return tail brings the captured
  initializer back to **ED2574 in141cycles**, with R0=0, SP=FFFED4 and R14=EDC60C.

**267 controls total**, checking complete RAM/CPU/timers/SQ/device state, original
managed activation, aliases, unaligned/overlapping buffers, live literal/source
and frame collisions. Every native run passed ARM64/no-runtime-emission checks;
runtime/compiler/twelve patches remain unchanged. No additional full startup
was run: the authoritative full artifact remains1002functions,324state/232device
checks and its unchanged two-scene baseline. The three new bodies (129instructions)
are not yet integrated into that artifact; do not promote this unit proof to boot.

The independent record-pattern audit initially dropped the captured8E guest-pointer
alias when constructing expected stored pointers. Its failed source and diagnostic
are preserved under `20261007-startup-movie-post-buffer/record-pattern-audit-failure1/`.
The corrected expectation uses the captured pointer, masking only RAM indices.
Native code, model and execution were unchanged; exact full-RAM pattern now passes.

Handoff checkpoint: `179438f9c3678fff9cbc2a7c38cbea56cc314b0baae73e3f3c6a58929092b97c` (227 identities).
Unit checkpoints are `movie-post-work-checkpoint.json` (227),
`movie-post-buffer-checkpoint.json` (246), and `movie-final-setup-checkpoint.json`
(237). Inputs, cached objects and frozen sources were reverified. No game or
Simulator is live. Available bytes: 2,274,852,864; full-capture reserve remains unmet.

**Next:** execute the retained ED2574 success path with the retained ED67F8 fill.
R0=0 selects a nine-word clear at EFB2A0 and then the original callback return.
Predict **59cycles**,36cleared bytes, PC/PR=EDA2E6 and SP=FFFED8. This is unexecuted;
full expected RAM/CPU/timers/SQ are saved in the handoff's `next-startup-preflight.json`.
Test live destination/stack overlaps and aliases/status; then inspect the next
caller before one combined full integration of the three new bodies. Restore
the2816MiB reserve first; preserve323prior named captures and232device checks.

G1 remains unresolved. No missing Katana/generated/header/contract/title-build
sources or supported PAL dump were acquired; Apple media integration remains
unproved. `BOOTSTRAP-BLOCKERS.md` retains the exact inventory and unsent source-only
request. Normal boot/gameplay/audio/save/mobile/device acceptance and the production
foundation remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-movie-initializer-return-handoff/` and
the `20261007-startup-movie-post-work`, `movie-post-buffer`, `movie-final-setup` unit runs.

**Previous bounded recovery — 7 October 2026: final-work wrapper verified in full startup, R3/C8 partial.**

Original EDA9A4 and its EDA9C8 child now execute together in full startup.
The child builds a 1,024-byte table (384 zeros, bytes 0..255, 384 FF bytes).
The wrapper performs the original 1,024-byte ED6708 copy with 16 PREF calls,
installs pointer 8EEF9D80 at EFBAB4, restores PR/SP/R12/R14, and returns to
ED8104 in **5,012 cycles**. The retained BSR/NOP continuation reaches genuinely
missing **ED8216 at 5,015 cycles**, without changing RAM/SQ state.

**324 state and 232 device comparisons pass; all 321 earlier named captures
remain byte-identical.** The provider return and mode/continuation trace prefix
are unchanged. The exact ARM64 executable passes both baseline scenes, ten
external audits, release corruption controls and six unchanged reference images.
The supporting units passed 57 prefix, 117 leaf/wrapper and 41 continuation
controls. Full-run closure: 1,002 functions (859 base plus 143 supplemental),
8,267 supplemental instructions and 935 managed entries. Runtime/compiler and
the twelve patches are unchanged; no runtime code emission was introduced.

**Metadata correction:** the actual sequential continuation trace contains
6,091 entries. The previous audit summary incorrectly repeated 5,391. The old
trace also contains 6,091 entries and is preserved unchanged. This correction
does not alter execution or enlarge the explicit 8,192 continuation limit.

Checkpoint: `e6a744f1435ba29a3560e7d89496de3bd9e7f6f73d96aacbbcffb02c1ac1da45`;
118,409 identities, 115,870 inherited by verified parent digest,
9,863 freshly hashed, including 7,324 inherited identities.
Storage inventory: 24,307 paths, 5,455 newly shared paths reverified.
Sharing exact completed baseline, observed/expected RAM, previously unshared RAM
groups and movie AICA snapshots recovered 902,950,912 bytes. All bytes, paths and
recorded owner/mode/access/modified times/xattrs remain; inode/birthtime changes
are declared. No content deleted. All capacity guards passed during execution.
After verification, 2,689,994,752 bytes remain; restore the 2,816 MiB capture
reserve before another full run. No game or Simulator remains live.

**Next:** execute original ED8216 (24 instructions, four blocks, one BSR).
Actual count is eight at EFB978; buffer is 8EEE8000 at EFB97C. Predict its
positive-count entry saves four words and reaches missing ED8246 in 17 cycles,
with PC=ED822E, PR=ED8236, R12=8EEEA000, R13=8, R14=0, SP=FFFEB4 and T=1.
Also test the zero/negative-count return (predicted 18 cycles), live literals,
aliases, unaligned/overlapping frames and wrapping buffer addition. These are
unexecuted predictions, saved in `next-startup-preflight.json`. Preserve 323
prior named captures and 232 device checks. One classification batch used;
execute before deeper review. The child's behavior is not yet claimed.

G1 remains unresolved under the latest source inventory in `BOOTSTRAP-BLOCKERS.md`:
Katana runtime/generated title sources/matching headers/contracts, exact retained
title CMake, supported PAL GDI and Apple media integration remain open. The
source-only request remains unsent. Full initialization, boot, gameplay,
audio/save/mobile/device acceptance and production foundation remain unproved.
Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-work-transform-startup/`,
`20261007-startup-movie-work-continuation/`, and `20261007-movie-work-transform-storage/`.

**Previous bounded recovery — 7 October 2026: final-work wrapper returns in native unit execution, R3/C8 partial.**

Original EDA9A4 prefix passes 48 argument/stack/alias/status and nine activation
controls; its two saved stack words match the pre-existing full-RAM prediction.
It exposes original EDA9C8 after five cycles. EDA9C8 has 26 instructions, seven
blocks and no calls. Its three ordered loops produce 384 zero bytes, bytes 0..255,
and 384 FF bytes, then install the table pointer in the RTS delay slot.

**48 leaf, 48 connected-wrapper, 12 zero-argument and nine activation controls
pass (117 total).** Whole RAM, CPU, timers and SQ/device state match the independent
model. Controls cover aliases, unaligned frames/buffers, colliding output and live
literal overlap. The captured leaf takes 4,366 cycles. The original wrapper,
including retained ED6708, takes **5,012 cycles**, copies 1,024 bytes to EF9C00,
performs 16 PREF calls, installs pointer 8EEF9D80 at EFBAB4 and restores PR/SP/R14.
Verified wrapper return: **ED8104**. Both unit runs pass ARM64/no-runtime-emission
checks. Runtime/compiler and the twelve patches remain unchanged.

This is connected native unit proof, **not a new full startup capture**.
The last full run remains `20261007-startup-movie-work-followup-startup`:
322 state/230 device comparisons and both baseline scenes passed. Its binary is
unchanged, so those regressions were not repeated for the separate unit harness.
Do not claim the two new bodies are integrated into that full-run artifact.

Unit checkpoint: `eab95e542aca7238ae86c360282d3f9c8f6e62fa62803c366c81831b088a6e3c` (240 identities).
Prefix checkpoint: `31be43b21e5edddb13b3397bf031538872dcaadc8629174d0bb866356f69054e`
(225 identities). Every original input and cached object consumed by the units
was reverified. No game or Simulator is live. Available bytes: 2,390,548,480;
the 2,816 MiB full-capture reserve is unmet. No files were deleted this turn.

**Next:** execute retained ED8104 BSR/NOP from the verified wrapper-return state.
Predict three cycles to missing ED8216, PC=ED8104, PR=ED8108 and unchanged RAM/SQ;
total from EDA9A4 would be 5,015 cycles. This continuation remains unexecuted.
The exact CPU/timer prediction is in `next-startup-preflight.json`. Then integrate
EDA9A4 and EDA9C8 together in one full startup capture after restoring the reserve,
preserving 321 earlier named captures and 230 device comparisons. Do not review
deeper callees before executing that observed continuation.

G1 is still unresolved: Katana runtime/generated title sources/matching headers
and contracts, exact retained title CMake, supported PAL GDI, and Apple media
integration remain open as recorded in `BOOTSTRAP-BLOCKERS.md`. The source-only
request remains unsent. Full initialization, normal boot, gameplay, audio/save,
mobile/device acceptance and the production foundation remain unproved.
Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-work-final/` and
`artifacts/recovery-aot/20261007-startup-movie-work-transform/`.

**Previous bounded recovery — 7 October 2026: byte-permutation helper returns, R3/C8 partial.**

Original EDA920 executes 64 ordered signed-byte iterations, producing two
64-byte permutation tables, then performs retained 16-word and 8-word copies.
**96 helper, 96 connected, 12 branch and nine activation controls pass (213 total)**.
Coverage includes negative byte indices, aliases, unaligned and overlapping
input/output tables, colliding outputs and stack-adjacent destinations. Zero and
wrapping arguments verify the real branch/copy-call boundaries with the copy
callee inactive; invalid-address copies are not executed or claimed. A separate
zero-local continuation proves the no-copy return path without substituting code.
Captured output is 128 permutation bytes plus 96 destination bytes, with one PREF
call. Complete RAM, including reused saved-register/PR/local/R12 stack slots,
matches the pre-execution prediction. The helper restores r10/r11/r12/r14/SP and
returns in **855 cycles**; the retained ED80FC prefix reaches missing **EDA9A4
at 860 cycles**, passing R4=8EEF9C00.

Full startup passes **322 state and 230 device comparisons**. All 319 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
The earlier complete table wrapper remains verified. Both baseline scenes, ten
audits and six reference images pass with the same ARM64 binary. Closure: 1,000
functions (859 base plus 141 supplemental), 8,223 supplemental instructions and
923 managed entries. Runtime, compiler and twelve patches are unchanged.
Initialization, normal boot, playback and gameplay remain unproved.

Checkpoint `movie-work-followup-startup-checkpoint.json`:
`82dce4773319a14a3bc8b27b7078a7d1a50ae2ddfaa544389710cfa64e15e650`; 115,870 identities,
113,431 inherited by verified digest,
8,108 freshly hashed (including 5,669 inherited).
Preservation inventory: 18,852 paths, 3,345 new and
435 previously shared paths reverified. Exact cross-phase
expectation/full-run captures share APFS extents. New full-run source snapshots
also use APFS clones, each checked for exact bytes, size, owner/mode, extended
attributes and timestamps; paths/content remain present. This prevents repeatedly
allocating identical large provenance snapshots. Available space:
2,719,350,784 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDA9A4 (18 instructions, five blocks, two calls). Predict its
entry saves R14 and PR, moves R4 into R14 in the BSR delay slot, and reaches
missing EDA9C8 after five cycles. Captured stack writes: FFFEC0=8EEF9560 and
FFFEBC=8CED8104; terminal PC=EDA9A4, PR=EDA9AC, R4/R14=8EEF9C00, SP=FFFEBC.
Test zero/high-bit/alias arguments and aligned/unaligned/overlapping frames.
Keep the unknown child explicit missing. The subsequent conditional copy and
pointer store remain unexecuted. Preserve 321 prior named captures and 230 device
checks. One classification batch used; execute before deeper review.
See `next-startup-preflight.json`.

G1 remains unresolved. Latest fresh source audit is
`bootstrap-input-audit-20261007-movie-table-fourth.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139. Katana
and supported PAL GDI remain absent; Apple media build is unproved. Exact
source-only request remains current and unsent in `BOOTSTRAP-BLOCKERS.md`.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-work-followup-startup/`,
`20261007-startup-movie-work-followup/`, `20261007-movie-work-followup-storage/`.

**Previous bounded recovery — 7 October 2026: table-copy wrapper returns to initialization, R3/C8 partial.**

Original EDC2F4 completes its two retained word copies and returns to EDC0CE.
**164 helper, 164 connected and nine activation controls pass**, including signed
offsets, aliases, unaligned/overlapping buffers, pointer-slot collisions and a first
copy that changes the later offset/callee and a relocated local frame. Captured
output is 192 bytes at EF9650..EF9710, two pointer updates and saved-frame/R12
stores, with three PREF calls. Complete RAM matches the pre-execution prediction.
The child returns in 181 cycles. The wrapper restores PR/SP and returns to
**ED80F6 at 185 cycles**. Its retained four-cycle prefix then reaches missing
**EDA920 at 189 cycles**, passing R14=8EEF9560 in R4.

Full startup proves the complete EDC0BA wrapper from entry through all four copy
helpers to return: **1,407 cycles**, with original PR/SP/R14 preserved. This is the
table wrapper's return; the surrounding initializer remains incomplete.
**320 state and 228 device comparisons pass**; all 316 prior named captures,
provider return, continuation prefix and 191 mode returns are unchanged. Both
baseline scenes, ten audits and six reference images pass with the same ARM64
binary. Closure: 999 functions (859 base plus 140 supplemental), 8,171 supplemental
instructions, 915 managed entries. Runtime, compiler and twelve patches unchanged.

Checkpoint `movie-table-fourth-startup-checkpoint.json`:
`92ec3a6daa94b4105d0ec08c40ab6d4337636f14d37f25bd47681977d3a3f163`; 113,431 identities,
111,005 inherited by verified digest,
5,026 freshly hashed (including 2,600 inherited).
Preserved storage inventory: 15,507 paths, 466 new and
215 previously shared paths reverified. Extending the bounded
cross-unit/full-capture RAM groups recovered 459,223,040 bytes while preserving
all bytes, paths and recorded metadata. Available space: 2,086,768,640
bytes. The 2,816 MiB capture reserve must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDA920 (52 instructions, eight blocks, two retained ED6708 calls).
It runs 64 ordered byte-permutation iterations, then conditional 16-word and
8-word copies. Predict 128 permutation bytes at EFBA34..EFBAB4 and 96 destination
bytes at EF9560..EF95C0, preserved r10/r11/r12/r14 and one PREF call. Test zero and
wrapping argument branches, signed byte indices, live literals, aliases, overlap
and frame reuse. Derive exact timing with the independent model before execution.
The captured branch should return to ED80FC; its five-cycle prefix should reach
missing EDA9A4 with R4=8EEF9C00. These are unexecuted predictions. Preserve 319
prior named captures and 228 device checks. One classification batch used;
execute before deeper review. See `next-startup-preflight.json`.

**Fresh G1 audit:** `bootstrap-input-audit-20261007-movie-table-fourth.json` verifies
runtime 0/1, generated 0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg
10139/10139. Runtime checkout and supported PAL GDI remain absent; Apple media
build remains unproved. Exact source-only acquisition request remains unchanged
and unsent in `BOOTSTRAP-BLOCKERS.md`. G1, normal boot, playback, gameplay,
audio/save/mobile/device acceptance and production foundation remain open.
Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-table-fourth-startup/`,
`20261007-startup-movie-table-fourth/`, `20261007-movie-table-fourth-storage/`.

**Previous bounded recovery — 7 October 2026: third table-copy helper returns, R3/C8 partial.**

Original EDC256 (30 instructions) completes retained 64-word and 16-word copies
and returns to EDC0CA. **164 helper, 164 connected and nine activation controls
pass**. Tests cover signed offset boundaries, aliases, unaligned/overlapping
buffers, safe stack-adjacent output and pointer-slot collision. A controlled first
copy changes a later callee alias and relocated local cursor/frame; the second
call consumes those live values without overwriting executing instructions.
Captured execution copies 320 bytes at EF9710..EF9850, updates two pointers and
preserves PR/SP/R14 with five PREF calls. Complete RAM matches the pre-execution
prediction without correction, including temporary R12 and saved-frame stores.
Return takes 257 cycles; the retained caller reaches missing **EDC2F4 at 260 cycles**,
passing R0=8EEF9710 in R4.

Full startup passes **317 state and 225 device comparisons**. All 314 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 998 functions (859 base plus 139 supplemental), 8,141 supplemental
instructions and 912 managed entries. Runtime, compiler and twelve patches are
unchanged. Full wrapper/initialization, playback, boot and gameplay remain unproved.

Checkpoint `movie-table-third-startup-checkpoint.json`:
`ef26509768c600a81e8bd61d3cc5dae57612a89bc5cbbaa12dbc274d7204c6ca`; 111,005 identities,
108,614 inherited by verified digest,
4,564 freshly hashed (including 2,173 inherited).
Preservation inventory: 15,041 paths, 199 new and
48 previously shared paths reverified.
Identical unit expectations and full startup RAM captures now share APFS extents
across matching groups; this distinct cross-family pass recovered 384,360,448 bytes.
It preserves all bytes, paths and recorded metadata, including previously shared
paths. The earlier startup-only pass remains a no-gain result, not a repeat remedy.
Available space: 2,541,735,936 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDC2F4 (30 instructions, three blocks, two retained ED6708 calls)
and the EDC0CE wrapper return. Predict 16-word and 32-word copies totaling 192 bytes
at EF9650..EF9710, two pointer updates, saved-frame/R12 stores and three PREF calls.
Predicted child return: 181 cycles, R0=8EEF9650, PC/PR=EDC0CE, SP=FFFEC0. The wrapper
should restore PR/SP and return to ED80F6 at 185 cycles with SP=FFFEC4. Observe that
boundary before extending the surrounding initializer; no later missing target is
assumed. These are unexecuted predictions. Preserve 316 prior named captures and
225 device checks. One classification batch used; execute before deeper review.
See `next-startup-preflight.json`.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated 0/1474,
headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139. Katana and supported
PAL inputs remain absent; Apple media build is unproved. The exact source-only
request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent. Full boot, gameplay,
audio/save/mobile/device acceptance and production foundation remain open.
Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-table-third-startup/`,
`20261007-startup-movie-table-third/`, `20261007-movie-table-third-storage/`.

**Previous bounded recovery — 7 October 2026: second table-copy helper returns, R3/C8 partial.**

Original EDC218 (31 instructions) completes two retained ED6708 word copies and
returns to EDC0C6. **164 helper, 164 connected and nine activation controls pass**.
Coverage includes signed offset boundaries, aliases, unaligned/overlapping buffers,
stack-adjacent destinations and colliding pointer slots. A controlled first copy
changes both the signed-offset literal and callee alias before the second call;
the original code and independent model both consume the new values. Captured
execution copies 256 bytes at EF9850..EF9950, updates two pointers, and preserves
PR/SP/R14 with four PREF calls. Its complete RAM matches the pre-execution
prediction without corrections, including temporary R12 and saved-frame stores.
Return takes 220 cycles; the retained caller reaches missing **EDC256 at 223
cycles**, passing R0=8EEF9850 in R4.

Full startup passes **315 state and 223 device comparisons**. All 312 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 997 functions (859 base plus 138 supplemental), 8,111
supplemental instructions and 909 managed entries. Runtime, compiler and twelve
patches are unchanged. Full wrapper/initialization, playback, boot and gameplay
remain unproved.

Checkpoint `movie-table-second-startup-checkpoint.json`:
`cd8baa5af7966d3d7f5d5ac5fbc9120061ed74734d46a458b34ca5eb724a667e`; 108,614 identities,
106,237 inherited by verified digest,
5,391 freshly hashed (including 3,014 inherited).
Preservation inventory: 14,842 paths, 1,117 new and
0 previously shared paths reverified. Exact duplicate
captures, archived emitter snapshots, compiled objects and generated source/header
copies share APFS extents; all contents, paths and recorded metadata are preserved.
The bounded 512-pair startup-RAM sharing pass yielded no net capacity gain and
must not be repeated as a capacity remedy without new evidence.
Available space: 2,488,803,328 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDC256 (30 instructions, three blocks, two calls) with retained
ED6708 and EDC0CA. Predict 64-word and 16-word copies totaling 320 bytes at
EF9710..EF9850, two pointer updates and saved PR/local-cursor/temporary-R12 stores.
Test live signed first decrement, fixed second decrement, repeated callee loads,
source/slot overlap, aliases and unaligned output. Predicted return is 257 cycles,
R0=8EEF9710, PR=EDC0CA and restored SP=FFFEC0, with five PREF calls. The caller
should pass R0 in R4 and reach missing EDC2F4 at 260 cycles. These are unexecuted
predictions. Preserve 314 prior named captures and 223 device checks. One
classification batch used; execute before deeper review. See `next-startup-preflight.json`.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139.
Katana and supported PAL inputs remain absent; Apple media build is unproved.
The exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-table-second-startup/`,
`20261007-startup-movie-table-second/`, `20261007-movie-table-second-storage/`.

**Previous bounded recovery — 7 October 2026: seven-copy table helper returns, R3/C8 partial.**

Original EDC178 (80 instructions) completes seven calls to retained ED6708 and
returns to EDC0C2. **160 helper, 160 connected and nine activation controls pass**.
The independent model covers ordered copies, live count/source/callee literals,
pointer slots, local cursor, saved PR/R14 and the retained copy's temporary R12
push. Tests cover count boundaries, aliases, unaligned/overlapping copies, safe
stack destinations and colliding pointer slots. Captured output is 688 bytes at
EF9950..EF9C00, seven pointer updates and four stack words. Before execution, the
prospective RAM was corrected to include the omitted R12 word at FFFEB0; the
original checkpoint is preserved. Corrected complete RAM matches. A retained audit assertion was also scoped to its
earlier first-table capture, because this helper legitimately reuses that stack
slot; its aggregate PREF assertion now includes the eight predicted new calls.
Both failed audits and their corrections are preserved. The helper
returns in 731 cycles with eight PREF calls; its retained wrapper reaches missing
**EDC218 at 734 cycles**, passing returned R0=8EEF9950 in R4. PR/R14/SP restore.

Full startup passes **313 state and 221 device comparisons**. All 310 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 996 functions (859 base plus 137 supplemental), 8,080
supplemental instructions and 906 managed entries. Runtime, compiler and twelve
patches are unchanged. Full wrapper/initialization, playback, boot and gameplay
remain unproved.

Checkpoint `movie-table-first-startup-checkpoint.json`:
`210513e2e38d935ca5ba27e556d4aa12f392cf3b0a5e0d8be744db13b955630a`; 106,237 identities,
103,885 inherited by verified digest,
4,488 freshly hashed (including 2,136 inherited).
Preservation inventory: 13,725 paths, 222 new and
0 previously shared paths reverified. Exact duplicate
captures and archived emitter-source snapshots share APFS extents with all bytes,
paths and recorded metadata preserved. Available space: 2,554,200,064 bytes.
The 2,816 MiB capture reserve must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDC218 (31 instructions, three blocks, two calls) with retained
ED6708 and EDC0C6. Predict two 32-word copies totaling 256 bytes at EF9850..EF9950,
two pointer updates and saved PR/local-cursor/temporary-R12 stack stores. Check
the signed decrement read twice, repeated live callee loads, source/slot overlap,
aliases, unaligned output and four PREF calls. Predicted return is 220 cycles,
R0=8EEF9850, PR=EDC0C6 and restored SP=FFFEC0. The caller should pass R0 in R4
and reach missing EDC256 at 223 cycles. These are unexecuted predictions.
Preserve 312 prior named captures and 221 device checks. One classification batch
used; execute before deeper review. See `next-startup-preflight.json`.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139.
Katana and supported PAL inputs remain absent; Apple media build is unproved.
The exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-table-first-startup/`,
`20261007-startup-movie-table-first/`, `20261007-movie-table-first-storage/`.

**Previous bounded recovery — 7 October 2026: table-tail wrapper entry reaches its first copy helper, R3/C8 partial.**

Original EDC0BA entry executes through its first BSR. **240 prefix and nine
activation controls pass**. The independent model verifies PR is pushed before
the signed halfword offset is loaded, then R4 is adjusted in the BSR delay slot.
Coverage includes zero/high-bit/wrapping arguments, positive/negative offsets,
aliases, unaligned stack and stack/literal overlap. Complete RAM/CPU/timer/device
state matches. The captured prefix reaches missing **EDC178 after five cycles**,
with R3=5B0, R4=8EEF9C00, PR=EDC0C2 and SP=FFFEC0. Its four-byte stack store
rewrites the existing ED80F6 value, so captured RAM is unchanged. The complete
13-instruction wrapper has four original calls; only its entry prefix is proved.

Full startup passes **311 state and 219 device comparisons**. All 309 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 995 functions (859 base plus 136 supplemental), 8,000
supplemental instructions and 898 managed entries. Runtime, compiler and twelve
patches are unchanged. Complete initialization, playback, normal boot and gameplay
remain unproved.

Checkpoint `movie-table-tail-startup-checkpoint.json`:
`2de81668390abf832e845d546e9ccf126c10d0c6566615d6077611d4d133ded3`; 103,885 identities,
101,563 inherited by verified digest,
4,309 freshly hashed (including 1,987 inherited).
Preservation inventory: 13,503 paths, 86 new and
0 previously shared paths reverified. Exact duplicate
captures and generated-source artifact copies share APFS extents; all bytes,
paths and recorded metadata are preserved. One generated-code preplan rejected
different filenames for equal content before replacing files; its zero-change
failure and corrected family/path validation are preserved separately.
Available space: 2,647,629,824 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDC178 (80 instructions, eight blocks, seven calls) with the
retained ED6708 word copy and EDC0C2 continuation. Source predicts seven copies
of 128,4,8,8,8,8,8 words, covering 688 bytes at EF9950..EF9C00, seven pointer-slot
updates and saved PR/R14/local-cursor stack writes. Predicted return is 731 cycles,
R0=8EEF9950, PR=EDC0C2, restored R14=8EEF9560 and SP=FFFEC0, with eight PREF
calls. The retained wrapper then passes R0 in R4 and reaches missing EDC218 at
734 cycles. These are unexecuted predictions. Test live counts/sources/callee,
aliases, unaligned and safe overlap, whole state and exact RAM footprint.
Preserve 310 prior named captures and 219 device checks. One classification batch
used; execute before deeper review. See `next-startup-preflight.json`.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139.
Katana and supported PAL inputs remain absent; Apple media build is unproved.
The exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-table-tail-startup/`,
`20261007-startup-movie-table-tail/`, `20261007-movie-table-tail-storage/`.

**Previous bounded recovery — 7 October 2026: table pointers installed and parent branches verified, R3/C8 partial.**

Original EDC040 (61 instructions) installs twenty pointers in 80 bytes at
EFBEB8..EFBF08 and returns to EDAB4C. **112 installer, 224 connected and nine
activation controls pass**. The independent model preserves the interleaved
loads before ordered stores and the final RTS-delay store. Controls cover aliases,
unaligned/stack/colliding outputs, signed/high-bit values, future-value overlap
and a controlled future-destination overwrite. Full RAM and CPU/device state match.
The child returns in 62 cycles. Both parent branches pass: 112 zero-argument
cases return in 70 cycles, and 112 nonzero cases restore PR/SP before the tail
JMP. The captured nonzero path restores R4=8EEF9650, PR=ED80F6 and SP=FFFEC4,
then reaches missing **EDC0BA at 71 cycles**. No stack writes occur in this pair.

Full startup passes **310 state and 218 device comparisons**. All 307 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 994 functions (859 base plus 135 supplemental), 7,987
supplemental instructions and 893 managed entries. Runtime, compiler and twelve
patches are unchanged. The captured nonzero initializer is still incomplete;
playback, normal boot and gameplay remain unproved.

Checkpoint `movie-eleventh-table-startup-checkpoint.json`:
`12472f1c4c38cc1976e58ef35271a8b786264f07ef843d35448d3b36676d6520`; 101,563 identities,
99,253 inherited by verified digest,
4,278 freshly hashed (including 1,968 inherited).
Preservation inventory: 13,417 paths, 56 new and
26 previously shared paths reverified. Only exact duplicate
captures were shared; contents and recorded metadata are preserved.
Available space: 2,542,534,656 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDC0BA (13 instructions, five blocks, four original calls).
Its entry saves PR, loads signed halfword 5B0 and adds it to R4 in the BSR delay
slot. Predict the first missing call EDC178 after five cycles, R3=5B0,
R4=8EEF9C00, PR=EDC0C2 and SP=FFFEC0. The saved PR store writes ED80F6 to
FFFEC0, whose captured bytes already equal that value. Test signed live offsets,
wrapping argument arithmetic, aliases and safe stack/literal overlap. Keep all
four callees explicit missing; complete-wrapper execution remains unproved.
Preserve 309 prior named captures and 218 device checks. One classification batch
used; execute before deeper review. See `next-startup-preflight.json`.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139.
Katana and supported PAL inputs remain absent; Apple media build is unproved.
The exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-eleventh-table-startup/`,
`20261007-startup-movie-eleventh-table/`, `20261007-movie-eleventh-table-storage/`.

**Previous bounded recovery — 7 October 2026: tenth table builder returns, R3/C8 partial.**

Original EDBEA2 (159 instructions) builds its 512-byte word table and returns to
EDAB46. All **80 table, 80 connected and nine activation controls pass**. The
independent model preserves ordered live literal loads, sixteen TST loops, eight
individual stores and the decrement-derived word. Controls cover aliases,
unaligned/caller-stack output, safe data-literal overlap, high-bit/zero/all-one
constants and the live indirect callee. Source-derived bytes and complete RAM
match, with no stack writes. Return takes 712 cycles; the retained caller reaches
missing **EDC040 at 716 cycles**. Final R4=EFCD08.

Full startup passes **308 state and 216 device comparisons**. All 305 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 993 functions (859 base plus 134 supplemental), 7,926
supplemental instructions and 892 managed entries. Runtime, compiler and twelve
patches are unchanged. Complete initialization, playback, normal boot and gameplay
remain unproved.

Checkpoint `movie-tenth-table-startup-checkpoint.json`:
`f0d5613f564f0aa00d2cd081467575cfd48bd058661c3d202970e11bbec04f38`; 99,253 identities,
96,960 inherited by verified digest,
4,219 freshly hashed (including 1,926 inherited).
Preservation inventory: 13,361 paths, 55 new and
1 previously shared paths reverified. Only exact duplicate
captures were shared; contents and recorded metadata are preserved.
Available space: 2,653,319,168 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDC040 (61 instructions, one block, no calls), which installs
twenty pointers at EFBEB8..EFBF08. Preserve interleaved live source/destination
loads, ordered stores and the final RTS-delay store. Test aliases, unaligned/stack
destinations, collisions and safe source overlap. The 80-byte output and 62-cycle
return to EDAB4C are unexecuted predictions. The retained parent then tests its
saved argument: zero should return after 70 cycles; the captured nonzero case
should restore R4=8EEF9650, PR=ED80F6 and SP=FFFEC4, then reach missing EDC0BA
via the original JMP after 71 cycles. Test both branches. Preserve 307 prior named
captures and 216 device checks. One classification batch used; see
`next-startup-preflight.json` for the exact execution contract.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139.
Katana and supported PAL inputs remain absent; Apple media build is unproved.
The exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-tenth-table-startup/`,
`20261007-startup-movie-tenth-table/`, `20261007-movie-tenth-table-storage/`.

**Previous bounded recovery — 7 October 2026: ninth table builder returns, R3/C8 partial.**

Original EDBE14 (65 instructions) builds its 128-byte table and returns to EDAB40.
All **16 table, 16 connected and nine activation controls pass**. Independent
checks cover TST-before-loop behavior, signed immediates, the final return-delay
store without pointer increment, aliases, unaligned/caller-stack output and live
output/callee literals. Complete RAM matches the source-derived pattern, with no
stack writes. Return takes 675 cycles; the retained caller reaches missing
**EDBEA2 at 679 cycles**. Final R4 points at the last byte, EFCB07.

Full startup passes **306 state and 214 device comparisons**. All 303 prior named
captures, provider return, continuation prefix and 191 mode returns are unchanged.
Both baseline scenes, ten audits and six reference images pass with the exact
ARM64 binary. Closure: 992 functions (859 base plus 133 supplemental), 7,767
supplemental instructions and 843 managed entries. Runtime, compiler and twelve
patches are unchanged. Complete initialization, playback, normal boot and gameplay
remain unproved.

Checkpoint `movie-ninth-table-startup-checkpoint.json`:
`48b3c1f4d19e605e3193cd99299a046f687ee1fd6d714c47383634e70afa5225`; 96,960 identities,
94,685 inherited by verified digest,
4,221 freshly hashed (including 1,946 inherited).
Preservation inventory: 13,306 paths, 72 new and
20 previously shared paths reverified. Only exact duplicate
captures were shared; contents and recorded metadata are preserved.
Available space: 2,768,076,800 bytes. The 2,816 MiB capture reserve
must be restored before the next full capture.
No game or Simulator remains live.

**Next:** execute EDBEA2 (159 instructions, 49 blocks, no calls) and retained
EDAB46. Predict 512 bytes at EFCB08 from sixteen repeated word groups and eight
individual stores. Check live literal ordering, the decrement-derived word,
aliases, unaligned/caller-stack output and safe overlap. The 712-cycle child
return and 716-cycle next call are unexecuted predictions. Expected R4=EFCD08,
R0=80108, R5=0, R6=40101, R7=50102, T=1, with no stack writes. The next caller
loads ABDC into R2 and calls missing EDC040. Preserve 305 prior named captures
and 214 device checks. One classification batch used; see `next-startup-preflight.json`.

G1 remains unresolved. Latest declared-path/byte audit is still
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139.
Katana and supported PAL inputs remain absent; Apple media build is unproved.
The exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current and unsent.
Full boot, gameplay, audio/save/mobile/device acceptance and production foundation
remain open. Previous/current goal turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-ninth-table-startup/`,
`20261007-startup-movie-ninth-table/`, `20261007-movie-ninth-table-storage/`.

**Previous bounded recovery — 7 October 2026: eighth table builder returns, R3/C8 partial.**

Original EDBD9C (60 instructions) builds its 128-byte table and returns to EDAB3A.
All **20 table, 20 connected and nine activation controls pass**. The independent
model covers nine DT/decrement/store loops and branch-delay pointer increments,
including the final signed immediate. Controls include aliases, unaligned output,
a caller-stack destination, output that overwrites the already-loaded pointer
literal, and a live indirect-callee literal. The source-derived table and complete
RAM footprint match, with no stack writes. The child returns after 537 cycles;
its original caller loads R3 from ABD4 and reaches missing **EDBE14 at 541 cycles**.

Full startup passes **304 state and 212 device comparisons**. All 301 earlier
named captures, provider return, continuation prefix and 191 mode returns remain
unchanged. Both supplied scenes, ten audits and six reference images pass with
the exact ARM64 executable. The linked inventory is 991 functions (859 base plus
132 supplemental), 7,702 supplemental instructions and 821 managed entries.
Runtime, compiler and twelve patches are unchanged. The enclosing initializer,
movie playback, normal boot and gameplay remain unproved.

Checkpoint: `movie-eighth-table-startup-checkpoint.json`, SHA-256
`f4bb7627e8f533e85b5aae572146aee58b51502667ee0c09c677350acfa7ab72`. It binds 94,685 identities,
inheriting 92,422 through the verified parent digest and freshly
hashing 4,212, including 1,949 inherited identities.
Preservation inventory: 13,234 paths; 81 new and
43 previously shared paths reverified. Exact duplicate
unit and startup captures now share APFS extents; all contents and recorded file
metadata are preserved. Available space is 2,662,424,576 bytes. The 2,816 MiB
capture reserve must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDBE14 (65 instructions, 22 blocks, no calls) with the retained
EDAB40 continuation. Seven byte-fill loops and two final stores predict 128 bytes
at EFCA88. The final byte is written in the RTS delay slot without advancing R4;
expected R4=EFCB07, R5=FFFFFF88, R6=66, R7=2, R0=22 and T=1. Verify signed
immediates, live output/callee literals, aliases, unaligned/safe stack destinations
and complete state. The recorded table and 675-cycle return/679-cycle next call
are unexecuted predictions. The caller next loads ABD8 into R3 and calls missing
EDBEA2. Preserve 303 earlier named captures and 212 device checks. One
classification batch used; `next-startup-preflight.json` is the next contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139. The
Katana checkout and supported PAL descriptor remain absent; Apple media build
remains unproved. The exact source-only request in `BOOTSTRAP-BLOCKERS.md` is
unchanged. Full initialization, boot, gameplay, media/save/mobile/device acceptance
and the production foundation remain open. Previous/current turns: progress.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-eighth-table-startup/`,
`20261007-startup-movie-eighth-table/` and `20261007-movie-eighth-table-storage/`.

**Previous bounded recovery — 7 October 2026: seventh table builder returns, R3/C8 partial.**

Original EDB486 (1,092 instructions) builds its 1,024-byte table and returns to
EDAB34. All **80 table, 80 connected and nine activation controls pass**. The
independent model covers the 12-byte stack frame, ordered pointer/counter/value
reloads, signed halfwords, and the initial loop plus 63 repeated groups. Controls
include aliases, unaligned output, a safe caller-stack destination, late-literal
overlap, and a live indirect-callee literal. The source-derived table and complete
RAM footprint match. The child returns after 6,726 cycles. Its original caller
loads R2 from ABD0 and reaches missing **EDBD9C after 6,730 cycles**.

Full startup passes **302 state and 210 device comparisons**. All 299 earlier
named captures, provider return, continuation prefix and 191 mode returns remain
unchanged. Both supplied scenes, ten audits and six reference images pass with
the exact ARM64 executable. The linked inventory is 990 functions (859 base plus
131 supplemental), 7,642 supplemental instructions and 802 managed entries.
Runtime, compiler and twelve patches are unchanged. The enclosing initializer,
movie playback, normal boot and gameplay remain unproved.

Checkpoint: `movie-seventh-table-startup-checkpoint.json`, SHA-256
`574369b52efadd7e2e6783944ebbcd02844876bddcd1e933ef9aa9560ef4f090`. It binds 92,422 identities,
inheriting 90,180 through the verified parent digest and freshly
hashing 4,148, including 1,906 inherited identities.
Preservation inventory: 13,153 paths; 75 new and
20 previously shared paths reverified. Exact duplicate
unit and startup captures now share APFS extents; all contents and recorded file
metadata are preserved. Available space is 2,665,238,528 bytes. The 2,816 MiB
capture reserve must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDBD9C (60 instructions, 19 blocks, no calls) with the retained
EDAB3A continuation. Its nine DT/store/branch-delay loops predict 128 bytes at
EFCA08, final R4=EFCA88, R5=0, R6=FFFFFF87, R7=2 and T=1, with no stack writes.
Independently verify decrement/T behavior, delay-slot pointer increments and the
signed final immediate. The recorded table and 537-cycle return/541-cycle next
call are unexecuted predictions. Test aliases, unaligned/safe stack destinations
and live output/callee literals, then expose the next indirect call through ABD4
to missing EDBE14. Preserve 301 earlier named captures and 210 device checks.
One classification batch used; `next-startup-preflight.json` is the next contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json`: runtime 0/1, generated
0/1474, headers 0/177, contracts 0/4, title 38/39 and FFmpeg 10139/10139. The
Katana checkout and supported PAL descriptor remain absent; Apple media build
remains unproved. The exact source-only request in `BOOTSTRAP-BLOCKERS.md` is
unchanged. Full initialization, boot, gameplay, media/save/mobile/device acceptance
and the production foundation remain open. Previous/current turns: progress.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-seventh-table-startup/`,
`20261007-startup-movie-seventh-table/` and `20261007-movie-seventh-table-storage/`.

**Previous bounded recovery — 7 October 2026: sixth table builder returns, R3/C8 partial.**

Original EDB312 (155 instructions) builds complete256-byte and64-byte tables and
returns through the retained EDAB30 parent continuation. **80 table,80 connected
and nine activation controls pass**. Coverage includes address aliases, unaligned output,
overlapping tables, saved-stack overlap, signed high-bit literals,
positive/negative byte extension and nested counters and complete CPU/RAM/timer/store-queue state. Independent320-byte
source patterns and the exact RAM write footprint match. The child returns in
971cycles; final R4=EFC608 is one past the second table; R11/R12/R14 restore.
The actual parent next BSR reaches missing **EDB486 at974cycles**. Remaining
parent calls and complete parent return are unproved.

Full startup confirms those effects: **300 state and208 device comparisons pass**.
All297 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is989 functions (859 base +130
supplemental),6550 supplemental instructions and609 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback, normal boot and gameplay remain unproved.

Checkpoint binds 90,180 identities; SHA-256
`2e99afcfe6b44bf667cb9d1d282ac6ac9329fe778b8c53a52fb99a5ac6481499`. The parent digest seals 87,957
historical identities; 4,101 current identities were hashed,
including 1,878 inherited identities. Unrelated history was
not reread. Preservation inventory has 13,078 paths, with
77 newly shared paths verified. Exact duplicate baseline,
passing-unit RAM and recent table captures share APFS storage without content removal.
Available space is2,644,443,136bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDB486 (1092 instructions,193 blocks,no calls) with the retained
EDAB34 continuation. Source predicts a1024-byte table at EFC608, built from two
zero words and63 repeated literal groups using a12-byte stack frame. Independently
model ordered stack pointer/counter/value reloads and signed halfwords; derive safe
overlap cases rather than assuming local-stack aliasing is harmless. The recorded
1024-byte pattern and6726-cycle return/6730-cycle next-call timing are unexecuted
predictions. The parent next loads live ABD0 into R2 and calls missing EDBD9C.
Preserve299 earlier named captures and208 device checks. One classification batch
used; `next-startup-preflight.json` records the next execution contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json` (runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139). Katana checkout and
supported PAL descriptor remain absent; Apple media build remains unproved.
The source-only request in `BOOTSTRAP-BLOCKERS.md` is unchanged. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-sixth-table-startup/`
(`movie-sixth-table-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-sixth-table/` and `20261007-movie-sixth-table-storage/`.

**Previous bounded recovery — 7 October 2026: fifth table builder returns, R3/C8 partial.**

Original EDB26C (83 instructions) builds a complete128-byte table and
returns through the retained EDAB2C parent continuation. **80 table,80 connected
and nine activation controls pass**. Coverage includes address aliases, unaligned output,
overlap with live literal reads, stack destinations, signed high-bit literals,
paired2/4/8/16-word loops and complete CPU/RAM/timer/store-queue state. Independent128-byte
source patterns and the exact RAM write footprint match. The child returns in
360cycles; final R4=EFC4C8 is one past the table, with no stack writes.
The actual parent next BSR reaches missing **EDB312 at363cycles**. Remaining
parent calls and complete parent return are unproved.

Full startup confirms those effects: **298 state and206 device comparisons pass**.
All295 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is988 functions (859 base +129
supplemental),6395 supplemental instructions and563 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback, normal boot and gameplay remain unproved.

Checkpoint binds 87,957 identities; SHA-256
`eb6a1e338e4fcde8498a6800448453b9fd9ee61391eaded94f009e7c2217c0fb`. The parent digest seals 85,756
historical identities; 4,051 current identities were hashed,
including 1,850 inherited identities. Unrelated history was
not reread. Preservation inventory has 13,001 paths, with
60 newly shared paths verified. Exact duplicate baseline
and passing-unit RAM captures share APFS storage without content removal.
Available space is2,735,603,712bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDB312 (155 instructions,46 blocks,no calls) with the retained
EDAB30 continuation. Source predicts256-byte and64-byte tables at EFC4C8/EFC5C8,
final R4=EFC608,R5=0,R6=100,R7=4,T=1 and restored R11/R12/R14/SP.
Independently model positive/negative byte extension, signed live masks, nested
repeat counters and ordered stores. The source-derived320-byte pattern and
971-cycle return/974-cycle next-call timing are unexecuted predictions. Test
aliases, unaligned/overlapping tables, saved-stack overlap and complete state;
then expose the original next BSR to missing EDB486. Preserve297 earlier named
captures and206 device checks. One classification batch used;
`next-startup-preflight.json` records the next execution contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json` (runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139). Katana checkout and
supported PAL descriptor remain absent; Apple media build remains unproved.
The source-only request in `BOOTSTRAP-BLOCKERS.md` is unchanged. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-fifth-table-startup/`
(`movie-fifth-table-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-fifth-table/` and `20261007-movie-fifth-table-storage/`.

**Previous bounded recovery — 7 October 2026: fourth table builder returns, R3/C8 partial.**

Original EDB1FC (39 instructions) builds a complete64-byte table and
returns through the retained EDAB28 parent continuation. **80 table,80 connected
and nine activation controls pass**. Coverage includes address aliases, unaligned output,
overlap with live literal reads, stack destinations, signed high-bit literals,
fixed4/8/16-word loops and complete CPU/RAM/timer/store-queue state. Independent64-byte
source patterns and the exact RAM write footprint match. The child returns in
171cycles; final R4=EFC448 is one past the table, with no stack writes.
The actual parent next BSR reaches missing **EDB26C at174cycles**. Remaining
parent calls and complete parent return are unproved.

Full startup confirms those effects: **296 state and204 device comparisons pass**.
All293 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is987 functions (859 base +128
supplemental),6312 supplemental instructions and538 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback, normal boot and gameplay remain unproved.

Checkpoint binds 85,756 identities; SHA-256
`967163edfbe50c045df1848144c6ba11d02c0aa0312080ed46075731060a375e`. The parent digest seals 83,574
historical identities; 4,017 current identities were hashed,
including 1,835 inherited identities. Unrelated history was
not reread. Preservation inventory has 12,941 paths, with
61 newly shared paths verified. Exact duplicate baseline
and passing-unit RAM captures share APFS storage without content removal.
Available space is2,765,520,896bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDB26C (83 instructions,25 blocks,no calls) with the retained
EDAB2C continuation. Source predicts a128-byte table at EFC448, final R4=EFC4C8,
R5=0,R6=16,R7=E02,T=1, unchanged SP and no stack stores. Independently model the
signed live literals, initial R7 preload and paired2/4/8/16 repeat counts.
The source-derived128-byte pattern and360-cycle return/363-cycle next-call timing
are unexecuted predictions. Test aliases, unaligned destinations, live-literal
overlap and complete state; then expose the original next BSR to missing EDB312.
Preserve295 earlier named captures and204 device checks. One classification batch
used; `next-startup-preflight.json` records the next execution contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json` (runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139). Katana checkout and
supported PAL descriptor remain absent; Apple media build remains unproved.
The source-only request in `BOOTSTRAP-BLOCKERS.md` is unchanged. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-fourth-table-startup/`
(`movie-fourth-table-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-fourth-table/` and `20261007-movie-fourth-table-storage/`.

**Previous bounded recovery — 7 October 2026: third table builder returns, R3/C8 partial.**

Original EDAFB6 (254 instructions) builds complete256-byte and64-byte tables and
returns through the retained EDAB24 parent continuation. **80 table,80 connected
and nine activation controls pass**. Coverage includes aliases, unaligned and
overlapping tables, saved-stack overlap, signed high-bit literals, shortened
loop ranges and complete CPU/RAM/timer/store-queue state. Independent320-byte
source patterns and the exact RAM write footprint match. The child returns in
730cycles; its last store leaves R4=EFC406, two bytes before the buffer end.
The actual parent next BSR reaches missing **EDB1FC at733cycles**. Remaining
parent calls and complete parent return are unproved.

Full startup confirms those effects: **294 state and202 device comparisons pass**.
All291 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is986 functions (859 base +127
supplemental),6273 supplemental instructions and528 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback, normal boot and gameplay remain unproved.

Checkpoint binds 83,574 identities; SHA-256
`2ba7b8c445b1c7e91f7989e75f77c00d42401d1a1da3e5d4c2926bbd4b3ae03f`. The parent digest seals 81,410
historical identities; 3,985 current identities were hashed,
including 1,821 inherited identities. Unrelated history was
not reread. Preservation inventory has 12,880 paths, with
63 newly shared paths verified. Exact duplicate baseline
and passing-unit RAM captures share APFS storage without content removal.
Available space is2,776,596,480bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDB1FC (39 instructions,10 blocks,no calls) with the retained
EDAB28 continuation. Source predicts a64-byte table at EFC408, final R4=EFC448,
R5=0,R6=A01,T=1, unchanged SP and no stack stores. Independently model the live
signed halfword literals and fixed4/8/16-word loops, including initial-load
ordering and overlap with later literal reads. The source-derived64-byte pattern
and171-cycle return/174-cycle next-call timing are unexecuted predictions.
Test aliases, unaligned destinations, live-literal overlap and complete state;
then expose the original next BSR to missing EDB26C. Preserve293 earlier named
captures and202 device checks. One classification batch used;
`next-startup-preflight.json` records the next execution contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json` (runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139). Katana checkout and
supported PAL descriptor remain absent; Apple media build remains unproved.
The source-only request in `BOOTSTRAP-BLOCKERS.md` is unchanged. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous turn: status-only, no progress. Current turn: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-third-table-startup/`
(`movie-third-table-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-third-table/` and `20261007-movie-third-table-storage/`.

**Previous bounded recovery — 7 October 2026: second table builder returns, R3/C8 partial.**

Original EDADAC (229 instructions) builds complete256-byte and64-byte tables and
returns through the retained EDAB20 parent continuation. **80 table,80 connected
and nine activation controls pass**. Coverage includes aliases, unaligned and
overlapping tables, saved-stack overlap, signed high-bit literals, shortened
loop ranges and complete CPU/RAM/timer/store-queue state. Independent320-byte
source patterns and the exact RAM write footprint match. The child returns in
683cycles; its last store leaves R4=EFC2C6, two bytes before the buffer end.
The actual parent next BSR reaches missing **EDAFB6 at686cycles**. Remaining
parent calls and complete parent return are unproved.

Full startup confirms those effects: **292 state and200 device comparisons pass**.
All289 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is985 functions (859 base +126
supplemental),6019 supplemental instructions and501 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback, normal boot and gameplay remain unproved.

Checkpoint binds 81,410 identities; SHA-256
`93c30cf1cb90ddeaef8f1fa7ca8e9ce65235fc7a1a03c06456a31d7e4ec962e1`. The parent digest seals 79,264
historical identities; 3,952 current identities were hashed,
including 1,806 inherited identities. Unrelated history was
not reread. Preservation inventory has 12,817 paths, with
64 newly shared paths verified. Exact duplicate baseline
and passing-unit RAM captures share APFS storage without content removal.
Available space is2,733,756,416bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDAFB6 (254 instructions,27 blocks,no calls) with the retained
EDAB24 continuation. Source predicts256-byte and64-byte tables at EFC2C8/EFC3C8,
restored R10-R14 and final R4=EFC406. Independently derive the ordered stores,
high-bit signed masks, paired decreasing counters, computed final-loop bound
and exact timing. Test aliases, table/stack overlap and complete state, then
verify the original next BSR exposes missing EDB1FC. The320-byte source pattern
is recorded as an unexecuted prediction. Total timing remains to be derived.
One classification batch used. Preserve291 earlier named captures and200 device
checks; `next-startup-preflight.json` records the next execution contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json` (runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139). Katana checkout and
supported PAL descriptor remain absent; Apple media build remains unproved.
The source-only request in `BOOTSTRAP-BLOCKERS.md` is unchanged. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-second-table-startup/`
(`movie-second-table-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-second-table/` and `20261007-movie-second-table-storage/`.

**Previous bounded recovery — 7 October 2026: first table builder returns, R3/C8 partial.**

Original EDAB14 (40 instructions) executes its first-call prefix and EDAB72
(251 instructions) builds its complete512-byte and128-byte tables. **80 table,
80 connected and nine activation controls pass**. Coverage includes aliases,
unaligned destinations, overlapping tables and saved stack, signed live literals,
shortened loop ranges and complete CPU/RAM/timer/store-queue state. Independent
source-derived640-byte patterns and the exact RAM write footprint also match.
Parent entry reaches the child at5cycles; the child returns after2015cycles at
relative2020. The actual parent next BSR reaches missing **EDADAC at2023cycles**.
Remaining parent calls and complete parent return are unproved.

Full startup confirms those effects: **290 state and198 device comparisons pass**.
All286 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is984 functions (859 base +125
supplemental),5790 supplemental instructions and474 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback, normal boot and gameplay remain unproved.

Checkpoint binds 79,264 identities; SHA-256
`16c5255d51771326e2e15f09e518af9b0c453790691fa6150ab074eaca8f1294`. The parent digest seals 77,123
historical identities; 3,928 current identities were hashed,
including 1,787 inherited identities. Unrelated history was
not reread. Preservation inventory has 12,753 paths, with
66 newly shared paths verified. Exact duplicate baseline
and passing-unit RAM captures share APFS storage without content removal.
Available space is2,708,652,032bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDADAC (229 instructions,27 blocks,no calls) with the retained
EDAB20 parent continuation. Source predicts256-byte and64-byte tables at
EFC188/EFC288, restored R13/R14 and final R4=EFC2C6 (the final store does not
increment the pointer). Independently derive ordered halfword writes, signed
literal/loop behavior and timing; test aliases, table/stack overlap and complete
state, then verify the original next BSR reaches missing EDAFB6. The320-byte
source pattern is recorded as an unexecuted prediction. Total timing remains
to be derived. One classification batch used. Preserve289 earlier named captures
and198 device checks; `next-startup-preflight.json` records the next contract.

Original G1 remains unresolved. Latest declared-path/byte audit:
`bootstrap-input-audit-20261007-movie-work-reset.json` (runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139). Katana checkout and
supported PAL descriptor remain absent; Apple media build remains unproved.
The source-only request in `BOOTSTRAP-BLOCKERS.md` is unchanged. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-first-table-startup/`
(`movie-first-table-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-first-table/` and `20261007-movie-first-table-storage/`.

**Previous bounded recovery — 7 October 2026: five-word clear and reset return, R3/C8 partial.**

Original ED7E44 (eight instructions) and ED8074 (RTS/NOP) execute with the retained
ED80E8 caller continuation. **48 clear,32 reset,48 connected and nine activation
controls pass**. Coverage includes address aliases, unaligned destinations,
ordered overlap with live literals, signed/wrapped caller offsets, status/FPU
and complete CPU/RAM/timer/store-queue state. The clear returns at9cycles, reset
enters at13 and returns at16, and the caller reaches missing **EDAB14 at21cycles**.
The five words at EFB8F4 are zero; the next call receives R4=8EEF9650.

Full startup confirms those effects: **287 state and195 device comparisons pass**.
All282 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is982 functions (859 base +123
supplemental),5499 supplemental instructions and391 managed entries. Runtime,
compiler and twelve patches are unchanged. Complete movie initialization,
playback and normal boot remain unproved.

Checkpoint binds 77,123 identities; SHA-256
`c306fbd78228b8102da405778def51eb30452b75ab562cbbd137c2e99c845337`. The parent digest seals 74,998
historical identities; 3,890 current identities were hashed,
including 1,765 inherited identities. Unrelated history was
not reread. Preservation inventory has 12,687 paths, with
73 newly shared paths verified. Exact duplicate baseline
and passing-unit RAM captures share APFS storage without content removal. An
initial unit-sharing plan rejected a failed historical test before replacing any
file; the corrected plan filters candidates to passing tests before grouping.
Available space is2,712,141,824bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute EDAB14 (40 instructions,14 blocks,12 calls) through its first
child EDAB72 (251 instructions,69 blocks,no calls). The parent saves PR and R4;
the child builds512-byte and128-byte tables at EFBF08/EFC108 and restores its
saved registers. Source-derived table bytes are recorded as unexecuted predictions.
Derive independent ordered-write/register/timing expectations, exercise safe
literal variants, aliases and overlap, then verify the real return to EDAB1C
and next BSR to missing EDADAC. Do not fabricate remaining helper returns.
Two review batches used: execute before further classification. Preserve286
earlier named captures and195 device checks; `next-startup-preflight.json`
records the prospective contract. Total timing remains to be derived.

Fresh declared-path/byte audit `bootstrap-input-audit-20261007-movie-work-reset.json`
confirms runtime0/1, generated0/1474, headers0/177, contracts0/4 and title38/39.
All10139 FFmpeg source files match; Apple media build remains unproved. Katana
checkout and supported PAL descriptor are absent. **Original G1 remains unresolved**;
the exact source-only acquisition request in `BOOTSTRAP-BLOCKERS.md` is unchanged.
Full initialization, normal boot, gameplay, media/save/mobile/device acceptance
and the production foundation remain open. Previous/current turns: progress.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-work-reset-startup/`
(`movie-work-reset-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-work-reset/` and `20261007-movie-work-reset-storage/`.

**Previous bounded recovery — 7 October 2026: movie work configuration returns, R3/C8 partial.**

Original ED8206 (eight instructions) and ED8348 (229 instructions) execute with
retained ED67F8 fill, ED6708 word copy and actual ED80DA continuation. **64 pointer,
72 connected and nine activation controls pass**. Coverage includes pointer flags,
wrapped counts, live literals, ordered overlapping copies, address aliases,
status/FPU and complete CPU/RAM/timer/store-queue state. Captured execution adjusts
the pointer to8EEE72A0, clears81920bytes, copies two28-byte fields and eight words,
and stores aligned work8EEE8000/end8EEF8000/tail8EEF9560. Configuration returns at
23361cycles; the actual caller reaches missing **ED7E44 at23366cycles**.
The retained eight-word copy produces no new PREF calls.

Full startup confirms those effects: **283 state and191 device comparisons pass**.
All274 earlier named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Both supplied scenes, ten audits and six unchanged
images pass with the exact ARM64 binary. Closure is980 functions (859 base +121
supplemental),5489 supplemental instructions and389 managed entries. Runtime,
compiler and twelve patches are unchanged. This does not prove complete movie
initialization, playback or normal boot.

Checkpoint binds 74,998 identities; SHA-256
`29d05fbe08ea181f41a968c365f834f796db03b6aff470d538504f1e7d621274`. The parent digest seals 72,892
historical identities; 3,961 current identities were hashed,
including 1,855 inherited identities. Unrelated history was
not reread. Preservation inventory has 12,614 paths, with
202 newly shared paths verified. Available space is
2,605,019,136bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute original ED7E44 (five-word clear, eight instructions) and ED8074
(RTS/NOP, two instructions) through the retained ED80E8 continuation. Source
predicts clear return9cycles, reset entry13/return16 and missing EDAB14 at21.
Verify the final delay-slot store, live destination literal, aliases, ordered
overlap, status/FPU and unchanged device state. Predicted caller PC80EE/PR80F6,
R4=8EEF9650 and SPFEC4 remain unexecuted. Two review batches used: execute before
further classification. Preserve282 earlier named captures and191 device checks;
`next-startup-preflight.json` records the exact prospective contract.

Original G1 remains unresolved; the source-only acquisition request remains in
`BOOTSTRAP-BLOCKERS.md`. Latest source audit:
`bootstrap-input-audit-20261007-movie-third-validation.json`. Full initialization,
normal boot, gameplay, media/save/mobile/device acceptance and the production
foundation remain open. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-work-config-startup/`
(`movie-work-config-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-work-config/` and `20261007-movie-work-config-storage/`.

**Previous bounded recovery — 7 October 2026: validation returns and cache flags execute, R3/C8 partial.**

Original EDC394 (22 instructions) and ED81CC (21 instructions) execute with
retained EDC348, ED8164 and ED80B8 continuations. **96 flag, 96 pair, 32 connected
and nine activation controls pass**. Coverage includes all cache-bit combinations,
live signed literals, stack/flag/literal overlap, each pair failure, aliases,
status/FPU and complete state. The three-pair check returns at50cycles; validation
returns at57. Restored `cache_shadow=98d` drives exactly one original MMIO read,
with unchanged register backing, and stores0/1 at EFB968/EFB96C. The helper exits
at83; actual setup continuation reaches missing **ED8206 at86cycles**.
Runtime MMIO shadow verification does not establish physical cache behavior.

Full startup independently confirms validation return and flag writes: **275
state and183 device comparisons pass**. All263 prior named captures, provider
return, continuation prefix and191 mode returns remain unchanged. Both supplied
scenes, ten audits and six unchanged images pass with the exact ARM64 binary.
Closure is978 functions (859 base +119 supplemental),5,252 supplemental
instructions and382 managed entries. Runtime/compiler and twelve patches are
unchanged. Complete third callback and movie initialization remain unproved.

Checkpoint binds 72,892 identities; SHA-256
`1dbbfe94276a84617a6bea46a1862ce2c757d0240ececdb15ff99805c35d67a6`. Parent digest seals 70,817
historical identities; 3,784 current identities were hashed, including
1,709 inherited identities. Unrelated history was not reread.
Preservation inventory has 12,412 paths, with 104 newly shared paths verified.
Final available space: 2,586,607,616 bytes; the2816MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute original ED8206 (eight instructions) and ED8348 (229 instructions)
with retained ED67F8 fill, ED6708 word copy and actual ED80DA continuation. Source
predicts adjusted pointer8EEE72A0, an81920-byte clear, two ordered28-byte copies,
eight-word copy, aligned work8EEE8000/end8EEF8000/tail8EEF9560, descriptor stores
and setup return at23361cycles; then original missing ED7E44 at23366. Predictions
remain unexecuted. Verify SHLL/SHLD wrap, safe counts, aliases, ordered overlap,
saved locals/registers and original PREF effects. Two review batches used:
execute before further classification. Preserve274 named captures and183 device
comparisons; preflight JSON records exact prospective boundaries.

Original G1 source gaps and source-only request remain unresolved; latest source
audit is `bootstrap-input-audit-20261007-movie-third-validation.json`.
Full initialization, playback, normal boot, gameplay, audio/save/mobile/device
acceptance and production foundation remain open. Previous/current turns:
progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-cache-flags-startup/`
(`movie-cache-flags-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-cache-flags/` and `20261007-movie-cache-flags-storage/`.

**Previous bounded recovery — 7 October 2026: four-pair validation returns, R3/C8 partial.**

Original EDC35C (27 instructions) and EDC348 (nine instructions) execute.
**160 leaf, 120 pair, 20 connected and nine activation controls pass**, including
signed/wrapped differences, the FFFFFFF4 mask, each early-failure position,
stack/literal overlap, aliases, status/FPU and complete state. The captured
four-pair check returns zero in **65 cycles**. The actual retained ED8154
continuation executes without replaying the ED811A prefix and reaches missing
**EDC394 at 71 cycles**. Complete validation return remains unproved.

Full startup independently confirms the call chain: **264 state and 172 device
comparisons pass**. All 253 prior named captures, provider return, continuation
prefix and 191 mode returns are unchanged. Both supplied scenes, ten audits and
six unchanged images pass with the exact ARM64 binary. Closure is 976 functions
(859 base +117 supplemental), 5,209 supplemental instructions and 369 managed
entries. Runtime/compiler and twelve patches are unchanged.

Checkpoint binds 70,817 identities; SHA-256
`9b675afb688613c7b6e386a547e016c032283c85b5afe7891322dd2c2dd155d0`. Parent digest seals 68,831
historical identities; 3,640 current identities were hashed, including
1,654 inherited identities. Unrelated history was not reread.
Preservation inventory has 12,308 paths, with 94 newly shared paths verified.
Final available space: 2,540,826,624 bytes; the 2816 MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute original EDC394 (22 instructions) and ED81CC (21 instructions),
retaining the real EDC348 comparator and ED8164/ED80B8 continuations. Captured
pair differences2/2/0 pass the mask by source inspection. Source predicts a
50-cycle three-pair return, validation return at57, cache-mode helper entry63,
exit83 and missing ED8206 at86. These predictions remain unexecuted.
ED81CC reads FF00001C: restore captured `cache_shadow=98d` explicitly; verify
one original MMIO read, unchanged register backing and flag writes0/1 at
EFB968/EFB96C. Test all8000/80 bit combinations and ordered overlap. Runtime
MMIO shadow evidence is not physical-hardware cache validation. Two review
batches used: execute before further classification. Preserve263 named captures
and172 device comparisons. Details and runtime provenance are in preflight JSON.

Original G1 source gaps and the source-only request remain unresolved; the latest
source audit is `bootstrap-input-audit-20261007-movie-third-validation.json`.
Complete initialization, playback, normal boot, gameplay, audio/save/mobile/
device acceptance and production foundation remain open. Previous/current turns:
progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-pair-check-startup/`
(`movie-pair-check-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-pair-check/` and `20261007-movie-pair-check-storage/`.

**Previous bounded recovery — 7 October 2026: validation reaches second child, R3/C8 partial.**

Original ED811A (49 instructions) and EDAB10 (two instructions) execute.
Native tests pass **32 leaf, 64 validation and nine activation controls**, including
alignment/magic failures, signed literals, stack/literal overlap, aliases,
status/FPU and complete state. Routes cover 16 successful guard prefixes,
16 alignment errors and 32 magic errors. Captured boundaries are **16/19/25
cycles**: both guards pass, the original RTS/MOV0 leaf returns, and the next
literal/JSR reaches missing **EDC35C**. Complete validation return is unproved.

Full startup passes **254 state and 162 device comparisons**. All 250 earlier
named captures, provider return, continuation prefix and 191 mode returns remain
unchanged. Both supplied scenes, ten audits and six unchanged images pass with
the exact ARM64 binary. Closure is 974 functions (859 base +115 supplemental),
5,173 supplemental instructions and 356 managed entries. Runtime/compiler and
twelve patches are unchanged. The unit fixture was repaired to use dc_write16
for literal-cache invalidation; original generated bodies are byte-identical.
Two integration naming collisions were caught before execution and corrected.
Failed sources/output identities are preserved in the checkpoint.

Checkpoint binds 68,831 identities; SHA-256
`36c3e923d199a6c3bda27abe34b16d2d3f3cf002f197ff5075683fa43d59f459`. Parent digest seals 66,258
historical identities; 4,192 current identities were hashed, including
1,619 inherited identities. Unrelated history was not reread.
Preservation inventory has 12,214 paths, with 83 newly shared paths verified.
Final available space: 2,588,925,952 bytes; the 2816 MiB capture reserve
must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute original EDC35C (27 instructions) and EDC348 (nine instructions).
Four captured literal pairs match. Source predicts a 65-cycle return, then the
actual retained ED8154 continuation reaches missing EDC394 at 71 cycles. These
predictions remain unexecuted. Preserve signed subtraction/NEG wrap, the
FFFFFFF4 mask and MOVT delay slot; this helper is not plain equality. Test early
failure at each pair and actual continuation dispatch without prefix replay.
Two review batches used: execute before further classification. Preserve 253
prior named captures and 162 device comparisons.

Fresh G1 declared-path/byte audit still finds runtime0/1, generated0/1474,
headers0/177, contracts0/4, title38/39 and FFmpeg10139/10139; runtime checkout
and supported PAL GDI absent. Source-only request unchanged and unsent.
Complete initialization, playback, normal boot, gameplay, audio/save/mobile/
device acceptance and production foundation remain unresolved.
Previous and current turns: progress; goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-third-validation-startup-repair2/`
(`movie-third-validation-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-third-validation-repair1/`,
`20261007-movie-third-validation-storage/` and
`artifacts/bootstrap-input-audit-20261007-movie-third-validation.json`.

**Previous bounded recovery — 7 October 2026: third callback reaches validation helper, R3/C8 partial.**

Original ED256A (27 instructions) and ED80AC (55 instructions) now execute.
Native tests pass 36 callback, 36 setup and nine activation controls, checking
literal/stack overlap, aliases, status/FPU and complete RAM/CPU/TMU/SQ/device
state. Captured six/thirteen-cycle boundaries verify saved PR/R14, two local
arguments (8 and EE72A0) and the original BSR delay-slot store through missing
**ED811A**. No third-callback return or cold-branch success is claimed.

Full startup independently confirms the nested call chain: **251 state and 159
device comparisons pass**, with all 248 prior named captures unchanged. Provider
return, continuation prefix and 191 mode returns are retained. Both supplied
scenes, ten audits and six unchanged images pass with the same ARM64 binary.
Closure is 972 functions (859 base +113 supplemental), 5,122 supplemental
instructions and 339 managed entries. Runtime/compiler and twelve patches are
unchanged. A stale count assertion (111 instead of 113) stopped the first
preflight before compilation; its source/output identities are preserved.
The repaired run and all validation completed successfully.

Checkpoint binds 66,258 identities; SHA-256
`ea568de7ddc380ff190b706098ce14405359fe9a28d01a0fb47d9bea5b393a05`. Parent digest seals 64,088
historical identities; 3,770 current identities were hashed, including
1,600 inherited identities. Unrelated history was not reread.
Preservation inventory has 12,131 paths, with 85 newly shared paths verified.
Final available space: 2,739,138,560 bytes; restore the 2816 MiB capture
reserve before another full capture. No game or Simulator remains live.

**Next:** execute original ED811A (49 instructions) and EDAB10 (two instructions).
Captured alignment and magic-value guards pass by source inspection; predicted
16/19/25-cycle boundaries lead through the original zero-return leaf to missing
EDC35C. These predictions are unexecuted. Check error guards, live stack/literal
ordering and status/FPU without fabricated returns. Preserve ED80CE's original
self-loop. Two review batches used: execute before further classification.
Preserve 250 prior named captures and 159 device comparisons.

G1 Katana/generated/header/provider/PAL gaps and the source-only acquisition
request are unchanged. Full initialization, playback, normal boot, gameplay,
audio/save/mobile/device acceptance and production foundation remain unresolved.
Previous and current goal turns: progress; goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-third-setup-startup-repair1/`
(`movie-third-setup-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-third-setup/` and `20261007-movie-third-setup-storage/`.

**Previous bounded recovery — 7 October 2026: second interface callback returns, R3/C8 partial.**

Original ED6840 and ED7800 RTS/NOP bodies execute. Native tests pass 64 leaf,
36 connected and nine activation controls, checking unchanged RAM/registers,
status/FPU, aliases, saved return addresses and method-index scaling. Exact
retained ED78DC/ED1CB6/EDA2E6 continuations execute without replaying completed
prefixes. Relative boundaries **3/7/10/15/22/41 cycles** verify the setup wrapper
return, second callback return and binder advance to EDC60C -> EDC8BC -> missing
**ED256A**. Full startup independently confirms the connected call chain; these
returns are not inferred solely from the unit continuation driver.

Full startup passes **249 RAM/CPU/TMU and 157 SQ/device comparisons**. All 242
prior named captures, provider return, continuation prefix and 191 mode returns
are unchanged. Both supplied scenes, ten audits and six unchanged images pass
with the same ARM64 binary. Closure is 970 functions (859 base +111 supplemental),
5,040 supplemental instructions and 315 managed entries. Runtime/compiler and
twelve patches remain unchanged. Checkpoint binds 64,088 identities;
SHA-256 `ff07ebd23854d0e31d8907dab570e8744c7e847ac5274e4aacfa7c7e17366126`.
Verified parent digest seals 62,228 historical identities; 3,416 current
consumed/output identities were rehashed, including 1,556 prior identities.
Unrelated history is inherited, not claimed freshly reread. Preservation inventory
has 12,046 paths; 74 newly shared paths were verified now.
Final available space is 2,672,439,296 bytes; capture2816MiB reserve is
not met and must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute original ED256A (27 instructions) and ED80AC (55 instructions).
Source predicts R4=8/R5=EE72A0, nested PR/R14 saves, two local argument stores
and six/thirteen-cycle boundaries through missing ED811A. Predictions remain
unexecuted. Preserve the original ED80CE self-loop/error paths; do not fabricate
child returns. The larger setup path and nine-word post-success clear remain
unproved. Retain original ED67F8. Two review batches used: execute before further
classification. Preserve 248 named captures and 157 device comparisons.
Complete movie initialization, playback, normal boot, gameplay, audio/save/mobile
and original Katana/PAL G1 remain unresolved. Source-only request unchanged and
unsent; production foundation undecided. Previous/current turns: progress.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-method-return-startup/`
(`movie-method-return-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-method-return/` and `20261007-movie-method-return-storage/`.

**Previous bounded recovery — 7 October 2026: movie work-area initializer returns, R3/C8 partial.**

Original ED78EA (53 instructions) and ED79A4 (seven instructions) execute with
retained ED67F8 and BOOT11157C. Native tests pass 36 record-clear, 28 connected
and nine activation controls, including zero/positive/safe negative counts,
32-bit MUL.L wrap, signed loops, aliases/status/FPU and complete state. The
captured initializer clears **832 bytes**, copies two 27-byte strings into
32-byte fields, clears three words, stores count=4 and writes four record flags.
Its original return is verified in **901 cycles**; its caller reaches missing
ED6840 in 912 cycles from wrapper entry. This is a work-area initializer return,
not the complete movie initialization or playback path.

Full startup passes **243 RAM/CPU/TMU and 151 SQ/device comparisons**. All 232
prior named captures, provider return, continuation prefix and 191 mode returns
are unchanged. Both supplied scenes, ten audits and six unchanged images pass
with the same ARM64 binary. Closure is 968 functions (859 base +109 supplemental),
5,036 supplemental instructions and 313 managed entries. Runtime/compiler and
twelve patches are unchanged. Checkpoint binds 62,228 identities;
SHA-256 `b7cf8bf8638ac0900713ee49132967efc8060890d0263e9f7cda46fd420f642e`.

Checkpoint bookkeeping now verifies the parent digest and hashes consumed inputs,
current outputs and newly shared files: 3,369 identities rehashed,
including 1,534 prior identities. The other historical identities
are inherited from the verified 60,393-entry parent, not claimed freshly reread.
All runtime/capture/regression checks above executed. Preservation inventory has
11,972 paths, including 96 newly shared paths verified now.
Final available space is 2,587,070,464 bytes; capture2816MiB reserve is
not met and must be restored before another full capture.
No game or Simulator remains live.

**Next:** emit the original two-instruction RTS/NOP bodies at ED6840 and ED7800.
Verify their returns and retained ED78DC/ED1CB6/EDA2E6 continuations without
replaying completed prefixes. Source predicts wrapper return, second callback
return and binder advance to EDC60C -> EDC8BC -> missing ED256A, with relative
boundaries 3/7/10/15/22/41 cycles. These predictions remain unexecuted. Two review
batches used: execute before further classification. Preserve 242 named captures
and 151 device checks. Normal boot, movie playback, gameplay, audio/save/mobile
and original Katana/PAL G1 remain unresolved. Source-only request unchanged and
unsent; production foundation undecided. Previous/current turns: progress.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-method-workarea-startup/`
(`movie-method-workarea-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-method-workarea/` and `20261007-movie-method-workarea-storage/`.

**Previous bounded recovery — 7 October 2026: second callback reaches setup initializer, R3/C8 partial.**

Original ED1CAC (16 instructions) and ED78C4 (19 instructions) execute through
two nested saved-PR stores to missing **ED78EA**. The actual BSR target, R4=4,
R5=EE6F60, PR=ED78CA and SP=FFFED0 are verified. Unit tests pass 36 callback,
36 setup and nine activation controls, including stack/literal overlap, aliases,
status/FPU and complete RAM/CPU/TMU/SQ/device state. Captured boundaries are
**six/ten cycles**; no callback/setup return or cold branch is claimed.

Full startup passes **233 RAM/CPU/TMU and 141 SQ/device comparisons**. All 230
prior named captures, provider return, continuation prefix and 191 mode returns
remain unchanged. Both supplied scenes, ten audits and six unchanged images pass
with the same ARM64 binary. Closure is 966 functions (859 base +107 supplemental),
4,976 supplemental instructions and 304 managed entries. Runtime/compiler and
twelve patches are unchanged. Checkpoint binds 60,393 identities,
rechecks 58,669 parent identities and 11,876 preserved paths;
SHA-256 `0cda093d58b3e0a7bda7d4aa9ee751ab5c38c9bd6f56215f6ff6e1fbe9d966ad`.
Final available space is 2,734,948,352 bytes; the 2816 MiB capture reserve
is not met and must be restored before another full capture.
No game or Simulator remains live.

**Next:** execute original ED78EA (53 instructions) and ED79A4 (seven instructions).
Retain real managed ED67F8 and resident BOOT11157C. Source predicts an 832-byte
clear, two 27-byte strings in 32-byte fields, a three-word clear, count=4 and four
record flags, with the work pointer stored at EFB8EC. Prove the initializer's
return and then the retained ED78C4 zero-result branch through missing ED6840;
these predictions are unexecuted. Two review batches used: execute before more
classification. Preserve 232 named captures and 141 device comparisons.
Normal boot, movie playback, gameplay, audio/save/mobile and original Katana/PAL
G1 remain unresolved. Source-only request unchanged and unsent; production
foundation undecided. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-method-setup-startup/`
(`movie-method-setup-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-method-setup/` and `20261007-movie-method-setup-storage/`.

**Previous bounded recovery — 7 October 2026: original interface callback returns, R3/C8 partial.**

Original ED1C10 (`RTS` with `MOV #0,R0` in its delay slot) now returns zero in
three cycles. The retained binder advances from EDC604 to EDC608 and reads
EDC878 -> **ED1CAC**, the real second callback. Native tests pass 32 leaf,
32 connected and nine activation controls, including null termination, repeated
original callbacks, the 15-slot bound, aliases/status/FPU and missing targets.
Captured binder boundaries are **27/30/49 cycles**. No callback is replaced with
a success stub, and the captured binder return remains unproved.

Full startup passes **231 RAM/CPU/TMU and 139 SQ/device comparisons**; all 228
prior named captures, provider return, continuation prefix and 191 mode returns
are unchanged. The exact binary passes both supplied scenes, ten audits and six
unchanged images. Closure is 964 functions (859 base +105 supplemental), 4,941
supplemental instructions and 294 managed entries. Runtime/compiler and twelve
patches remain unchanged. Checkpoint binds 58,669 identities,
rechecks 56,965 parent identities and 11,866 preserved paths;
SHA-256 `d85818d855719cb6d14cc16078e1be592d606e738c01c22bc39b44bf177a2de2`.
Final available space is 3,137,773,568 bytes; the 2816 MiB capture reserve
is met.
No game or Simulator remains live.

**Next:** execute original ED1CAC (16 instructions) and ED78C4 (19 instructions),
with their actual saved-PR frames and literal/BSR/JSR arguments. Source predicts
R4=4, R5=EE6F60 and relative boundaries six/ten cycles through missing ED78EA;
these predictions are unexecuted. Two source-review batches used: execute before
further classification. Preserve 230 named captures and 139 device comparisons.
Normal boot, movie playback, gameplay, audio/save/mobile and original Katana/PAL
G1 remain unresolved. Fresh declared-path/hash audit still finds runtime0/1,
generated0/1474, headers0/177, contracts0/4 and title38/39; FFmpeg10139/10139
remains verified, Apple media unbuilt and the supported PAL GDI absent. Evidence:
`artifacts/bootstrap-input-audit-20261007-movie-method-init.json`.
Source-only request unchanged and unsent; production
foundation undecided. Previous/current turns: progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-method-init-startup/`
(`movie-method-init-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-method-init/` and `20261007-movie-method-init-storage/`.

**Previous bounded recovery — 7 October 2026: movie interface copy and binder execute, R3/C8 partial.**

Original EDA288/EDA2B2 now execute: the retained BOOT10E964 copies the 60-byte
interface table, and the binder reaches the real indirect callback ED1C10.
The unit passes 56 copy, 72 binder, eight wrapper and nine activation controls.
Full startup passes **229 RAM/CPU/TMU and 137 SQ/device comparisons**; all 224
prior named captures, provider return, continuation prefix and 191 mode returns
are unchanged. The exact binary passes both supplied scenes, ten audits and six
unchanged images. Normal boot, movie playback and playable Sonic remain unproved.

The first unit failed an incorrect 78-cycle expectation; the original instruction
model and native execution agree on **79 cycles**. Only that expectation was
corrected; generated game code is identical and failed evidence is preserved.
Closure is 963 functions (859 base +104 supplemental), 4,939 supplemental
instructions and 293 managed entries. Runtime/compiler and twelve patches are
unchanged. Checkpoint binds 56,965 identities, rechecking
55,220 parent identities and 11,433 preserved paths;
SHA-256 `741bde830af5e97884e42802d07278669b610044864bd41516dfa1f592612a21`.
Final space is 2,694,041,600 bytes; the 2816 MiB capture reserve is
not met and must be restored before another full capture.
No game or Simulator remains live.

**Next:** compile and test original ED1C10 (`RTS; MOV #0,R0` delay slot), then
execute the binder continuation to discover its actual next callback. Its
three-cycle zero return is source-derived and not yet executed. One classification
batch used. Preserve prior captures and real bindings; no complete binder or
initializer return is claimed. Original Katana/source/PAL G1 remains unresolved;
source-only request unchanged and unsent, production foundation undecided.
Goal remains active; previous/current turns are progress.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-interface-startup/`
(`movie-interface-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-interface-repair1/` and `20261007-movie-interface-storage/`.

**Previous bounded recovery — 7 October 2026: rate fields and full movie-buffer clear execute, R3/C8 partial.**

Original MOVIE ED4620 (five instructions) and ED97A8 (two instructions) execute
through the retained initializer and original ED67F8 word fill. The unit passes
**72 rate cases, 72 return-leaf cases, 28 connected cases and nine activation
controls**. These cover extreme rate values, aliases/status/FPU, zero/positive
counts, safe negative-count cases with 32-bit MUL.L wraparound, signed loop
selection, clear bounds and per-record stores. Twenty captured boundaries match
full RAM/CPU/TMU/SQ/device state through 5,594 cycles. Startup stores rate fields
(0,0,59940), clears **16,196 bytes at 8CEE2210**, and writes the per-record word
at +64. The two ordinary PREF hints from the earlier word copy remain unchanged.

Full startup passes **225 RAM/CPU/TMU and 133 SQ/device comparisons**. All 218
prior named captures, provider return, continuation prefix and 191 mode returns
remain unchanged. Arena return, 30 sprite packets, both supplied scenes, ten
audits and six unchanged images pass with the same ARM64 binary. Original
ED19A2/ED19A8 literal load/JSR establishes missing EDA288, with R4=8CEE6F18,
R5=8CEDC604 and PC/PR ED19A2/ED19AC. The 26 audited module words still explain
all 23 changed module bytes. The buffer/record stores are outside the module;
bytes outside the buffer are unchanged across the retained fill.
Initializer return and movie playback remain unproved.

Full closure **961 functions** (859 base +102 supplemental; 4,883 supplemental
instructions), 284 managed entries, cap860 and bounds256/8192 unchanged.
Runtime/compiler and twelve core patches unchanged. Checkpoint **55,220
identities**, SHA-256 `405b06870c4548d5e399a3f5a80229ddd3cba446429b120a26d4985e3df9b591`;
all 53,475 parent identities and 11,368 preserved paths reverified.
Sharing 79 identical completed baseline captures recovered
896,253,952 bytes while preserving content. Live sharing verified
668 of 687 captures, avoiding 4,221,435,904 logical bytes of duplicate writes.
Final available space **2,682,073,088 bytes**; capture2816MiB guard is
not met; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute original EDA288 (15 instructions, three blocks, two calls)
and EDA2B2 (41 instructions, six blocks, one indirect call). Exact BOOT10E964
entry is already registered in core and callback; retain it for the 60-byte
copy rather than adding another implementation. Its reviewed dispatch is
10E97A and prospective return takes 39 cycles. Verify descending load/store
ordering, overlap, saved source pointer, PR/GPR frames, table-null/unsigned-range
returns and method-index scaling. Captured source table EDC604 points to EDC844;
method zero resolves to missing ED1C10 with R4/R5/R6/R7 all zero. Prospective
**unexecuted** wrapper boundaries are 9/48/52/78 cycles (root
5594/5603/5642/5646/5672). Two review batches used: execute before further review.
Preserve 224 named captures plus the terminal boundary and 133 device checks.
No complete wrapper/initializer or movie-playback claim.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production
foundation undecided. Previous/current goal turns: **progress**. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-rate-init-startup/`
(`movie-rate-init-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-rate-init/` and `20261007-movie-rate-init-storage/`.

**Previous bounded recovery — 7 October 2026: movie word copy, descriptor copy and record clear execute, R3/C8 partial.**

Original MOVIE ED6708 (55 instructions) and ED1A90 (nine instructions) execute
through the retained initializer and original BOOT10E958 descriptor copy.
The unit passes **324 word-copy cases,16 clear cases and nine activation
controls**, including remainder/block counts, ordered overlapping loads/stores,
stack overlap, address aliases, status and physical FPU banks. Fourteen captured
boundaries match full RAM/CPU/TMU/SQ/device state through970cycles. The word copy
moves188bytes and makes two ordinary-memory PREF hints; only pref_calls changes.
The retained helper copies the16-byte descriptor, and the caller/clear helper
zero24bytes of the following record. Both new bodies use managed activation.

Full startup passes **219 RAM/CPU/TMU and127 SQ/device comparisons**. All212
prior named captures, provider return, continuation prefix and191 mode returns
remain unchanged. Arena return,30 sprite packets, both supplied scenes, ten
audits and six unchanged images pass with the same ARM64 binary. The original
ED195C/ED1962 literal load/JSR establishes missingED4620, with R4=8CEE6F08,
R5=59940 and PC/PR ED195C/ED1966. The26 audited module words still explain all23
changed module bytes; new record/stack stores are outside the module image.
No initializer return or movie-playback claim.

Full closure **959 functions** (859 base +100 supplemental;4,876 supplemental
instructions),282 managed entries, cap860 and bounds256/8192 unchanged.
Runtime/compiler and twelve core patches unchanged. Checkpoint **53,475
identities**, SHA-256 `454e1951b0f79bd3222eea9b510d6dca28f62a5e751ddfe5134532d87ab0a6f7`;
all51,799 parent identities and11,289 preserved paths reverified.
Sharing93 identical completed baseline captures recovered
744,181,760bytes while preserving content. Live sharing verified
650 of669 captures, avoiding4,107,403,264 logical bytes of duplicate writes.
Final available space **2,677,559,296bytes**; capture2816MiB guard is
not met; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute original five-instruction ED4620 (stores zero,zero,R5; six
cycles) and two-instruction ED97A8 (original RTS/NOP; three cycles), both absent
from current closures. Retain original ED67F8. After those leaves, ED1908's
MUL.L and stores select a4,049-word/16,196-byte clear atEE2210, then a per-record
word store at+64. Prospective **unexecuted** root boundaries are
970/976/981/984/1000/5576/5594, then explicit missingEDA288 with R4=EE6F18 and
R5=EDC604. Verify original rate words, saved registers, aliases/status/FPU,
MUL.L/count semantics, RAM/device state and timing. Two review batches used:
execute before more classification. Preserve218 named captures plus the
terminal boundary and127 device comparisons. Initializer return remains open.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production
foundation undecided. Previous/current goal turns: **progress**. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-word-copy-startup/`
(`movie-word-copy-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-word-copy/` and `20261007-movie-word-copy-storage/`.

**Previous bounded recovery — 7 October 2026: movie work-area clear and both strings execute, R3/C8 partial.**

Original MOVIE ED67F8 (35 instructions) and BOOT11157C (23 instructions) now
execute through the retained descriptor initializer. The unit passes **120
word-fill cases, 288 bounded-string cases and nine activation controls**,
including zero/remainder/block counts, nonzero fill, early/no terminator,
truncation, padding, high-bit bytes, overlapping strings, aliases and FPU banks.
The captured path clears376bytes and copies two27-byte strings into32-byte
fields with zero padding. Whole RAM/CPU/TMU/SQ/device boundaries match at
4/17/168/175/458/465/748/755cycles. MOVIE activation is managed; the BOOT helper
is resident and remains registered when MOVIE is retired.

Full startup passes **213 RAM/CPU/TMU and121 SQ/device comparisons**. All206
prior named captures, provider return, continuation prefix and191 mode returns
remain unchanged. Arena return,30 sprite packets, both supplied scenes, ten
audits and six unchanged images pass with the same ARM64 binary. The first
full run omitted six new capture labels from the observation filters; adding
those labels was the only repair. Both runs have identical terminal RAM/state,
and the first run's frozen sources and1,460 artifacts are preserved.

The next explicit missing call is ED6708, established by original ED1938/ED1940
literal load/JSR, with R4=8CEE6E24/R5=8CEDC8F0/R6=47 and PC/PR ED1938/ED1944.
The26 audited module words still explain all23 changed module bytes; the new
work-area stores are outside the module image. Initializer return is unproved.

Full closure **957 functions** (859 base +98 supplemental;4,812 supplemental
instructions),273 managed entries, cap860 and bounds256/8192 unchanged.
Runtime/compiler and twelve core patches unchanged. Checkpoint **51,799
identities**, SHA-256 `10a1430a572fcd5760cc5e376fd17739fd315b55b8e73f0b87b2b913415177d0`;
all48,740 parent identities and11,196 preserved paths reverified.
Sharing360 identical completed baseline captures recovered967,655,424bytes
while preserving content. Live sharing verified632 of651 captures, avoiding
3,993,370,624 logical bytes of duplicate writes. Final available space
**2,704,093,184 bytes**; capture2816MiB guard is
not met; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute original55-instruction MOVIE ED6708 word copy (seven blocks,
no calls), retaining original16-byte BOOT10E958 already present in core/callback.
ED6708 copies47 words/188bytes here and performs two ordinary-memory PREF hints.
Check ordered loads/stores, overlap, saved R12/stack, aliases/FPU, timing and
pref_calls increments; SQ contents/counters remain unchanged on this RAM path.
Prospective **unexecuted** timing is168cycles for the copy; root boundaries
755/923/930/947/954 then explicit missingED1A90. The retained copy moves the
16-byte descriptor toEE6EE0; the caller clearsEE6EF0 and preparesR4=EE6EF4.
One review batch used. Preserve212 named captures plus the terminal boundary
and121 device checks. No initializer return/movie-playback claim.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production
foundation undecided. Previous turn: **no progress (status only)**; this turn:
**progress**. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-workarea-startup-repair1/`
(`movie-workarea-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`capture-label-correction.json`, `next-startup-preflight.json`),
`20261007-startup-movie-workarea/` and `20261007-movie-workarea-storage/`.

**Previous bounded recovery — 7 October 2026: descriptor initializer reaches word-fill service, R3/C8 partial.**

Original ED18E4 (18 instructions) and ED1908 (95 instructions) execute through
their first unresolved service. The unit passes **128 initializer variants and9
activation controls**, preserving the descriptor pointer, five saved GPRs,
two saved PRs, status, stack aliases and physical FPU banks. Captured boundaries
4/17 match full RAM/CPU/TMU/SQ/device state. The call to missingED67F8 has
R4=8CEE6DE4/R5=0/R6=94, with PC/PR ED1908/ED1920. The descriptor atFFFF14
remains unchanged. No initializer return or work-area clear is yet executed.

Full startup passes **207 RAM/CPU/TMU and115 SQ/device comparisons**, including
the predicted terminal state. All204 prior named captures, provider return,
continuation prefix and191 mode returns remain unchanged; the prior terminal
matches initializer entry after only its registered-PC normalization. Arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images pass with the same ARM64 binary. Managed activation has266 entries.
Original literal load/JSR ED191A/ED191C establishes ED67F8; the error text does
not identify the address. The26 audited module words still explain all23 changed
bytes; the new initializer writes are confined to its saved stack frames.

Full closure **955 functions** (859 base +96 supplemental;4,754 supplemental
instructions), cap860 and bounds256/8192 unchanged. Runtime/compiler and twelve
core patches unchanged. Checkpoint **48,740 identities**, SHA-256 `7b8af3af9efd4c16e3ce05990b0b2025cce5253bfe11607ee752e3d79efa56cb`;
all47,211 parent identities and10,836 preserved paths reverified.
Sharing526 identical completed baseline captures recovered552,230,912 bytes
while preserving content. Live sharing verified626 of633 captures,
avoiding3,955,359,744 logical bytes of duplicate writes.
Final available space **2,749,239,296 bytes**; capture2816MiB guard
fails; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute original35-instruction MOVIE ED67F8 word fill and23-instruction
BOOT11157C bounded string copy. Neither has external calls; both are absent from
the current core/callback/supplemental closures. ED67F8 takes a count of32-bit
words: captured94 clears376bytes. BOOT11157C copies through a bound and pads with
zero after a terminator. Both captured source strings have27 nonzero bytes and
bounds32. Keep managed activation for MOVIE and ordinary registration for BOOT.
Retain ED18E4/ED1908. Verify zero/remainder/block counts and nonzero fill values;
early/no terminator, zero padding, high-bit bytes and aliases for the copy.
Captured predictions, **unexecuted**: fill151cycles, each copy283cycles; root
boundaries4/17/168/175/458/465/748/755, then explicit missingED6708. Two review
batches used: execute before further classification. No initializer return claim.
Preserve206 named captures plus the predicted terminal boundary and115 device checks.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous/current goal turns: **progress**. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-descriptor-init-startup/`
(`movie-descriptor-init-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-descriptor-init/` and `20261007-movie-descriptor-init-storage/`.

**Previous bounded recovery — 7 October 2026: movie-slot selection and descriptor copy execute, R3/C8 partial.**

Original ECF0F8 (187 instructions) and ED0350 (58 instructions) execute with
retained16-byte copy helper10E958. The unit passes **528 setup cases and9
activation controls**: free slots0..31, both descriptor selectors,256 invalid-
selector errors and16 all-full errors, register/stack aliases and FPU banks.
Captured boundaries33/46/63/70 match full RAM/CPU/TMU/SQ/device state. Startup
selects first slotEDD114, copies the descriptor toFFFF14 and writes59940 as its
fourth word, then reaches explicit missingED18E4. The first unit attempt expected
48 activation entries; original CFG review proves28+17=45. Only that assertion
was corrected. Failed artifacts and their frozen source are preserved and bound
by the repaired unit's `manifest-count-correction.json`.

Full startup passes **205 RAM/CPU/TMU and113 SQ/device comparisons**, including
the predicted terminal state. All200 prior named captures, provider return,
continuation prefix and191 mode returns remain unchanged. Arena return,30 sprite
packets, both supplied scenes, ten audits and six unchanged images pass using
the same ARM64 binary. Managed activation has244 entries. TerminalPC/PR are
ED0388/ED0390; original literal load/JSR atED0388/ED038C establish ED18E4.
The error text does not identify the address. The26 audited module words still
explain all23 changed bytes; the new setup writes are confined to the stack.
Full slot setup, validation return and decoder execution remain unproved.

Full closure **953 functions** (859 base +94 supplemental;4,641 supplemental
instructions), cap860 and bounds256/8192 unchanged. Runtime/compiler and twelve
core patches unchanged. Checkpoint **47,211 identities**, SHA-256 `13951dbbca44be7d8aba135d4367ef8a3a09b70e95d0ba95081826bcb790db7e`;
all45,651 parent identities and10,310 preserved paths reverified.
Sharing297 identical completed baseline captures recovered612,347,904 bytes
while preserving content. Live sharing verified614 of627 captures,
avoiding3,879,337,984 logical bytes of duplicate writes.
Final available space **2,716,536,832 bytes**; capture2816MiB guard
fails; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute ED18E4 (18 instructions) and its first helper ED1908
(95 instructions). The observed descriptor initializer saves its caller and
enters setup that prepares R4=8CEE6DE4/R5=0/R6=94 for missingED67F8.
Captured predictions, **unexecuted**: helper entry4cycles, missing call17cycles.
Verify descriptor-pointer preservation, five saved GPRs, two PR slots, literal
arguments, aliases/FPU and whole RAM/CPU/TMU/SQ/device state. Two review batches
used: execute before any further classification. No initializer return claim.
`tests/recovery/startup-movie-descriptor-init-generator.cpp` is prepared, unrun.
Preserve204 named captures plus the predicted terminal boundary and113 device checks.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous/current goal turns: **progress**. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-slot-setup-startup/`
(`movie-slot-setup-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-slot-setup-repair1/` and `20261007-movie-slot-setup-storage/`.

**Previous bounded recovery — 7 October 2026: complete movie buffer-size return, R3/C8 partial.**

Original26-instruction ED0222 now executes with retained ED02BA/ED01BC/ED0256
and real signed divider10E67C. The unit passes **420 wrapper variants and9
activation controls**, including negative/extreme dimensions, signed rounding,
32-bit multiplication wraparound, selectors, aliases and physical FPU banks.
Nine connected boundaries22/40/120/143/154/170/196/236/256 match full state.
Captured dimensions are320/480/160/240; frame outputs460832/1382432.
The complete size wrapper returns **2,949,408 bytes at256 guest cycles**, with
PR/SP/R14 restored. The original caller stores that result atECEF024 and buffer
pointer8C900000 atECEF020, then reaches missingECF0F8.

Full startup passes **200 RAM/CPU/TMU and108 SQ/device comparisons**. All196 prior
named captures, provider return, continuation prefix and191 mode returns remain
unchanged. The earlier terminal boundary matches dimension entry after only its
registered-PC normalization. Arena return,30 sprite packets, both supplied scenes,
ten audits and six unchanged images pass using the same ARM64 binary. Managed
activation has199 entries. TerminalPC/PR EB0166/EB0174 and the original literal
load/JSR atEB016A/EB0170 establish ECF0F8; the error text does not name it.
The26 audited module words explain all23 changed bytes. Size bodies write only
the stack; the original caller performs the two additional parameter stores.

Full closure **951 functions** (859 base +92 supplemental;4,396 supplemental
instructions), cap860 and bounds256/8192 unchanged. Runtime/compiler and twelve
core patches unchanged. Checkpoint **45,651 identities**, SHA-256 `3bb85bbd8863cb89f88025ca52e249833056fc6f493f2851d6fc52a95b007b47`;
all44,138 parent identities and10,013 preserved paths reverified.
Sharing45 identical completed baseline captures recovered688,685,056 bytes
while preserving content. Live sharing verified602 of615 captures,
avoiding3,803,316,224 logical bytes of duplicate writes.
Final available space **2,727,653,376 bytes**; capture2816MiB guard
fails; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute ECF0F8 (187 instructions) and its first helper ED0350
(58 instructions), retaining real10E958 for the16-byte descriptor copy. All32
slot occupied words are zero in the captured state. Verify free slots0..31,
all-full ECF512 error, selector1/2 descriptors and invalid-selector ECF512 error,
stack and register residues, aliases/FPU and whole RAM/CPU/TMU/SQ/device state.
Captured predictions, **unexecuted**: helper entry33, copy entry46/return63,
missingED18E4 at70cycles. Each preceding occupied slot adds14cycles; all-full
error entry466cycles. No slot-setup/validation return or decoder claim.
Two review batches used: execute before reviewing ED18E4 or other new bodies.
`tests/recovery/startup-movie-slot-setup-generator.cpp` is prepared, unrun.
Preserve200 named phases and108 device comparisons.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous/current goal turns: **progress**. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-size-return-startup/`
(`movie-size-return-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-size-return/` and `20261007-movie-size-return-storage/`.

**Previous bounded recovery — 7 October 2026: stream-size calculation executes in startup, R3/C8 partial.**

Three original movie-size bodies ED02BA/ED01BC/ED0256 (151 instructions) are
integrated using retained signed divider10E67C. The isolated unit passes180
wrapper variants and9 activation controls, including selector2/direct versus
rounded sizes, negative byte counts, stack pointers, aliases and physical FPU
banks. All six captured boundaries22/40/120/143/154/170 match full state.
The first helper returns six words71000/10000/61000/12000/12000/80C0 (hex).
The second helper reaches explicit missingED0222; no replacement return is used.

Full startup passes **197 RAM/CPU/TMU and105 SQ/device comparisons**, including
its predicted terminal state. All190 prior named captures, provider return,
continuation prefix and191 mode returns remain unchanged. Arena return,30 sprite
packets, both supplied scenes, ten audits and six unchanged images pass using
the same ARM64 binary. Managed activation has196 entries. Terminal PC/PR are
ED0256/ED0274 at170 cycles after wrapper entry; originalBSR atED0270 establishes
ED0222. The diagnostic itself does not name the missing address. The26 audited
module words still explain all18 changed bytes; new size writes are on the stack.

Full closure **950 functions** (859 base +91 supplemental;4,370 supplemental
instructions), cap860 and bounds256/8192 unchanged. Runtime/compiler and twelve
core patches are unchanged. Checkpoint **44,138 identities**, SHA-256 `c75842e1aca7f3125d8ada1a9e4ab52139517cc9d2d9d356e78c3307e3db8e08`;
all42,663 parent identities and9,968 preserved paths reverified.
Sharing44 identical completed baseline captures recovered672,399,360 bytes while
preserving content. Live sharing verified584 of603 captures, avoiding3,689,283,584
logical bytes of duplicate writes. Final available space **2,698,821,632 bytes**;
capture2816MiB guard fails; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute original26-instruction ED0222 (no external calls), which returns
width/height, rounded half-width and half-height through four pointers. Retain
the current151 instructions and real10E67C. Test signed rounding, arithmetic
wraparound, output pointers and complete frame-size/wrapper return. Captured
predictions, **unexecuted**: dimension outputs320/480/160/240; dimension26cycles,
frame helper82cycles and complete wrapper256cycles. Frame outputs460832/1382432;
wrapper return2949408. MUL.L costs2 cycles; the frame return block costs40.
One review batch used; no additional classification is needed before execution.
Preserve196 prior named captures plus the separately predicted terminal boundary.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous status-only turn: **no progress**; this goal turn: **progress**.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-size-startup/`
(`movie-size-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-size/` and `20261007-movie-size-storage/`.

**Previous bounded recovery — 7 October 2026: movie configuration returns to its caller, R3/C8 partial.**

Third callback registration ED5556 is integrated using retained ED5324/ED533C
mask helpers and both earlier observers. Its unit passes2,176 parent cases
(including128 explicit full-table errors),128 mask and9 activation controls;
7/25/50/66/71 captured-cycle boundaries match full state. All three callback
tables now contain the original callbacks: EFB2C4=ECFC58/0, EFB344=ECFBDC/0,
EFB3C4=ECFC92/0. The configuration routine ECFCE4 returns to **EB00DA**, with
its saved PR, stack and R14 restored. This return is observed in full startup;
a separate whole-initializer oracle is not claimed. Runtime/compiler and twelve
core patches remain unchanged.

Full startup passes **190 RAM/CPU/TMU and98 SQ/device comparisons**, arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images. All184 prior named captures, provider return, continuation prefix and191
mode returns remain unchanged. Managed activation has184 entries, with each mask
helper installed once. The original caller proceeds through its32-byte clear and
parameter setup before stopping at **ED02BA**. Captured register arguments are
1/0x384000/320/480, with stack argument6. Twenty-six source-derived module store
words explain all18 changed bytes; instructions and every other module byte
remain unchanged.

Full closure **947 functions** (859 base +88 supplemental;4,219 supplemental
instructions), cap860 and bounds256/8192 unchanged. Checkpoint
**42,663 identities**, SHA-256 `33206f523a278ce330858ebe29ca73f77c3c29093e22e0bc6ca7033d449a9235`;
all41,244 parent identities and9,924 preserved paths reverified. Sharing243 exact
completed baseline captures recovered749,989,888 bytes without removing content.
Live capture sharing verified566 of585 files, avoiding3,575,250,944 logical bytes
of duplicate writes. Final available space **2,669,043,712 bytes**;
unit400MiB/full1536MiB guards pass;
capture2816MiB guard fails; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** execute the size-calculation wrapper ED02BA (50 instructions) with its
two reviewed direct helpers ED01BC (51) and ED0256 (50), reusing real signed
divider10E67C. Verify pointer-bearing stack frames, selector2/direct and other/
rounded-size paths, negative byte counts, six output words, status/stack residues,
aliases/FPU and full RAM/CPU/TMU/SQ/device state. Captured predictions, **unexecuted**:
first helper entry22, divider entry40/return120, first helper return143, second
helper entry154, explicit missingED0222 at170 cycles. Expected first-helper outputs
are71000/10000/61000/12000/12000/80C0 (hex). ED0256 has a16-cycle prefix before
ED0222, which is not yet reviewed or registered. Two classification batches used:
execute before reviewing it. Do not claim a complete size-calculation return.
`tests/recovery/startup-movie-size-generator.cpp` is prepared, unrun.
Preserve190 named phases/98 device checks and original managed activation.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-callback-register3-startup/`
(`movie-callback-register3-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-callback-register3/` and `20261007-movie-callback-register3-storage/`.

**Previous bounded recovery — 7 October 2026: second callback table integrated; third table passes in isolation, R3/C8 partial.**

Original39-instruction ED5494 now executes in full startup, reusing the verified
ED5324/ED533C interrupt-mask helpers and retaining the first callback observer.
Its unit passes **2,176 parent cases** (including128 explicit full-table errors),
128 mask and9 activation controls. Captured boundaries **7/25/50/66/71 cycles**
match full RAM/CPU/TMU/SQ/device predictions. Available-slot registrations restore
the interrupt mask; full-table execution reaches missing ED5374 at199 cycles.
No error-handler return claim. Runtime/compiler and twelve core patches unchanged.

Full startup passes **184 RAM/CPU/TMU and92 SQ/device comparisons**, arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images. All178 prior named captures, provider return, continuation prefix and191
mode returns remain unchanged. Activation has173 entries; the retained mask
helpers are installed once. Callback tables EFB2C4 and EFB344 containECFC58/0
andECFBDC/0 respectively. The third tableEFB3C4 remains empty. Seventeen original
module store words explain all ten changed bytes; instructions and every other
module byte remain unchanged.

Full closure **946 functions** (859 base +87 supplemental;4,180 supplemental
instructions), cap860 and bounds256/8192 unchanged. Full checkpoint
**41,244 identities**, SHA-256 `6aff8ba8cbb5fd7909ff18c4299fb168ec1fdbcd2cd5802c4441e48d841a95e3`;
all39,875 parent identities and9,681 preserved paths reverified. No new storage
compaction was needed for this full run. Live capture sharing verified548 of567
files, avoiding3,461,218,304 logical bytes of duplicate writes.

The next full-startup target **ED5556** now passes in isolation. Original literal
andJMP atECFDA2 plus capturedR2/PC/PR identify it. The parent restores its PR/SP
before this tail transfer; captured return isEB00DA. Its39 original instructions
registerECFC92/0 in third tableEFB3C4 using retained ED5324/ED533C. The unit passes
**2,176 parent cases** (including128 full-table errors),128 mask and9 activation
controls. Boundaries7/25/50/66/71 and explicit full-table error199 match. Whole RAM
checks preserve both prior callback tables. Third-table full startup and complete
movie initializer return remain unproved.
Unit checkpoint **205 identities**, SHA-256 `46650c5ac3caa914c124fd5d0886b0e1f8f01e49e81000371b7fae68d3cd2474`.

**Next:** integrate only ED5556. Preserve184 named phases and92 device checks;
add normalized entry plus five verified boundaries (expected190/98 total).
Retain both existing callback observers and the same managed mask helpers, adding
only11 new parent entries (expected184 entries). Expected full closure947 functions,
88 supplemental/4,219 instructions. Then execute before more review.
Current available space **2,403,278,848 bytes**; unit400MiB guard
passes. Capture2816MiB guard
fails; restore its reserve before full startup.
No game or Simulator remains live.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-callback-register2-startup/`
(`movie-callback-register2-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-callback-register2/`, `20261007-startup-movie-callback-register3/`
and `20261007-movie-callback-register2-storage/`.

**Previous bounded recovery — 7 October 2026: first callback table integrated; second table passes in isolation, R3/C8 partial.**

Original ED53C2 and its interrupt-mask helpers ED5324/ED533C (39 + 12 + 11
instructions) are integrated through original MOVIE activation. The unit passes
**2,176 parent cases**, including128 full-table errors, plus128 mask and9
activation controls. All16 slot positions, null/non-null callbacks, zero/nonzero
arguments, all16 masks, aliases and FPU banks are covered. Captured boundaries
**7/25/50/66/71 cycles** match full RAM/CPU/TMU/SQ/device predictions. Full-table
execution reaches explicit unregistered ED5374 at199 cycles; its return is unproved.
Runtime/compiler and twelve core patches remain unchanged.

Full startup passes **178 RAM/CPU/TMU and86 SQ/device comparisons**, arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images. All172 prior named captures, provider return, continuation prefix and191
mode returns remain unchanged. Activation has162 entries. First callback table
EFB2C4 containsECFC58/0; all remaining slots and the second table are empty.
Seventeen original module store words explain all ten changed bytes; module
instructions and every other module byte are unchanged.

Full closure **945 functions** (859 base +86 supplemental;4,141 supplemental
instructions), cap860 and bounds256/8192 unchanged. Full checkpoint
**39,875 identities**, SHA-256 `61472f22e66f5f85d277ee09ff41f9672430e18d41a44de73f5663d184c9a344`;
all38,532 parent identities and9,681 preserved paths reverified. Sharing79 exact
completed baseline captures recovered1,209,888,768 bytes. Live capture sharing
verified530 of549 files, avoiding3,347,185,664 logical bytes of duplicate writes.

The next full-startup target **ED5494** is now verified in isolation. Original
literal/JSR atECFD96 and capturedR3/PC/PR identify it. Its39 original instructions
registerECFBDC/0 in the second tableEFB344, reusing the exact already compiled
ED5324/ED533C bodies and manifests. **2,176 parent cases** (including128 full-table
errors),128 mask and9 activation controls pass, with the same7/25/50/66/71
boundaries and199-cycle explicit full-table error. Whole RAM checks preserve the
first callback table. No second-table full-startup integration claim is made.
Unit checkpoint **204 identities**, SHA-256 `1abb6ef7ef934b2676eec7d77ce8ad5bcb614de578129bb5b7215ef54a394ed8`.

**Next:** integrate only ED5494 using the verified unit. Preserve178 named phases
and86 device checks; add normalized entry plus five unit boundaries (expected
184/92 total). Reuse the existing managed mask helpers without duplicate entries;
add only the11 ED5494 manifest entries (expected173 total). Preserve the first
callback observer while observing the second helper calls. Expected full closure
946 functions,87 supplemental/4,180 instructions. Then execute before more review.
Current available space **3,045,367,808 bytes**; unit400MiB guard
passes. Capture2816MiB guard
passes.
No game or Simulator remains live.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-callback-register-startup/`
(`movie-callback-register-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-callback-register/`, `20261007-startup-movie-callback-register2/`
and `20261007-movie-callback-register-storage/`.

**Previous bounded recovery — 7 October 2026: movie mode configuration returns, R3/C8 partial.**

Original ECF096 and ED56EE (49 + 18 instructions) execute through validated
MOVIE module activation, reusing retained signed divider10E67C. The unit passes
**768 configuration,16 tail,140 division and9 activation controls**. Both configuration
flags, signed/zero division, quotient and SR.Q/M/T, mode2/3/other branches,
stack residues, address aliases and physical FPU banks are covered. Captured
boundaries **26/106/117/131 cycles** match whole RAM/CPU/TMU/SQ/device predictions.
The captured configuration computes59940/2=29970 and completes its mode2 tail.
Runtime/compiler and twelve core patches are unchanged.

Full startup passes **172 RAM/CPU/TMU and80 SQ/device comparisons**, arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images. All167 prior named captures, provider return, continuation prefix and191
mode returns remain unchanged. Original-image activation now has149 entries.
Seventeen source-derived module store words account for all ten changed bytes;
all other module bytes, including instructions, remain unchanged.

Closure **942 functions** (859 base +83 supplemental;4,079 supplemental
instructions), cap860 and bounds256/8192 unchanged. Checkpoint **38,532 identities**,
SHA-256 `b94f887bd2e859a883e1cc866834b27d5ef6b6fb86d0a0e8a9c2862bea5616ee`;
all37,240 parent identities and9,602 preserved paths reverified. Sharing42 exact
completed baseline captures recovered703,385,600 bytes. Live capture sharing
verified515 of531 files, avoiding3,252,158,464 logical bytes of duplicate writes.
Final available space **2,108,751,872 bytes**; unit400MiB/full1536MiB guards
pass. Capture2816MiB guard fails; restore its reserve before another full run.
No game or Simulator remains live.

**Next:** original39-instruction callback registration **ED53C2**, with12/11-
instruction interrupt-mask helpersED5324/ED533C. Original literal/JSR atECFD8E
and capturedR2/PC/PR identify the target; the managed error contains no address.
The sixteen callback slots atEFB2C4 are empty; captured arguments areECFC58/0.
Unexecuted predictions: mask-enter entry7/return25, mask-leave entry50, complete
first-slot return71; each occupied slot adds10; full-table error entry199.
Test all slot positions, full table, null/non-null callback and argument, all16
interrupt masks, aliases/FPU, stack and complete state. Full-table errorED5374
is reviewed but remains explicitly unregistered; do not claim its return.
Its10EF3C/10F020 callees exist in the core archive. Two classification batches
used: execute before more source review. Preserve172 phases/80 device checks.
`tests/recovery/startup-movie-callback-register-generator.cpp` is prepared, unrun.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only acquisition request unchanged/unsent; production foundation
undecided. Previous turn was status-only (**no progress**); this turn is **progress**.
Goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-mode-config-startup/`
(`movie-mode-config-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`),
`20261007-startup-movie-mode-config/` and `20261007-movie-mode-config-storage/`.

**Previous bounded recovery — 7 October 2026: callback installation and default restoration verified, R3/C8 partial.**

Original14-instruction6453A2 is integrated. The prior preflight was incomplete:
it inspected the separate callback closure but omitted the retained core archive.
Both645002 and644EF0 were already compiled and registered there. The first unit
link caught a duplicate645002 symbol; that failed build is preserved. Reusing the
retained helper first passed384 nonnull cases, then source review of644EF0 and
expanded tests passed **1,536 helper and192 parent controls**, including **832
complete null-callback default restorations**. Null callbacks restore defaults;
they do not reach a missing target. The earlier missing-path prediction is
superseded. Verify all linked libraries/registrations before emitting another body.

Both paths restore all16 interrupt masks and preserve expected full RAM/CPU/TMU/
SQ/device state. Captured nonnull boundaries are **11/61/66 cycles**; slot7 null
helper/wrapper returns take66/82. Indices0–8, out-of-range values, shifted-index
wraparound, argument/callback variants, aliases and FPU banks are covered.
Full startup verifies callbackECFA68 at8A723C and argument0 at8A7240.
No runtime/compiler change; twelve core patches remain pinned.

Full startup passes **167 RAM/CPU/TMU and75 SQ/device comparisons**, arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images. All163 prior named captures, provider return, continuation prefix and191
mode returns are unchanged. Original-image activation remains133 entries; module
instructions remain unchanged after the same eight audited data stores.

Sharing42 identical completed baseline binary/PPM captures recovered585,564,160
bytes. Live capture sharing verifies503 of516 files, avoiding3,176,136,704 logical
bytes of duplicate writes. Original content/metadata is preserved; all**9,560**
previously preserved paths are reverified. Closure **940 functions** (859 base
+81 supplemental;4,012 supplemental instructions), cap860 and continuation bounds
256/8192 unchanged. Checkpoint **37,240 identities**, SHA-256
`3c5ff655ff2754507953a45b06b00619dedac638e5aacafbc7e0f26aba62f92f`;
all35,919 parent identities reverified. No game or Simulator remains live.
Final available space2,604,773,376 bytes: unit400MiB/full1536MiB guards pass;
capture2816MiB guard fails. Restore the capture reserve before another full run.

**Next:** execute49-instruction ECF096 and18-instruction ED56EE as managed
MOVIE code, reusing retained85-instruction signed division10E67C (present in both
core and callback libraries). The runtime's managed-target error has no address;
original BSR B98A atECFD7E and capturedPC ECFD78/PR ECFD82 prove targetECF096.
Captured arguments are1/5/2/2. Unexecuted predictions: divide59940 by2→29970;
division entry26 cycles, nonzero helper80, tail entry117, mode2 tail14, complete
return131. Verify flag4 divisor doubling, flag2 selecting50000 instead of59940,
signed/zero division and SR.Q/M/T, configuration globals, mode2/3/other tail paths,
stack residues and full RAM/CPU/TMU/SQ/device state. Preserve original module
identity checks and activation; add no ordinary registration for MOVIE addresses.
Two classification batches used after the latest full run: execute before more
review. Preserve167 named phases and75 device checks.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. The retained recovery runtime helpers do not supply missing Katana
sources. Source-only request unchanged/unsent; production foundation undecided.
Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-callback-install-startup/`
(`movie-callback-install-startup-checkpoint.json`, `next-startup-preflight.json`),
`20261007-startup-movie-callback-install-repair2/retained-helper-correction.json`
and `20261007-movie-callback-install-storage/`.

**Previous bounded recovery — 7 October 2026: context/stream allocation integrated, R3/C8 partial.**

Original5FF8D2 and5FFA90 are integrated (48+30 instructions). The isolated unit
passes **128 constructor and640 parent controls** across all16 interrupt masks,
RAM aliases and FPU banks. It verifies zero size/pointer/callback paths, positive
and nonpositive slot counts, exhaustion and first-free selection. Constructor
execution takes284 cycles; captured parent boundaries are **18/302/328/449**.
Full startup verifies all260 context bytes, callbackECF9F2, argument0, stored
SR40000001, contextEE20B4, second slotEDE190 and restored caller interrupt mask.
No helper was substituted; runtime/compiler and twelve core patches are unchanged.

Full startup passes **163 RAM/CPU/TMU and71 SQ/device comparisons**, arena
return,30 sprite packets, both supplied scenes, ten audits and six unchanged
images. All158 prior named captures, provider return, continuation prefix and191
mode returns are unchanged. Original-image activation remains133 entries; module
instructions remain unchanged after the same eight audited data stores.

The remaining exact duplicate RAM captures could not satisfy the storage reserve;
no full build was attempted then. Sharing **82 identical completed baseline VRAM
captures** recovered578,871,296 bytes and allowed the guarded run. Live capture
sharing verified488 of504 files, avoiding3,081,109,504 logical bytes of duplicate
writes. Original contents/metadata were preserved. A checkpoint bookkeeping
assertion initially counted three already-preserved baseline paths twice; corrected
set union and full re-verification establish **9,518 distinct preserved paths**.
The failed bookkeeping script is retained. This did not require a native rerun.

Closure **939 functions** (859 base +80 supplemental;3,998 supplemental
instructions), cap860 and continuation bounds256/8192 unchanged. Checkpoint
**35,919 identities**, SHA-256
`9bb969ff6e3f4c89b0df616d1480f4c97b5124395cb7f592886e3227900648ba`;
all34,707 parent identities reverified. No game or Simulator remains live.
Final available space2,695,835,648 bytes: unit400MiB/full1536MiB guards pass;
capture2816MiB guard fails. Restore that reserve before another full run.

**Next:** execute14-instruction6453A2 and42-instruction645002. The observed
caller ECFD68/PR ECFD74, JSR atECFD70 and literalECFE0C identify6453A2.
It installs callbackECFA68 with argument0 into slot7 of table8A6FD4, at
8A723C/8A7240. Unexecuted predictions: helper entry11 cycles, nonnull helper50,
complete wrapper66; verify all16 masks, arguments, indices, aliases/FPU banks,
stack residues and full RAM/CPU/TMU/SQ/device. Null callbacks reach still-missing
644EF0 after22 helper/33 connected cycles: verify that explicit boundary without
claiming a complete removal path. Two classification batches used: execute before
more review. Preserve163 named phases and71 device checks. Bounded generator
`tests/recovery/startup-movie-callback-install-generator.cpp` is prepared, unrun.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only request unchanged/unsent; production foundation undecided.
Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-stream-startup/`
(`movie-stream-startup-checkpoint.json`, `capture-storage-audit.json`,
`next-startup-preflight.json`), `20261007-startup-movie-stream/` and
`20261007-movie-stream-storage/`.

**Previous bounded recovery — 7 October 2026: complete movie allocation integrated, R3/C8 partial.**

Original slot search/status/setters5FF980/5FFA2C/5FFA24/5FFA28 are integrated
(33 instructions). All28 search and512 parent controls pass. Full startup now
verifies the complete5FF932 allocation: search enters at33 cycles, returns at75,
and allocation returns at131 with slot/owner8CEDE188 and the interrupt mask
restored. All155 prior named captures, provider return, continuation prefix and
191 mode returns are unchanged. The run passes **158 RAM/CPU/TMU and66 SQ/device
comparisons**, arena return,30 sprite packets, both supplied scenes, ten audits
and six unchanged images. Runtime/compiler and twelve core patches are unchanged.

Capture storage now compares live RAM/AICA/flash bytes before APFS cloning and
verifies output bytes afterward; unequal/missing references fall back to actual
writes. Focused controls cover equality, changed bytes/size, missing references,
empty buffers, existing outputs and write failure. Full-run audit verifies
**479 shared files of489 captures**, avoiding3,024,093,184 logical bytes of
additional writes. Reference hashes are bound before and after execution; no
past capture is substituted for live state. Existing storage guards are unchanged.
Sharing322 completed duplicate RAM/AICA files also recovered2,950,897,664 bytes;
all9,439 previously preserved paths remain byte-identical.

Closure **937 functions** (859 base +78 supplemental;3,920 supplemental
instructions), cap860 and continuation bounds256/8192 unchanged. Checkpoint
**34,707 identities**, SHA-256
`5a15e1f304e9ad38fcdd3319fa7135ec05a4534ef5df58df0a45f845a027328b`;
all33,515 parent identities reverified. Original-image activation remains133
entries and module instructions remain unchanged after eight audited data stores.
No game or Simulator remains live. Final available space2,690,150,400 bytes:
unit400MiB/full1536MiB guards pass; capture2816MiB guard fails. Restore the
capture reserve before the next full run; the next isolated unit may proceed.

**Next:** execute48-instruction5FF8D2 and its30-instruction helper5FFA90 using
real existing locks/search/getters/setters. Observed caller is ECFD58/PR ECFD68;
original JSR atECFD64 and literalECFE00 identify5FF8D2. Captured arguments are
bufferEDE1B8,size4000,callbackECF9F2,argument0. The helper builds a context at the
buffer end with64 backward zero stores, sentinel, callback/argument/SR fields.
Unexecuted predictions: helper284 cycles; parent helper entry/exit18/302,
search entry328 and complete return449 with second slotEDE190. Test zero
size/pointer/callback, exhaustion/selection, aliases/FPU banks, all16 masks,
stack residues and whole RAM/CPU/TMU/SQ/device. Two classification batches used:
execute before additional review. Preserve158 named phases and66 device checks.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only request unchanged/unsent; production foundation undecided.
Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-slots-startup/`
(`movie-slots-startup-checkpoint.json`, `capture-storage-audit.json`,
`next-startup-preflight.json`), `20261007-startup-movie-slots/` and
`20261007-capture-storage/`.

**Previous bounded recovery — 7 October 2026: complete movie allocation passes in isolation, R3/C8 partial.**

The four original BOOT routines at5FF980/5FFA2C/5FFA24/5FFA28 (33 instructions)
pass28 search and512 complete-parent controls using the real count/buffer
getters, setters and interrupt-mask helpers. Captured search/allocation return
in42/131 cycles. All16 interrupt masks are restored; free-slot selection,
exhaustion, nonpositive counts, busy owner, aliases and FPU banks are covered.
Whole RAM/CPU/TMU/SQ/device comparisons pass. No runtime/compiler change.

Unit evidence: `artifacts/recovery-aot/20261007-startup-movie-slots/`;
`movie-slots-checkpoint.json` binds200 identities, SHA-256
`0278bc05d4419929421b84a854d07c675b8560f09266150b0b29ade2499978be`.
Sharing322 identical completed RAM/AICA files recovered2,950,897,664 bytes;
contents remain preserved. Storage report:
`artifacts/recovery-aot/20261007-movie-allocation-storage/image-sharing/report.json`.

**Next:** integrate these four bodies into full startup, preserve the155 named
prior phases and64 device checks, and measure the next terminal. The last full
startup checkpoint remains33,515 identities (933 functions); the new routines
are not yet integrated there. Consider byte-verified APFS sharing when writing
unchanged captures to avoid repeated storage exhaustion; actual execution guards
remain unchanged. No additional source classification is needed before integration.
Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Goal active; this turn made progress.

**Previous bounded recovery — 7 October 2026: movie critical section verified, R3/C8 partial.**

Original5FF932 and interrupt-mask enter/leave5FFA5C/5FFA74 are integrated
(39+12+11 instructions). The repaired unit passes **192 parent and128 lock cases**
across all16 interrupt masks, aliases, preserved status/FPU state and saved-mask
upper bits. Original enter/leave execute in18/16 cycles; parent boundaries are
**12/30/33 cycles**, and the busy-guard complete return remains15 cycles.

The initial expectation was wrong: it treated STC_SR/LDC_SR as ordinary
instructions, omitting pinned issue costs2/4. The first failed run hid the timer
assertion behind its missing-target catch; a diagnostic run exposed it. Both
failed runs and frozen sources are preserved. Corrected expectations pass all
state/timer checks. Runtime/compiler and twelve core patches were unchanged.
The previous13/12 helper and28 connected-cycle predictions are superseded.

Full startup passes **156 RAM/CPU/TMU and64 SQ/device comparisons**, including
the prospectively predicted failure, arena return,30 sprite packets, both supplied
scenes, ten audits and six unchanged images. All152 prior named captures,
provider return, continuation prefix and191 mode returns remain unchanged.
Original-image activation remains133 entries; module code remains unchanged
after eight audited data stores. Actual next target is **5FF980**, with
PC5FF952/PR5FF956, SR400000F1 and the saved interrupt mask0 at SPFFFF44.
Full allocation and its eventual interrupt-mask restoration remain unproved.

**Restore storage capacity before any further native execution.** Then generate
only four reviewed bodies:23-instruction search5FF980,6-instruction status reader
5FFA2C, and2/2-instruction setters5FFA24/5FFA28. Use existing real count/buffer
getters and parent/lock bodies. Verify complete search and complete allocation:
first/middle/last free slots, nonpositive count, exhaustion and busy owner;
include slot/global stores, restored mask and stack residues. Unexecuted source
predictions: first-free search42 cycles, parent131; five exhausted slots return
in201; nonpositive parent81; busy15. Two classification batches used: execute
before more review. Preserve155 prior named phases and map the156th predicted
terminal to the new search entry using PC-only normalization; preserve64 device
checks and original loading/stores.

Closure **933 functions** (859 base +74 supplemental;3,887 supplemental
instructions), cap860 and bounds256/8192 unchanged. Checkpoint **33,515 identities**,
SHA-256 `a0c87becd2ce38555d46f3b447a2d4815612fb4137f91400840cf928f0540e72`; all32,288 parent identities and9,117 preserved paths reverified.
Sharing316 exact RAM/AICA duplicates recovered2,994,991,104 bytes without removing
content. Actual execution guards were unchanged. Final availability
**270,684,160 bytes**: unit400MiB, full1536MiB and capture2816MiB guards
all fail. Preserve evidence and originals; recover capacity before another run.
Safety/pins pass. No game or Simulator remains live.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only request unchanged/unsent; production foundation undecided.
Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-critical-startup/`
(`movie-critical-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`) and
`20261007-startup-movie-critical-repair1/` (`timing-repair-audit.json`).

**Previous bounded recovery — 7 October 2026: resident movie initializer returns, R3/C8 partial.**

Original5FF860 and its3/7-instruction getters5FFA16/5FFA08 are integrated
(42 instructions total). The unit passes **192 argument/fill/PR/alias/FPU cases**:
real10EF3C is reached in37 cycles and the captured40-byte clear/full return
finishes in253; null buffers return in7, invalid signed counts in10. Independent
models verify the complete RAM/CPU/TMU/SQ/device state, stack residues, global
pointer/count/status and buffer clear. No helper or clear was substituted.

Full startup passes **152 RAM/CPU/TMU and60 SQ/device comparisons**, arena return,
30 sprite packets, both supplied scenes, ten audits and six unchanged images.
All149 previous named captures, provider return, continuation prefix and191 mode
returns remain unchanged. Module activation remains133 entries, with original
code unchanged after the same eight audited data stores. Native startup now
reaches **5FF932** with R4=0, PR=ECFD58 and guard677144=FFFFFFFF.

Next execute its39-instruction allocation/critical-section caller and original
12/11-instruction mask enter/leave5FFA5C/5FFA74. Predictions:12 cycles to enter,
13-cycle enter,28 connected cycles to still-missing5FF980; busy guard returns in15.
Enter saves the old interrupt mask at SP−16 and sets it to15; leave predicts12
cycles restoring that mask. Verify all16 masks, preserved status/FPU/registers,
16-byte frame and saved argument; retain real SR effects. These predictions are
unexecuted. Two classification batches used: execute before further review.
Preserve152/60 comparisons and keep the missing allocator explicit.

Closure **930 functions** (859 base +71 supplemental;3,825 supplemental
instructions),12 core patches, cap860 and bounds256/8192 unchanged. Checkpoint
**32,288 identities**, SHA-256 `6c403284422d145b100072676fc09461f2555ab4bad0779f386fa6787432258a`;
all31,175 parent identities and8,801 preserved paths reverified. Sharing309 exact
RAM/AICA duplicates recovered2,921,754,624 bytes; all contents preserved. Storage
selection used projected required capacity; actual execution guards were unchanged.
Final availability 932,827,136 bytes:400MiB unit guard passes;
1536MiB full and2816MiB capture guards fail. Restore capacity before a full run.
Safety/pins pass. No native repair was needed; no game or Simulator remains live.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only request unchanged/unsent; production foundation undecided.
Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-resident-init-startup/`
(`movie-resident-init-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`) and the
`20261007-startup-movie-resident-init/` unit.

**Previous bounded recovery — 7 October 2026: original movie zero return integrated, R3/C8 partial.**

Original ED0940 executes RTS/MOV #0,R0 and returns in three cycles. The unit
passes **32 input-R0/PR/alias/FPU cases and 9 activation controls**; complete
RAM/CPU/TMU/SQ/device state matches the independent model. Full startup passes
**149 RAM/CPU/TMU and 57 SQ/device comparisons**, arena return, 30 sprite packets,
both supplied scenes, ten audits and six unchanged images. All 147 previous
named captures, provider return, continuation prefix and 191 mode returns remain
unchanged. Original-image activation installs 133 entries; module code remains
unchanged after the same eight audited data stores.

The original parent advances through a 4,112-byte clear and resident5FFDAC /
60055C / 600588 calls, then rejects at **5FF860**. These later calls are observed;
their complete state is not independently modeled. The next original initializer
has 32 instructions and two direct getters, 5FFA16/5FFA08 (3/7 instructions).
Next generate these three bodies together and verify the complete return using
real10EF3C. Captured buffer EDE188/count5 predicts a40-byte clear and253 cycles;
null predicts7 cycles and non-null signed count<2 predicts10, returning−1.
Check saved PR/SP, stack residues, global pointer/count/status, clear bytes and
timers across argument/alias/FPU controls. These predictions remain unexecuted.
Two classification batches followed native execution: execute before more review.
Preserve149/57 comparisons and exact original-image activation in full startup.

Closure **927 functions** (859 base +68 supplemental;3,783 supplemental
instructions),12 core patches, cap860 and bounds256/8192 unchanged. Checkpoint
**31,175 identities**, SHA-256 `59e458b58ecc42b214c03cdbcba02efa41fcd4ba7a041487cb0beb278795efd4`;
all30,107 parent identities and8,492 preserved paths reverified. Sharing552 exact
RAM/AICA duplicates recovered3,381,149,696 bytes with all contents preserved.
Final availability 1,418,289,152 bytes:400MiB unit guard passes;
1536MiB full and2816MiB capture guards fail. Restore capacity before a full run.
Safety/pins pass. No native repair was needed; no game or Simulator remains live.

Normal boot/gameplay, movie/audio/save/mobile and original Katana/PAL G1 remain
unresolved. Source-only request unchanged/unsent; production foundation undecided.
Previous/current goal turns are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-return-zero-startup/`
(`movie-return-zero-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`) and the
`20261007-startup-movie-return-zero/` unit.

**Previous bounded recovery — 7 October 2026: movie guarded clear returns, R3/C8 partial.**

Original ED0A2C (14 instructions) now executes its real 10EF3C clear and returns
in full startup. The unit passes **80 state controls and 9 activation controls**:
signed zero/negative guards increment and clear 3,072 bytes in 15,381 cycles;
positive guards return in 7 without clearing. Captured entry-to-clear is 13
cycles. Independent models verify RAM, CPU, timers, store queues and device state.

Full startup passes **147 RAM/CPU/TMU and 55 SQ/device comparisons**, arena
return and 30 sprite packets. Both supplied regression scenes, ten audits and
six unchanged images pass on the same ARM64 binary. All 144 prior named captures,
provider return, continuation trace prefix and 191 mode returns remain unchanged.
Original-image activation installs 132 CFG entries. Exactly eight module data
bytes differ after original stores; all other module bytes remain unchanged.

Next missing target **ED0940** comes from original ECFDC8 and captured R2/PC/PR.
Its original two instructions are RTS with MOV #0,R0 in the delay slot. Next
verify its complete predicted three-cycle return, preserved PR/SP/RAM and R0=0;
then integrate while retaining 147/55 comparisons. Restore/replay only the eight
audited data words around original-image validation in isolated fixtures; keep
original loading and stores in full startup. One classification batch followed
native execution. The next return prediction is not executed proof.

Closure: **926 functions** (859 base + 67 supplemental; 3,781 supplemental
instructions), 12 core patches, base cap 860 and bounds 256/8192 unchanged.
Checkpoint **30,107 identities**, SHA-256
`bd3090ade1e92b73342a6dfe76dd266cd6d3e522cfa566b0c5b5a070c21990f8`; all 29,055 parent identities and 7,940 preserved paths reverified.
Sharing 749 identical RAM/AICA captures recovered 3,755,646,976 bytes while
preserving contents. Final available space: 1,470,611,456 bytes.
The 400 MiB unit guard passes; 1,536 MiB full and 2,816 MiB capture guards do not.
Restore capacity before another full run. Repository safety/pins pass; no repair
was needed and no game or Simulator remains live.

Normal boot/gameplay, movie playback, audio/save/mobile and original Katana/PAL
G1 remain unresolved. Source-only request unchanged/unsent; production foundation
undecided. This and the previous goal turn are **progress**; goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-guarded-clear-startup/`
(`movie-guarded-clear-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`) and the
`20261007-startup-movie-guarded-clear/` unit.

**Previous bounded recovery — 7 October 2026: movie record initialization returns, R3/C8 partial.**

Original ED5A4E (72 instructions) is integrated. Its independently modeled
record fields and eight active-FPU1 stores match all RAM/CPU/TMU/SQ/device state
at registered658BA0. The unit passes **48 flag/fill/PR/RAM-alias/FPU-bank controls
and9 activation controls**: nonzero flag71 cycles, zero69. Full startup executes
both resident658BA0 calls, returns from the parent and executes6620DC/661E00.
Complete resident return state remains outside the independent model.

Full startup passes **144 RAM/CPU/TMU and52 SQ/device comparisons**, arena return,
30 sprite packets, both supplied scenes, ten audits and six unchanged images.
All142 previous named captures, provider return, continuation trace prefix and
191 mode returns remain unchanged. The prior predicted failure maps to the new
record entry with PC-only normalization. Original-image activation installs129
CFG entries. Exactly seven module data bytes still differ; code is unchanged.
Observed final record flags are1008, fields32/76 are3 and eight float fields are1.

Next missing target **ED0A2C** is identified from original ECFDC4 and captured
R3/PC/PR. Its14 instructions test signed counter EDD0CC; actual0 takes the
increment-to1 and3072-byte clear at EE61C4 through real10EF3C. Next independently
model the full return: predicted13 cycles to clear,15,381 through return;
positive guard predicts7 cycles and no clear. Test signed positive/zero/negative,
wrap, fill patterns, aliases and preserved PR/SP. Restore/replay only the seven
audited module data words around original-image activation in isolated fixtures;
retain original load/stores and144/52 comparisons in full startup. One source
classification batch followed execution. Next predictions are not native proof.

Closure **925 functions** (859 base +66 supplemental;3,767 supplemental
instructions),12 core patches, base cap860 and bounds256/8192 unchanged.
Checkpoint **29,055 identities**, SHA-256
`76919d32c6f7be6d9bb80fa8453737854913eaaf72594ead7d61405a78bdcb5c`; all28,033 parent identities and7,191 preserved paths
reverify. Sharing176 exact duplicate diagnostic captures recovered2,951,913,472
bytes without removing content. Final available space: 1,041,133,568 bytes.
Enforce400MiB unit,1536MiB full and2816MiB capture guards before more execution.
Repository safety/pins pass. No repair was needed; no game or Simulator is live.

Normal boot/gameplay, playback, audio/save/mobile and original Katana/PAL G1
remain unresolved. Source-only request unchanged/unsent; no production foundation
decision. This and the previous goal turn are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-record-init-startup/`
(`movie-record-init-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`) and
`20261007-startup-movie-record-init/`.

**Previous bounded recovery — 7 October 2026: connected movie clears verified, R3/C8 partial.**

Original ED5712 (9 instructions) and ED5ADE (24 instructions) are integrated.
Both execute the existing10EF3C routine. Independent models verify the complete
404-byte and192-byte clears, first PR restoration, second saved PR and transfer
to missing **ED5A4E**. Each unit passes32 fill-pattern/PR/stack-alias/FPU controls
and9 activation controls. Combined startup boundaries match cycles
**7/2035/2039/2046/3014/3018** from ED5712 entry.

Full startup passes **143 RAM/CPU/TMU and51 SQ/device comparisons**, including
the prospectively predicted terminal state, plus arena return and30 sprite
packets. All136 previous named captures, provider return, continuation trace
prefix and191 mode returns remain unchanged. Both supplied scenes, ten audits
and six images pass on the same ARM64 binary. Original-image activation installs
125 CFG entries. Module bytes still differ only at seven audited data bytes;
original instructions are unchanged. No startup state was injected.

Next: verify **72-instruction ED5A4E** record initialization and its tail to
already compiled658BA0. Actual flag EDD0E8=1 selects the nonzero path; review
predicts71 cycles (zero path69), scalar fields and eight FPU1 stores at offsets
112…140. These predictions are not executed evidence. Test zero/nonzero flag,
RAM aliases and active FPU bank. Retain exact original-image activation with
seven-word restoration/replay only in isolated fixtures. When integrating,
replace the current predicted failure with the new entry (PC-only normalization),
preserve all142 named phases and add the resident-tail prediction. The next
body was reviewed from the observed unit call before this full run; no further
classification batch followed the full execution.

Closure: **924 functions** (859 base +65 supplemental;3,695 supplemental
instructions). Core patches12, base cap860 and continuation bounds256/8192 remain
unchanged. Checkpoint **28,033 identities**, SHA-256
`f4fbca79b97af7553a5b663bd0c0400b6176e4353aebbd89048625d15ddd5c02`; all26,958 parent identities and7,015 preserved paths
reverify. Sharing210 duplicate diagnostic captures recovered3,522,228,224 bytes
without removing content. Final available space: 1,329,041,408 bytes;
unit guard passes, full/capture guards do not. No game or Simulator remains live.
Repository safety/pins pass; both units and full execution needed no repair.

Normal boot/gameplay, playback, audio/save/mobile and original Katana/PAL G1
remain unresolved. Source-only acquisition request unchanged/unsent; production
foundation undecided. This and the previous turn are **progress**; goal active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-init-startup/`
(`movie-init-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`), plus
`20261007-startup-movie-clear-tail/` and `20261007-startup-movie-init/`.

**Previous bounded recovery — 7 October 2026: movie counter return verified, R3/C8 partial.**

Original ED09E0's five instructions now execute and return in full startup.
The complete six-cycle return matches the independent RAM/CPU/TMU/SQ/device
model: counter EDD098 changes 0→1, R3/R4 match, PR/SP/SR remain unchanged.
The isolated unit passes **32 counter-wrap/PR/stack-alias/FPU controls and
9 module-activation controls**, including unsigned overflow to zero.

Full startup passes **136 RAM/CPU/TMU and 44 SQ/device comparisons**, the arena
return and 30-packet sprite check. All 134 previous captures, provider return,
continuation trace prefix and 191 mode returns remain unchanged. Both supplied
scenes, ten audits and six images pass on the same ARM64 binary. Original-image
activation installs 120 CFG entries. The terminal module differs from its disc
image only at seven audited data bytes; instructions remain unchanged.

Next missing target: **`0x8CED5712`**, identified from original ECFDB8 and captured
R2/PR, not the address-free error. Review finds nine instructions: save PR,
call existing10EF3C to clear404 bytes at EFB54C, restore PR and transfer to
unregistered ED5ADE. Next verify its **seven-cycle prefix** to the real clear
callee (PR=ED571E, SP−4). In isolated fixtures restore/replay only the seven
audited module data words around exact original-image activation; require whole
captured RAM equality before execution. Full startup keeps original load/stores.
One classification batch has followed execution; retain 136/44 comparisons.

The build contains **922 functions** (859 base + 63 supplemental; 3,662 supplemental
instructions), with unchanged 12 core patches, base cap860 and continuation
bounds256/8192. Checkpoint: **26,958 identities**, SHA-256
`5554f4a4e3147eb1b67d7b65f0b899f7e6f563e95111750b9c48110e7d871865`. All25,996 parent identities and6,805 preserved paths
reverify. Sharing190 identical diagnostic RAM captures recovered2,868,334,592
bytes without removing content. Final available space: 1,187,573,760 bytes;
unit guard passes, full/capture guards do not. One preflight-only attempt caught
a mistyped historical checkpoint hash before compilation; its preserved evidence
and exact correction are in `preflight-repair-audit.json`. No native repair was
needed. Repository safety/pins pass; no game or Simulator remains live.

Normal boot, gameplay, playback, audio/save/mobile acceptance and original
Katana/PAL G1 remain unresolved. The source-only request remains unchanged and
unsent; no production foundation decision has been made. This and the previous
goal turn are **progress**; the goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-counter-startup-repair1/`
(`movie-counter-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`) and
`20261007-startup-movie-counter-return/`.

**Previous bounded recovery — 7 October 2026: movie configuration helper returns, R3/C8 partial.**

Original ECFCE4 (97 instructions) and ED5614 (42 instructions) are integrated.
Each isolated unit passes 16 state controls and 9 module-activation controls;
its captured prefix takes 6 and 15 cycles respectively. Full startup executes
the helper's four existing clear calls and scheduler call, returns, and executes
647D62. The next missing module target is **`0x8CED09E0`**, identified from the
original caller/literal and captured registers. Its five instructions increment
EDD098 and return; the predicted six-cycle return remains to be tested.

Full startup passes **134 RAM/CPU/TMU and 42 SQ/device comparisons**, the arena
return and 30-packet sprite check. All 131 prior captures, provider return,
continuation trace prefix and 191 mode returns remain unchanged. The same ARM64
binary passes both supplied scenes, ten audits and six unchanged images.
Activation validates the original movie image before installing 119 CFG entries.
At the terminal capture, exactly six data bytes differ: four configuration stores,
scheduler result 13 at EDD0D4 and counter 1 at EDD0D8. All other module bytes are
unchanged. Complete helper return state was not independently modeled.

The build contains **921 functions** (859 base + 62 supplemental; 3,657 supplemental
instructions), with 12 core patches and unchanged 256/8192 continuation bounds.
Checkpoint: **25,996 identities**, SHA-256
`f8b7c5df1152c16e4a42fd806c123852b76aef49de263b47f8ac3670a8798229`. All 25,012 parent identities and 6,615 preserved paths
reverify. Sharing 230 identical diagnostic RAM captures recovered 3,640,020,992
bytes without removing content. Final availability: 1,423,790,080 bytes;
unit storage guard passes, full-run/capture guards do not. No game or Simulator
is live. Repository safety and pins pass.

Next: generate only ED09E0 and verify its whole-RAM/CPU/timer return, including
counter wrap. For isolated activation, restore only the six audited data words,
validate the original image, replay them and require exact captured RAM equality.
Full startup must retain its original load and stores. One classification batch
has followed execution. Preserve all 134/42 comparisons and storage guards.

Normal boot, gameplay, playback, audio/save/mobile acceptance and original
Katana/PAL G1 remain unresolved. The source-only request is unchanged and unsent;
no production foundation decision has been made. This turn is **progress**;
the preceding status-only turn was **no progress**. The goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-helper-startup/`
(`movie-helper-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `next-startup-preflight.json`), plus the
`20261007-startup-movie-config-prefix/` and `20261007-startup-movie-helper-prefix/` units.

**Previous bounded recovery — 7 October 2026: original movie mode code returns, R3/C8 partial.**

Original608700 (231 instructions) is integrated. Its captured prefix passes
**32 flag/parameter/stack-alias/FPU controls** and a **52-cycle** whole-RAM /
CPU/TMU/SQ/device comparison to registered6082D4. The44-byte frame preserves
four GPRs, three active-FPU values and PR, with12 local bytes. Registration
preserves all prior resident bindings. The unit deliberately stops before the
resident call; full startup executes it and returns into MOVIE. Complete mode
return state and display behavior are not independently proved.

Full startup passes **131 planned RAM/CPU/TMU comparisons** and **39 SQ/device
comparisons**, the prospective arena return and30-packet sprite check. All129
prior named captures, provider-return capture, complete continuation trace prefix
and191-return mode-log prefix remain unchanged. The exact ARM64 binary passes
two supplied-scene regressions, ten audits, release controls and six unchanged
images. Original91-sector loading and87-alias identity-bound activation remain
verified; default256/explicit8192 continuation limits are unchanged.

After **5,391 resumes**, managed dispatch rejects uncompiled module function
**`0x8CECFCE4`**. The error contains no target address: originalEB00D6 loads the
target fromEB01C0, matching capturedR3 and PR=EB00DA, with R4=ECEFEC. Review
finds97 instructions/20 call sites; firstED5614 is not registered. The whole
module now differs from its original image by exactly four configuration-word
stores **1/5/2/2 atECEFEC** (four changed bytes). All other bytes, including
instructions, remain original. Provider-return bytes still match the disc;
terminal bytes correctly reflect these source-derived data stores.

Next generate onlyECFCE4 and its module aliases. In the isolated unit, first
prove the captured image equals original plus those four stores; restore only
those words, activate against the exact original image, then replay the stores
and require the whole RAM snapshot to match before execution. Model the
six-cycle R14/PR save, literal/JSR and R4-to-R14 delay slot to missingED5614:
PC=ECFCE4, PR=ECFCEE, SP-8. Test argument/PR/stack controls. Full startup keeps
its original load and stores; do not weaken module validation or inject an
alternative image. One classification batch used; preserve131/39 checks.

Closure859+60=**919 functions**,3518 supplemental instructions; base cap860 and
12 core patches remain unchanged. Checkpoint **25,012 identities**,
SHA-256 `668dcf02db22bdd96cc4f2ab535634d95f30edfa96a4caec8e640907a7991897`.
All24,104 prior identities and6,385 preserved paths reverify. Sharing32 redundant
generated RAM captures raised measured availability by533,360,640bytes without
removing content or changing original inputs. Final availability
901,484,544bytes meets unit400MiB but not full1536MiB/capture2816MiB.
Repository safety and pins pass; no game or Simulator remains live.

Config/module completion, playback, normal boot/gameplay/audio/save/mobile and
original Katana/PAL G1 remain unproved. The exact source-only acquisition request
is unchanged and unsent. No production foundation decision has been made.
This and the previous goal turn are **progress**; the goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-mode-startup/`
(`movie-mode-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`module-data-mutation-audit.json`, `movie-load-audit.json`,
`next-startup-preflight.json`) plus `20261007-startup-movie-mode-prefix/`.

**Previous bounded recovery — 7 October 2026: original movie tail transfer integrated, R3/C8 partial.**

Original644492's four instructions now execute in startup. Its isolated unit
passes16 register/argument/PR/stack-alias/FPU controls and a **five-cycle**
whole-RAM/CPU/TMU/SQ/device comparison to registered657AA8. R3 becomes657AA8,
R4 becomes8947E4, and RAM/PR/SP remain unchanged. The unit deliberately stops
before that resident routine; full startup executes onward without replacing it.

Full startup passes **129 planned RAM/CPU/TMU comparisons** and **37 SQ/device
comparisons**, the prospective arena return and30-packet sprite check. All127
prior named captures, provider-return capture, complete continuation trace prefix
and191-return mode-log prefix remain unchanged. Exact-binary supplied scenes
pass ten audits, release controls and six unchanged images. Original91-sector
MOVIE loading and87-alias quiescent identity-bound activation remain verified.

After5,357 resumes the service reaches missing **`0x8C608700`**, atPC10D4DA /
PR10D4EA, with original arguments **R4/R5/R6=1/5/2**, SPFFFF54. Review finds231
instructions and10 call sites; the captured prefix calls already registered
6082D4. Next generate only this reviewed body and independently model its four
GPR/three active-FPU saves, PR and12 locals, original numeric stores and first
resident call. Keep low mode bits1 while varying flag bits4/5, stack aliases and
FPU bank in controls. Stop explicitly before6082D4 in the unit, then execute it
in full startup. Before that call, captured flags select320/240 stores; later
code can change these, so this is not completed mode setup or display proof.
One classification batch used; retain129/37 checks and provider/scene evidence.

Closure859+59=**918 functions**,3287 supplemental instructions; base cap860,
12 core patches and default256/explicit8192 continuation limits are unchanged.
Checkpoint **24,104 identities**, SHA-256
`6fb57c1a673e4dc2364b7b29f846732a688736bb83ddeef7d9510713bcdcdb3b`.
All23,214 prior identities and6,353 preserved paths reverify. Sharing2,780 exact
duplicate2MiB AICA-memory observations preserved every byte/path/required
metadata and raised measured availability by5,837,344,768bytes. Original
sources/discs/assets/saves were excluded; this changes storage sharing only.
Final availability3,355,471,872bytes meets unit400MiB, full1536MiB and capture2816MiB guards.
Repository safety and pins pass; no game or Simulator remains live.

Complete tail-callee return state, movie service completion, playback, normal
boot/gameplay/audio/save/mobile and original Katana/PAL G1 remain unproved.
The exact source-only acquisition request is unchanged and unsent. No production
foundation decision has been made. This and the previous goal turn are
**progress**; the goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-tail-startup/`
(`movie-tail-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`movie-load-audit.json`, `next-startup-preflight.json`) plus
`20261007-startup-movie-tail-prefix/` and `20261007-movie-tail-aica-storage/`.

**Previous bounded recovery — 7 October 2026: original movie table scan returns, R3/C8 partial.**

Original10D4C6 (56 instructions) and60C534 (89 instructions) are integrated.
Their isolated reductions each pass16 saved-state/stack-alias/FPU controls:
**five cycles** reach the first missing60C534 boundary; **36 cycles** reach
registered657AA8 from the table scan. The second unit intentionally stops before
that resident call. Full startup executes the original callees and returns from
the scan:57 active records become0 across the captured2048-slot table, with the
terminal markerFFFFFFFF. Those table fields are observed comparisons, not an
independent complete-return or called-routine model.

Full startup passes **127 planned RAM/CPU/TMU comparisons**, **35 SQ/device
comparisons**, the prospective arena return and30-packet sprite check. All124
prior named captures, provider-return capture, complete prior continuation trace
prefix and191-return mode-log prefix remain unchanged. Exact-binary supplied
scene regressions pass ten audits, release controls and six unchanged images.
Original91-sector MOVIE loading and87-alias quiescent identity-bound activation
remain intact; no original input, runtime implementation or startup state was
substituted. Default256/explicit8192 continuation bounds are unchanged.

After5,357 resumes, the original service reaches missing **`0x8C644492`** at
PC10D4CE/PR10D4D4/SPFFFF54. Review finds a four-instruction tail transfer to
already registered657AA8, with argument8C8947E4. Next generate only this body and
model its five-cycle literal/literal/JMP/NOP prefix: R3=657AA8, R4=8947E4,
PC=657AA8, PR/SP unchanged. Verify captured and argument/PR/stack controls with
an explicit registered-callee entry stop before full integration. One new
classification batch used; keep127/35 checks and provider/scene evidence.

Closure859+58=**917 functions**,3283 supplemental instructions; base cap860 and
12 core patches remain unchanged. Checkpoint **23,214 identities**,
SHA-256 `db07fe1887d9451412a156d404afab57c1db9dbed78d2edb63a838ff35c5adb6`.
All22,296 prior identities and3,573 preserved paths reverify. Sharing187 exact
duplicate16MiB RAM predictions/observations reclaimed3,083,218,944 measured
bytes while preserving all content/paths/required metadata; originals, sources,
discs, assets and saves were excluded. Availability890,818,560bytes
meets unit400MiB but not full1536MiB/capture2816MiB. Repository safety and pins
pass; no game or Simulator remains live.

Complete scan return state, movie service completion, playback, normal boot,
gameplay/audio/save/mobile and original Katana/PAL G1 remain unproved. The exact
source-only acquisition request is unchanged and unsent. No production
foundation decision has been made. This and the previous goal turn are
**progress**; the goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-service-startup/`
(`movie-service-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`movie-table-observation.json`, `movie-load-audit.json`,
`next-startup-preflight.json`) and the `startup-movie-service-prefix` /
`startup-movie-setup-prefix` unit directories dated20261007.

**Previous bounded recovery — 7 October 2026: original MOVIE callback integrated, R3/C8 partial.**

The observed13-instruction callback8CEB06F4 is compiled and included with the
movie entry in the same quiescent, exact-image activation: **87 CFG aliases**.
No module bytes are injected into full startup and no runtime implementation is
replaced. The isolated callback prefix passes16 argument/PR/stack-alias/FPU
controls plus9 activation/retirement controls. Its PR-save/literal/JSR/NOP
sequence reaches registered648422 in **five guest cycles**; the isolated test
intentionally stops before that resident routine.

Full original startup executes onward and passes **124 planned RAM/CPU/TMU
comparisons** and **32 SQ/device comparisons**, the prospective arena return and
30-packet sprite check. All122 prior named captures, provider-return capture,
complete prior continuation trace prefix and191-return mode-log prefix remain
unchanged. The same ARM64 binary passes two supplied-scene regressions, ten
audits, release controls and six unchanged images. The original91-sector movie
load still matches independent owner-disc extraction, including padding.

After **5,357 resumes**, execution reaches missing resident **`0x8C10D4C6`** from
MOVIE block8CEB0096, PR8CEB009C, R4=0, SP8CFFFF5C. Its56 original instructions
and8 call sites are reviewed; the first target60C534 is currently unregistered.
The captured routine/literal bytes match the bound BOOT image. Other missing
callees644492/608700 are an inventory, not a reason to translate speculative
closure. Complete callback return state, movie execution, playback, normal
boot/gameplay/audio/save/mobile and original Katana/PAL G1 remain unproved.
The exact source-only acquisition request is unchanged and unsent.

Closure859+56=**915 functions**,3138 supplemental instructions; base cap860,
12 core patches and default256/explicit8192 continuation limits are unchanged.
Checkpoint **22,296 identities**, SHA-256
`c08bf89d2fc1fb8c20650eabaa1f1b60164c1224cc7e66862d97105c9f7342d4`.
All21,442 prior identities and3,386 preserved paths reverify. Sharing212 exact
redundant generated RAM captures reclaimed2,996,699,136 measured bytes. Sixteen
of those share identical contents across different observation-phase names;
all bytes/paths/required metadata remain preserved. Originals/discs/assets/saves
were excluded. Availability838,938,624bytes meets unit400MiB but not
full1536MiB/capture2816MiB. No game or Simulator remains live. Repository safety
and pinned references pass. No production foundation decision has been made.

Next generate only10D4C6 and independently model its five-cycle prefix: save PR,
load60C534, JSR with R4 pushed in the delay slot. Expected missing-target state
keeps PC10D4C6, PR10D4CE and decrements SP by8. Verify captured state and
argument/PR/stack controls before adding more bodies. Keep124/32 checks,
identity-bound activation and provider/scene evidence. One classification batch
used; execute a native reduction before broader source review.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-callback-startup/`
(`movie-callback-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`movie-load-audit.json`, `next-startup-preflight.json`) plus
`20261007-startup-movie-callback-prefix/`. This and the previous goal turn are
**progress**; the goal remains active.

**Previous bounded recovery — 7 October 2026: loaded MOVIE entry executes, R3/C8 partial.**

Original startup loads MOVIE.BIN from the owner CDI, then activates its compiled
entry through the pinned module API after the provider returns and host dispatch
unwinds to depth zero. Activation compares all184,592 loaded bytes to the bound
reference, installs83 reviewed CFG aliases and advances no guest cycles. The
reference is never injected into full-startup RAM. Reload into an active slot
is rejected pending a separately verified quiescent retirement path.

Full startup passes **122 planned RAM/CPU/TMU comparisons** and **30 SQ/device
comparisons**, the prospective arena return and30-packet sprite check. All120
prior named captures, provider-return capture, complete prior continuation trace
prefix and191-return mode log remain unchanged. Exact-binary supplied-scene
regressions pass ten audits, release controls and six unchanged images.
The original366-instruction entry's captured44-cycle prefix to608F74 matches its
independent model; the isolated unit passes48 execution and9 activation controls.
That unit deliberately stops before608F74; full startup executes onward.

After **5,342 resumes**, managed dispatch rejects the uncompiled movie callback
**`0x8CEB06F4`**. The error text has no target address: original649872/649874 and
captured node[R14=8C896B80]/R3 establish the target, PR649878 and argument R4=0.
Review finds13 original instructions and four calls, all to registered resident
routines. This is the next bounded native test; complete movie execution,
playback, normal boot, gameplay/audio/save/mobile and original Katana/PAL G1
remain unproved. The exact source-only acquisition request remains unchanged
and unsent. No production foundation decision has been made.

Closure859+55=**914 functions**,3125 supplemental instructions; base cap860,
12 core patches and default256/explicit8192 continuation limits are unchanged.
Checkpoint **21,442 identities**, SHA-256
`538387da6dc5363a1a7c82064ef9b43413e4de302d3a58a0f5f75b292a4a3320`.
All20,609 prior identities and3,174 preserved paths reverify. Sharing237 exact
redundant generated RAM captures reclaimed3,015,925,760 measured bytes while
preserving content; originals/discs/assets/saves were excluded. Availability
622,751,744bytes meets unit400MiB, not full1536MiB/capture2816MiB.
No game or Simulator remains live. Repository safety and pinned references pass.

Next generate only callbackEB06F4, bind its CFG aliases to the same loaded module
identity and independently model its PR-save/literal/call prefix to registered
648422. Test captured state and PR/stack/FPU controls before full integration.
Keep122/30 checks, quiescent activation, provider and exact-binary regressions.
One classification batch used; do not broaden to its registered callees.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-entry-startup/`
(`movie-entry-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`movie-load-audit.json`, `next-startup-preflight.json`) and
`20261007-startup-movie-entry-prefix/`. Goal turn classification: **progress**;
the goal remains active.

**Previous bounded recovery — 7 October 2026: original MOVIE.BIN load verified, R3/C8 partial.**

Original09F660's two-instruction BRA/NOP transfer is integrated. Full startup
passes **120 planned RAM/CPU/TMU comparisons**, **28 SQ/device comparisons**,
the prospective arena return and30-packet sprite check. All118 previous named
captures, continuation trace and191-return mode log remain unchanged. The exact
binary passes both supplied-scene regressions: ten audits, release controls and
six unchanged images. Default256/explicit8192-resume bounds remain unchanged.

The original file provider returns to10D492 with R0=0 after **91 sectors /
186,368 transferred bytes**,38,387 guest cycles after entry. An independent
read of the pinned owner CDI's extent199534 verifies **184,592-byte MOVIE.BIN**;
all186,368 bytes including sector padding match RAM at8CEB0000 both at provider
return and the final failure. The entry bytes were not already identical.
Module SHA-256:
`f6a69684cd83a4e79cd03747dca008546374e022e3d1368ed2de3d7d915bb6c7`.
This verifies file bytes and the observed return, not the complete provider
state or movie playback. The original loader then reaches missing AOT module
entry **`0x8CEB0000`** after5,331 resumes, PC10D49C/PR10D4A2/R4=0/SPFFFF9C.

The transfer reduction passes16 argument/PR/stack/bank controls and a2-cycle
whole-RAM/CPU/TMU/SQ/device comparison. It intentionally stops before the
registered09E8A0 provider; actual file loading is proved by the full run above.
The callback/supplemental closure is859+54=**913 functions**,2759 supplemental
instructions; base cap860 and12 core patches are unchanged. No runtime or
service implementation was replaced, and no startup game state was forced.

Checkpoint **20,609 identities**, SHA-256
`817911e84596b643ca6822a9329d809cd9e1b5ea7b4b9f17795ba89ab71d258e`.
All19,795 prior identities and2,937 preserved paths reverify. Sharing213
byte-identical generated RAM captures reclaimed2,883,600,384 measured bytes;
source/discs/assets/saves were excluded. The final16-file share restores unit
capacity after full capture/regressions. Availability666,546,176bytes
meets unit400MiB, not full1536MiB/capture2816MiB. Pins, ARM64/macOS13/SDK27 and
repository safety pass. No game or Simulator remains live.

Next generate only the original366-instruction MOVIE entry at8CEB0000 from
these verified bytes. Its35 call sites are an inventory, not a request to
translate all callees. Model the saved GPR/FPU frame and captured prefix to
first registered608F74(R4=8CEB06F4); actual mode argument is0 and platform
word[8C7649EC] is1. Bind AOT registration/dispatch to the loaded module identity
and keep overlay lifetime explicit. Do not inject bytes into full startup as
a substitute for its original file load. One classification batch used.
Keep120/28 checks and provider/scene evidence. Module execution, video playback,
normal boot/gameplay/audio/save/mobile and original Katana/PAL G1 remain
unproved. The exact source-only acquisition request is unchanged and unsent.

Evidence: `artifacts/recovery-aot/20261007-startup-file-transfer-startup/`
(`file-transfer-startup-checkpoint.json`, `movie-load-audit.json`,
`startup-frame-sprites-audit.json`, `next-startup-preflight.json`) plus
`20261007-startup-file-transfer-prefix/`. This and the previous goal turn are
progress; the goal remains active.

**Previous bounded recovery — 7 October 2026: original loader entry and null-owner return, R3/C8 partial.**

Original10D460 and1096CA are integrated. Full startup passes **118 planned
RAM/CPU/TMU comparisons**, **26 SQ/device comparisons**, the prospective arena
return and actual30-packet sprite check. All114 previous named captures,
continuation trace and191-return mode log remain unchanged. The same binary
passes both supplied-scene regressions: ten behavior audits, release controls
and six unchanged images. It executes **5,331 resumes**, then rejects missing
**`0x8C09F660`** while requesting **`MOVIE.BIN`** at destination8CEB0000.

The two added original bodies contain51+14 instructions. The callback/supplemental
closure is859+53=**912 functions**,2757 supplemental instructions; base cap860,
12 core patches, default256 and explicit8192-resume experiment remain unchanged.
Each new reduction passes16 controls. The loader's saved-PR/argument prefix
matches whole RAM/CPU/TMU/SQ/device state at6cycles; the captured null-owner
return and next-call entry match at9/13cycles relative to owner entry.

Counterevidence is retained. The loader's first prediction used call-PC10D466
instead of the emitted block-PC10D460. The next-callee inventory missed the
provider-generated registration for609950; it is reused, not reimplemented.
An isolated delegated run timed out after a CH2 transfer with its device clock
disabled; its exact wait cause is unproved. The reduced unit stops explicitly
before registered609950 execution and claims only entry-state verification.
Full startup then executes the real provider with device scheduling and passes
that boundary, reaching09F660. No game flags or service results were forced.
The nonzero-owner branch and complete10D460 return remain unproved.

Checkpoint **19,795 identities**, SHA-256
`0e65142640939cfbee489a3dfc00aafcadc3033a94a9ec965722f766cbdc2247`.
All18,864 previous identities and2,724 preserved paths reverify. Sharing177
byte-identical generated RAM captures reclaimed2,666,811,392 measured bytes;
source/discs/assets/saves were excluded. Final availability567,685,120
bytes meets unit400MiB, not full1536MiB/capture2816MiB. Pins, ARM64/macOS13/SDK27
and repository safety pass. No game or Simulator remains live.

Next emit original09F660's BRA/NOP transfer to already registered09E8A0,
using the captured MOVIE.BIN/destination arguments. Include provider registration
sources in availability checks. Test actual file loading in full startup with
the disc provider/device clock; do not assume the later8CEB0000 code is loaded.
One classification batch used. Preserve118/26 checks and restore capture space
before another full run. Original Katana/header/generated/provider/media/PAL G1
remains unresolved; source-only request unchanged/unsent. Normal boot, visible
movie/gameplay, audio, saves and mobile acceptance remain unproved.

Evidence: `artifacts/recovery-aot/20261007-startup-mode-loader-startup/`
(`mode-loader-startup-checkpoint.json`, `startup-frame-sprites-audit.json`,
`next-startup-preflight.json`) plus the preserved loader/owner unit attempts.
This and the previous goal turn are progress; the goal remains active.

**Previous bounded recovery — 7 October 2026: original startup mode transitions, R3/C8 partial.**

The extended startup executes **5,305 resumes** and **191 completed mode returns**,
advancing original modes **1 → 2 → 15 → 0** without forcing guest state. Mode2
begins on the152nd return, mode15 on190, and mode0 on191. The next iteration
rejects missing original **`0x8C10D460`** at caller055C58/PR055C60/SPFFFFA4,
R4=0. Normal boot and visible presentation remain unproved.

All **114 planned RAM/CPU/TMU comparisons**, **22 SQ/device comparisons**, the
prospective arena-return check and actual30-packet sprite stream still pass.
The entire previous1,725-row continuation trace is an unchanged prefix.
Both supplied-scene regressions pass with this exact binary: ten behavior
audits, release controls and six unchanged images. The closure remains910
functions (859 base +51 supplemental),2692 supplemental instructions,12 core
patches and base cap860. No guest body or runtime implementation changed.

The new prospective counter/mode/parent checks pass. A separate linear
adjustment expectation was too broad: visit33 gives255, not264. The already
reviewed and tested09D0D0 routine clamps OLD values above255 before adding;
its existing source model matches all191 returns retrospectively. Preserve
`mode-prediction.json` and `mode-progress-audit.json`; this correction is not a
new prospective whole-frame proof. The guest implementation was not changed.

The8192-resume experiment follows measured26–27-resume steady frame intervals;
the default remains256. Native execution ended at a concrete missing target,
not the extended budget. Sharing117 byte-identical generated RAM captures
reclaimed1,962,868,736 measured bytes; source/disc/assets/saves were excluded.
Checkpoint **18,864 identities**, SHA-256
`9b117fb48a5e6ab6850e2dafeaa208978b2af781bc587baae589a93d3de0c289`.
All18,154 prior identities and2,547 preserved paths reverify. Final capacity
928,382,976bytes meets unit400MiB, not full1536MiB/capture2816MiB. Pins,
ARM64/macOS13/SDK27 and repository safety pass. No game or Simulator remains live.

Next execute the source-derived prefix of original51-instruction10D460 using
its captured state, following the actual first1096CA call before expanding
other callees. One classification batch used. Its later8CEB0000 loaded target
is not assumed present. Preserve114/22 comparisons and the mode-transition
checkpoint. Restore capture capacity before another full run; keep all guards.
Original Katana/runtime/generated/header/provider/media and supported PAL G1
remain unresolved; the source-only request is unchanged and unsent. Visible
boot, gameplay, audio, saves and mobile acceptance remain open.

Evidence: `artifacts/recovery-aot/20261007-startup-mode-transition/`
(`mode-transition-checkpoint.json`, `mode-progress-audit.json`,
`startup-frame-sprites-audit.json`, `next-startup-preflight.json`). This turn
and the previous measured-budget turn are progress; the goal remains active.

**Previous bounded recovery — 7 October 2026: integrated sprite sequence and frame return, R3/C8 partial.**

Full startup passes **114 planned whole-16-MiB RAM / 69 CPU / 25 timer comparisons**,
**22 store-queue/device comparisons**, and the separate prospective arena-return
check. All101 previous named phases remain unchanged, including their AICA/flash
captures. The old missing090E40 boundary maps to its registered entry by PC only;
RAM, the other68 CPU fields and25 timer fields remain identical.

Original090E40 and60A1F0 are integrated:859 base +51 supplemental = **910 functions**,
2692 supplemental instructions. These two bodies add108 instructions; base cap860
and12 core patches are unchanged. The six original sprite calls return. Full
startup observes an empty TA stream at their entry and exactly **30 appended
packets / 960 bytes** at exit, matching the isolated sequence's bytes and tagged
source/PC provenance. Header bytes have an independent source model; the complete
sprite vertex/renderer behavior is not independently proved by this match.

The original frame epilogue and parent zero-return path pass16 controls and
connected **7/19/28/32/40/55-cycle** RAM/CPU/TMU/SQ/device predictions. The real
04562E draw is skipped because its captured flag is zero,08C25E increments the
counter6→7, and the parent returns to0556E0. The earlier eight-cycle epilogue
prediction overcounted ordinary branches; the pinned issue model and emitted
blocks support seven cycles. The failed unit run is preserved; runtime timing
was not changed. Two full-run observer corrections are also preserved: tail
captures now wait for the first sprite-sequence exit, and mode return observes
all existing continuation aliases to catch the first resumed iteration.

Both supplied-scene regressions pass against this exact binary: ten behavior
audits, release-corruption controls and six unchanged images. Full startup then
reaches the unchanged **1024-resume experiment limit** (1025 pre-resume snapshots,
including the rejected next resume). No missing function was encountered. The
terminal frame counter is **27**, adjustment216, mode1; the parent counter is33.
The last128 snapshots traverse several frame/scheduler paths. The final wait flag
8A7340 is0, so a stalled wait is **not established**. The original mode1 branch
changes mode only when the old counter exceeds150, on its152nd visit. The current
budget has not reached that transition. Default256 remains unchanged.

Checkpoint: **18,154 identities**, SHA-256
`eabf1ab731e583fa5d29cb6c1784d4168235b49526232345bba9cc40edc91e04`.
All16,114 prior identities and2,430 preserved diagnostic paths reverify; old
observer sources remain frozen. Pins, ARM64/macOS13/SDK27 and repository safety
pass. Sharing blocks between637 identical generated RAM captures reclaimed
7,955,877,888 measured bytes, preserving contents, paths and checked metadata;
original sources/discs/assets/saves were excluded. Final free space1,551,798,272
bytes meets unit400MiB but not full1536MiB or the **2816MiB capture-plan guard**
for114 captures. No game or Simulator remains live.

Next measure continuations per completed mode1 iteration and its original wait
predicate/callback64542C, IRQ acknowledgement and counter changes. Use that
measurement plus the152-visit source threshold to justify a larger bounded
experiment if needed; do not force mode/flags or assume the loop is stalled.
Restore capture capacity first, retain114/22 comparisons and the same-binary
scene regressions, and preserve default256. Visible presentation, normal boot,
gameplay, audio, saves and mobile acceptance remain unproved.

Evidence: `artifacts/recovery-aot/20261007-startup-frame-sprites-startup-repair2/`
(`startup-frame-sprites-audit.json`, `frame-sprites-startup-checkpoint.json`,
`final-verification.json`, `next-startup-preflight.json`) and
`20261007-startup-frame-return-repair1/`. Original G1 is still unresolved under
the latest source audit: matching Katana/runtime/generated/header/contracts
sources and supported PAL input remain missing. The exact source-only request
is unchanged and unsent. This and the preceding goal turn are progress.

**Previous bounded recovery — 7 October 2026: setup epilogue and record tail, R3/C8 partial.**

Original `045618` passes 64 value/overlap/alias/bank controls and the captured
five-cycle word-copy return. The existing `090EB0` epilogue and `090F0C` four
global clears/real `089F20` tail entry match independent whole-RAM/CPU/timer
predictions at cumulative **5 / 10 / 23 cycles**. The delegated record routine
returns in the isolated run, but its complete return state remains observed only.

Full startup passes **78 planned whole-16-MiB RAM / 69 CPU / 25 timer comparisons**,
retaining all74 prior expectations, plus the unchanged prospective arena-return
comparison. Same-binary supplied-scene regressions, ten behavior audits, release
controls and six unchanged images pass. It reaches missing **`090F2A` after310
continuations**. One original four-instruction body was added: 859 base +41
supplemental =900 functions. Base cap860, 12 core patches and default256/measured1024
continuation limits are unchanged. No guest or service implementation was stubbed.

Next execute original91-instruction `090F2A`, whose actual mode is1 and counter0.
Its six-cycle prefix saves R14/PR and calls registered `052700`; `052944` is also
registered, while the later `052766` is missing. Follow actual execution before
assuming which call becomes the next blocker. One classification batch used.
The complete setup/record return, normal boot, gameplay/audio/save/mobile and
original Katana/PAL G1 remain unverified. Source-only request unchanged/unsent.

Checkpoint: 13,681 identities, SHA-256
`be5ff2eaa01989696c3a8fce21055b2ace2c83c35838214d9c1a85c22763ee7e`.
All13,139 prior identities and1,093 preserved diagnostic paths reverify.
ARM64/macOS13/SDK27 and donor pins pass. Measured availability926,146,560bytes:
unit400MiB met, full1536MiB unmet. Restore capacity before another full run;
all storage guards remain unchanged.

Evidence: `artifacts/recovery-aot/20261007-startup-global-word-copy-startup/` and
`20261007-startup-global-word-copy-return/`. Storage recovery shared disk blocks
between140 byte-identical generated RAM captures while preserving bytes, paths
and metadata; original inputs were excluded. All native runs terminal; no Simulator.
This and the preceding goal turn are progress.

**Previous full-startup recovery: 20261006-235341, R3/C8 partial; original G1 unresolved.**
Original010B7A now returns in82 modeled cycles through real607124/64B9E4.
All38 whole-RAM/CPU/timer comparisons pass, preserving36 prior expectations;
exact-binary scene regressions and six images pass. Its native reduction passes
7,936 cases. Closure859/83993/2396 fits unchangedcap860;51guards pass,20sources
frozen,229 diagnostic paths preserved and3,748 identities bound. New missing
10B992 is observed only: actual gate2 selects a four-record loop with missing
10B906/10B226/10B2E0 and registered10A3F2. Model and execute the causal path
without helper substitutes; retain all38 expectations. One classification
batch used. Check capacity before build. Katana/PAL and normal boot/gameplay/
audio/save/mobile/production remain open; all terminal/no Simulator.
See [STATUS.md](STATUS.md) for exact scope, identities and failures.

Updated: **5 October 2026**. Private-only. There is no game/core app build recipe
yet because the required Katana/generated source and supported disc are absent.

## Public-source recovery experiment

The authoritative current recovery result and next step are the latest checkpoint above. Original G1 and production acceptance remain unresolved.

Current full-startup evidence is under
`artifacts/recovery-aot/20261007-startup-global-word-copy-startup/`.
Use fresh output names and preserve prior failures:

```sh
python3 work/dreamcast-recovery/run-startup-global-word-copy-startup.py NEW_OUTPUT_NAME
python3 work/dreamcast-recovery/audit-startup-global-word-copy-startup.py NEW_OUTPUT_NAME
python3 work/dreamcast-recovery/baseline-loop-regression.py NEW_OUTPUT_NAME
```

The runner writes78 predictions before execution, retaining all74 prior phases.
It adds only the original045618 body and observes its caller's original continuation
bindings across disc/IRQ unwinds. The audit also preserves the independent arena
return prediction. Existing GD-ROM HLE and the declared disc map remain in use;
the measured1024 ceiling is explicit and default256 unchanged. Missing090F2A
is the next measured boundary, not successful normal boot.

The isolated recipe is
`work/dreamcast-recovery/run-startup-global-word-copy-return.py NEW_OUTPUT_NAME`.
It checks64 controls, then exact5/10/23cycle word-copy, setup epilogue and four
global-store/record-tail entry boundaries. It delegates the real record routine
and records its return without claiming a complete independent return oracle.
The preceding arena and scalar-setter recipes remain available; their isolated
missing001006 result is a service/disc-state fixture limitation, not missing
Dreamcast game code. Do not translate that HLE address or fake its result.

This cached experimental recipe requires the declared private inputs and retained
build artifacts; it is not a clean production bootstrap. The base 859-function
catalog remains at cap 860; 41 added bodies were separately bounded and verified,
without claiming complete cold-path coverage. Storage guards remain 1,536 MiB
initially, 900 MiB before compilation/link and 600 MiB before full execution;
unit tests retain 400 MiB. Remeasure capacity before the next run. Restore the
full guard before integration; do not lower it.



```sh
python3 scripts/run-recovery-aot.py
python3 scripts/run-recovery-aot.py --disc-probe 'ref/Sonic Adventure.cdi'
```

Requires native macOS ARM64, Python 3, CMake, Ninja, Git and Apple CLT.
The first command stages a pinned public source checkout if absent, builds the
generator, runs twelve focused tests and eight synthetic execution variants
(branch, memory, FPU, indirect calls, GBR stores and GBR loads; the latter two
also run with the GPR cache enabled).
The second performs static inspection of the existing owner-held CDI, writes
private extracted boot inputs, and verifies that the original hash is unchanged.
Neither launches Sonic or a Simulator. Each records a timestamped JSON report
and logs under ignored `docs/artifacts/recovery-aot/`. See
[RECOVERY-LOOP.md](RECOVERY-LOOP.md) for scope, results and the next R3 experiment.
These commands do not supply the missing Katana package or clear original G1.
The runner verifies/applies the exact ordered experimental patches described in
[patches/recovery/README.md](../patches/recovery/README.md); unrecognized candidate
changes cause a failure without being reset. Reports include its SHA-256.

For static closure and exact call/instruction CSV reports from a previously
extracted boot image:

```sh
python3 scripts/run-recovery-aot.py \
  --coverage docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  --inspect-address 0x8C037246 --inspect-address 0x8C03724A
```

This mode also runs a minimal unsupported-instruction diagnostic and a coverage
reporting control. A successful audit is not a successful game translation;
inspect `r3_complete`, `gbr_gap_present`, limits and unresolved calls in its JSON.

For PRS decoding, auxiliary-module placement hypotheses, source-only reference
codec identities and static resident/module linkage, follow
[RECOVERY-PRS.md](RECOVERY-PRS.md). Static tools write only private outputs.

To compile and execute the isolated PRS routine from the verified owner-held
boot image as native ARM64:

```sh
python3 scripts/run-recovery-prs-aot.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN
```

This requires the private PRS extraction/reference artifacts described in that
document. It rejects any other boot identity, reuses the pinned source/patch
guards, and compares five synthetic streams and four real module streams with
independent decoder results. Game-derived C++ and the test executable stay in
ignored artifacts. Each execution has a 30-second timeout. The result proves
that one decompression routine executes; it does not launch the game, exercise
the complete loader, or close R3/R4/G1. Missing runtime dispatch targets fail.

To exercise original SBOARD registration with declared task/heap state:

```sh
python3 scripts/run-recovery-registration.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
```

Exact input hashes and pinned source/patches are required. Five original routines
are emitted privately; four allocation/scheduling cases run under a 30-second
execution timeout. The fixture supplies task/heap state, so a pass is not startup
or scheduler proof. Installed gameplay/cleanup continuations remain unimplemented
in this slice and must fail explicitly when invoked. No game/Simulator launch.

To exercise original BIOS wrappers against the owner's CDI using the candidate's
public GD-ROM HLE service:

```sh
python3 scripts/run-recovery-disc-aot.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  docs/artifacts/recovery-aot/20261005-080717/disc-map.txt
```

This verifies Mini Cart's full sectors under both read commands, status transitions,
guards, input identities and explicit missing-disc/unmapped-sector errors. It
requires exact owner boot/CDI identities and uses the existing disc map. The
four wrappers execute as native ARM64; the BIOS service is explicitly host HLE.
The game file provider and full loader remain unexecuted. No game/Simulator launch.

To execute original provider startup, filename reading and the combined PRS loader:

```sh
python3 scripts/run-recovery-provider.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  docs/artifacts/recovery-aot/20261005-080717/IP.BIN \
  docs/artifacts/recovery-aot/20261005-080717/disc-map.txt \
  docs/artifacts/recovery-aot/20261005-122414/functions.csv \
  docs/artifacts/recovery-aot/20261005-124122/expected-sectors.bin
```

The final argument is the independently verified padded Mini Cart sector output
from the preceding disc test. The runner checks its exact hash and derives the
known decoded reference with the bounded Python decoder. It executes original
work-area/provider initialization and directory selection, then checks successful
and missing filename/module cases, closed handle, exact bytes, guards and ABI.
All originals remain unchanged; generated game-derived material stays ignored.
The 20-second execution uses explicit public BIOS HLE and internal-MMIO shadow
state. It does not prove cache side effects, loaded-module activation or gameplay.

To join actual SBOARD loading to verified precompiled registration:

```sh
python3 scripts/run-recovery-provider.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  docs/artifacts/recovery-aot/20261005-080717/IP.BIN \
  docs/artifacts/recovery-aot/20261005-080717/disc-map.txt \
  docs/artifacts/recovery-aot/20261005-122414/functions.csv \
  docs/artifacts/recovery-aot/20261005-124122/expected-sectors.bin \
  --sboard work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
```

The runner verifies the SBOARD identity and binds it to a compiled manifest,
links separately named original registration code, and checks two loads plus four
task/heap cases per load. Correct bytes before activation and old code after
failed replacement must reject. Host activation boundaries and task/heap fixture
remain explicit; the scheduler is not booted.

The source-free module activation test is independent:

```sh
python3 scripts/run-recovery-modules.py
```

It checks A -> B -> A at overlapping addresses, resident/direct-hint calls, exact
image identity, failed manifests, aliases, cache retirement and unloaded targets.
Its source/generated/binary hashes and commands are recorded with the evidence.

To bind the original stage selector and audit its module identities:

```sh
python3 scripts/run-recovery-stage-inventory.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  work/dreamcast-recovery/prs-candidates/report.json --audit
```

Omit `--audit` for the fast binding/identity contract only. The full run uses
actual table entries where known, explicitly labels three load-base hypotheses,
checks 33 new inputs against the pinned decoder, and retains five prior audits.
It emits static reports only. Successful completion does not require zero unknown
instructions: inspect per-module UNKNOWN/RAW counts and unresolved targets; STG01
currently retains a discovery problem. No original bytes or maps are patched.
The helper supports `--entry=ADDRESS` independently of the load base when auditing
an interior root; this avoids adding bogus function roots just to satisfy tooling.

## Native macOS input tests

```sh
scripts/run-host-tests.sh
```

Uses the independently installed Apple CLT with a command-local developer
selection. Builds ARM64 macOS input-test binaries; does not compile Katana or a
game app. All three suites currently pass: 46 + 50 + 46 checks.

The preserved comparison lane is:

```sh
SONICPAD_HOST_TOOLCHAIN=llvm scripts/run-host-tests.sh
```

`scripts/check-prerequisites.sh` separately probes macOS/Foundation and full
Xcode. It currently exits 2 for the full-Xcode license requirement while the
macOS CLT probe passes. Do not change global developer selection to use CLT.

## Read-only bootstrap verification

```sh
python3 tests/host/test_bootstrap_verifiers.py
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

The source audit checks reference HEADs, pin/lock consistency, actual file sizes
and hashes, materialized runtime source against its pinned source ZIP, and all
materialized FFmpeg files against the pinned source archive. It writes a detailed
missing/incompatible-file report. It does not extract or acquire anything.

Its default retained-source layout is:

```text
.local/source-only/r354/
  title-source/                       38/39 currently recovered
  sdk/include/                       missing
  sdk/generated/include/             missing
  product/generated/                 missing source and metadata
  katana-source-178448be.zip           missing source archive
  katana/                            materialized source matching ZIP paths
```

For an authenticated source-only package elsewhere:

```sh
scripts/check-source-closure.sh --source-root /path/to/r354-source \
  --runtime-root /path/to/materialized-katana \
  --report docs/artifacts/authenticated-source-audit.json
```

This audits the known r354 input identity. A newer compatible package needs its
own declared manifest/ABI review; do not relax hashes to accept an arbitrary pack.
Source audit exit 0 means its input identities match; it **does not mark G1 met**.
The transitive build graph, providers/ABI, auxiliary code route and supported
disc/extraction still require verification. Exit 2 means absent/incompatible inputs.

The game verifier accepts the canonical three-track GDI layout and streaming
track hashes. It cross-checks local configuration against the pinned upstream
supported-disc definition. It rejects CDI/CHD/ISO/archive inputs, missing tracks,
wrong layout/offsets, hash mismatches and paths escaping the chosen directory.
Descriptor hash is recorded; extraction and runtime acceptance remain separate.

## Overlay dispatch diagnostic

```sh
python3 scripts/run-recovery-overlay.py
```

Uses only synthetic SH-4 input with the pinned public candidate and exact patch
sequence. Tests bulk literal invalidation, cross-page/alias/empty operations and
reports stale compiled-code selection separately. A successful exit currently
still reports both stale-code flags true: overlay activation is not implemented.
No Simulator or game app is launched. Evidence binds native binary/source hashes.

## Public-source recovery only

```sh
python3 scripts/recover-title-source.py
python3 scripts/fetch-media-source.py
```

Title recovery currently exits 2 for the missing exact r354 CMake file; FFmpeg
source acquisition exits 0. Source/evidence remain ignored. The source-only
request and exact remaining inputs are in [BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md).
No Windows libraries, player installer or removed recovery archive is an Apple
source bootstrap.


### Explicit static data classification control

Run the source-free discovery counterexample and positive/negative controls:

```sh
python3 scripts/run-recovery-discovery.py
```

For the already-decoded, exact owner STG01 input, validate motion structures and
compare the annotated closure against its preserved pre-annotation audit:

```sh
python3 scripts/audit-recovery-motion-data.py \
  work/dreamcast-recovery/prs-candidates/candidate-0752/decoded.bin \
  docs/artifacts/recovery-aot/20261005-133022/STG01
```

This does not download inputs or launch a game. It checks the input hash, model
vector count, motion layout, finite vertices/unit normals and original consumer
instructions before supplying explicit ranges. Any entry/instruction loss outside
those ranges fails. Current evidence: 135649 controls, 135713 real audit; all
under `docs/artifacts/recovery-aot/20261005-`. Unresolved calls and later code
writes remain separate. Never derive data ranges solely from unknown opcodes.


### Event header and loader inventory

Audit all207 owner event modules sequentially, with actual header-derived roots
and independent PRS decoding (static only, no game launch):

```sh
python3 scripts/run-recovery-event-inventory.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  work/dreamcast-recovery/prs-candidates/report.json --audit
```

Omit `--audit` to verify only identities and loader/header bindings. Each run uses
new timestamped private evidence. `passed` means audits finished, not event
playback/full code coverage; inspect each `has_unknown` and unresolved call record.
Current207-audit result: `20261005-140837/event-inventory.json`.

Refresh existing static linkage without rerunning code discovery:

```sh
python3 scripts/refresh-recovery-linkage.py \
  docs/artifacts/recovery-aot/20261005-140837/combined-input.json \
  docs/artifacts/recovery-aot/20261005-140143 \
  work/dreamcast-recovery/linkage-review-new
```

Output must be a new ignored directory. This verifies resident/module identities,
records input hashes, preserves caller/module/site provenance and reconciles
module/aggregate records. It never promotes candidates automatically. The optional
`--motion-audit` argument replaces only STG01 using a passing exact-input data/loss
audit. Existing baseline records are reproducible; current combined245-image
report is `20261005-140837/linkage/`.


### Current menu and auxiliary evidence

Private source-bound audit scripts retained with the owner inputs:

```sh
python3 work/dreamcast-recovery/loader-review/menu-modules/audit-advertise-summary.py
python3 work/dreamcast-recovery/loader-review/menu-modules/audit-alert.py
python3 work/dreamcast-recovery/loader-review/remaining-prs/audit-payloads.py
```

They produce new timestamped evidence and do not execute a game. Menu scripts
verify the original call/header contracts, source pin/patches, reference decoder
and explicit static roots. The payload script verifies HTML spans and the Chao
transfer signature; it does not emulate or validate VMU execution. Current
results141622/141730/142208; nine still-unclassified motion/path input identities
and original load destinations are recorded in142348/remaining-prs.json.

For the latest248-image static linkage, use
`docs/artifacts/recovery-aot/20261005-142912/combined-input.json` as the inventory
argument to `refresh-recovery-linkage.py`, with resident audit140143 and a new
ignored output directory. The reports preserve unclassified roles and unresolved
calls; they must not be used as a full-game completion claim.

### Current library root and native camera experiment

```sh
python3 work/dreamcast-recovery/loader-review/library-roots/audit-chao.py
python3 work/dreamcast-recovery/loader-review/library-roots/audit-camera.py
```

The first re-roots AL_MAIN at its original AL_GARDEN00 caller and checks for lost
code. The second verifies 101 raw camera files and original consumer contracts,
then generates one original 48-instruction routine and executes it on ARM64 for
all 4,893 records. It verifies expected state, guards, original input, return and
registers, and rejects an unregistered callback. These are supplied-global/state
fixtures; neither boots the game or proves the selected callbacks/rendering.
Camera evidence is `20261005-144437/camera-report.json`; the ordinary control
`report.json` in that directory describes only the synthetic setup control.
All original inputs and generated outputs remain private and ignored.

### Loaded SBOARD callback continuation

```sh
python3 scripts/run-recovery-provider.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  docs/artifacts/recovery-aot/20261005-080717/IP.BIN \
  docs/artifacts/recovery-aot/20261005-080717/disc-map.txt \
  docs/artifacts/recovery-aot/20261005-140143/functions.csv \
  docs/artifacts/recovery-aot/20261005-124122/expected-sectors.bin \
  --sboard work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin \
  --sboard-callback
```

The optional callback flag adds thirteen original routines and eight cases across
two actual loads; default provider/registration behavior is preserved. It uses
declared player/task/heap/queue fixtures, executes the real state-zero path and
checks available/exhausted task pools for both character branches. The next phase
must fail explicitly at unregistered8C0194FC. Current passing evidence145336;
145246 retains the initial incorrect caller-PC boundary assertion and trace.
This is native callback execution, not original task scheduling or gameplay.

### Original video/camera prerequisites

```sh
python3 work/dreamcast-recovery/loader-review/viewport-native/run-camera.py
```

The private runner binds the owner boot/IP/camera hashes and six source patches.
It executes original video setup with a caller-derived1/0/1 configuration, then
original camera initialization for both SBOARD acts and projection calculations
for12743 and16384 angle values. Current result150530 has four passing camera
cases, explicit state/task checks and unchanged camera bytes. Runtime BIOS/HLE
and MMIO model success is not proof of a displayed image. Declared stage, heap,
task pool and host camera-file loading remain separate from original startup.
This has not yet been joined with SBOARD's state2 callback in one execution.

Keep verbose per-call logging disabled for this path. The original video-memory
clear makes2097152 four-byte copy calls; logging every call caused the20-second
timeout and56MiB trace in150041. The same initialization with in-memory history
only passed150208. Preserve that failed evidence; do not mistake it for a proven
infinite loop or patch the original clear away.

### Connected original video/camera/SBOARD path

```sh
python3 scripts/run-recovery-provider.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  docs/artifacts/recovery-aot/20261005-080717/IP.BIN \
  docs/artifacts/recovery-aot/20261005-080717/disc-map.txt \
  docs/artifacts/recovery-aot/20261005-140143/functions.csv \
  docs/artifacts/recovery-aot/20261005-124122/expected-sectors.bin \
  --sboard work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin \
  --sboard-camera
```

This mode also requires the hash-verified camera references at
`work/dreamcast-recovery/bin-candidates/candidate-0097/decoded.bin` and
`candidate-0098/decoded.bin`. They are comparison oracles; original game code
loads the camera bytes through the real provider. No camera-data download.
Run151600 passes four connected cases across two actual SBOARD loads. One heap
and task pool per case survive camera setup, stage registration and callback
state0→2, with original projection update and explicit rejection at0C905480.
The initial stage/player records, free nodes, heap and selection remain fixtures;
this does not boot the scheduler. Provider/Mini Cart/failed-load controls remain
active. `report.json` and `joined-camera-contract.json` bind evidence and limits.
The earlier callback-only mode retains its separate exhaustion/registration cases.

Continuation update152213: `--sboard-camera` now runs eight cases, with three/four
initial free nodes, and executes905480 plus the initial stage/act writes of
9054F6. Available allocation rejects at904AC0; exhausted allocation proceeds to
052700 and rejects there. Both leave state2. The module task-pointer write is
tracked explicitly; failed loads preserve the live pre-failure image. This mode
still does not boot/schedule the game. Earlier152213 report/contract bind this
specific scope; earlier151600 evidence remains historical.

### Synthetic offscreen output boundary

```sh
python3 work/dreamcast-recovery/loader-review/output-boundary/run.py
```

The private prototype links the runtime archive from152213 without pulling in
its guest code/image (symbol-checked). It loads no disc/BIOS/game bytes. Source,
header, archive and parent-report hashes are recorded. It submits synthetic TA
packets through store queues, writes STARTRENDER and checks two full scanouts
against an independent pixel oracle. Evidence152538/render-report.json contains
native identity, import/symbol checks and PPM hashes. This proves bounded
software raster/depth/scanout behavior, not an Apple window or game frame.
The runner selects CLT only for its subprocesses. No Simulator is required.

### Bounded AppKit presentation and synthetic PCM

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools /usr/bin/clang++ \
  -std=c++20 -O2 -fobjc-arc -arch arm64 -mmacosx-version-min=13.0 \
  -I docs/artifacts/recovery-aot/20261005-152213/provider-generated \
  work/dreamcast-recovery/loader-review/output-boundary/presentation.mm \
  docs/artifacts/recovery-aot/20261005-152213/provider-build/generated/libdreamcast_generated.a \
  -framework AppKit \
  -o work/dreamcast-recovery/loader-review/output-boundary/PresentationProbe
python3 work/dreamcast-recovery/loader-review/output-boundary/present.py
```

The private app runs for at most120 seconds (runner timeout150). Inspect its red
frame, click **Toggle depth test**, inspect blue, and let the bounded process
exit. Use one probe instance. Current152955 result proves visible transitions
only: exact RGB/edge tests fail due to unresolved color tagging/scaling. CUA
screenshots verified the display; PNGs are AppKit view exports. Read the attached
presentation-contract.json before interpreting the runner's launch-pass field.
No existing app shell was replaced; this is an isolated output experiment.

```sh
python3 work/dreamcast-recovery/loader-review/output-boundary/audio-run.py
```

Current153448 tests exact441-frame stereo PCM/WAV capture in loop/one-shot/mute
modes. It uses synthetic AICA sample RAM, register writes and one explicit ARM7
interpreted branch; no game firmware, media download or host audio playback.
Source/dependency/binary/WAV hashes, commands and scopes are in audio-report.json.

### Corrected presentation and Apple AudioQueue output

```sh
python3 work/dreamcast-recovery/loader-review/output-boundary/present-v2.py
```

This runner builds the corrected app and full-capture verifier itself. Inspect
red, click **Toggle depth test**, inspect blue, and close the window. Close exits
the process; a120-second fallback bounds unattended runs.154000 passes all pixels
at declared2/255 sRGB quantization tolerance (observed1/255 max). The separate
three-case color-scale reduction, source hashes and limits are in its contract.
Old presentation.mm/present.py and152955 failures remain historical evidence.

```sh
python3 work/dreamcast-recovery/loader-review/output-boundary/audio-output-run.py
```

This consumes153448/audio-0.wav, rechecks every input sample, then plays two brief
synthetic phases at queue volume0.25.154214 verifies completion of three buffers
per phase, advancing sample clock, stopped state, reset and restart; OSStatus or
timeout fails explicitly. No audible-quality, route-change or Sonic-audio claim.
All programs are bounded host probes; never run alongside a previous probe/game.

### SBOARD initialization and original-startup gate

The existing `--sboard-camera` provider command now verifies12 cases using the
hash-bound108-entry stage map. Each case reloads the original stage, reaches
state0→2→3, checks child hierarchy/work or pool exhaustion, and calls the original
player-work lookup to measure the next missing startup prerequisite.155509 passes;
lookup returns null, so the downstream stage3 update is intentionally not run.
See initialization details and exact input identities in155509/initialization-contract.json.
The `--sboard-callback` mode retains its earlier scope; regression155425 passes.

```sh
python3 scripts/run-recovery-provider.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  docs/artifacts/recovery-aot/20261005-080717/IP.BIN \
  docs/artifacts/recovery-aot/20261005-080717/disc-map.txt \
  docs/artifacts/recovery-aot/20261005-140143/functions.csv \
  docs/artifacts/recovery-aot/20261005-124122/expected-sectors.bin \
  --sboard work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin \
  --sboard-camera
```

Guest execution remains bounded to20 seconds; compile steps have separate limits.
Use one probe at a time. Passing this command does not boot the scheduler or close
G1/R3/R4. The next measured dependency is the original writer of8C7906E8, not a
missing Katana download or permission to fill the table with synthetic pointers.

The same `--sboard-camera` command includes twelve earlier stage cases, two
joined startup/stage cases and two isolated scheduler-prefix cases.171115 verifies
bothSONIC.PRS andSON_EFF.PRS batches:57 registered textures,127776 exact copied
VRAM bytes, rendering headers, permanent list links and allocation ownership.
The independent reference models bank selection/alignment and every copied range;
all other8MiB VRAM bytes must equal the pre-call snapshot. Each reference file is
extracted/decoded independently with fixed encoded/decoded hashes.

Both files share scratch memory. Verify the retainedSONIC compressed prefix,
exact effect compressed bytes, final effect decoded bytes plus retainedSONIC
suffix, outer guards,21 read sectors, closed handle and resource counter1.
Original09AC9C release must return the counter to0, free57 records and all used
VRAM extents, and preserve VRAM bytes. Original666E80 then resets free lists.

The scheduler must reject at0B440E after both batches. Texture-upload flags cover
these57 textures only; they prove neither all game textures nor rendered output.
No completed scheduler tick, Sonic frame, DMA completion or normal boot follows.
See171115/texture-batch-contract.json and texture-batch-reference.json.

Original video608442 initializes two4MiB banks; outer graphics056242 remains
unexecuted. Manager60C96A uses its original caller arguments directly. Supplied
heap8CE40000 avoids loader scratch0CD00000. Only the large callback unit uses
diagnostic-O0. No performance claim; process/descendant timeout cleanup remains.


The same command's172737 checkpoint extends both repetitions through original
auxiliary task creation, model initialization and the first animation blend.
`startup-model-check.hpp` verifies five occupied task nodes, both auxiliary queues,
73 source model nodes,34 records,12 disabled rows and3264 vertex/normal bytes.
All prior57-texture release/reset and stage checks still pass. The strict next
boundary is05A040 atPC07C41E/PR07C426/SP8CFFEEFC; numerical animation values and
matrix traversal remain unverified. See172737/startup-model-contract.json.
This remains supplied startup, not a completed scheduler tick or normal boot.


At173842 the same command runs original matrix initialization with05624E's
arguments, then reaches the character display entry0AFD9A. The new
`startup-motion-check.hpp` independently checks19 keyed channels/two blend frames
and73 hierarchical matrices; max element error2.23368e-7. Both repetitions also
check stack restoration, yaw-only collision center and all prior regressions.
See173842/motion-pose-contract.json. Outer graphics, arbitrary motion/deformed
mesh correctness, actual drawing and normal boot remain open. The strict next
boundary isPC0AB136/PR0AB13C/SP8CFFEFBC at0AFD9A.


At175004 the same command executes outer graphics056242, checking camera,
screen/projection/clipping and an exact60-byte RAM background packet in two
repetitions. All prior texture/model/motion/task regressions pass. The callback
slice now spans18 program files; the runner compiles/prefixes/hashes them all
with diagnostic-O0. See175004/graphics-startup-contract.json. The character is
culled by its default camera; no Sonic frame is proved. The strict next boundary
is094240 atPC0AB150/PR0AB16E/SP8CFFEFBC. Continue original character/camera
execution; normal boot and complete scheduler remain open.


At180247 the same command verifies the first group1 return and three camera
group0 invocations on the same live player/heap/pool, plus source camera bytes,
50 history records, collision registration/radius/history and prior regressions.
Display is then called with the original camera-produced state. Its visibility
check must return1 and reject at0585C6 (PC0AFDC8/PR0AFDCE/SP8CFFEFD8). See
180247/character-camera-contract.json. This is still a supplied call sequence,
not a full frame schedule. No submitted Sonic draw or normal boot is proved.

At181955 the same provider command verifies original612AF0's workspace using
056420's exact buffer/capacity arguments, before graphics setup. Both extended
cases reach material helper0378B8 atPC03799A/PR03799E/SP8CFFEF68 and record22 SQ
commits/0 TA packets. This supersedes the180247 boundary, not the normal-boot
limit. Eight exact core patches and the181921 synthetic regression are recorded
in the patch README. `181955/mesh-entry-contract.json` binds the current result;
final mesh values, polygon submission, full frame and normal boot remain open.


The same `--sboard-camera` provider command now passes checkpoint184528.
`184528/polygon-projection-contract.json` binds both original polygon-setup
repetitions, source-derived graphics VRAM reservations,57 texture uploads/release,
and43 first-mesh projections with a corrupt-coordinate negative control.
System heap initialization is once before video; test stack8CE3F000 and isolated
stage heap8CE10000 avoid live allocations. Next original function037C30 rejects
atPC037A90/PR037A9A/SP8CE3EF68. Compile the audited textured-strip routine and
verify its output packets.22 SQ commits/0 TA packets are not draw/frame evidence.
Original G1, complete startup/scheduler, camera pose, full geometry and R3/R4
remain open; no new source acquisition or Simulator launch is required for this
next bounded test.


Checkpoint190025 uses the same provider command and eight core patches.
`190025/first-strip-contract.json` binds two first-mesh FIFO checks (header+
98 vertices/12 strips), two corruption controls, original vector copies and
one event0320/six-continuation graphics return. The joined manifest includes
emitted resident block addresses as well as function starts. Raw staged packets
and parsed `ta_packets` are separate counters:99 staged/0 parsed per display.
Next037950 updates the mesh pointer. A complete Sonic model/frame is still open;
retain first-strip verification when continuing through later meshes.


Checkpoint202744 uses the same provider command/eight patches and passes both
original character display returns. `202744/display-return-contract.json` binds
first-mesh value checks,53 material pointer updates and819 staged packets per
case. The test-only037950 observer always calls the original body and restores
registration/dispatch settings; it preserves first-mesh checks before later
workspace reuse.0 packets parsed; no rasterized frame. Next trace original scene
completion/flush, including655900→65568A and buffered translucent submission,
without injecting readiness or STARTRENDER. Details and source hashes are in
RECOVERY-PRS's202744 entry. G1/R3/R4 and adoption remain open.


Current --sboard-camera command is a deliberate failing diagnostic at205723.
It restores original Maple startup and returns from608DF0, then exports three
PPM buffers and frame-packets.bin before rejecting visual acceptance. All submitted
vertex RGB/offset colors and rendered RGB are zero. Read STATUS/RECOVERY-PRS for
the lighting-table producer investigation. Do not describe this exit1 as a green
regression;202744 remains the latest full pass. No core-patch change is needed
for the next bounded source/lighting investigation.


Checkpoint211105 supersedes205723's deliberate failing diagnostic. The same
--sboard-camera command passes both extended cases, exact original lighting-file
loads, frame/DMA/interrupt returns and cleanup. Private output includes each
frame-N-packets.bin and scene-N registration/render/present PPM. Captures show a
partial Sonic clipped at the top, no level scene. Report flags keep full-frame,
independent lighting/camera, normal boot and gameplay acceptance false. Next join
stage and character/camera paths as documented in STATUS; no render override.


Current --sboard-camera command at213237 again exits1, now at group2 callback
0B450E after verified stage/player/camera joining and snowboard texture upload.
The runner preserves this failed report and first-case capture; it does not
claim a full regression pass.211105 remains the latest full two-case pass.
Before adding0B450E, execute original09F5A0 to load local S_MOT at0CC00000 and
verify its bytes/live-memory preservation. See STATUS and the213237 evidence
entry in RECOVERY-PRS. No Katana/PAL or production-foundation gate is cleared.


Checkpoint213949 supersedes213237: the unchanged --sboard-camera command now
passes both full fixture repetitions through original S_MOT load, group2 return,
streamed snowboard texture release and existing character cleanup/reset. It still
does not run group3 or a complete scheduler/normal boot. The report requires the
new checks twice and leaves full_task_tree_teardown_verified:false. Private
verified-source snapshots and group2-startup-contract.json preserve the checkpoint.


Checkpoint215723 supersedes213949: --sboard-camera also passes original group3
and both installed model-link destructors in both repetitions. The runner requires
both new labels twice. Full task-tree teardown and normal boot remain false.
The failed215545 second-run render is retained; original model-link restoration
fixes it without changing packet assertions. Exact sources and244-identity
manifest are preserved in215723/verified-source and group3-startup-contract.json.
Next experiment is original09A544 update traversal before resource release.


Checkpoint221028 changes --sboard-camera into a deliberate failing update probe:
original09A544 now runs before cleanup and stops at8C076990, exit1. A return
guard requires state inspection before accepting a full update.215723 remains
the latest complete bounded regression; current cleanup and second repetition
are unverified. See RECOVERY-PRS's221028 entry and private source snapshots.
Next compile locally audited076990 and verify collision/terrain/SET prerequisites.


Checkpoint222407: --sboard-camera first joins the omitted original terrain task
using09A1CE(2,1,053720), after retained baseline checks. It currently exits1 at
037F6C in terrain mesh drawing. A new terrain-return guard precedes the existing
full-update guard; neither is an acceptance pass.221731/221927 proved the previous
update used empty terrain.215723 remains the latest full bounded regression.
Exact current sources are preserved in222407/verified-source. See RECOVERY-PRS.


Checkpoint223726: the same --sboard-camera command deliberately exits1 on the
second terrain clipping call's unmapped UV read00205710. Register wrappers expose
the bank-state mismatch; neither terrain nor update return guard is removed.
Current exact sources are in223726/verified-source. Check captured transfers with:

```sh
python3 scripts/audit-recovery-terrain-output.py \
  docs/artifacts/recovery-aot/20261005-223726 \
  work/dreamcast-recovery/loader-review/terrain-reference/sandboard-decoded.bin \
  work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
```

This passes50 textures/645616 bytes and source-record/pointer bounds only.
The reference files remain private owner-disc derivatives. See RECOVERY-PRS.


Checkpoint224845 adds an explicit `--startup-bank1` experiment. Append it to the
existing `--sboard-camera` command to reproduce the current terrain frame; it is
rejected without that camera fixture. Default bank0 behavior remains unchanged.
The option supplies the clipping routine's bank precondition; its normal startup
producer has not been established. Current source snapshots:224845/verified-source.
The command intentionally exits1 after terrain task and original frame boundary
return. Inspect224845/scene-2-render.png and terrain-frame-observation.json;
presentation still contains the preceding character frame. Use the terrain audit
command above with capture directory20261005-224845. No gameplay/full regression
pass follows; the full-update guard remains. See RECOVERY-PRS's224845 entry.


Collision continuation checkpoint230120 still uses `--sboard-camera --startup-bank1`.
Its nonzero exit at missing6447C0 is expected evidence, not a regression pass.
Run the independent static-neighborhood check with:

```sh
python3 scripts/audit-recovery-collision-selection.py docs/artifacts/recovery-aot/20261005-230120 work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
```

This verifies12 selected records and links, not triangle collision or gameplay.
See STATUS.md for the next bounded continuation and preserved update guard.


Checkpoint231156 continues original update after verifying collision preparation.
The run still uses `--sboard-camera --startup-bank1` and exits at missing07AD36;
it is not a full regression pass. Preserve both `collision-ram.bin` (before
update) and `update-ram.bin` (changed/failure state). Run these external checks:

```sh
python3 scripts/audit-recovery-collision-selection.py docs/artifacts/recovery-aot/20261005-231156 work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
python3 scripts/audit-recovery-collision-geometry.py docs/artifacts/recovery-aot/20261005-231156 work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
```

The second script requires the first script's matching-RAM audit. It verifies
929 original triangle records in the supplied fixture, not ground contact,
normal startup, arbitrary transforms/formats or gameplay.


Checkpoint231711 includes read-only per-call collision observations. After the
selection and geometry audits (with capture directory231711), verify misses:

```sh
python3 scripts/audit-recovery-collision-misses.py docs/artifacts/recovery-aot/20261005-231711
```

It requires `collision-queries.csv`, pre-/post-update RAM and the matching geometry
audit. Passing means captured static-mesh misses are geometrically justified;
intersecting spheres fail pending a positive-contact oracle. The provider still
exits at missing05A3CA; full-update/cleanup acceptance remains guarded.


Checkpoint232348 retains the same provider command and explicit bank1 hypothesis.
It stops at missing07CC2C after snowboard return and an auxiliary angle update;
it is not a full regression pass. Use directory232348 with the existing terrain,
selection, geometry and collision-miss audit commands. See STATUS.md for the
next original continuation and the unverified normal snowboard placement.


Checkpoint233831 is the first original09A544 return. The same provider command
with explicit `--startup-bank1` deliberately exits1 at the inspection guard.
After the four existing terrain/collision audits, run:

```sh
python3 scripts/audit-recovery-update-state.py docs/artifacts/recovery-aot/20261005-233831
```

This checks one supplied-state update's root links and selected retained state,
not general movement correctness, normal startup, cleanup or gameplay. Four
negative controls are captured privately. The exact source/oracle manifest is
`233831/update-return-contract.json`. Keep the inspection guard until its bounded
assertions are integrated; then test original cleanup/repetition before display.
215723 remains the latest full bounded regression.


Checkpoint235343 removes the inspected full-update stop and passes the canonical
`--sboard-camera --startup-bank1` provider command. It requires two update/display/
release cases and six frame interrupt continuations. Each case uses original
collision reset029000 and drains original display/frame work before freeing
textures. Full task-tree teardown and normal startup are still unproved.

Separate `startup-1` and `startup-2` directories hold each case's RAM, query and
terrain captures. Run the existing selection, geometry, misses, terrain and
update-state audits against each directory (and the canonical root if desired).
Their execution log is copied from the completed run solely for return markers.
The final source/oracle/evidence manifest is `235343/update-display-contract.json`.

Passing this harness does not accept visual repeatability: combined render3/6
have475 differing pixels around clipped Sonic. Inspect STATUS.md before extending
the frame loop; investigate original resident model initialization/pose order.
Do not rerun unchanged tests or force resident angle bytes to match a screenshot.


### Optional original outer-loop experiment (6 October)

Append `--startup-loop` to the provider command with
`--sboard-camera --startup-bank1`. It completes the supplied prefix's collision
pass and original coroutine-root setup, retaining first-tick reference checks.
It then loads CON_REGULAR with the09AD7E table arguments, invokes the original
0534C0 cadence producer with0503C0's(2,2), and enters running wrapper0506BA with
the existing device clock enabled. This remains supplied startup, not normal boot.

012816 restores the flag-off baseline after removing duplicate654DE4, which reset
the installed event dispatcher. Original graphics and Maple interrupt sources
are checked at entry. Both supplied startups pass; all ten external audits,
release controls and six unchanged renders pass. The controller decoder also
runs168 isolated controls per startup without changing live state.

015039 runs two supplied0506BA/0534F0 cycles with one continuous guest clock.
Original0534F0 waits for zero and reloads cadence; do not replace it with608DF0
or toggle the clock between cycles. The current run completes four updates and
two renders, then stops at the explicit inspection guard (exit1). This is not
normal startup: mode7649F0 remains0, excluding the stage-specific callback in
053570's mode4/5/9 branch. Follow STATUS.md for that missing producer path.
The exact binary passes `baseline-regression/`. Each output directory owns its
disposable VMU; the original root image is preserved and ignored.
`python3 scripts/audit-recovery-running-cycles.py CAPTURE_DIR` checks RAM
continuity, deferred removal, timer deltas, retained links, positions and cadence
with two corruption controls; it makes no gameplay or mode4 acceptance claim.
The old0508DE continuation is pause/menu handling and must not be restored as
normal frame sequencing. The eventual wrapper/frame inspection guard remains.

`scripts/audit-recovery-hud-textures.py CAPTURE_DIR DECODED_CON_REGULAR_PVM`
checks56 links/54 payloads and two corruption controls.
`scripts/audit-recovery-hud-sprite.py CAPTURE_DIR PRE_PACKET_RAM` checks the first
shared HUD sprite packet against the fixed004952 pre-packet capture; it passes
005138.014015 rejects that historical absolute packet destination: the current
first HUD packet is elsewhere. This fixed-address oracle must not be presented
as passing for the new running sequence. The rejection is preserved.
The native header check executes40 isolated controls without changing live RAM.

`loop-*` and `cadence-ram.bin` captures remain separate from first-tick audits.
Generator bound592, missing-target failures and unknown-instruction rejection
remain active. Neither the experiment nor the flag-off baseline clears G1.

### Optional original executable entry (6 October)

Append `--original-startup` instead of `--startup-loop` to the same provider
command with `--sboard-camera --startup-bank1`. The latter compiles the flag-off
baseline, but the original-entry branch executes before its bank1/state writes.
It registers resident AOT targets, starts010000 with HLE post-BIOS registers,
and captures fresh before/failure RAM and registers. No SBOARD overlay is activated
and no supplied game mode/task/video setup runs. IP.BIN handoff is still unproved.

Supply an explicit `--system-flash` input with `--original-startup`. The factory
fixture `work/dreamcast-recovery/flash-config/us-factory-v1.bin` retains erased
settings;034703 verifies original formatting without a configuration record.
The latest040512 experiment instead declares
`work/dreamcast-recovery/flash-config/us-stereo-test-v2.bin`, a separate synthetic
configuration accepted by the original checksum/query. Its source-only recipe is
`work/dreamcast-recovery/make-stereo-test-flash.py`; the126-case settings reduction
and independent input validator are recorded in
`20261006-system-settings-reduction/report.json`. The original input stays intact.
Both images are exactly131072 bytes and hashed in each report. Missing input
rejects SYSINFO_INIT. Current stereo boundary is missing original610F32 after89
sectors,39 queued sound commands and default material setup; this is not a passing
boot, audible playback, texture-data or graphics-output acceptance.

For flag-off regressions clear SONICPAD_RECOVERY_ORIGINAL_STARTUP,
SONICPAD_RECOVERY_OUTER_LOOP and SONICPAD_RECOVERY_SYSTEM_FLASH. The latest
exact-binary evidence is in `20261006-042919/baseline-regression/`.
Patch9 and31 isolated contract checks cover SYSINFO and read-only flash services;
Patch12 adds in-memory flash programming/erase; unsupported SYSINFO commands
still reject. No automatic fixture formatting or settings success is injected.

Source-only recipe for the declared synthetic factory fixture (not console firmware
or a production settings image):

```python
from pathlib import Path
b = bytearray([255]) * 131072
for offset in (0x1A000, 0x1A0A0):
    b[offset:offset + 16] = b'00110Dreamcast  '
b[0x1A056:0x1A05E] = b'SPADLAB1'
p = Path('work/dreamcast-recovery/flash-config/us-factory-v1.bin')
p.parent.mkdir(parents=True, exist_ok=True)
assert len(b) == 131072
if p.exists():
    assert p.read_bytes() == b  # preserve differing existing input
else:
    p.write_bytes(b)
```

Expected SHA256: `ab1957532e270b33fa7a17fb17fbdba8c11a17af02765b8dc2d49af1b7ec6562`.
All other bytes remainFF, including unformatted user/settings partitions. The core
does not silently generate this fixture. Flash READ follows the documented byte
count result; Flycast REIOS returns0 instead. The KallistiOS wrapper forwards the
raw result, so it does not resolve that discrepancy. Hardware parity is open.
The public candidate RTC is fixed at1577836800; this is not a real-time accuracy
claim. The continuation repair follows saved PCs after RTE clears the async flag;
it does not reset guest registers or restart the entry function.

The original-startup branch explicitly enables the ARM7 interpreter at128
instructions per slice, with idle fast-forward and synthetic queue completion
disabled. Patch10 models read-only empty G2 FIFO under synchronous bus writes;
patch11 installs the public HLE boot's ARM self-loop at sound-RAM offset0. The
original title owns reset, fill, driver upload and release.030606 verifies zero
ARM7 faults and reset held at66ADC0. Sound-RAM snapshots and ARM state accompany
main-RAM captures. Native SH4 AOT does not imply native ARM7 execution. The
isolated idle/G2 checks are not sound firmware or playback acceptance.


031607 completes one original G2 DMA of33,376 bytes from0CD00020 to00800000.
031248 verifies the exact whole sound-RAM image before reset release.031607
executes the original completion interrupt and firmware, with745,969 reported ARM
instructions and zero faults; the original firmware initializes its SEGA marker
atF8. `audit-sound-firmware-boot.py` is preserved under its verified-oracles folder.
Sound-RAM mutation after release is expected; do not apply the pre-release whole
RAM upload oracle to running firmware. Sound playback, complete ARM7 semantics
and bus/CPU timing remain unproved. Next metadata reader65E834 uses the original
byte/word G2 readers; their234-case reduction passes before033003 metadata
readback.033505 independently verifies all eight original kind1 handles.

034703 formats the erased synthetic partition through original607E88 and raw
BIOS services. Its captured `original-startup-*-system-flash.bin` files record
in-memory changes; the declared input stays unchanged. Patch12 implements only
bit-clearing programming and exact partition erase, with31 read-only and32
write/erase controls. Hardware timing, direct flash-MMIO and disk persistence
are unproved. The isolated reproduction is preserved with source snapshots in
`20261006-flash-write-reduction/`; the old-runtime negative control rejects a
valid programming command. Its formatted partition has no configuration block,
so do not assume
formatting establishes valid audio settings or bypass the caller's retry path.


042001 executes original32 program submissions through647958/6478DC; the
1229-descriptor/300-queue reduction and prospective ring prediction live in
`20261006-sound-program-reduction/`. The captured first channel is0, established
by stackB0 rather than scratchR0=4;040512's unexecuted channel4 prediction is
superseded without changing its private checkpoint.

042659 adds original6443EC/658BA0/6588A0, with232 current-flag material packet
cases and90 dimension cases in `20261006-material-startup-reduction/`.
042919 adds original042100 and verifies six copied globals. The exact39-record
ring and material words pass independent audits. No complete texture upload,
rendered startup, command delivery or audio follows from those checks.

Current selected closure750 functions/78,940 instructions/2,189 external records,
cap752, twelve patches unchanged.054159 stops at original64B648 from6069F8 in
modeled interrupt context:PC606A22,PR606A28,SP89691C,SR60000060,SPC645DB8.
Registration has returned to the title caller; first original frame-wait entry
is proved retrospectively from saved055842/053542 and exceptionSPC645DB8.

The original read-only readiness query reproduces1,792 raw and1,792 conversion
cases in `20261006-peripheral-ready-reduction/`. Source snapshots, generated
binaries and exact commands are retained. Its prospective final32-call trace
comparison was not met because startup advanced104,152 further calls; the
rejected oracle remains in054159/rejected-oracle. No status/queue completion
was injected and no changed startup was rerun to repair that audit.

The prospective1,024-byte callback/ready-store prediction from the prior callback
reduction passes on053823 and054159. Both exact binaries pass the existing
supplied-scene baseline once. Language, sound and material/companion audits
remain valid at the captured boundaries; modeled interrupts do not prove hardware,
input, presented startup or playback. Continue using the declared stereo flash.

Next reduce original64B648/66459A/66CBA0 scheduling with the captured manager,
context and original callback64B64E before another changed startup. Keep one
oracle at a time; preserve original control/FPU/context-transfer semantics.
Checkpoint054159/registration-frame-wait-startup-checkpoint.json binds708
identities and the previous checkpoint. Disk free11GiB after data-preserving
APFS block sharing; remeasure before another full build. Original Katana/PAL, production adoption and gameplay remain unresolved.

Baseline storage compaction preserves591 completed capture files byte-for-byte
while sharing identical APFS data blocks. Permissions, owner, extended attributes
and access/modified timestamps are retained; inode/birth times change. Logs,
predictions, sources and original/private inputs were excluded. Free space is
now11GiB, about7.8GiB reclaimed. All708 checkpoint identities and591 capture
hashes were reverified; no evidence path or contents were removed.
Supplement: `054159/baseline-clone-checkpoint-supplement.json`.


## 2026-10-06 — R3/C8 original scheduler context transfer,061041

Original64B648→66459A→66CBA0 captures the caller through112574 and restores
an eligible worker through112500. Startup reachesPC/PR64B64E,SP8997B4,
SR60000060,SPC645DB8 and rejects before executing that uncompiled worker.
No synthetic callback invocation or status/queue completion was injected.

The seven-function/255-instruction reduction passes576 restore,816 idle/deferred/
cancel/release and288 transfer cases:1,680 total, including both physical FPU
banks, raw context patterns, interrupt masks, cancellation pointer installation,
release and sequence wrapping. The prospective full16MiB RAM check passes
outside80 unknown caller MACH/MACL/GBR/FR/FPUL capture bytes; restored GPR/control
and16 active FR words match the existing descriptor. Full-run inactive FPU bank
64 bytes were not predicted. Neither reduction nor full run executes64B64E.

The changed exact binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Callback/ready, language,39-command
sound ring, metadata/handles/driver and material/companion audits pass. Twelve
core patches and private inputs unchanged. Closure756 functions/79,144
instructions/2,190 external records, cap760. Mode0,sectors89,1,280,676 reported
ARM instructions, zero faults and zero nonzero audio frames. All processes
terminal; no Simulator launched. Free space10GiB.

Checkpoint061041/handler-scheduler-startup-checkpoint.json binds381 identities,
previous054159 checkpoint and storage supplement; previous708 identities
reverified. SHA256:
`ced6c0ee71e493ea489bd3835289bfb7260c1be5b95318fde56ddbd37da2eecf`.
Next reduce the actual selected64B64E operation and yield/context-return
prerequisites before one changed startup; do not compile unused branches blindly.
No presented title, normal boot, gameplay, hardware parity, real input/save,
audio playback, production adoption or G1 acceptance. Katana/PAL blockers and
source-only request remain unchanged. Goal turn classification: progress.


## 2026-10-06 — R3/C8 original idle worker yield and scheduler return,062416

Original64B64E selects operation0, yields through6645AA→66CB9C→66CB5E,
captures its152-byte context, sets due sequence1 and restores the caller through
112500. Original66CBD2 resumes, finishes the eight-slot scan and returns to
606A28. No callback, queue/status completion or connection flag was injected.
Next missing64BF74:PC606A28,PR606A34,SP89691C,SR60000061,SPC645DB8.

The worker reduction has11 functions/547 instructions/22 external records,
cap20. `20261006-peripheral-worker-reduction/repair-3/` passes1,080 signed delay/
negative/wrap and1,512 idle/out-of-range operation cases:2,592 total, both FPU
banks, exact152-byte context/8-byte stack/RAM/flash/sound. Actual frozen worker
yield and resumed scheduler return pass; uncompiled operation1 rejects64BFE0.
Unselected peripheral operations remain missing dependencies.

Preserved failures: initial C++ mixed-type auto declaration prevents execution;
repair-1's oracle expected32 scheduler slots instead of captured8; repair-2's
negative control expected instructionPC744, whereas the generated diagnostic
reports source block73E. `rejection-control/` isolates and proves the64BFE0
rejection at73E/return748. The misleading failed assertion text is retained;
the operation did reject. No core/runtime repair or state normalization followed.

Prospective prediction saved before the full run verifies read-only captures at
first worker entry, first yield and first scheduler return:three exact16MiB RAM
images, no exclusions, all GPR/control and both physical FPU banks. Device clock/
ARM7/AICA counters were not predicted. This audit does not prove the later
failure boundary's entire state or subsequent operation completion. Source
reviews, frozen inputs, prediction and commands are bound to the checkpoint.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Callback/ready, language,39-command sound
ring, metadata/handles/driver and material/companion audits pass. Closure760
functions/79,436 instructions/2,210 external records, cap768; twelve patches and
original/private inputs unchanged. Mode0,sectors89,1,280,711 reported ARM
instructions, zero faults and zero nonzero audio frames. All experiments terminal;
no Simulator launched. Free space9.2GiB.

Checkpoint062416/peripheral-worker-startup-checkpoint.json binds510 identities
and previous061041 checkpoint; previous381 identities reverified. SHA256:
`3f5cc3dcce7e29b97c81710eccdd51c1bae04321db7ea55da5a3b542c675e1e6`.
Next reduce64BF74→664AEC with64A596/64A5A8/6630E0 status/address helpers:
verify eight-record bitmask and ABI, then one changed startup, no forced flags.
No hardware parity, normal boot, presented title, operation completion, gameplay,
real input/save, audio playback, production adoption or G1 acceptance.
Katana/PAL acquisition lane unchanged. Goal turn classification: progress.


## 2026-10-06 — R3/C8 original peripheral connection mask,063555

Original64BF74→664AEC uses the mapped identifiers and64A596/64A5A8/6630E0
address helpers to check the active byte and flag mask0x2. Modeled record0 returns
connected, yielding mask1. Original6069F8 stores that mask, reads readiness and
stops before606B38:PC606A76,PR606A7A,SP89691C,SR60000060,SPC645DB8.
No full-run connection/ready flag or completion was forced; callback not executed.

`20261006-peripheral-status-reduction/repair-1/` passes3,840 per-record and12,288
aggregate cases,16,128 total:all256 masks, both identifier orders/FPU banks,
zero/nonzero/sign-bit active bytes and flag branches, source-defined MACL effects,
ABI and exact memory guards outside reviewed16/40-byte child stack scratch.
Its frozen caller prefix independently predicts every stack byte/global/control
field and rejects before the same606B38. Eight functions/252 instructions/six
external records, cap16. Initial frozen oracle used8937E8 forR14; original literal
606C60 is8937E4. Failed source/log preserved and corrected oracle passed without
any runtime change.

The prospective full-run prediction was saved before launch. Entire16MiB RAM,
including stack, and all58 GPR/control/physical FPU fields match; no excluded
bytes. Device clock/ARM7/AICA counters were not predicted. Prior worker entry,
yield and scheduler-return exact predictions still pass. Runtime material factor
check remains retrospective; prior readiness trace rejection remains preserved.

The changed exact binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Callback/ready, language,39-command
ring, sound metadata/handles/driver and material/companion audits pass. Closure762
functions/79,497 instructions/2,210 external records, cap768; twelve patches and
original/private inputs unchanged. Mode0,sectors89,1,280,806 reported ARM
instructions, zero faults and zero nonzero audio frames. All processes terminal;
no Simulator launched. Free space8.5GiB.

Checkpoint063555/peripheral-status-startup-checkpoint.json binds441 identities,
previous062416 checkpoint; previous510 identities reverified. SHA256:
`01eaa5c3eb64396504a4e9287b68553e3983760316e19f0dcda501cffdbd44a2`.
Next reduce606B38 with64B9E4/64B9F4/64BA04:busy-record early return and original
operation1/argument/status writes across eight records. Then one changed startup
and follow the actual worker dependency, without synthetic queue/status completion.
Modeled peripheral state is not actual hardware/input; no callback completion,
presented title, normal boot, gameplay, actual save/audio playback, production
adoption or G1 acceptance. Katana/PAL acquisition lane unchanged. Goal turn:progress.


## 2026-10-06 — R3/C8 original request queue and first frame-wait return,064555

Original606B38→64B9E4/64B9F4/64BA04 queues operation1/status1 and zero arguments
for modeled record0. Read-only first-request trace retains all four original
calls. The prospective1,024-byte records and32-byte outer guards match at first
request and final failure; request execution/completion remains unverified.
Latest full RAM/control/FPU and device/audio counters were not predicted.

`20261006-peripheral-request-reduction/` passes3,584 callback busy/free,2,560
operation and2,048 argument cases:8,192 total. All eight slots, all16 interrupt
masks/both FPU banks, exact entire RAM including12/24-byte callback stack writes,
ABI and neighbor records; actual frozen callback also passes. Four functions/
64 instructions/zero external records, cap8. No request completion injected.

Dispatcher continuations053542→055842 match the original first0534F0 call
return. Later modeled interruptSPC0534F4 proves entry into the second frame wait.
`frame-wait-return-audit.json` is retrospective and source-bound; no presented
frame or hardware/timing proof. Before queued worker operation1 executes, the
next missing function is65E0B8 from647CD8:PC647CF2,PR647CF8,SP89693C,
SR60000061,SPC0534F4. Original64BFE0 remains unexecuted, not silently compiled.

Old registration failure-layout oracle now rejects: original request legitimately
changes the status/op bytes0→1. Failed source/log remain in064555/rejected-oracle.
A retrospective delta proves only those two record bytes changed. The unchanged
original registration prediction passes the existing first-worker-entry capture;
that earlier phase was selected retrospectively. No runtime/game repair or
changed startup rerun followed this audit failure. Prior worker phases match
all original prospective RAM/control/FPU predictions.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Language,39-command sound ring, sound
metadata/handles/driver and material/companion audits pass. Closure765 functions/
79,553 instructions/2,210 external records, cap768; twelve patches and original/
private inputs unchanged. Mode0,sectors89,1,657,281 reported ARM instructions,
zero faults and zero nonzero audio frames. All experiments terminal; no Simulator
launched. Free space7.9GiB.

Checkpoint064555/peripheral-request-startup-checkpoint.json binds411 identities
and previous063555 checkpoint; previous441 identities reverified. SHA256:
`ef28a4e85e8748ceaf0bce21c74bcc19dfd7d3a858cd9a73964c0b863a799f54`.
Next reduce65E0B8 with66AF04/66A8E8/66A7A8 and already compiled65E9EC:
driver-read ABI, descriptor iteration/wrap, error exits and limit of32 successful
calls, before one changed startup. Preserve pending peripheral request and no
fake queue/command completion. No real input/hardware/save, presented title,
normal boot, gameplay, audio playback, production adoption or G1 acceptance.
Katana/PAL acquisition lane unchanged. Goal turn classification:progress.


## 2026-10-06 — R3/C8 first original sound batch and modeled DMA,071002

Original65E0B8 executes66AF04, packs32 of39 descriptors through66A8E8 and
flushes through66A7A8 into the existing original DMA submission path. The
prospective packet bytes/padding, full256-record ring, manager fields and guards,
and pending peripheral records match first-sound-batch and final failure. Seven
commands remain pending; software busy1, firmware busy byte0. Callback66A810 and
command completion remain unverified. Latest whole RAM/control/FPU/counters were
not predicted.

The15-function/652-instruction reduction passes1,473 cases:384 byte reads,288
packet packers,192 flushes and609 service cases, plus the actual frozen queue.
All16 interrupt masks/both FPU banks, signed capacity/count, cursor wrap, sparse
records, pending termination, overflow/busy/errors and untouched packet padding
are checked. Standalone pack/flush stacks and guards are exact; reader/service
comparisons allow only source-bounded100/136-byte stack scratch. The reduction
explicitly rejects before uncompiled647364; no DMA/completion stub. Three failed
oracles/logs/sources are preserved: initial128-byte service scratch was too small,
a diagnostic rerun confirmed it, then a nonzero driver byte exposed a missed
G2 counter increment before the busy error. Only oracle expectations changed.

Full startup observes one additional512-byte modeled G2 DMA to sound RAM13200;
bytes match the independently prospective packets. Counter/channel/location
proof is retrospective, not hardware/timing/parity evidence. Full startup gets
past the sound service and stops at10F048 from647CD8:PC647D04,PR647D0C,
SP89693C,SR60000060,SPC0534F4. Original peripheral operation1 remains pending;
64BFE0 has not executed.

Original39-command initialization and sound metadata predictions pass the
existing first-worker-entry capture, selected retrospectively. Their old final
queue/two-transfer assertions no longer apply after legitimate packet packing/
third DMA; no queue normalization or rerun. Prior worker RAM/control/FPU phase
predictions, pending request, registration phase, frame-wait return, language,
sound handles/driver and material audits pass. The exact binary passes two
supplied scenes, ten external audits, release controls and six unchanged images.
Closure769 functions/79,782 instructions/2,209 external records, cap772; twelve
patches and original inputs/flash/root VMUs unchanged. Mode0,sectors89,
1,658,016 reported ARM instructions, zero faults/nonzero audio frames. All
experiments terminal; no Simulator launched. Free space7.5GiB.

Checkpoint071002/sound-service-startup-checkpoint.json binds537 identities and
previous064555 checkpoint; previous411 identities reverified. SHA256:
`a04d0c40c305dffbd3b966055d96c498f388d1ae413d99b23a29f651b3fbe62d`.
Next reduce original10F048: read channel-indexed callback from G2 table
(global5FF84C, offsetEC+4*channel), store throughR5 and return0; verify source
addressing, ABI and guards before another changed startup. Keep pending peripheral
request and sound completion visible. No hardware/input/save, presented title,
normal boot, gameplay, audible playback, production adoption or G1 acceptance.
Katana/PAL blockers and source-only acquisition request unchanged. Goal turn:progress.


## 2026-10-06 — R3/C8 G2 counter queries and selected operation1 handler,072216

Original10F048 and10F064 copy channel0 transfer counters2/3 from the G2
table into8948FC/8948F8. The prior callback-table label was inaccurate: these
are channel counters, not function pointers. Both getter calls remain in the
read-only first-query trace. The prospective query outputs/guards, sound packet/
ring/manager regions and pending peripheral records match first-query/failure.
Latest full startup RAM/control/FPU/device counters were not predicted.

The five-function/91-instruction reduction passes7,168 cases:4,608 getter,
2,304 unsigned-comparison and256 early-return cases. All16 interrupt masks,
both T values/FPU banks, four channels and explicit scaling/wrap cases, high-bit
unsigned comparisons, exact entire RAM including stack, ABI and neighbor guards
are checked. Positive pending count explicitly rejects at missing65E0B8; no
service bypass. The actual frozen647D04 suffix returns0535D0 with independently
specified entire16MiB RAM and all58 register/control/FPU fields exact. That is
an isolated saved-continuation proof, not a whole full-startup prediction.

Full startup resumes the original scheduler and worker:66CB88→64B8B4, then
operation1 dispatch reaches uncompiled64BFE0 atPC64B73E,PR64B748,SP8997B4,
SR60000060,SPC0534F4. The selected request handler has not executed. Actual
peripheral root variant0/slot0 active byte1/type0 select the ordinary64BFE0
branch. Source also contains the variant1 tail64C1B0; it remains explicit.
No request/command completion, device response or connection flag was injected.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Prior worker RAM/control/FPU phase predictions,
pending request, registration phase, frame-wait return, language, sound handles/
driver and material audits pass. Original39-command/metadata predictions remain
valid at earlier first-worker-entry, selected retrospectively. The prior modeled
512-byte sound DMA still matches prospective packets; seven commands pending,
software busy1, callback66A810/completion unverified. Closure771 functions/
79,802 instructions/2,207 external records, cap772; twelve patches, original
inputs, flash and root VMUs unchanged. Mode0,sectors89,1,658,046 reported ARM
instructions, zero faults/nonzero audio frames. All experiments terminal; no
Simulator launched. Free space6.6GiB.

Checkpoint072216/g2-counters-startup-checkpoint.json binds430 identities and
previous071002 checkpoint; previous537 identities reverified. SHA256:
`a2a46edcc68e0714936dea065760f2af19c9e7fb4f6194a26014c943403260aa`.
Next reduce original64BFE0 selected operation1 path with64B91E/64B27C/64F224/
64EF30 dependencies as required; verify source-defined record/stack/error and
callback behavior before a changed startup. Preserve variant1 and readiness/
completion boundaries. No hardware/input/save, presented title, normal boot,
gameplay, audible playback, production adoption or G1 acceptance. Katana/PAL
blockers and exact source-only acquisition request unchanged. Goal turn:progress.


## 2026-10-06 — R3/C8 original operation1 setup/notification boundary,073430

Original64BFE0 selected operation1 setup writes device phase2/command17,
then64B91E reaches the already registered original callback010F0E. The callback
has not executed. All three calls remain in the read-only first-setup trace.
The prospective entire16MiB RAM and all58 GPR/control/physical FPU fields match
first-peripheral-operation-setup and final failure, with no excluded bytes.
Device clock/ARM7/audio counters were not predicted.

The three-function/257-instruction reduction passes9,088 cases:512 notification
shim,6,144 setup/variant and2,432 format-validator cases, plus the actual frozen
setup. All eight slots/all16 interrupt masks/both FPU banks, ordinary/variant1
branches, inactive and signed nonzero active bytes, callback/null paths and
four source format masks with19 signed/range/geometry conditions are checked.
Entire RAM including stack and neighbor guards is exact. The actual frozen
setup reaches010F0E with all58 fields exact. Missing010F0E,64B27C and64EF30
explicitly reject; no callbacks, transfer routines or responses were stubbed.
The validator is verified in isolation, not executed in the full run yet.

Actual full boundary:PC64B936,PR64C052,SP89979C,SR60000060,SPC0534F4;
R4slot0,R5operation1,R6phase1,R7command17. Original request status/operation
remain pending1. The callback source records R6/R7/R5 in76-byte rows at6B6134
(offsets8/12/16), returns0 and has MACL76*slot; it does not complete the request.
The next transfer/retry routine64B27C calls6645E4 and real Maple SDK dependencies.
Variant1, error/geometry/yield/response and completion boundaries remain explicit.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Prior worker RAM/control/FPU phase predictions,
pending request, registration phase, frame-wait return, language, sound handles/
driver and material audits pass. Earlier sound initialization predictions pass
first-worker-entry with retrospective phase selection. The modeled512-byte sound
DMA still matches prospective packet bytes; seven commands pending, software
busy1, sound callback66A810/completion unverified. Closure774 functions/80,059
instructions/2,210 external records, cap776; twelve patches, original inputs,
flash and root VMUs unchanged. Mode0,sectors89,1,658,054 reported ARM instructions,
zero faults/nonzero audio frames. All experiments terminal; no Simulator launched.
Free space5.8GiB.

Checkpoint073430/peripheral-command-startup-checkpoint.json binds432 identities
and previous072216 checkpoint; previous430 identities reverified. SHA256:
`c52ef0a96bbefe4b4d771c5d56441c20ba72984cdc247a685bb12ae6fd807adc`.
Next reduce original010F0E notification, then64B27C/6645E4 and actual Maple
prerequisites. Verify source-defined notification rows, stack/ABI/MACL and
transfer/retry/yield/errors before a changed startup. No fake response, callback
completion or input flags. No hardware/input/save, presented title, normal boot,
gameplay, audible playback, production adoption or G1 acceptance. Katana/PAL
blockers and exact source-only request unchanged. Goal turn classification:progress.


## 2026-10-06 — R3/C8 progress callback and first mapped SDK transfer prerequisite,102235

Original010F0E now records progress1/17/1 in row0 at6B6134, then returns
to64C052. Original64B27C begins its first transfer attempt through6645E4,
64A596/64A5A8/6630E0 and64A87C. The mapped SDK identifier is1; full status
word0E is preserved through the readiness bit test. Execution stops before
66315E. All five notification/transfer prerequisite calls remain in the trace.

The prospective entire16MiB RAM and all58 GPR/control/physical FPU fields match
first-peripheral-transfer-attempt and final failure with no excluded bytes.
Original40/48-byte transfer/SDK frames and child PR scratch match exactly.
Device clock/ARM7/audio counters were not predicted. The original request status/
operation remain1; no retry result, yield/response or request completion follows.

The nine-function/405-instruction reduction passes7,168 cases:2,048 notification,
2,048 notification shim and3,072 first-SDK ready/absence cases, plus the actual
frozen notification/caller continuation. All eight slots/all16 interrupt masks/
both FPU banks, callback T values, exact whole RAM/stack and mapped active/flag
branches are checked. Ready paths explicitly reject66315E; absent paths explicitly
reject6645AA. These tests do not execute SDK yield or transfer responses. The
initial frozen oracle expected status2 instead of fullflags0E. Its source/log and
diagnostic rerun are preserved; only the oracle was corrected, no runtime change.

Latest boundary:PC64A87C,PR66462E,SP899744,SR60000060,SPC0534F4;
R4identifier1,R0flags0E,MACL120. Source66315E calls6630D0 to address a44-byte
SDK record at root(8AC634)+40+44*identifier, then marks byte13=1. It has not
executed. The subsequent64A6C2 wrapper calls6641D4 and optionally64A394;
request issue/poll/yield/error and completion semantics still need execution proof.

The exact binary passes two supplied scenes, ten external audits, release controls
and six unchanged images. Prior worker RAM/control/FPU phase predictions, pending
request, registration phase, frame-wait return, language, sound handles/driver and
material audits pass. Earlier sound initialization predictions pass first-worker-
entry with retrospective phase selection. Modeled512-byte sound DMA still matches
prospective packets; seven commands pending, software busy1, sound callback66A810/
completion unverified. Closure778 functions/80,376 instructions/2,230 external
records, cap780; twelve patches, original inputs, flash and root VMUs unchanged.
Mode0,sectors89,1,658,066 reported ARM instructions, zero faults/nonzero audio
frames. All experiments terminal; no Simulator launched. Free space4.9GiB.

Checkpoint102235/peripheral-transfer-startup-checkpoint.json binds507 identities
and previous073430 checkpoint; previous432 identities reverified. SHA256:
`09a703afb89d833ee403b3e539fe8990b046704dfbb7cc9c2c0be251772b545b`.
Next reduce66315E/6630D0 SDK setup byte, then64A6C2/6641D4 and optional64A394
request prerequisites. Verify exact SDK records, stack/ABI/MACL and actual
issue/poll/yield/errors; no synthetic responses or completion. No hardware/input/
save, presented title, normal boot, gameplay, audible playback, production adoption
or G1 acceptance. Katana/PAL blockers and source-only acquisition request unchanged.
Goal turn classification:progress.

## 2026-10-06 — R3/C8 original command 10 dispatch reaches sound callback,103934

Original `66315E` now marks SDK record 1's setup byte. Original `64A6C2` /
`6641D4` takes the free lock with TAS, and `664200` constructs command 10,
two payload words `[2, 0]` and zero auxiliary stack arguments. The already
compiled provider routine `663550` executes and returns ticket **42** to
`66421E`. Peripheral operation 1 remains pending; response/completion is unverified.
Startup continues to missing sound DMA callback **`66A810`**, called from
`65E27E`. This is an execution prerequisite, not a presented title or normal boot.

The isolated seven-function/264-instruction reduction passes **4,224 cases**:
3,072 setup, 512 payload and 640 wrapper/free-or-held-lock cases. All 48 SDK
records, all 16 interrupt masks, both FPU banks, both setup T values, entire RAM
including stack/guards and all 58 CPU/FPU fields match independent expectations.
The actual frozen continuation also passes. Missing request/logging dependencies
reject explicitly in isolation; no return values, responses or completion are stubbed.

The first full run (`103456`) disproved the predicted missing-target boundary:
`663550` was missing only from the isolated closure and already registered in
the separate provider closure. The outer continuation observer missed its call.
That rejected observation and exact sources are preserved. A scoped read-only
dispatch observer now always calls the same original provider function. Its entry
capture prospectively matches **all 16 MiB of RAM and all 58 fields**, with no
excluded bytes; all seven setup/request calls are retained. PC is the dispatched
target `663550`, as the original dispatcher specifies. Later failure RAM, AICA,
flash and all 58 fields match the unobserved run retrospectively. Device/audio
counters and request-dispatch exit are observed, not prospectively predicted.

Both exact binaries pass the two supplied scenes, ten external audits, release
controls and six unchanged images. Earlier worker, registration, language,
frame-wait, sound initialization/driver/handles, batch transfer and material
checks pass within their recorded scopes. Seven sound commands remain pending,
software sound busy is 1 and callback completion remains unverified. Mode 0,
89 sectors, 1,659,264 reported ARM7 instructions, zero ARM7 faults, 14,306 AICA
frames and zero nonzero audio frames. Three G2 DMA runs total 67,264 bytes.
Callback closure: **783 compiled functions / 80,452 instructions / 2,231 external
records**, cap 788; this does not establish execution coverage of every function.
Twelve core patches, original inputs, system flash and root VMU are unchanged.

Checkpoint `103934/peripheral-issue-startup-checkpoint.json` binds **783 file
identities**; the previous checkpoint's 507 identities were reverified. SHA256:
`aa42b9106a4ed69b07d28d9505ef94dc0766958e59c41eec7afa418e7cdbadd6`.
All experiments are terminal; no Simulator was launched. Free disk: **3.5 GiB**.
The current source audit still verifies 0/1 Katana archives, 0/1,474 generated
entries, 0/177 headers, 0/4 contracts, 38/39 title/providers and 10,139/10,139
FFmpeg entries. Original G1 and the supported PAL dump remain unresolved; the
source-only request in `BOOTSTRAP-BLOCKERS.md` remains current.

Next: reduce original sound callback `66A810` and synchronization `66CE84`;
verify batch flag/count/busy writes and waiting-task effects before another
changed startup. Independently reduce request queue/response branches before
claiming peripheral completion. No real input/save, hardware parity, audible
playback, presented title, gameplay, mobile, production adoption or G1 acceptance.
Goal turn classification: **progress**.


Evidence:

- `docs/artifacts/recovery-aot/20261006-peripheral-issue-reduction/`: generator,
  exact oracle, generated ARM64 binary, report, frozen sources and predictions.
- `docs/artifacts/recovery-aot/20261006-103456/`: initial full execution,
  rejected observer/prediction, frozen sources, earlier phase audits and baseline.
- `docs/artifacts/recovery-aot/20261006-103934/`: corrected read-only observation,
  prospective entry audit, dispatch exit capture, observer transparency comparison,
  phase audits, exact-binary baseline and checkpoint.
- `docs/artifacts/bootstrap-input-audit-20261006-request-dispatch.json`: current
  original-route source audit, intentional exit 2; reference pins unchanged.

Executed with process-local `DEVELOPER_DIR=/Library/Developer/CommandLineTools`:

```sh
python3 work/dreamcast-recovery/run-peripheral-issue-reduction.py
python3 work/dreamcast-recovery/predict-peripheral-issue-startup.py
python3 work/dreamcast-recovery/predict-peripheral-dispatch-observation.py
python3 work/dreamcast-recovery/audit-peripheral-dispatch-observation.py 20261006-103934
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-103934
python3 work/dreamcast-recovery/checkpoint-peripheral-issue.py 20261006-103934
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-request-dispatch.json
```

The reduction/prediction runners preserve unique evidence directories and reject
rerunning into them. Full startup used the canonical provider runner recorded in
each report; both full runs intentionally exit 1 at the missing original sound
callback. That exit is not a complete build/boot pass. The previous goal turn
was a status-only check (no progress); this turn executed and verified the next
available bounded step. No additional external source acquisition is assumed.

## 2026-10-06 — R3/C8 original sound DMA callback cleanup,105618

Original `66A810` now executes from `65E27E`, calls byte publisher `66CE84`
and returns to `65E284`. The publisher reads an aligned sound word through
`66CE4C` / `10F080`, changes mailbox byte 0 to 1 and writes the word back through
`66E97C` / `10F390`. The callback clears all 32 transferred packet flags, batch
count and software busy flag. The source does **not** establish the previously
hypothesized waiting-task effects; this is a sound-RAM byte publication path.

The fifteen-function/641-instruction isolated reduction passes **6,912 cases**:
3,840 aligned-byte publication and 3,072 callback cases. All four byte lanes,
nonnegative address boundaries, signed batch counts, all 16 interrupt masks,
both T values and both FPU banks are checked. Entire 16 MiB RAM, 2 MiB AICA RAM,
stack/guards and all 58 CPU/FPU fields match independent expectations, with no
excluded bytes. The actual frozen callback also passes. The reduction uses
idle G2 channels, critical mode 1 and ARM7 disabled; active-channel and other
critical-mode branches are outside that isolated scope.

In startup, both callback **entry and return** prospectively match entire 16 MiB
RAM, all 58 fields and the frozen-device 2 MiB AICA write predictions. Device
clocks and ARM7/audio counters were not predicted. The scoped read-only observer
always calls the original registered routine. The callback dispatch is retained
in the entry trace; earlier inner calls can be evicted from the 32-call exit
history, so complete inner trace retention is not claimed.

Startup advances to missing **`647D5C`**, called from `055848` with `R4=0`.
Source review identifies a three-instruction store of R4 to `677894`. Latest
boundary: PC `055848`, PR `055856`, SP `FFFFBC`, SR `40000001`, SPC `64542C`.
At final failure, batch count and busy are 0, the modeled mailbox has returned
to 0 and **seven ring commands remain pending**. Peripheral operation/status
remain 1; response/completion remains unverified. No audible samples follow:
mode 0, 89 sectors, 2,035,797 reported ARM7 instructions, zero ARM7 faults,
15,042 AICA frames and zero nonzero audio frames. G2 runs remain 3 / 67,264 bytes.
Mailbox turnover does not establish complete firmware consumption or playback.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Prior worker, registration, language,
frame-wait, driver/handles, initialization and material checks pass in their
recorded scopes. Earlier command10 dispatch still prospectively matches entire
16 MiB RAM/all 58 fields, retains seven calls and returns observed ticket42.
The original first512-byte sound transfer is verified at callback entry, before
cleanup, using an unchanged auditor and exact captured-file aliases; that phase
was selected retrospectively, while packet contents remain prospective.
Callback closure: **787 compiled functions / 80,569 instructions / 2,232 external
records**, cap 792. This is not execution coverage of every compiled function.
Twelve patches, original inputs, system flash and root VMU remain unchanged.

Checkpoint `105618/sound-completion-startup-checkpoint.json` binds **480 file
identities**, with source files bound through immutable snapshots. Previous
783 checkpoint versions reverify: two intentionally edited live source paths
match their preserved prior snapshots; all were checked live before this turn's
edits. SHA256: `0d8bf32afc8e5cdd601916025b31e83bf3d1f133de79b54d65767464932cc519`.
One checkpoint packaging attempt assumed the peripheral reduction sources were
in the full-run snapshot directory. Its source/rejection are preserved; the
lookup now uses the actual reduction snapshot. No runtime or oracle was changed.
All experiments are terminal; no Simulator was launched. Free disk: **3.0 GiB**.

Next: verify the original `647D5C` store/ABI and saved `055848` caller, then one
changed startup. Keep the pending peripheral operation and seven ring commands
visible. Original G1/source/PAL requirements, pinned SunPad and the single-stick
Dreamcast profile remain unchanged. No synthetic readiness/responses, firmware
consumption, audible playback, normal boot, title, gameplay, real input/save,
hardware/mobile, production adoption or G1 acceptance follows from this step.
Goal turn classification: **progress**.


Evidence is under `docs/artifacts/recovery-aot/20261006-sound-completion-reduction/`
and `docs/artifacts/recovery-aot/20261006-105618/`. The first directory contains
independent expected callback-return RAM/AICA/58-field files, saved before the
actual frozen call and full startup. The latter contains phase captures, audits,
source snapshots, baseline and the checkpoint. Checkpoint packaging repair is
isolated under `checkpoint-repair-1/`; it did not rerun or alter game execution.

Executed with process-local `DEVELOPER_DIR=/Library/Developer/CommandLineTools`:

```sh
python3 work/dreamcast-recovery/run-sound-completion-reduction.py
python3 work/dreamcast-recovery/predict-sound-completion-startup.py
python3 work/dreamcast-recovery/audit-sound-completion-startup.py 20261006-105618
python3 work/dreamcast-recovery/audit-sound-transfer-before-completion.py 20261006-105618
python3 work/dreamcast-recovery/audit-peripheral-dispatch-phase.py 20261006-105618
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-105618
python3 work/dreamcast-recovery/checkpoint-sound-completion.py 20261006-105618
```

The canonical full provider command is saved in `report.json`; its exit1 at
missing647D5C is not a full build/boot pass. Reduction/prediction/phase-view
runners preserve unique artifact directories and refuse to overwrite them.
No published changes, Simulator, save writes or global Xcode changes occurred.
Original Katana/source/PAL G1 remains a separate unresolved acquisition gate.

## 2026-10-06 — R3/C8 original bootstrap flag and signed guard,111827

Original647D5C clears677894 from1→0. Caller055856 enters647D62; count677898
0→1,65EAA0 resets67FF20 and65F208 tails6607D2, which rejects explicitly.
The count update is not initializer completion. Five-function/83-instruction
reduction (cap8,13external records) passes640 cases:256stores+384guards,
all16interrupt masks/bothT/bothFPUbanks, signed/wrapping counts, whole16MiB RAM,
stack/guards/all58fields. Actual frozen caller and positive skipped-caller
09F6D2 rejection pass. Five reduction and nineteen full-run sources frozen.

One changed startup prospectively matches entire16MiB RAM/all58fields at
first-service-init-prerequisite and failure, no exclusions; four calls retained.
PC65F208,PR647D7C,SPFFFFB8,SR40000000,SPC64542C; stack parentPR05585C.
Device/audio counters unpredicted. Original core/twelve patches untouched;
read-only failure capture only. No initializer or response stubs.

Earlier sound callback entry/return still match their original RAM/AICA/58field
prediction with archived source versions. Exact binary passes two supplied
scenes/ten external audits/release controls/six unchanged images. All prior
phase audits pass within recorded scopes. Mode0,sectors89,ARM7instructions
2,035,801,faults0,AICAframes15,042,nonzero0,G2runs3,total67,264bytes.
Peripheral operation/status1 and seven sound ring commands pending;batch0/busy0.
Callback closure791functions/80,636instructions/2,239external records,cap796.

Checkpoint111827/bootstrap-service-startup-checkpoint.json binds499 immutable/
protected identities; previous480 reverify. SHA256
26ae25dfd86b8cebe76c7722828a02580605968d98b8da322f5fac8c35aacd5f.
Fresh source audit:runtime0/1,generated0/1474,headers0/177,contracts0/4,
title/providers38/39,FFmpeg10139/10139. Supported PAL verifier exits2 for absent
private-inputs/pal-v1003/sonic-adventure.gdi. Exact blocker inventory/source-only
request unchanged; no new lead/repeated acquisition search. All processes
terminal, no Simulator launched;free1.9GiB, no evidence deleted.

Next execute6607D2/66B8E4 reduction:zero4E0/820/220byte ranges,51280808080
word stores, saved65F208 caller and missing66E8DC tail. Two static batches
consumed; no further inventory before execution.6607CE originalRTS/NOP and
66B8B0 original two-global stores remain unexecuted. No module init completion,
playback/normalboot/title/gameplay/input/save/hardware/mobile/production/G1
acceptance. Goal turn classification:progress.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-bootstrap-service-reduction.py
python3 work/dreamcast-recovery/predict-bootstrap-service-startup.py
# Canonical full provider command from the prior section: one run, output111827.
python3 work/dreamcast-recovery/audit-bootstrap-service-startup.py 20261006-111827
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-111827
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-111827
python3 work/dreamcast-recovery/checkpoint-bootstrap-service.py 20261006-111827
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-bootstrap-service.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

The reduction, prediction and checkpoint create unique artifacts; inspect/reverify
existing evidence rather than rerunning these commands over the same paths.
The phase auditor checks the unchanged sound prediction against frozen source
versions after intentional live board/generator edits. It does not loosen RAM,
AICA or CPU expectations. Source/disc audit exits2 are unresolved gates.

## 2026-10-06 — R3/C8 original module buffers/table,112911

Original6607D2 zeroes4E0bytes at8A8C90 and820bytes at8A9170, stores512
80808080 words at8A9990.66B8E4 zeroes220bytes at8DA98C and tails missing
66E8DC. Explicit failure remains, module/outer initializer incomplete.
Six-function/61-instruction reduction(cap12,1external record) passes704cases:
256init/shim+128childclear+64originalRTS+256two-globalsetter. Two buffer
patterns/all16IRQmasks/bothT/bothphysicalFPUbanks; exactwhole16MiB RAM,
stack/guards/all58fields, actual frozen65F208 caller. Original6607CE RTS/NOP
and66B8B0 setter verified in isolation, unexecuted in startup. Independent
expected RAM/state saved before frozen execution. Five reduction/nineteen
full-run sources frozen; no runtime/oracle repair or response stub.

One changed startup prospectively matches entire16MiB RAM/all58fields at
first-module-init-entry/prerequisite/failure, no exclusions. Prior65F208 failure
PC advances to6607D2 target per original dispatcher; restRAM/fields unchanged.
Read-only entry observer always calls original registered target. Five bootstrap
and five initializer calls retained. LatestPC66B8F0,PR660800,SPFFFFB4,
SR40000001,SPC64542C. Device/audio counters unpredicted. Mode0,sectors89,
ARM7instructions2,038,282,faults0,AICAframes15,047,nonzero0,G2runs3/
67,264bytes. Seven ringcommands/peripheral operation/status1 pending;batch0/busy0.

Exact binary passes two supplied scenes/ten external audits/release controls/
six unchanged images. Earlier sound callback original prospective16MiB RAM/
2MiB AICA/58fields still matches using archived sources; prior phase audits
pass within recorded scopes. Closure795functions/80,682instructions/2,239
external records,cap800; twelve patches/private originals/flash/rootVMU unchanged.
Checkpoint112911/module-init-startup-checkpoint.json binds509 immutable/protected
identities, previous499 reverified; SHA256
ab562a771341d2d4d0fe77df30682d5a404acffdf190259011a9f9b99136dde8.
Fresh source audit unchanged(runtime0/1,generated0/1474,headers0/177,contracts0/4,
providers38/39,FFmpeg10139/10139); supported PAL descriptor absent. OriginalG1
and exact source-only request unchanged; no source lead/repeated retrieval.

Free disk reached1.1GiB. Existing verified compact-baseline-clones.py shares
APFS blocks for728 byte-identical completed baseline.bin/ppm captures; saves,
expected-decoded assets, original inputs, source, logs/predictions excluded.
All paths/contents/permissions/owner/xattrs/access/modified timestamps retained;
inode/birthtime change. First transaction then full pass recorded; all728capture
hashes and previous499checkpoint identities reverified. Available space rose
3,237,175,296bytes(~3.0GiB), free4.1GiB. Plan/reports/helper checkpoint-bound.
No evidence path/content removal. All experiments terminal, noSimulator launched.

Next execute66E8DC:64-byte clear and eight8-byte records with indices15..8;
verify saved66B8F0 caller through6607CE/66B8B0 and module return647D7C,
then65EAA8 nested critical-section count/savedmask before10F6AC. Two static
batches consumed; no further inventory before execution. No initialization
completion/playback/normalboot/title/gameplay/input/save/hardware/mobile/
production/G1acceptance. Goal turn classification:progress.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-module-init-reduction.py
python3 work/dreamcast-recovery/predict-module-init-startup.py
# Canonical original-startup provider command from prior sections, once:112911.
python3 work/dreamcast-recovery/audit-module-init-startup.py 20261006-112911
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-112911
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-112911
python3 work/dreamcast-recovery/checkpoint-module-init.py 20261006-112911
```

Reduction/prediction/checkpoint are unique-artifact commands: reverify existing
outputs rather than overwrite them. The original entry observer is scoped and
always calls the registered6607D2 routine. Final startup remains an intentional
missing-target failure, not normal boot. Storage sharing was applied only after
all game/oracle processes were terminal, with an exact byte/metadata plan and
first/full reports. Its capture inode/birthtime changes do not alter the source/
prediction/execute-log chronology used by the prospective auditors.

## 2026-10-06 — R3/C8 original table/module return and critical entry,114203

66E8DC clears64bytes at8EE064 and assigns eight records indices15..8.
Original6607CE/66B8B0 execute and module returns647D7C;65EAA8 increments
67FF20 count0→1, saves IRQmask0 at67FF24 and raises IRQmask15. Outer init
stops before10F6AC:PC647D82,PR647D88,SPFFFFB8,SR400000F1,SPC64542C.
Eight-function/131-instruction reduction(cap16,9external records) passes576
cases:128table+64module-return+384critical-entry, all16IRQmasks/bothT/both
physicalFPUbanks, two table patterns/signed wrapping counts, entire16MiB RAM/
stack/58fields and actual frozen caller. Expected module-return/critical states
saved before execution. First fixture incorrectly replayed66B8F0's already
executed POP_PR, popping twice/skipping setter. Rejected five sources/report/log
preserved under20261006-table-init-reduction; repair1 resumes66E8DC once.
No runtime/game/oracle expectation change; one fixture invocation repair.

One changed startup prospectively matches entire16MiB RAM/all58fields at module
entry/return and critical prerequisite/failure, no exclusions; table/RTS/setter/
critical calls retained. Observer always calls original initializer. Exact
binary supplied-scene regression/ten audits/release controls/six images and
prior phase audits pass. Earlier sound callback RAM/AICA/58fields matches
archived predictions. Clocks/audio unpredicted; mode0,sectors89,ARM7instructions
2,038,331,faults0,AICAframes15,047,nonzero0,G2runs3/67,264bytes.
Seven ring commands/peripheral operation/status1 pending,batch0/busy0.
Closure797functions/80,720instructions/2,237external records,cap800; twelve
patches/private originals/flash/rootVMU unchanged.547 immutable/protected
checkpoint identities and prior509 reverified; checkpointSHA
3cf00013603abc6b7b8d5093b1a3ecb0d177df0d0cce029f019022b22e23718a.
Fresh source audit unchanged(runtime0/1,generated0/1474,headers0/177,contracts0/4,
providers38/39,FFmpeg10139/10139). Supported PAL verifier exits2:descriptor absent.
All runs terminal/noSimulator launched/free3.2GiB; no compaction/deletion this turn.

Next execute10F6AC zero280/400byte ranges7B038C/7B060C and6620DC signed
guard67FFA4/conditionalC00byte clear8AA838; verify captured647D82 caller,
then follow next661E00. Two static batches consumed. Preserve criticalcount1/
IRQmask15 and pending peripheral/seven commands. Outer init,IRQdelivery/restore,
playback/normalboot/title/gameplay/input/save/hardware/mobile/production/G1
remain unverified. Exact source-only request remains current. Goal turn:progress.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-table-init-reduction.py
python3 work/dreamcast-recovery/predict-table-init-startup.py
# Canonical original-startup provider command from prior sections, once:114203.
python3 work/dreamcast-recovery/audit-table-init-startup.py 20261006-114203
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-114203
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-114203
python3 work/dreamcast-recovery/checkpoint-table-init.py 20261006-114203
```

Unique reduction/prediction/checkpoint outputs already exist; inspect/reverify,
never overwrite. The runner points to repair1. The frozen missing-target state
has already executed66B8F0's tail delay slot; replaying that block is invalid.
Resume66E8DC directly once, then follow recorded continuation PCs. Full startup
starts at010000 and executes its own tail exactly once. Original critical entry
raises the interrupt mask; delivery/restoration need separate evidence.

## 2026-10-06 — R3/C8 original two-clear/signed-guard initialization,114927

10F6AC zeroes280/400bytes at7B038C/7B060C.6620DC signed counter67FFA4
0→1 takes C00-byte clear8AA838; positivecounts skip. Four-function/76-instruction
reduction(cap8,6external records) passes896cases:128two-clear+768signed-guard,
two canary patterns/all16IRQmasks/bothT/bothphysicalFPUbanks/signed wrapping
counts, entire16MiB RAM/stack/all58fields and actual647D82 caller. Expected
RAM/state saved before frozen execution; no runtime/guest/oracle repair.

One changed startup prospectively matches entire16MiB RAM/all58fields at
first-guarded-init-prerequisite/failure, no exclusions; six calls retained.
Explicitmissing661E00:PC647D8E,PR647D94,SPFFFFB8,SR400000F1,SPC64542C.
Earlier module entry/return prediction/source versions pass in a separate phase
auditor; former final critical/failure scope excluded explicitly. Earlier sound
callback RAM/AICA/58fields and all prior phase checks pass. Exact binary two
supplied scenes/ten external audits/release controls/six images pass.
Device/audio counters unpredicted. Mode0,sectors89,ARM7instructions2,041,011,
faults0,AICAframes15,052,nonzero0,G2runs3/67,264bytes. Criticalcount1/saved
IRQmask0/IRQmask15 retained; seven commands/peripheraloperation/status1 pending,
batch0/busy0. Outer initializer,IRQrestore/delivery and response/playback unproved.
Closure799functions/80,746instructions/2,235external records,cap804; twelve
patches/private originals/flash/rootVMU unchanged. Five reduction/nineteen
full-run sources frozen.527 immutable/protected checkpoint identities; previous
547 reverified. CheckpointSHA
59b43f584e2ac130ff88a34b884fa6a9f7187266b57b946239ce41603c8ee38b.

Checkpoint packaging first searched full-run snapshots for prior isolated table
tests; rejected source retained undercheckpoint-repair-1. Repair1 uses actual
20261006-table-init-reduction-repair1/verified-source; no runtime/oracle/build/
startup rerun. Fresh source audit unchanged(runtime0/1,generated0/1474,headers0/177,
contracts0/4,providers38/39,FFmpeg10139/10139); PAL verifier exits2 for absent
descriptor. Exact source-only request unchanged. All experiments terminal/no
Simulator launched/free2.3GiB; no deletion/block-sharing this turn.

Next execute661E00 signedguard67FF6C/conditional480-byte clear8AA3B8 and
65FA6C C80-byte clear8A8010/return1; saved647D8E caller and next65F328.
Two static batches consumed; next execute reduction. Preserve criticalcount1/
IRQmask15 and pending peripheral/seven commands. No outerinit/IRQdelivery/restore/
playback/normalboot/title/gameplay/input/save/hardware/mobile/production/G1
acceptance. Goal turn classification:progress.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-guarded-init-reduction.py
python3 work/dreamcast-recovery/predict-guarded-init-startup.py
# Canonical original-startup provider command from prior sections, once:114927.
python3 work/dreamcast-recovery/audit-guarded-init-startup.py 20261006-114927
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-114927
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-114927
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-114927
python3 work/dreamcast-recovery/checkpoint-guarded-init-repair1.py 20261006-114927
```

Unique artifacts already exist; inspect/reverify rather than overwrite. The
phase auditor preserves unchanged module entry/return expectations and archives
source versions after authorized board/generator edits. It excludes the old
later critical/failure phases, which the new primary guard prediction covers.
The first packaging script/source and lookup rejection are retained separately;
repair1 selects the existing isolated-table source archive.

## 2026-10-06 — R3/C8 original second guard and clear/return1,115713

661E00 signed guard67FF6C0→1 clears480bytes at8AA3B8; positivecounts skip.
65FA6C clearsC80bytes at8A8010, returns1. Four-function/67-instruction reduction
(cap8,4external records) passes896cases:128clear/return1+768signedguard, two
canary patterns/all16IRQmasks/bothT/bothphysicalFPUbanks/positive/nonpositive/
wrapping counts, entire16MiB RAM/stack/58fields and frozen647D8E caller.
Independent expected RAM/state saved before execution; no runtime/guest/oracle repair.

One changed startup prospectively matches entire16MiB RAM/all58fields at
prerequisite/failure,no exclusions; five calls retained. Missing65F328:
PC647D9A,PR647DA0,SPFFFFB8,SR400000F1,SPC64542C. Criticalcount1/savedIRQmask0/
IRQmask15/priorguard1 preserved. Earlier module/sound RAM/AICA/field predictions
and phase audits pass; exact binary two supplied scenes/ten external audits/
release controls/six images pass. Counters unpredicted; mode0,sectors89,
ARM7instructions2,043,473,faults0,AICAframes15,057,nonzero0,G2runs3/67,264bytes.
Seven commands/peripheraloperation/status1 pending,batch0/busy0.
Closure801functions/80,769instructions/2,233external records,cap804; twelve
patches/private originals/flash/rootVMU unchanged. Five reduction/nineteen full
sources frozen;524 immutable/protected identities and previous527 reverify.
CheckpointSHAffc3ad3331cccebab2641c95a2621c7b2ac67e4553ee7862a4af76f8a2165c2b.
Fresh G1 audit unchanged(runtime0/1,generated0/1474,headers0/177,contracts0/4,
providers38/39,FFmpeg10139/10139); supported PAL verifier exits2,descriptor absent.
All runs terminal/noSimulator/free1.5GiB; no repair/rerun/deletion/block-sharing.

Next execute65F328→66B0B0 prologues/6488A4 dependency before planned480/560byte
clears. Verify savedPR/stack,wholeRAM/58fields and actual register_target entries
in both providers: headers omit internalblock aliases.66B0B0 is absent from
current actual registrations;6488A4 still needs the same check before a full
prediction. Two static batches consumed; next reduction must execute. Preserve
criticalcount1/IRQmask15 and pending peripheral/seven commands. No outerinit/
IRQrestore/delivery/playback/normalboot/title/gameplay/input/save/hardware/mobile/
production/G1acceptance. Exact source-only request current. Goal turn:progress.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-final-buffer-reduction.py
python3 work/dreamcast-recovery/predict-final-buffer-startup.py
# Canonical original-startup provider command from prior sections, once:115713.
python3 work/dreamcast-recovery/audit-final-buffer-startup.py 20261006-115713
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-115713
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-115713
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-115713
python3 work/dreamcast-recovery/checkpoint-final-buffer.py 20261006-115713
```

Unique artifacts already exist; inspect/reverify rather than overwrite. Return1
is executed by the original RTS delay slot, not a host-provided success value.
Before the next missing-target prediction, inspect generated register_target
records, including block aliases, in provider and callback closures. Declaration
headers alone are insufficient for that registration question. Storage must stay
bounded while original/private inputs and checkpoint contents remain preserved.

## 2026-10-06 — R3/C8 saved-PR prologues,120658

The captured647D9A caller and256 cases pass. Startup reaches missing6488A4;
whole16MiB RAM/all58fields match prospective predictions at prerequisite/failure.
Earlier phase audits and exact-binary baseline pass. Commands (already completed):

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-init-prologue-reduction.py
python3 work/dreamcast-recovery/predict-init-prologue-startup.py
# Canonical original-startup provider command, once:120658.
python3 work/dreamcast-recovery/audit-init-prologue-startup.py 20261006-120658
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-120658
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-120658
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-120658
python3 work/dreamcast-recovery/checkpoint-init-prologue.py 20261006-120658
```

Checkpoint binds794 identities and rechecks previous524/168 APFS-shared captures.
Next reduction resumes6488A4 once; don't replay already-executed prologues.
Two static batches consumed. Original G1 and all gameplay/mobile gates remain open.

## 2026-10-06 — R3/C8 original nested clears/returns,121821

512 cases/frozen missing6488A4 resumed once; five original clears/two globals and
saved returns pass. Original startup reaches65EACE. Prospective whole16MiB RAM/
all58fields at nested return/prerequisite/failure and prior regressions pass.
Completed commands:

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-nested-clear-reduction.py
python3 work/dreamcast-recovery/predict-nested-clear-startup.py
# Canonical original-startup provider command, once:121821.
python3 work/dreamcast-recovery/audit-nested-clear-startup.py 20261006-121821
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-121821
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-121821
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-121821
python3 work/dreamcast-recovery/checkpoint-nested-clear.py 20261006-121821
```

805 functions/cap808 is static closure, not execution coverage. Checkpoint668
identities/previous794/all36 APFS-shared capture hashes verified. No changed
startup rerun or runtime/oracle repair. Free521MiB; check disk capacity before a
new full run. Next reduce original65EACE count/IRQmask restoration and frozen
return05585C, preserving pending commands; device-clock/interrupt behavior and
all gameplay/mobile/G1 gates remain open.

## 2026-10-06 — R3/C8 original critical-section exit,123020

2,688 cases/frozen65EACE once; count1→0/IRQmask15→0/original05585C return pass.
Original startup now stops at09F6D2. Prospective wholeRAM/all58fields at four
phases and SSR/SGR/bank preservation pass. Earlier phase/exact-binary tests pass.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-critical-leave-reduction.py
# Pre-run prediction was issued as a tool command; this saved reconstruction verifies it without overwriting:
python3 work/dreamcast-recovery/predict-critical-leave-startup.py
# Canonical original-startup provider command, once:123020.
python3 work/dreamcast-recovery/audit-critical-leave-startup.py 20261006-123020
python3 work/dreamcast-recovery/audit-nested-clear-phase.py 20261006-123020
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-123020
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-123020
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-123020
python3 work/dreamcast-recovery/checkpoint-critical-leave-repair1.py 20261006-123020
```

Original packaging failure preserved; repair1 finds the actual isolated nested
source snapshots. No runtime/oracle repair or game rerun.973 identities/previous
668 and301 APFS-shared capture pairs verified.806 compiled/cap808 is not coverage.
Next execute09F6D2 record words/global and055862 stack restores before60549E;
free1.3GiB, all experiments terminal. Gameplay/mobile/G1 gates remain open.

## 2026-10-06 — R3/C8 file records/caller stack,124152

256 cases/frozen09F6D2 once, sixteen word resets/separate global and055862
flag/stack restoration pass. Original startup now stops60549E; prospective16MiB
RAM/68 captured CPUfields at four phases plus DBR observation relation pass.
Earlier phase and exact-binary tests pass; no runtime/oracle repair or rerun.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-file-pool-reduction.py
python3 work/dreamcast-recovery/predict-file-pool-startup.py
# Canonical original-startup provider command, once:124152.
python3 work/dreamcast-recovery/audit-file-pool-startup.py 20261006-124152
python3 work/dreamcast-recovery/audit-critical-leave-phase.py 20261006-124152
python3 work/dreamcast-recovery/audit-nested-clear-phase.py 20261006-124152
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-124152
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-124152
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-124152
python3 work/dreamcast-recovery/checkpoint-file-pool.py 20261006-124152
```

808 identities/previous973/all119 startup+72 baseline APFS-shared captures
verified.807 compiled/cap808 is not coverage. Next60549E→604490 selected callback
and original0556B6 loop prefix; verify wholeRAM/69 fields including now-captured
DBR. Need bounded cap expansion if adding both functions reaches808. Free1.2GiB;
all processes terminal, gameplay/mobile/G1 gates remain open.


## 2026-10-06 — R3/C8 startup dispatch and main-loop prefix,125850

Recorded commands below produced immutable evidence; runners refuse to overwrite
existing artifact directories.512 cases/all69CPUfields, then one changed startup
wholeRAM/four phases, earlier phases and exact-binary regression pass.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-dispatch-reduction.py
# Rejected boundary-PC expectation6054AC preserved; repair1 uses actual block6054A8.
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-dispatch-reduction-repair1.py
python3 work/dreamcast-recovery/predict-startup-dispatch.py
# Canonical original-startup provider command, once:125850.
python3 work/dreamcast-recovery/audit-startup-dispatch.py 20261006-125850
python3 work/dreamcast-recovery/audit-file-pool-phase.py 20261006-125850
python3 work/dreamcast-recovery/audit-critical-leave-phase.py 20261006-125850
python3 work/dreamcast-recovery/audit-nested-clear-phase.py 20261006-125850
python3 work/dreamcast-recovery/audit-table-init-phase.py 20261006-125850
python3 work/dreamcast-recovery/audit-sound-completion-phase.py 20261006-125850
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-125850
python3 work/dreamcast-recovery/checkpoint-startup-dispatch.py 20261006-125850
```

789 identities/prior808 and68 startup+56 baseline shared hashes reverified.
Original055820 tail returns0556B6, loop prefix reaches06DD60; loop incomplete.
Next timer-init reduction must check MMIO/stack/69fields, preserve existing
64507E/645002 registrations and distinguish unrecorded actual starting TMU state.
Free777MiB; check capacity before compiling. Gameplay/mobile/G1 remain open.


## 2026-10-06 — R3/C8 timer initializer,133000

Recorded runners refuse to overwrite their evidence directories. The reduction
repairs correct independent expectations (block ticks, R1, weighted issue cost);
they do not modify the runtime or game. The probe is an observer-only rebuild
using31 verified objects. Actual startup133000 uses the canonical provider runner
with `SONICPAD_RECOVERY_CAPTURE_TIMER_ONLY=1`, recorded in its command environment.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-prefix-reduction.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-prefix-reduction-repair1.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-state-probe.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-init-reduction.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-init-reduction-repair1.py
python3 work/dreamcast-recovery/derive-timer-cycle-model.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-init-reduction-repair2.py
python3 work/dreamcast-recovery/predict-timer-init-startup.py
# Initial wrapper132926 rejected before launching a game; corrected canonical wrapper:
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-timer-init-startup-repair1.py
python3 work/dreamcast-recovery/audit-timer-init-startup.py 20261006-133000
# Original baseline-regression failed ENOSPC; exact same binary, new output directory:
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression-timer-repair1.py 20261006-133000
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-timer-init.json
python3 work/dreamcast-recovery/checkpoint-timer-init.py
```

Prefix1024cases/8hypothetical timerstates; actualprobe25TMUobservations/69CPU
prediction; full1344cases/actualfrozencall. Whole16MiB RAM/69CPU/25TMUprospective
at timerentry/return.195cycles describe current emittercosts, not hardwaretiming.
Earlierphasecaptures omittedcurrent; prior789identities reverified. Both probe
and currentbinary baseline pass; currentretry preservesfailedpartialoutput.
Checkpoint binds1,142identities.810compiled/cap812 does not imply gamecoverage.
Next648422 reduction uses actuallyregistered60634E/606358; firstabsent65F914.
One staticbatch remains beforeexecution. Source-onlyrequest remains unchanged:
missingKatana closure/supportedPALdump keep originalG1unresolved.


## 2026-10-06 — R3/C8 timed startup work and task scan,135001

These commands produced existing evidence; runners refuse to overwrite it.
The prospective arithmetic preflight rejection is preserved under
`20261006-startup-work-reduction/preflight-rejection`. No build/game ran before
correcting the71-cycle scan/117-cycle prefix model.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-work-reduction.py
python3 work/dreamcast-recovery/predict-startup-work.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-work.py
python3 work/dreamcast-recovery/audit-startup-work.py 20261006-135001
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-135001
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-startup-work.json
python3 work/dreamcast-recovery/checkpoint-startup-work.py 20261006-135001
```

The full runner records `SONICPAD_RECOVERY_CAPTURE_WORK_ONLY=1`; original
observers always call the translated routines.1,920reduction cases and one actual
frozen call pass. Four prospective phases compare entire16MiB RAM/69CPU/25TMU:
workentry,recordscanentry/return,failure. Earlier captures deliberately omitted;
previous1,142identities reverified. Current checkpoint binds424identities.
Exact-binary supplied-scene baseline passes.812compiled/cap816 is inventory,
not gamecoverage. Next execute65F25A→6615A2 prefix with24-byte saved stack/R4=SP,
then explicit missing6607A0 rejection. Two static batches consumed. Free~1.5GiB,
measure before another build. OriginalG1/normalboot/gameplay/mobile remain open.


## 2026-10-06 — R3/C8 critical-section task scan,140051

Recorded commands produced immutable evidence; runners refuse to overwrite it.
All runs select CLT per process and disable GPR caching/hot traces. Prefix and
fullscan sources are frozen separately before the full startup integration.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-task-critical-prefix-reduction.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-task-critical-reduction.py
python3 work/dreamcast-recovery/predict-task-critical-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-task-critical-startup.py
python3 work/dreamcast-recovery/audit-task-critical-startup.py 20261006-140051
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-140051
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-task-critical.json
python3 work/dreamcast-recovery/checkpoint-task-critical.py 20261006-140051
```

The full runner records `SONICPAD_RECOVERY_CAPTURE_CRITICAL_ONLY=1`; observers
always call original code.2,048prefixcases,3,584fullcases and actual frozen calls
pass entire16MiB RAM/69CPU/25TMU expectations. Three prospective startup phases
include normal IRQmask/stack restoration and23cycles caller continuation before
10E4CC.129/125cycles describe current emitter costs, not hardware accuracy.
Earlier captures omitted; previous424identities reverified. Currentcheckpoint547
plus latest supplementalsharing88 identities. Exact-binary baseline passes.
APFS sharing56baseline observations per135001/140051 preserves allhashes/paths.
816compiled/cap820 is inventory; no normalboot/gameplay/mobile/G1 acceptance.
Next10E4CC sixteen40-byte records; registered65EAA8/65EACE, actual67FF20/24=0,
active10E46E absent. One staticbatch remains beforeexecution. Free~1.0GiB;
check capacity again before compiling.


## 2026-10-06 — R3/C8 sixteen-record scan,141129

Recorded runners refuse to overwrite existing evidence. The reduction includes
original10E4CC/65EAA8/65EACE; active10E46E stays unregistered and must reject.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-record16-reduction.py
python3 work/dreamcast-recovery/predict-record16-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-record16-startup.py
python3 work/dreamcast-recovery/audit-record16-startup.py 20261006-141129
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-141129
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-record16.json
python3 work/dreamcast-recovery/checkpoint-record16.py 20261006-141129
```

The full runner records `SONICPAD_RECOVERY_CAPTURE_RECORD16_ONLY=1`; read-only
observers always call original routines.6,144cases/actual frozen call and three
prospective whole16MiB/69CPU/25TMU phases pass. Originalnesting0→1→0/IRQmask,
stack restoration and23caller cycles before65FEDE are independently checked.
178/148cycle model describes emitter costs, not hardware timing. Earlier captures
omitted; prior547 and supplemental88identities reverified. Currentcheckpoint3,973
identities includes all3,195 APFS-shared completed.o/.a targets/references and
terminalreport/helper hashes. Source/assets/saves/inputs/live builds excluded;
no content/path removal. Exact-binary supplied-scene baseline passes.
817compiled/cap820 is inventory. Next65FEDE32x100-byte inactive records, active
65FED0 absent. One staticbatch remains beforeexecution;free~2.3GiB, remeasure.
OriginalG1/normalboot/gameplay/mobile remain unverified.


## 2026-10-06 — R3/C8 thirty-two-record scan and timed-work return,142605

Recorded runners refuse to overwrite evidence. First reduction preserves a
reversed saved-register stack expectation; repair1 changes expected indexing
only. Original65FEDE and64847C suffix each resume actual frozen state once.
Active65FED0/660754 remain absent and explicitly reject.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-record32-reduction.py
python3 work/dreamcast-recovery/predict-record32-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-record32-startup.py
python3 work/dreamcast-recovery/audit-record32-startup.py 20261006-142605
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-142605
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-record32.json
python3 work/dreamcast-recovery/checkpoint-record32.py 20261006-142605 'Reduce original010B58 signed guard; actual flag1/count0 predicts0. WholeRAM/69CPU/25TMU and frozen caller, then0556D8. One static batch remains.'
```

`SONICPAD_RECOVERY_CAPTURE_RECORD32_ONLY=1` captures entry/return/workreturn and
failure. Three prospective phases verify whole16MiB RAM/all69CPU/all25TMU;
later failure/audio/device counters unpredicted.4,608cases,236/121emittercycles,
no hardware timing accuracy. Original648422 returns0556D4/SPFFFFC8,totalelapsed3;
next010B58. Exact-binary regression passes. Earlier phases omitted/prior3,973
identities reverified;438current identities. Packaging table-union correction
preserves rejected checker; no startup rerun.12patches/inputs/flash/rootVMU unchanged.
818compiled/cap820 is inventory;free~1.6GiB/noSimulator. Source/PAL/G1 blockers
unchanged. One work-routine return does not prove complete main-loop or gameplay.


## 2026-10-06 — R3/C8 original signed guard and caller,143441

Runners refuse to overwrite prior evidence. Only original010B58 is added to the
full closure;0556D8/0556DC already exist as real caller continuation registrations.
The reduction includes010B58/0556A0 and preserves explicit0558C0/010B7A rejection.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-main-guard-reduction.py
python3 work/dreamcast-recovery/predict-main-guard-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-main-guard-startup.py
python3 work/dreamcast-recovery/audit-main-guard-startup.py 20261006-143441
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-143441
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-main-guard.json
python3 work/dreamcast-recovery/checkpoint-main-guard.py 20261006-143441 'Reduce original0558C0 mode0 prologue/table dispatch, first04E310 missing. FullRAM/stack/69CPU/25TMU and frozen caller; original rejection, invalid-mode return. One classification batch remains.'
```

`SONICPAD_RECOVERY_CAPTURE_MAIN_GUARD_ONLY=1` captures read-only original guard
entry/return and failure.4,096cases and three prospective unchangedwhole16MiB
RAM/69CPU/25TMU phases pass. Originalguard actualflag1/count0 returns0, conditional
caller rejects0558C0.8/13/15/16guard+5caller emittercycles; no hardware timing.
No oracle/core/game repair. Exactbinary regression passes;415identities/allprior
438 reverified. Earlier captures omitted/audio/device counters unpredicted.
819compiled/cap820 is inventory; next function needs a bounded adjusted cap.
Next cached mode0 tableoffset68→05593C, first04E310 absent.12patches/private inputs/
flash/rootVMU unchanged. OriginalG1/normalboot/gameplay/mobile remain unverified.
Free~1.0GiB: remeasure beforebuild; allterminal/noSimulator, no deletion/sharing.


## 2026-10-06 — R3/C8 original mode dispatcher prefix,144501

Runners refuse to overwrite evidence. Only original0558C0 is added to full
closure, bounded cap824. One-function reduction keeps04E310/04562E missing;
default epilogue calls04562E then08C25E, so its return remains unverified.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-mode-prefix-reduction.py
python3 work/dreamcast-recovery/predict-mode-prefix-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-mode-prefix-startup.py
python3 work/dreamcast-recovery/audit-mode-prefix-startup.py 20261006-144501
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-144501
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-mode-prefix.json
python3 work/dreamcast-recovery/checkpoint-mode-prefix.py 20261006-144501 'Execute original04E310 prefix through actualmissing051354, savedPR/R4=0 delay/fullRAM/stack/69CPU/25TMU and frozen caller. Later051354 must includeoriginalregistered09AC9C. Two classification batches consumed: execute next.'
```

`SONICPAD_RECOVERY_CAPTURE_MODE_PREFIX_ONLY=1` records read-only original entry
and missing04E310 boundary.1,280cases/two prospective wholeRAM/stack/69CPU/25TMU
phases pass;31/32/27emittercycles, no hardware claim. Exactbinary regression passes.
996identities/allprior415verified; corrected rejected handoff preserved, no
runtime/oracle/execution rerun.190completed.o/.a and168diagnosticcaptures shared
using terminal-bound plans; metadata/bytes/paths retained exceptinode/birthtime.
Assets/saves/inputs/source/live outputs excluded; hash/helper/report identities
checkpoint-bound. Helpers/plans/reports live under private reduction object-sharing/
baseline-sharing; unused narrowplan retained. Free~1.2GiB: remeasure beforebuild.
820compiled/cap824 is inventory. Mode/table unchanged, pending sevencommands/
peripheralop1. Actual rootchild051354 absent;09AC9C already present in callback
provider and must not be falsely rejected in a child-integrated test. Allterminal/
noSimulator; originalG1/defaultreturn/initialization/normalboot/gameplay/mobile open.


## 2026-10-06 — Initialization-call and language/resource prefixes, 20261006-150748

These runners refuse to overwrite evidence. Prediction precedes full startup.
Only04E310 is added to full closure; child051354 remains absent there. Original
09AC9C/09F5A0 and its loader chain are registered in full provider. The isolated
language test deliberately excludes09F5A0 to bound its prefix, not to model a
production missing target. No missing-helper stub or readiness injection.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-init-call-prefix.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-language-entry-reduction.py
python3 work/dreamcast-recovery/predict-init-call-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-init-call-startup.py
python3 work/dreamcast-recovery/audit-init-call-startup.py 20261006-150748
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-150748
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-init-call.json
python3 work/dreamcast-recovery/checkpoint-init-call.py 20261006-150748
```

512 caller and9,216 language/resource cases pass; both prospective full-startup
phases and exact-binary baseline pass. Rejected auxiliary trace label retained;
no runtime/game/state-prediction change or startup rerun. Checkpoint449identities/
all996prior verified, SHA256f8c2d1638221b5fb9a49beabcdeb60ae8687f15235db078f220c6d2798c31faa. 821compiled/cap824 is inventory.
Free~0.49GiB: remeasure before another build. No Simulator launched. Next
independent owner-CDI s_mot.PRS extraction/decode expectations and registered
motion-loader reduction. Initialization completion/normalboot/gameplay/mobile/
production/G1 remain unresolved.


## 2026-10-06 — Motion loader with transferred original startup prefix

Private output: `20261006-motion-loader-reduction-repair3`. Same unchanged
provider library/callback objects from150748, verified by hashes before/after.
Fresh oracle is ARM64 and screened for executable-memory/process-launch imports.
Source model and independent owner-disc extraction precede execution. Previous
failed outputs/frozen source versions are preserved; runners refuse overwrite.

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-motion-loader-reduction-repair3.py
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-motion-loader.json
python3 work/dreamcast-recovery/checkpoint-motion-loader.py
```

Three fixture fixes: typed declarations, separate69CPU/25TMU input files, and
existing full-provider BIOS HLE registration before restoring transferredRAM/CPU.
No runtime/game/data/return injection. One directcall passes105sectors/215040
sector bytes/488608 decoded bytes,64original guardbytes, savedR8–R15/PR and
returnPC/result/cache shadow. Captured wholeRAM/69CPU/25TMU are not fully modeled
afterloading; live opaque audio/clock/interrupt state omitted. Full051354 remains
absent from startup150748. Full binary baseline reverified, not rerun unchanged.
Checkpoint204identities/prior449verified; SHA2562364cd216fc0b8b01e710e880d2fcd9542b359e3a3d72b8681079b749f2d57c2.
Free~0.30GiB: capacity check before full integration. Next original051354
fullstartup with prospective prefix and motion checks; originalG1/normalboot/
gameplay/save/mobile/production remain open.


## 2026-10-06 — Full language prefix and live motion IRQ boundary, 20261006-152405

One changed fullstartup adds only051354. Read-only observers retain original
continuation bindings across IRQ unwind. Source predictions precede launch.

```sh
python3 work/dreamcast-recovery/predict-language-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-language-startup.py
python3 work/dreamcast-recovery/audit-language-startup.py 20261006-152405
python3 work/dreamcast-recovery/audit-language-startup-prefix.py 20261006-152405
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-152405
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-irq-word-reduction.py
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-language-startup.json
python3 work/dreamcast-recovery/checkpoint-language-startup.py
```

Full motion-return audit remains **unmet**: originalIRQ redirects duringPRS04E288
and scheduler rejects664556 before its return. Two prospective prefix phases
pass wholeRAM/69CPU/25TMU;105sectors/215040encodedsector bytes verified. No complete
decode claim. Separate partial audit records failure honestly; unmet checker kept.
Original664556 reduction2048cases passes wholeRAM unchanged/69CPU/25TMU/fivecycles,
frozen IRQcaller returns255. Reducedruntime/image source identities match cache;
no runtime/game/oracle repair. Exact full-binary baseline passes.613identities/
prior204verified; SHA25663599ef336866b8765f396213254b266325bc7dada01774015ff2073fc2e9258.822compiled/cap824 is inventory.

Storage plan/helpers/reports underlanguage-startup-storage preserve112completed
144501/150748 diagnosticcaptures:498136892logical bytes/full availablechange
481906688, hashes/paths/metadata retain exceptinode/birthtime. No protected contents
removed. Allterminal/noSimulator; free~0.16GiB. Next capacity check, add only
664556 (823/cap824), predict IRQ helper entry/return and measure originalIRQ/decode
resumption. Fullinitializer/normalboot/gameplay/mobile/production/G1 remain open.


## 2026-10-06 — Original IRQ word accessor integration, 20261006-153926

Only664556 added to callback closure. Read-only entry/return observer calls the
original and restores the binding. Source predictions precede one changed startup.

```sh
python3 work/dreamcast-recovery/predict-irq-word-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-irq-word-startup.py
python3 work/dreamcast-recovery/audit-irq-word-startup.py 20261006-153926
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-153926
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-irq-word-startup.json
python3 work/dreamcast-recovery/checkpoint-irq-word-startup.py 20261006-153926
```

Three prospective wholeRAM/69CPU/25TMU phases pass, R0=255/fivecycles/RAMunchanged.
Original scheduler tenreadcalls then rejects64A8FC; IRQcompletion/decode/motion
return remain unmet. Exact-binary baseline passes.1017identities/all613prior
verified; SHA25663b384a200b8f1120a8366e920feaa1c1e69854203ab9ccd6e4eb26f650a672f.823compiled/cap824 is inventory.

Storageplans/report/helper versions underirq-word-startup-storage preserve172
completedproviderobjects/56completedbaselinecaptures/143completedreducedobjects,
all hashes/metadata/paths retained exceptinode/birthtime, no protected content
removed. Free~0.20GiB: capacity check before next build; allterminal/noSimulator.
Next source-bound64A8FC VMU function-word reduction with registered64A882.
Actualsource-only halfwords1/512/4/1/0 need execution verification; preserve
null/success, stack/RAM/CPU/TMU/frozenIRQcase. Two classification batches consumed:
execute before more inventory. OriginalG1/normalboot/gameplay/save/mobile open.

## VMU function-word decoder — checkpoint155608

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-vmu-fields-reduction.py
python3 work/dreamcast-recovery/predict-vmu-fields-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-vmu-fields-startup.py
python3 work/dreamcast-recovery/audit-vmu-fields-startup.py 20261006-155608
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-155608
python3 work/dreamcast-recovery/checkpoint-vmu-fields-startup.py
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-vmu-fields-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

These are recorded commands, not instructions to overwrite dated outputs.
For reproduction select fresh directories and preserve rejected artifacts.
The reduction reuses a byte-verified cached ARM64 runtime archive with matching
resident-image/runtime source and compiler/SDK identity; it supplies its own
original decoder/lookup functions, with no child stub. Success273+4*upperpopcount
and null253+4*popcount cycle models pass; actual caller281cycles/return0.
WholeRAM/stack/69CPU/25TMU verified, both null/success paths retained.

Fullstartup predicts VMU entry/return before execution and observes unchanged
original code through a read-only wrapper. Earlier motion-entry capture is
omitted to constrain output storage; its previously verified return contract
remains explicitly unmet. Fullstartup rejects next64EF30; exactbinary two-scene/
ten-audit/release/six-image baseline passes. Frozen source sets, new storage
plans/reports and next-channel preflight are bound by checkpoint859identities,
SHA256 `186bdb52b300444469eaf832a3fac6fafced37fe6021036eec1e329d1861997d`; all1017prior/62reduction identities reverified.

Storage helpers operate only on completed diagnostic/build outputs. Five fresh
sharing groups preserve63+56+11+62+3files. Generated-image sharing explicitly
extends prior object/capture sharing to three identical generated dc_image.cpp
copies; no original BOOT/disc/runtime source selected. First/full measurements
remain separate, inode/birthtime changes declared, content and other metadata
retained. Free~0.12GiB requires a new capacity check before another build.
All oracles terminal, no Simulator launched. Next: original64EF30 channel-init
prefix, channel0/flags0,0,0, firstmissing64EB72; preserve statusclear/stack/MACL/
index/flags and both routes with wholeRAM/69CPU/25TMU before integration.

Later final verification records~9.18GiB available, confirmed twice after the
checkpoint had~0.12GiB. Change origin undetermined; no content deletion occurred
in this turn. See155608/final-verification.json. Remeasure before the next build.

## Original channel-init prefix — checkpoint160823

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-channel-init-reduction.py
python3 work/dreamcast-recovery/checkpoint-channel-init-reduction.py
python3 work/dreamcast-recovery/predict-channel-init-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-channel-init-startup.py
python3 work/dreamcast-recovery/audit-channel-init-startup.py 20261006-160823
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-160823
python3 work/dreamcast-recovery/checkpoint-channel-init-startup.py
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-channel-init-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Recorded commands, not permission to overwrite dated outputs. Select fresh output
names for reproduction. Initial reduction24cycles fails timer equality; its
frozen sources/report/generation/logs remain under channel-init-reduction.
The corrected runner uses channel-init-reduction-repair1, sourceMUL.L2 (instead
of the oracle's assumed1), total25cycles. No guest/runtime/provider/input patch.
Repair passes2048cases/512zero/1536present flags, whole16MiB RAM/five stack words/
69CPU/25TMU/bothfirst-child branches. Real missing children remain unregistered;
no handler stub or flags changed to simulate success. Cached ARM64 runtime/image
source identity is verified; own generated parent replaces unused cached symbols.

Live two phases are predicted before execution: firstchannelentry and firstchild
rejection. ExactRAM/CPU/timers match; first child64EB72 actually missing at64EF5A.
No initializer, IRQ or motion return completion. Exact fullbinary baseline passes
2suppliedscenes/10audits/releasecontrols/6unchangedimages.20full source versions,
4sources per reduction attempt frozen. Checkpoint438identities/all859prior and
92reduction reverified, SHA256 `3b44afc3af1544a2d738458e4f0ab324645298c678259b4509f40213cce9b9db`.
Sourceclosure still exits2; exact supported PAL descriptor absent. No Simulator.
No new storage compaction/removal this turn; free~7.05GiB before finalchecks.
Next reduce64EB72 original12-instruction wrapper: actualdescriptor8C8A5230,
sourceR5FF/R68C6B4334, tail64B350 unregistered. Preserve one stack write/R14-SP
restore/PR/JMPdelay/MACL/index/indirectreads/fullRAM/69CPU/25TMU. Source14cycles
remain unexecuted; no completion or VMU persistence claim.

## Original channel wrapper — checkpoint161749

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-channel-wrapper-reduction.py
python3 work/dreamcast-recovery/checkpoint-channel-wrapper-reduction.py
python3 work/dreamcast-recovery/predict-channel-wrapper-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-channel-wrapper-startup.py
python3 work/dreamcast-recovery/audit-channel-wrapper-startup.py 20261006-161749
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-161749
python3 work/dreamcast-recovery/checkpoint-channel-wrapper-startup.py
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-channel-wrapper-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Recorded commands; reproduce into fresh directories without overwriting evidence.
Original12instructions compile without native overrides/RAW or unknown ops.
Verified cached ARM64 runtime/image source and compiler/SDK identity reused;
own generated wrapper provides its actual code/registrations.7168cases include
seven modulo32 selectors, four arguments, P1/P2 descriptor and stack aliases,
IRQ/T/FPU state. One savedstackword/R14-SP restore/JMPdelay/PR-SR preservation/
record28+32/descriptor8 reads/whole16MiB RAM/69CPU/25TMU/14cycles pass. Tail64B350
actually unregistered; no fake return or forcedflag. DiagnosticPC is emitted
basic-block start64EB72; original JMPinstruction64EB86, handler dispatch still
honestly rejects. Actualcaller R5FF/R68C6B4334/PR64EF5E/SP8997A0 confirmed.

Two live phases predict before execution and match; exact fullbinary baseline
passes2suppliedscenes/10audits/releasecontrols/6unchangedimages.20fullsources and
4reduction sources frozen. Checkpoint410identities/all438prior+62reduction verified,
SHA256 `cf6fbab6c0073be268539c4841acaa78fb9b8f7b6746fea8d2a3f8fc2045bd11`. Handler/initializer/IRQ/motion completion remain open.
No Simulator; source auditexit2/PAL descriptor absent/verifierexit2. No storage
compaction/content removal this turn; free~6.24GiB before finalchecks.
Next reduce64B350 prefix to callback6647BE with44byteframe/eightpushes/three locals/
R5-R6 reload/MACL/index/globalretry10/canceladdress/signedliterals/JSRdelay-PR/
wholeRAM/69CPU/25TMU. Source29cycles unexecuted. Later64B91E is alreadyregistered
and unreachable before this firstmissingcallback; do not call it missing or stub.
No retry-count/readyflag/callback-result injection or VMU-persistence acceptance.


### Original channel-handler prefix checkpoint — 20261006-163159

Original64B350 now reaches actually unregistered6647BE. The isolated10752cases
and two prospective live phases verify whole16MiB RAM/69CPU/25TMU/29cycles,
44byteframe/11writes and originalJSRdelay-PR. No readiness/retry/cancel override.
Exactfullbinary baseline passes2suppliedscenes/10audits/releasecontrols/6images.
827functions/cap828 is inventory; handler/IRQ/motion completion and G1 unmet.

```sh
python3 work/dreamcast-recovery/audit-channel-handler-startup.py 20261006-163159
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-channel-handler-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Unique reduction/prediction/full-run/checkpoint/baseline outputs already exist;
inspect/reverify, never overwrite or rerun unchanged experiments. Twenty full
source versions are frozen. The new checkpoint419identities/all410prior+62reduction
verified has SHA256c05641ce13557f8fd86673dad7a338276fe64e5954b1acc6214066ff1ce0f237.
Next execute original6647BE reduction with its registered readiness664B2C and
accessors. Actualhandle1/presentbyte1/error0/status0xE predictsready1, then first
unregistered64A708 (fifth stack argument/record-count loop). Verify the source
prediction, do not force readiness. Later66453C remains absent/unreachable
before firstcommand;64B91E registered. Oneclassificationbatch used. Full callback
integration needs measured cap increase after reduction passes. No new Simulator.


### Original readiness and command-call checkpoint — 20261006-164338

Original6647BE executes its registered readiness664B2C/accessors and reaches
missing64A708 with R4handle1/R5command2/R6-R7zero/fifthargumentFF. Reduction
14336readinesscases and21504callbackcases verify readiness alternatives plus
nonpositive signedcount returns and positivecount command rejection. Both live
prospective wholeRAM/69CPU/25TMU phases match after183 modeledcycles; nested
stack/fifthargument/retry10/cancel1 preserve. Exactfullbinary baseline passes.
Initial metadata hash validation failed before generation/execution; sources
were unchanged. Rejected metadata/report/source versions remain; repair1 passed.

```sh
python3 work/dreamcast-recovery/audit-channel-command-startup.py 20261006-164338
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-channel-command-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Unique experiment/checkpoint/baseline outputs already exist; inspect/reverify,
never overwrite/restart unchanged experiments. Twenty full sources and four
sources per reduction attempt frozen. Checkpoint432identities/all419prior+72reduction
verified SHA2565ab9178cd3c5c93f9dc204c5ff40dc06034ad4a50a331472f995c114fbe4e014;828functions/cap830 inventory.
Next reduce original64A708 first-child prefix:12byteframe/two writes/fifthargument
forwarding/R4-R7/JSR-NOPdelay-PR/fullRAM/CPU/timers/frozen caller to unregistered
664226. Later64A394 is registered but not reached; source8cycles unexecuted,
oneclassificationbatch used. No command/callback/handler/IRQ/motioncompletion,
VMUpersistence, G1, normalboot, gameplay or mobile acceptance.


### Original command-wrapper prefix — 20261006-165051

Original64A708 first-child prefix passes8192isolatedcases and two prospective
live phases: whole16MiB RAM/69CPU/25TMU/8modeledcycles/12byteframe/two writes/
fifthargument forwarding/R4-R7/SR-MACL/JSR-NOPdelay-PR. Actualhandle1/command2/
R6-R7zero/fifthFF/retry10/cancel1 preserve; firstmissingchild664226. Later64A394
registered but not reached. Exactfullbinary scene regressions pass.

```sh
python3 work/dreamcast-recovery/audit-command-wrapper-startup.py 20261006-165051
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-command-wrapper-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Unique experiment/checkpoint/baseline outputs already exist; inspect/reverify,
never overwrite/restart unchanged experiments. Twenty full sources and four
reduction sources frozen. Checkpoint411identities/all432prior+62reduction verified
SHA2567361bf25402d404d73124eaf604d3716679cac0702bc4ce3067467acb3c3ce96;829functions/cap830 inventory.
Next reduce664226 lock/first-child prefix: actual8C664538 byte0 predictsTAS.B
byte80/T1/acquiredchild664258 unregistered. Preserve real lock/neighborbytes,
PR/two stackwords/fifthargument/R4-R7/delays/RAM/CPU/timers; no free-lock injection.
Busy663088 alreadyregistered inprovider, not missing. Incorrect draft wording
preserved/corrected beforecheckpoint. Pinned emitter-derived10cycles unexecuted;
oneclassificationbatch used. No wrapper/command/IRQ/motioncompletion/VMU/G1/
normalboot/gameplay/mobile acceptance.


### Original lock acquisition — 20261006-170200

Original664226 TAS.B prefix verifies16384isolatedcases/two prospective live
wholeRAM/69CPU/25TMU phases/10cycles, actuallock00-to80/T1 and acquiredmissing
664258. Busy663088 is registered in full; only isolatedbusyentry prefix verified.
Initial oracle compile assertion error fixed after terminal in fresh-repair1;
actual execution exit0 subsequently rejected by stale runner PASS marker.
Separate saved-execution validator checks source/input/binary/result identities,
without reexecution or overwriting raw reports. Both source versions preserved.

```sh
python3 work/dreamcast-recovery/audit-command-lock-startup.py 20261006-170200
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-command-lock-startup.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Unique experiment/checkpoint/baseline/validation outputs already exist; inspect/
reverify, never overwrite or rerun unchanged execution. Twenty full sources and
four sources per reduction attempt frozen. Checkpoint441identities/all411prior+
91reduction verified SHA2564a0cdd3ba25628a74b356e761796c44e0f64b128e7fdaa0745557245abcab12e;830functions/cap832 inventory.
Next reduce664258 requestpacking to registered663550 call entry, not a missing
provider: modulo32 shifts-OR/two request words/sixwrites-24byteframe/parameter
pointer/two zero stackarguments/R4handle/R5=11 inBSRdelay/R7=2/SR-MACL/fullRAM/
CPU/timers. Sourcewords2/FF/lock80,20cycles unexecuted. Prospectively compare
parent/child entry and then observe real existing-provider continuation; no
assumedreturn/failure, unlock, childstub or G1/boot/gameplay/VMU/mobile acceptance.

### Original request packing and live motion return — 20261006-171357

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-command-pack-reduction.py
python3 work/dreamcast-recovery/checkpoint-command-pack-reduction.py
python3 work/dreamcast-recovery/predict-command-pack-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-command-pack-startup.py
python3 work/dreamcast-recovery/audit-command-pack-startup.py 20261006-171357
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-171357
python3 work/dreamcast-recovery/checkpoint-command-pack-startup.py 20261006-171357
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-command-pack-startup.json
```

These are recorded commands, not instructions to overwrite existing evidence.
A new execution requires fresh output directories and a changed hypothesis.
Isolated parent-only reduction deliberately omits663550's body; full startup
invokes its real registered provider. 8192 cases and both prospective live phases
match RAM/CPU/timers, request words2/FF and six writes/24-byte frame/20 cycles.
Original motion returns with the retained105-sector/215040-byte encoded/488608-byte
decoded contract, guards, saved registers, PC/PR/result/cache shadow all matching.
The next actual missing target is08BD1C; no full boot or gameplay claim follows.
Exact-binary regression passes;426 checkpoint identities verified. Original G1
and the source-only acquisition request remain unresolved.

### Original startup byte write and return — 20261006-171851

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-byte-reduction.py
python3 work/dreamcast-recovery/checkpoint-startup-byte-reduction.py
python3 work/dreamcast-recovery/predict-startup-byte-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-byte-startup.py
python3 work/dreamcast-recovery/audit-startup-byte-startup.py 20261006-171851
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-171851
python3 work/dreamcast-recovery/checkpoint-startup-byte-startup.py 20261006-171851
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit-20261006-startup-byte-startup.json
```

Recorded commands use immutable existing evidence directories; use fresh output
paths for a changed experiment. Original08BD1C four instructions execute with
2048 cases and actualcaller matching wholeRAM/69CPU/25TMU/5modeledcycles.
Actual8C756C60 byte0→4, literal, neighbors, R0ptr/R3=4, SR-MACL/saved regs/
unchangedstack/RTSdelay-PR match two prospective live phases. Retained original
motion return still passes. Next actually missing026E80 initializes tables before
its unregistered026BE0 child. Exact-binary baseline passes;417 checkpoint
identities/all426prior+62reduction reverified. No table/status injection, normal
boot, gameplay, VMU/save/mobile or G1 acceptance. Acquisition request unchanged.

### Original table initialization — 20261006-172614

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-table-reduction.py
python3 work/dreamcast-recovery/checkpoint-startup-table-reduction.py
python3 work/dreamcast-recovery/predict-startup-table-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-table-startup.py
python3 work/dreamcast-recovery/audit-startup-table-startup.py 20261006-172614
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-172614
python3 work/dreamcast-recovery/checkpoint-startup-table-startup.py 20261006-172614
```

Recorded existing outputs are immutable; fresh experiments need fresh directories.
2048 cases and two live phases verify wholeRAM/69CPU/25TMU/101cycles/36writes/
three stackwords/12-byteframe/FLDI1 activebank/tables/status/BSRdelay-PR.
Actual missing026BE0 remains explicit; full initializer has not returned.
Motionreturn and exact-binary regressions pass.559checkpoint identities verified.
112 completed171357/171851 bin/ppm captures APFSshare exact bytes with passed
161749 reference captures, preserving paths/permissions/times/xattrs; no content
removed, inode/birthtime changes declared. Metadata failure reports/drafts remain.
Next full026BE0 must execute real registered10EF3C helper (13312/384 overlapping
clears), verify flags/records/full return and parent epilogue before continuing.
Two classification batches used; execute next. G1/source-only request unchanged.

### Original clear/record initializer and parent return — 20261006-173930

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-clear-reduction.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-clear-reduction-repair1.py
python3 work/dreamcast-recovery/checkpoint-startup-clear-reduction.py
python3 work/dreamcast-recovery/predict-startup-clear-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-startup-clear-startup.py
python3 work/dreamcast-recovery/audit-startup-clear-startup.py 20261006-173930
python3 work/dreamcast-recovery/baseline-loop-regression.py 20261006-173930
python3 work/dreamcast-recovery/checkpoint-startup-clear-startup.py 20261006-173930
```

Recorded evidence directories are immutable; new experiments require fresh paths.
Initial build failed on mixed-width auto variables; a narrow declaration split
in freshrepair1 passes. Both raw reports/source versions remain. Original97
instructions/3functions emit without overrides/unknown/RAW instructions.
2048 cases execute both real helper calls and verify overlapping13312/384 clears,
enable1/26byte8 records-512stride/wholeRAM/69CPU/25TMU/68608cycles. Actual parent
continuation026ED0 returns after9cycles with finalFF byte/zero word/savedPR-R13-
R14-SP. Three prospective live phases plus retained motionreturn and exact-binary
regression pass. Next08C218 has alreadyregistered shared08C220 and helper0513F8;
verify their real behavior/full return instead of omitting known providers.
600checkpoint identities verified. OriginalG1/source-only request unchanged.


## 2026-10-06 — Original flag entry/shared initializer return,175126

Frozen execution evidence: `artifacts/recovery-aot/20261006-175126/`,
`20261006-startup-flags-reduction/` and
`20261006-startup-flags-startup-prediction/`. The private runner/oracle sources
are preserved with each report. Isolated entry plus shared callable body plus
real getter:3functions/54instructions/0external records;4096cases pass. No helper
stub or hostflags. Original fullentry executes27instructions and returns04E31E;
actualvalue0 costs29modeled cycles. Two prospective live phases match whole16MiB
RAM/69CPU/25TMU; retained motionreturn and exactbinary2scene/10audit/release/
6image baseline pass. One fullrun only, terminalmissing08BCEA, not normalboot.

The next executable experiment is original08BCEA,9instructions/3writes,
byte4@756C60/zero16@756C64/zero32@756C78/RTS-store delay/fullreturn04E324 and
10modeled cycles. There are no helpers to omit; actual entry is unregistered in
both generated tables. Test original writes even where live oldvalues already
match. Preserve allneighbors and completeCPU/TMU/stack/SR-MACL-FPU. Only after
isolated checks, integrate that seed with measuredcap838; require motionreturn
and the exactbinary baseline again. Do not rerun an unchanged fullbuild.

APFS sharing of112identical completed172614/173930 diagnostic captures recovers
about474MiB; bytes/paths/metadata preserved except declaredinode/birthtime changes.
No source/input/assets/save removal.600prior identities verified before/after.
Fresh G1 audit exits2 with identical missing closure; PAL verifier exits2 because
private-inputs/pal-v1003/sonic-adventure.gdi remains absent.


## 2026-10-06 — Original status writer/full return,175916

Frozen execution artifacts: `artifacts/recovery-aot/20261006-175916/`,
`20261006-startup-status-reduction/` and
`20261006-startup-status-startup-prediction/`. Private runner/oracle sources are
preserved alongside the reports. One original routine/nine instructions/zero
external records;4096cases pass: byte4/zero16/zero32 with nonzero prior values,
independent P1/P2 destinations and stack aliases/IRQ-T-FPU/neighbors/unchanged
stack/RTS-store32 delay/R0-R4/SR-MACL-FPU/fullreturn04E324/10modeledcycles. Frozen
already4-zero-zero still runs original stores; no host writes. Two prospective
live wholeRAM69CPU25TMU phases, motionreturn and exactbinary2scene/10audit/
release/6image baseline pass. One fullrun, terminalmissing091A20; not normalboot.

Next execute original091A20 prefix through its seven clear loops, overlapping
2416writes/2336unique cleared bytes, twozero words and savedPR/4byteframe to
actually unregistered0922E0. Verify DT/BF-S increments/delays/BSRdelay/PR091A8C/
missinglookupPC091A80/SPFFFF9C/T1/all RAM69CPU25TMU/9691modeledcycles/frozencaller.
Later04AE40 is also unregistered but unreached; fullreturn/finalzero79E584 remain
unexecuted. No knownhelper omission or hostclear. Integrate only091A20/cap838
after isolated checks and retain motionreturn/exactbinary regression. One
classificationbatch used; execute before further inventory.

Storage sharing:56completed baseline captures/677compiled.o-.a outputs;
802566574logical bytes/about767MiB recovered, hashes/paths/metadata preserved
except declaredinode/birthtime. All569prior identities/733outputhashes verified.
No source/assets/saves/inputs removed. Fresh sourceaudit and PAL verifier both
exit2; exact Katana/source-only request unchanged.


## 2026-10-06 — Original buffer prefix/first missing child,180910

Frozen artifacts: `artifacts/recovery-aot/20261006-180910/`,
`20261006-startup-buffer-reduction/` and
`20261006-startup-buffer-startup-prediction/`. Private runner/oracle sources are
preserved alongside the reports. One routine/62instructions/two externalrecords;
4096cases pass seven clears96/512/344/96/512/344/512/2416stores/2336unique bytes,
original overlaps/guards/twozero32words/savedPR4byteframe/DT-BF-S increments/
BSRzero delay/T1/registers/SR-MACL-FPU/P1-P2 buffers-words-stack/wholeRAM69CPU25TMU/
9691modeledcycles. Actual missing0922E0 retained, no knownhelper omission.
Both prospective live entry/firstmissing-child failure phases, motionreturn and
exactbinary2scene/10audit/release/6image baseline pass. One fullrun only.
Parentreturn/finalzero/fullinitializer/IRQ/audio/boot/gameplay remain unproved.

Next full original0922E0,31instructions/no calls: validR4 selectors0/2/3/5/6/7
writebyte1@base79D9A0+65..70; invalid values do not store. Test all branches,
BT-S MOV andBRA ADD delays, RTS/NOP, R0/R5/R6/T, unchangedstack/PR-SP/SR-MACL-FPU/
RAM69CPU25TMU/P1-P2 destination/cycles11/13/15/17/19/20. FrozenR4zero predicts
byte79D9E1zero-to1/return091A8C/11cycles; unexecuted. Then execute original parent
091A8C continuation4cycles to genuinely missing04AE40, preserving savedparentPR/
clearedbuffers and leaving epilogue/finalzero/fullreturn unexecuted. Integrate
only0922E0/proposedcap840 after isolated checks; retain motionreturn/exactbinary
regression. Oneclassificationbatch used: execute before broader inventory.

APFS sharing preserves56 completedcaptures/61compiledobjects/298955726logical
bytes, about285MiB recovered; bytes/paths/metadata retained except declaredinode/
birthtime changes. All1270prior identities/117outputhashes verified before/after.
No source/asset/save/input content removed. Fresh sourceaudit/PAL verifier both
exit2 with unchanged missing closure; exact source-only request unchanged.


## 2026-10-06 — Original selector fullreturn,182631

**Latest full-startup checkpoint — 20261006-182631: original selector returns.**

Original `0922E0` passes **6,656 isolated cases** (3,072 valid/3,584 invalid):
selectors0/2/3/5/6/7 write byte1 at79D9A0+65..70; invalid values make no store.
Branch/delay slots, R0/R5/R6/T, stack/PR/SP, SR/MACL/FPU, P1/P2 aliases,
whole16MiB RAM/69CPU/25TMU and11/13/15/17/19/20 modeled cycles pass.
The actual caller selects0, changes byte79D9E1 from0 to1 and returns091A8C in11
cycles. Original parent continuation then reaches genuinely missing **`04AE40`**
in four cycles: PC091A8C/PR091A92/SPFFFF9C/SR40000101. SavedparentPR/cleared
buffers are preserved; parent epilogue/finalzero/fullreturn remain unexecuted.

**Three prospective live phases match without exclusions:** helper entry,
fullreturn and original parent first-missing-child failure. Prior motionreturn
and exact-binary regression pass: two supplied scenes, ten external audits,
release controls and six unchanged images. Closure838functions/82,835instructions/
2,392external records, cap840, is inventory; one new seed/no new runtime patches.
Twenty full/four reduction sources frozen;12patches/inputs/flash/rootVMU
unchanged; all processes terminal; no Simulator. Checkpoint **2636 identities**,
all636prior/83reduction reverified; SHA256 `e262a3588eee5c6a4cbf686f11e7148602aee03a5d87bf421b16f6c4d5667f06`.
Actual mode0/cycles115737366/sectors215/ARM7instructions
2845396/faults0/AICAframes16623/
nonzero0/IRQ36.

Content-preserving APFS sharing covers2,164 completed diagnostic captures:
3,800,057,920 logical bytes, **3,546,189,824 available bytes recovered**.
All636prior identities and all2,164 file pairs pass before/after verification.
Paths/bytes/uid/gid/mode/atime/mtime/xattrs retained; inode/birthtime changes
were declared. No source, game input, asset or save selected or deleted.
Narrow object/recent-baseline plans remain unexecuted. Two postprocessing
metadata errors were repaired with rejected versions retained; no game replay.

**Next:** original `04AE40` (24instructions) calls the already recovered and
registered real `10EF3C` to clear512bytes at79DC08, then writes six globals and
returns091A92. Source predicts2,594modeled cycles, **unexecuted**. Execute its
full body and real helper, then original parent091A92 epilogue (six cycles),
restore savedparentPR04E32A/SPFFFFA0 and zero79E584 in the RTS delay slot.
Parent fullreturn04E32A remains unexecuted. One classification batch used:
execute before broader inventory; integrate only04AE40/current cap840 if the
measured closure fits; retain prospective comparisons/motionreturn/baseline.

Fresh G1 audit remains runtime0/1,guest0/1474,headers0/177,contracts0/4,
title38/39,FFmpeg10139/10139; exactPAL descriptor absent. Source-only request
unchanged/unsent. G1/fullinitialization/IRQ/audible audio/normal boot/gameplay/
save/VMU/mobile/production remain open. Goal turn: **progress**.



## 2026-10-06 — Original global/buffer parent fullreturn,183420

**Latest full-startup checkpoint — 20261006-183420: buffer initializer completes its return.**

Original `04AE40`, its real registered `10EF3C` clear helper, and the original
`091A92` parent epilogue pass **8,192 isolated cases**:512byte clear/sixglobal
stores/PRpush-pop/delay slots/R0-R7/T1/SR-MACL-FPU/independent P1-P2 buffer,
word-group, byte-group, tail-word, parent-word and stack aliases/whole16MiB RAM/
69CPU/25TMU. Helper returns091A92 in2,594modeled cycles; parent returns04E32A
in six cycles, restoring PR/SP and clearing79E584 in its RTS delay slot.

**Three prospective live phases match without exclusions:** global initializer
entry/fullreturn and buffer parent fullreturn. Subsequent caller behavior was
explicitly not predicted. It reaches newly missing **`04E60E`** atPC04E32A/
PR04E332/SPFFFFA0/SR40000101. Prior motionreturn and exact-binary regression pass:
two supplied scenes, ten external audits, release controls and six unchanged
images. One new seed/no new runtime patches. Closure839functions/82,859instructions/
2,391external records, cap840, remains inventory. Twenty full/four reduction
sources frozen;12patches/inputs/flash/rootVMU unchanged;allterminal/noSimulator.
Checkpoint **439 identities**, all2,636prior/68reduction reverified; SHA256 `e60fd8c5385806f643cdd9c73e74835713c8fb6e84ac4223ac884a99370649f6`.
Mode0/cycles115739971/sectors215/ARM7instructions
2845690/faults0/AICAframes16624/
nonzero0/IRQ36.

**Next:** execute original04E60E prefix to genuinely unregistered0547EA.
Its first12instructions saveR14/PR in two stackwords, store16(21)@74D434,
zero16@1A1B5C and zero32@7AC25C in the JSR delay; source predicts13cycles/
PC04E60E/PR04E626/SPFFFF98, **unexecuted**. Preserve registered real later
08C212/08BD1C and tail04E5E0. Nearby registered04E60A is a continuation of
04E5E0, distinct from missing04E60E; the rejected ELF-entry review is retained.
Real0547EA source is15instructions/no calls with float0/-1/0 vector and two
zero globals; source-predicted16cycles, unexecuted. Two classification batches
used: execute before further inventory. After isolated checks, add only04E60E
with measured closure/cap842 if needed; retain global/buffer/motion returns,
prospective entry/failure comparisons and exact-binary baseline. Full04E60E
conditionalbranches/tail/return remain unexecuted.

Fresh G1 remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; exactPAL descriptor absent. Source-only request unchanged/
unsent. G1/fullinitialization/IRQ/audible audio/normal boot/gameplay/save/VMU/
mobile/production remain open. Goal turn: **progress**.



## 2026-10-06 — Original state prefix/first missing child,184314

**Latest full-startup checkpoint — 20261006-184314: original state prefix reaches its first child.**

Original `04E60E` passes **8,192 isolated cases**: savedR14/PR in two stackwords/
eightbyteframe, store16(21)@74D434, zero16@1A1B5C and zero32@7AC25C in the JSR
delay slot, R1/R2/R3/R14, unchanged SR/MACL/FPU, independent P1/P2 destinations/
stack, whole16MiB RAM/69CPU/25TMU and13modeled cycles. Real known later
08C212/08BD1C/0513F8 and tail04E5E0/04E60A remain registered. Isolatedclosure:
five functions/81instructions/one genuine external record; no knownhelper stub.

**Four prospective live phases match without exclusions:** retained global
and buffer fullreturns, state entry and predicted missing-child failure.
Missing **`0547EA`**, PC04E60E/PR04E626/SPFFFF98/SR40000101. ChildFPU/conditional
branches/tail/fullstateinitializer return remain unexecuted. Prior motionreturn
and exact-binary baseline pass: two supplied scenes, ten external audits,
release controls and six unchanged images. One new seed/no new runtime patches.
Closure840functions/82,906instructions/2,385external records,cap842,is inventory.
Twenty full/four reduction sources frozen;12patches/inputs/flash/rootVMU
unchanged;allterminal/noSimulator. Checkpoint **435 identities**, all439prior/
68reduction reverified; SHA256 `513c411b37e082b77e715416c1a7f9f7118ddb92277448eef7c46c482804dcf3`. Mode0/cycles115739984/
sectors215/ARM7instructions2845692/faults0/
AICAframes16624/nonzero0/IRQ36.

**Next:** execute complete original0547EA,15instructions/no calls. FLDI1/FNEG/
FLDI0 set architecturalFR3=-1/FR4=0; five original32bit stores produce float0/
BF800000/float0 at758C5C/60/64 and zero758C68/6C. Predict R0=8/R2=758C6C/
R3=758C68/R4=0/RTSzero32delay/unchangedPR-SP-SR-MACL-otherFPU/fullreturn04E626/
wholeRAM69CPU25TMU/**16modeled cycles**, unexecuted. ActualFPSCR40001 hasPR0/SZ0;
FR3currently41800000/FR4=3F800000/FPUL10/inactiveXF3-XF4zero; vector/globals
currentlyzero. Preserve active/inactive FPUbanks/FPSCR/FPUL and original saved
R14-PR frame; test nonzero starts, guards and independentP1-P2 destinations-stack.
Keep real registered parent04E626/laterhelpers/tail; do not claim parentfullreturn.
After isolated proof integrate only0547EA/currentcap842 if measured closure fits,
prospectively compare entry/fullreturn, observe subsequent original continuation,
retain global/buffer/motion returns/exactbinary baseline. Review is reused with
pinnedBOOT/map/provenance/checkpoint bound and live literals/state reverified;
zero new sourceclassification batches used, no archive search or new sourcelead.

Fresh G1 remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; exactPAL descriptor absent. Source-only request unchanged/
unsent. G1/fullinitialization/IRQ/audible audio/normal boot/gameplay/save/VMU/
mobile/production remain open. Goal turn: **progress**.



## 2026-10-06 — Vector return and next original initializer,185155

**Latest full-startup checkpoint — 20261006-185155: vector initializer returns; parent return observed.**

Original `0547EA` passes **8,192 isolated cases** and its prospective live entry
and return comparisons. The 15 original instructions execute in 16 modeled
cycles: FLDI1/FNEG/FLDI0 set architectural FR3 to -1 and FR4 to zero; five
32-bit stores initialize the vector and two globals. Tests cover independent
P1/P2 destination and stack aliases, both active FPU banks and supported PR=0,
SZ=0 contexts. Whole 16 MiB RAM, 69 CPU fields and 25 TMU fields match; inactive
FPU bank, FPSCR, FPUL, PR, SP and the parent's saved frame remain intact.

**Four prospective live phases pass:** retained global and buffer returns,
vector entry and vector return. The original `04E60E` parent subsequently
returns to PC=PR `04E332`, with SP `FFFFA0` and R14 restored. That parent return
is observed; its full RAM/CPU/TMU transfer was not prospectively modeled.
Startup then stops at genuinely unregistered **`089F20`**, with PC `04E332`,
PR `04E338`, SP `FFFFA0`, SR `40000100`. The raw execution report retains this
intentional failure. Normal initialization and boot remain unverified.

Prior motion return and the exact-binary baseline pass: two supplied scenes,
ten external audits, release controls and six unchanged images. One new seed,
no new runtime patches. Closure: 841 functions, 82,921 instructions and 2,384
external records, cap 842; this is inventory, not gameplay coverage. Twenty
full-run and four reduction sources are frozen. Checkpoint: **436 identities**,
all 435 prior and 63 reduction identities reverified; SHA256 `85cc0fd4613b4b593807e8bbe0703be96ce88b0c1c8aec1bb4a57ee237e24071`.
Mode 0; 115,740,051 cycles; 215 sectors; ARM7 2,845,699 instructions, zero faults;
AICA 16,624 frames, zero nonzero frames; 36 IRQ entries. These counters do not
prove audible audio or IRQ completion. Inputs, twelve patches and protected
flash/root VMU are unchanged; all game processes terminal, no Simulator.

**Next:** prove the prefix of original `089F20` using its real registered
`10EF3C` clear helper. Its source allocates a 1,184-byte local record within a
1,192-byte frame, clears the record, performs 33 additional zero-word stores,
and writes 23 byte triples (99,59,59) followed by five zero triples. The first
genuinely missing child is `0A36C0`. The unexecuted source prediction is 6,682
modeled cycles, missing-lookup PC `089FB6`, PR `089FBC`, SP `FFFAF8`. Test saved
R14/PR, guards, original overlaps/delay slots, independent stack aliases and
whole RAM/CPU/TMU. Preserve the real later `10E9FC` copy helper; no missing-child
stub. After isolated proof integrate only `089F20`, increase cap to 844 only if
measured closure requires it, compare prospective entry/failure and retain the
vector/global/buffer/motion returns plus exact-binary baseline. One source
classification batch used; execute before broader inventory. Remaining copies,
epilogue, full return and persistence remain unexecuted.

Fresh G1 inventory remains runtime 0/1, guest sources 0/1474, headers 0/177,
contracts 0/4, title 38/39 and FFmpeg 10139/10139. The exact PAL v1.003 descriptor
is absent. The source-only request remains unchanged and unsent. G1, full
initialization, audio, normal boot, gameplay, save/VMU, mobile and production
acceptance remain open. Goal turn: **progress**.



## 2026-10-06 — Original local-record prefix/first missing child,190227

**Latest full-startup checkpoint — 20261006-190227: original record initializer reaches its first missing child.**

Original `089F20` prefix passes **4,096 isolated cases** and its prospective
live entry/failure comparison. It uses the real registered `10EF3C` helper to
clear a 1,184-byte local record in a 1,192-byte frame, preserves saved R14/PR,
performs 33 additional zero-word stores, and fills 23 byte triples (99,59,59)
plus five zero triples. Original overlaps, signed immediates, shifts and delay
slots remain intact. Tests cover P1/P2 stack aliases, four stack positions,
saved R14 values, IRQ masks, T and both FPU banks. Whole 16 MiB RAM, 69 CPU and
25 TMU fields match after **6,682 modeled cycles**. The real later `10E9FC`
copy helper remains available; no missing-child stub or host initializer.

**Six prospective live phases pass:** retained global/buffer/vector returns,
vector entry, record entry and missing-child failure. The first genuine missing
child is **`0A36C0`**, PC `089FB6`, PR `089FBC`, SP `FFFAF8`, SR `40000101`.
The raw execution report retains its intentional exit 1. The preceding parent
`04E60E` return remains observed with PC/PR/SP/R14 restored; its full transfer
was not prospectively modeled. Remaining record copies, full parent return,
persistence, initialization and normal boot remain unverified.

Prior motion return and exact-binary baseline pass: two supplied scenes, ten
external audits, release controls and six unchanged images. One new seed, no
new runtime patches. Closure842functions/83,101instructions/2,394external
records is inventory. Cap844 allows the measured842closure, which would exhaust
the previous842bound. Twenty full-run and four reduction sources are frozen.
Checkpoint **444 identities**, all436prior and64reduction reverified; SHA256
`c63e53fedd0cdf27ff84fb15f8726b78a225f7741fe40fef5adbc039ba0bbc13`. Mode0/cycles115746733/sectors215/ARM7instructions2846454/faults0/
AICAframes16625/nonzero0/IRQ36. These counters do not establish audible audio
or IRQ completion. Inputs/twelve patches/protected flash/root VMU unchanged;
all game processes terminal, no Simulator.

**Next:** execute original `0A36C0`,25instructions/no calls. Source-predicted
26 modeled cycles copy the original three words at15D390 (2328,1770,0BB8) twice
to record offsets372 and384: six32-bit stores/24bytes. Verify P1/P2 independent
source/record/stack aliases, guards and sequential load/store behavior under
source/destination overlap. Predict R1/R2 from sourcewords4/8,R3=sourceaddress,
R4=2,R5=record+396,R6=2,T1,other registers/FPU preserved, fullreturn089FBC.
The parent continuation at089FBC adds four cycles before genuinely missing
0A36F2, PC089FBC/PR089FC2/sameSP/R4=SP/R3=0A36F2. Both predictions remain
unexecuted. After isolated proof integrate only0A36C0/currentcap844 if measured
closure fits; prospective child entry/fullreturn/nextmissing plus retained
record/vector/global/buffer/motion contracts and exact-binary baseline. One
classificationbatch used; execute before broader inventory.

Fresh G1 remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; required PALv1.003 descriptor absent. Source-only request
unchanged/unsent. G1/fullinitialization/audio/normalboot/gameplay/save/VMU/
mobile/production remain open. Goal turn: **progress**.



## 2026-10-06 — Original first record-copy child/full return,191301

**Latest full-startup checkpoint — 20261006-191301: first record-copy child returns.**

Original `0A36C0` passes **14,336 isolated cases**, including fourteen disjoint
or overlapping source positions and independent P1/P2 source/record/stack
aliases. Its ordered loads and six32-bit stores copy original words2328/1770/
0BB8 twice into local-record offsets372 and384. Whole16MiB RAM/69CPU/25TMU,
guards and preserved registers/FPU match after **26 modeled cycles**, including
RTS/NOPdelay. The original parent continuation adds four cycles, restores R4
from SP in the JSRdelay, and rejects at genuinely missing **`0A36F2`**.
Actual record parent, previous helpers and real10EF3C/later10E9FC remain present.
No bulkcopy, host initializer or missing-child stub was substituted.

**Eight prospective live phases pass:** retained global/buffer/vector returns,
vector entry, record entry, copy-child entry/fullreturn and predicted next-child
failure. Missing0A36F2 atPC089FBC/PR089FC2/SPFFFAF8/SR40000101. Rawexecution
retains intentionalexit1; no normalboot claim. Preceding04E60E parent return
remains observed with PC/PR/SP/R14 restored, without a prospective fulltransfer
proof. Remaining record copies/fullrecordparentreturn/persistence stay unverified.

Motion return and exact-binary baseline pass: two supplied scenes, ten external
audits, release controls and six unchanged images. One new seed/currentcap844,
no new runtime patches. Closure843functions/83,126instructions/2,393external
records is inventory. Twenty full-run/four reduction sources frozen. Checkpoint
**479 identities**, all444prior and88reduction reverified; SHA256 `3c9db43d989fbbef44cac9650fc56354238eeecadf2f3275caba38002c9f7ddf`.
Mode0/cycles115746763/sectors215/ARM7instructions2846457/faults0/AICAframes16625/
nonzero0/IRQ36. Counters do not prove audible audio or IRQ completion. Inputs/
twelve patches/protected flash/rootVMU unchanged; allterminal/noSimulator.

The initial reduction failed only its frozen-caller assertion because its
harness selected the previous185155 capture. Original failedreport/log/source
versions remain preserved; the fresh repair1 harness uses190227. Oracle,
generator and runtime remained unchanged. The corrected capture also passes.

Disk preparation preserves364 completed output paths:168exact baseline captures,
177identical compiled objects and19near RAMcaptures, with differing pages rewritten
to reproduce every target's original bytes. Full sharingpasses increased available
space by1,174,634,496bytes; sample reports are separate. All444prior identities
and all364target hashes reverified afterward. Paths/permissions/timestamps/xattrs
retained; inode/birthtime changes declared. No content removed. Plans and reports
are under `artifacts/recovery-aot/20261006-startup-pair-storage/`.

**Next:** execute original0A36F2,25instructions/no calls. Source predicts26cycles
and six32-bit stores copying15D39C words2710/1770/0BB8 twice into record offsets396
and408, fullreturn089FC2/R1-R2sourcewords/R3source/R4=2/R5=record+420/R6=2/T1.
Verify original ordered loads/stores under overlap, independentP1/P2 aliases,
guards and wholeRAM69CPU25TMU. Keep actualrecordparent, prior0A36C0 and realclear/
latercopy helpers. Parent089FC2 adds fourcycles to genuine missing0A3724,
PC089FC2/PR089FC8/sameSP/R4=SP/R2=0A3724/R3source. Predictions unexecuted.
After isolated proof integrate only0A36F2/proposedcap846 if measured closure
would exhaust current844; prospectively compare entry/fullreturn/nextfailure
and retain priorpair/record/vector/global/buffer/motion plus exact-binary baseline.
One classificationbatch used; execute before broader inventory.

Fresh G1 remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; required PALv1.003 descriptor absent. Source-only request
unchanged/unsent. G1/fullinitialization/audio/normalboot/gameplay/save/VMU/
mobile/production remain open. Goal turn: **progress**.



## 2026-10-06 — Original second record-copy child/full return,192131

**Latest full-startup checkpoint — 20261006-192131: second record-copy child returns.**

Original `0A36F2` passes **14,336 isolated cases**, including fourteen disjoint/
overlapping source positions and independent P1/P2 source/record/stack aliases.
Ordered loads and six32-bit stores copy original15D39C words2710/1770/0BB8 twice
into local-record offsets396 and408. Whole16MiB RAM/69CPU/25TMU/guards/preserved
registers/FPU match after26modeledcycles/RTS-NOPdelay. Parent089FC2 executes its
fourcycle JSR/SPdelay and reaches genuine missing **`0A3724`**, with PC089FC2,
PR089FC8,SPFFFAF8,SR40000101. Actualrecordparent/prior0A36C0/real10EF3C and later
10E9FC remain present. No host initializer, bulkcopy or missing-child stub.

**Ten prospective live phases pass:** retained global/buffer/vector/first-pair
returns, vector/record/first-pair entries, second-pair entry/fullreturn and the
predicted next-child failure. Motionreturn and exact-binary supplied-state scene
baseline pass: two scenes, ten external audits, release controls and six unchanged
images. Rawexecution retains intentionalexit1. Preceding04E60E parent return
remains observed without fullprospective transfer proof. Remaining record/full
parentreturn/persistence/fullinitialization/normalboot remain unverified.

One new seed/no runtimepatch change. Measuredclosure844functions/83,151instructions/
2,392externalrecords is inventory. Cap846 avoids exhausting previous844bound.
Twenty full-run/four reduction sources frozen. Checkpoint **480 identities**,
all479prior and79reduction reverified; SHA256 `4c551cedd47eefaea81d20e2b039ef5ea14eb5c146ff776cc6eb91ee31192f9f`.
Mode0/cycles115746793/sectors215/ARM7instructions2846460/faults0/AICAframes16625/
nonzero0/IRQ36. Counters do not prove audible audio or IRQ completion. Inputs/
twelve patches/protected flash/rootVMU unchanged; allterminal/noSimulator.

Disk preparation preserves64 completed191301 outputs:56exact baseline captures
and8near RAMcaptures, differing pages rewritten to restore each target's original
bytes. Fullsharingpasses increased available space by348,078,080bytes; sample
reports separate. All479prior identities and64target hashes reverified. Paths/
permissions/timestamps/xattrs retained; inode/birthtime change declared; no content
removed. Evidence: `artifacts/recovery-aot/20261006-startup-pair2-storage/`.

**Next:** original0A3724,104instructions/no calls, six15-byte records/90byte stores
at local-record432..521. Source15D3A8 is `0a00000f0000140000040000070000`.
Source predicts202modeledcycles:5setup+2*97body+2return+RTSextra1. Verify ordered
byte loads/stores with overlapping source/destination, unaligned byte pointers,
high-bit bytes/sign extension, independentP1/P2 aliases/guards/wholeRAM69CPU25TMU.
Predict R0signextended finalbyte14,R3source,R4=6,R5=record+522,R6=6,T1,other
registers/FPU unchanged/fullreturn089FC8. Parent089FC8 BSR089DC0/SPdelay adds4cycles;
next genuine missing089DC0 atPC089FC8/PR089FCC/sameSP/R4=SP/R2still0A3724. These
predictions remain unexecuted. Keep realparent/priorpairchildren/clear-copyhelpers.
After isolated proof add only0A3724/currentcap846 if measured closure fits;
prospective entry/fullreturn/nextfailure and retained priorcontracts/baseline.
Oneclassificationbatch used; execute before broader inventory.

Fresh G1 remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; requiredPALv1.003 descriptor absent. Source-only request
unchanged/unsent. G1/fullinitialization/audio/normalboot/gameplay/save/VMU/mobile/
production remain open. Goal turn: **progress**.



## 2026-10-06 — Original six byte records/full return,193353

**Latest full-startup checkpoint — 20261006-193353: six byte records complete.**

Original `0A3724` passes **14,336 isolated cases**: six15-byte records/90byte
stores, four byte alignments, fourteen disjoint/overlapping source positions,
independentP1/P2 source/record/stack aliases, high-bit signed byte loads and
original ordered memory effects. Source15D3A8 fills local-record432..521.
Whole16MiB RAM/69CPU/25TMU/guards/preserved controls-FPU match after **202 modeled
cycles**, two97instruction body iterations/BF-S-add15delay/RTS-NOPdelay. Parent
089FC8 directBSR/SPdelay takes **three cycles** and reaches genuine missing
**`089DC0`**, PC089FC8/PR089FCC/SPFFFAF8/SR40000101. The prior unexecuted fourcycle
parent estimate incorrectly included a literal load; corrected from source
before testing. Originalparent/priorpairchildren/realclear-latercopy retained.
No bulkcopy, hostinitializer or missing-child stub was substituted.

**Twelve prospective live phases pass:** retained global/buffer/vector/pair1/
pair2 returns and entries, record entry, six-record entry/fullreturn and predicted
next-child failure. Motionreturn and exact-binary supplied-state baseline pass:
two scenes, ten external audits, release controls and six unchanged images.
Rawexecution retains intentionalexit1. Preceding04E60E parent return remains
observed without a fullprospective transfer proof; fullrecordparentreturn/
persistence/fullinitialization/normalboot remain unverified.

One new seed/currentcap846/no runtimepatch change. Measuredclosure845functions/
83,255instructions/2,391externalrecords is inventory. Twenty full-run/four
reduction sources frozen. Checkpoint **495 identities**, all480prior and84reduction
reverified; SHA256 `98ea20c00bd60046fd694b3fba55f96762169a4965309da9e7fd47b0c8fa97ab`. Mode0/cycles115746998/sectors215/
ARM7instructions2846484/faults0/AICAframes16625/nonzero0/IRQ36. Counters do not
establish audible audio or IRQ completion. Inputs/twelve patches/protected
flash/rootVMU unchanged; all game processes terminal, no Simulator.

Storage preparation preserves188completed outputs:56exact baseline captures,
122identical compiled objects and10near RAMcaptures restored to exact target
bytes. Fullsharingpasses increased available space by480,317,440bytes; sample
reports separate. All480prior identities and188target hashes reverified. Paths/
permissions/timestamps/xattrs retained; inode/birthtime changes declared; no
content removed. Evidence: `artifacts/recovery-aot/20261006-startup-sixrecords-storage/`.

**Next:** original089DC0,47instructions/no calls. Source15B568 words1F40/1770/0BB8
copy twice to local-record324/336, then15B574 words4E20/3A98/2710 copy twice to348/
360 (12longstores/48bytes/four12-byte records). Source predicts48modeledcycles:
5setup+18body+4setup+18body+RTS-NOPdelay+RTSextra1. Model ordered loads/stores under
independentP1/P2 two-source/record/stack aliases, overlap/cross-group dependencies
and nonzero guards; secondsource may be modified by firstwrites. Predict R1/R2
secondsourcewords4/8,R3secondsource,R4unchangedrecordpointer,R5record+372,R6/R7=2,
T1,otherregisters/FPU preserved/fullreturn089FCC/wholeRAM69CPU25TMU. Parent089FCC
BSR089E1E/SPdelay adds3cycles; genuine nextmissing089E1E atPC089FCC/PR089FD0/
sameSP/R4=SP/R2secondsourceword8. These predictions remain unexecuted. Preserve
realparent/priorchildren/clear-copyhelpers. After isolated proof add only089DC0/
proposedcap848 if measured closure exhausts current846; prospective entry/
fullreturn/nextfailure plus retained contracts/exact-binary baseline. One source
classificationbatch used; execute before broader inventory.

Fresh G1 remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; exactPALv1.003 descriptor absent. Source-only request unchanged/
unsent. G1/fullinitialization/audio/normalboot/gameplay/save/VMU/mobile/production
remain open. Goal turn: **progress**.



## 2026-10-06 — Original four table records,194729

**Latest full-startup checkpoint — 20261006-194729: four table records complete.**

Original `089DC0` passes **28,672 isolated cases**. Two source tables are each
copied twice into four 12-byte records at local offsets 324–371, using twelve
original ordered long stores. Tests cover fourteen disjoint/overlapping source
pairs, four byte alignments, independent P1/P2 aliases for both sources, the
record and stack, IRQ masks 0/5/10/15, both T values and both physical FPU banks.
The model preserves reads that can change after earlier writes, including
dependencies between the copy groups. Whole 16 MiB RAM, 69 CPU fields and
25 TMU fields match after **48 modeled cycles**. The child returns to `089FCC`;
the original parent's direct BSR/SP delay adds three cycles and reaches genuine
missing **`089E1E`**, PC `089FCC`, PR `089FD0`, SP `FFFAF8`, SR `40000101`.
Original parent, earlier children, clear and later copy helpers remain present.
No missing-target stub, host initializer or bulk copy was substituted.

**Fourteen prospective live comparisons pass**, retaining the earlier global,
buffer, vector, record, pair and six-record phases. The motion-return contract
also passes. The exact same binary passes two supplied-state scenes, ten
external audits, release controls and six unchanged reference images. The raw
full-run report retains intentional exit 1. Earlier `04E60E` parent return is
observed without a prospective whole-state transfer proof. Full record-parent
return, persistence, full initialization and normal boot remain unverified.

Only `089DC0` was added. Measured callback inventory: **846 functions,
83,302 instructions, 2,390 external records**. This exhausts the old cap of846;
the cap is848. Twenty full-run and four reduction sources are frozen; twelve
runtime patches unchanged. Checkpoint **512 identities**, all495 prior and91
reduction identities reverified; SHA-256 `2bf4ba4ce7580dc9b37694124f7f084f8f3f7f03d54c745d150467caa1236406`.
Mode0/cycles115747049/sectors215/ARM7instructions2846489/faults0/
AICAframes16625/nonzero0/IRQ36. Counters do not prove audible audio or IRQ
completion. Protected originals/flash/root VMU unchanged; all game processes
terminal, no Simulator launched or booted.

Storage preparation preserves **308 completed paths**:56 exact baseline captures,
240 identical compiled objects and12 near RAM captures. Three older terminal
builds had never been selected for object sharing. Full sharing passes increased
available space by613,400,576bytes; samples reported separately. All308 content
hashes and495 prior identities reverified. Paths/permissions/owner/timestamps/
xattrs preserved; inode/birthtime changes declared; no content removed.
Evidence: `artifacts/recovery-aot/20261006-startup-fourrecords-storage/`.

**Next:** original `089E1E`, eleven instructions/no calls. Source `15B580`
contains2000/1500/1000. Three ordered long stores fill offsets420–431; the final
store is in the RTS delay slot. Source predicts12 modeled cycles/fullreturn
`089FD0`, then the parent's three-cycle BSR/SP delay to missing `089E34`,
PC089FD0/PR089FD4. Verify ordered overlap, independent aliases, alignments,
unchanged T/other registers/FPU, whole RAM and timers before integration.
An isolated eight-function closure would exhaust its cap8: use10 for that
boundary. The next full closure847 fits currentcap848. These predictions are
unexecuted. One classification batch used; execute before broader inventory.
Disk is tight: preserve/share completed diagnostics and verify capacity first.

Fresh G1: runtime0/1, guest sources0/1474, SDK headers0/177, contracts0/4,
title providers38/39, FFmpeg source10139/10139. FFmpeg source presence is not
Apple media integration. Exact PAL v1.003 descriptor absent; exact bootstrap
inventory/source-only request unchanged and unsent. G1/audio/normal boot/
gameplay/save/VMU/mobile/production adoption remain open. Goal turn: **progress**.



## 2026-10-06 — Triplet return and storage repair,200306

**Latest full-startup checkpoint — 20261006-200306: triplet return verified.**

Original `089E1E` passes **14,336 isolated cases**. Three ordered long stores
copy source `15B580` (2000/1500/1000) to local-record offsets420–431. The third
source word is read after the first two stores; the third store executes in
the RTS delay slot. Cases cover fourteen disjoint/overlapping source positions,
four byte alignments, independent P1/P2 source/record/stack aliases, IRQ masks
0/5/10/15, both T values and both physical FPU banks. Whole16MiB RAM,69CPU and
25TMU fields match after **twelve modeled cycles**; T and unrelated registers/
FPU state are preserved. The child returns to `089FD0`. Original parent BSR/SP
delay adds three cycles and rejects genuine missing **`089E34`**, PC089FD0,
PR089FD4,SPFFFAF8,SR40000101. Real parent/prior children/clear-latercopy retained.

**Sixteen prospective live comparisons pass**, including all retained startup
phases, new entry/fullreturn and predicted next failure. Motionreturn passes.
The exact same binary passes two supplied-state scenes, ten external audits,
release controls and six unchanged reference images. Raw execution retains
intentional exit1. Earlier04E60E parent return remains observed without a full
prospective transfer proof; full record-parent return/persistence/fullinit/
normalboot remain unverified. Only089E1E added; fullcap848 unchanged. Measured
inventory847functions/83313instructions/2389external records. Isolated8functions/
462instructions/4external records required cap10 instead of exhausted cap8.
Twenty full-run/four reduction sources frozen; twelve runtime patches unchanged.
Checkpoint **732 identities**, all512prior and84reduction reverified;
SHA-256 `282076438350264e383860c4befd6c1bfe09aaf4ec3cd6e7c943b789138dfd3c`. Mode0/cycles115747064/sectors215/ARM7instructions2846491/faults0/
AICAframes16625/nonzero0/IRQ36. Counters do not prove audible audio or IRQ
completion. Originals/flash/root VMU unchanged; all processes terminal/noSimulator.

**Storage failure preserved:** first full attempt `195721` stopped during
generation, before any game execution. Its callback output was empty and its
report write raised ENOSPC; a raw report could not be saved. Partial outputs,
twenty source identities and explicit failure metadata are retained, not
reconstructed as a successful run. Subsequent space fluctuations exceeded the
selected file sizes; the cause is unestablished. Initial sharing preserved164
completed files and reverified512prior identities. A fresh scope excluded all
previously near-shared targets and preserved54 additional captures. All84
reduction and199 failed partial identities were reverified too. Paths/content/
permissions/owner/timestamps/xattrs retained; inode/birthtime changes declared;
no content removed. Recorded available-byte increases are observations, not
fully attributable reclaimed space. The unexecuted76-target plan is preserved.
Guarded **repair1** `200306` keeps game code/oracle unchanged, requires1536MiB
before launch and checks capacity before each command. All51 checks passed;
2185162752bytes were available before game execution. First failure is not a
guest fault or regression. Both storage scopes/failed attempt are checkpoint-bound.

**Next:** original `089E34`,89instructions/no calls. Six distinct nine-byte
records/54 ordered signed-byte loads-stores fill offsets522–575 from15B58C.
Source predicts170modeledcycles (7setup+2x80body+RTS/NOPdelay+RTSextra1), return
089FD4,R0signed finalbyte,R3source+45,R4source,R5=R7=6,R6record+576,T1.
Model overlap across source rows/destination, aliases, alignments, guards,
saved frame, whole RAM/CPU/TMU. Then original parent with observedR14=0 predicts
74cycles: six byte stores4 atSP+594..599, byteSP+616 OR2, wordSP+752=0x1A00,
wordSP+604=100; nine stores/eleven bytes total. Genuine nextmissing08A394 at
PC08A034/PR08A038/R0=604/R1SP+616/R2=100/R3=0x1A00/R4SP/R5=4/R6=R7=6/R14SP/
unchangedSP/T1. These predictions remain unexecuted. Isolated9functions fits10;
full848 would exhaust current848, propose850 only for that measured boundary.
One classification batch used; execute before broader inventory. Preserve the
failed attempt and capacity checks. Fullparentreturn remains unverified.

Fresh G1 remains runtime0/1,guest sources0/1474,headers0/177,contracts0/4,
title providers38/39,FFmpeg source10139/10139. Source presence does not prove
Apple media integration; exact PAL v1.003 descriptor absent. Exact bootstrap
inventory/source-only acquisition request unchanged and unsent. OriginalG1/
audio/normalboot/gameplay/save/VMU/mobile/production remain open. Goal: **progress**.



## 2026-10-06 — Nine-byte record checkpoint,201553

**Latest full-startup checkpoint — 20261006-201553: six nine-byte records verified.**

Original `089E34` passes **14,336 isolated cases** and returns after **170
modeled cycles**. Its 54 ordered signed-byte loads/stores copy six distinct
source rows from `15B58C` to record offsets 522–575. Cases cover source/record/
stack aliases, overlap between rows and destination, four byte alignments,
IRQ masks, both T values and both physical FPU banks. Whole 16 MiB RAM,
69 CPU fields and 25 TMU fields match; prior records and guards are retained.
Original parent `089FD4`, with observed initial R14=0, then performs nine
stores (eleven bytes) in 74 modeled cycles: six flags at SP+594..599, OR2 at
SP+616, 0x1A00 at SP+752 and 100 at SP+604. It rejects genuine missing
**`08A394`**, with guest PC `08A008`, PR `08A038`, SP `FFFAF8`, SR `40000101`.
The original parent, earlier children and real clear/copy helpers remain present.

**Eighteen prospective live RAM/CPU/TMU comparisons pass**, including all
retained phases, new child entry/return and predicted next failure. Motion
return passes. The exact same binary passes two supplied-state scenes, ten
external audits, release corruption controls and six unchanged reference
images. Raw execution retains intentional exit1 at the missing target, without
timeout. Only `089E34` was added. Measured closure: 848 functions, 83,402
instructions, 2,388 external records; cap850 was required because 848 would
exhaust the strict cap. Isolated closure: 9 functions/551 instructions/3 external
records, cap10. Twenty full-run sources frozen; twelve runtime patches unchanged.
Checkpoint **560 identities**, with all732 prior and118 reduction identities
reverified; SHA-256 `f81c4f96c21f7efd567ad7cdcb19f4ecf944d25ad83c67358842348b29aabb24`.

**Failed oracle preserved:** the first isolated test expected call instruction
PC `08A034`; generated code preserves containing-block PC `08A008` on missing
lookup. This was a PC expectation error. A fresh repair1 oracle changes only
that expected PC and its diagnostic label; guest generated code is byte-identical.
The failed report/log/binary/generated sources and four frozen input sources
remain preserved. The initial failure did not establish whole RAM/TMU success;
the fresh 14,336-case repair does. No game/runtime correction or missing-target
stub was used. Storage sharing preserves133 completed diagnostic paths and
reverifies732 prior identities; full-pass available-byte increases total
532,885,504, an observation rather than attribution of every space fluctuation.
Paths/bytes/owner/mode/timestamps/xattrs preserved; inode/birthtime changes
declared, no content removed. Capacity guards retained. All processes terminal;
no Simulator launched or booted. Originals, flash and root VMU unchanged.

**Next bounded experiment:** original `08A394`,74 instructions/no direct calls.
It computes a reflected16-bit checksum over 1,180 bytes at record offsets4..1183,
initialFFFF/polynomial8408/finalXORFFFF, with eight ordered shifts per byte.
Model every register and branch-dependent emitter cycle, four ordered stack
pushes/16 written bytes, corresponding pops and RTS restore delay, aliases,
overlap, guards and whole RAM/CPU/TMU. Stack writes can affect aliased input;
R1 changes only on odd paths. Return to `08A038`, then model original parent
checksum longstore and three already-registered real `10E9FC` copies before
unregistered `08BD14`. These predictions are unexecuted; do not stub helpers.
Isolated9+one exhausts cap10: propose12 only if measured; full848+one849 fits850
unless measured transitive closure proves otherwise. One classification batch
used; execute before broader inventory. Exact next contract is in
`artifacts/recovery-aot/20261006-201553/next-startup-preflight.json`.

Fresh G1 remains runtime0/1, generated guest sources0/1474, headers0/177,
contracts0/4, title providers38/39, FFmpeg source10139/10139. Apple media
integration and exact PAL v1.003 remain absent/unverified. Exact bootstrap
inventory/source-only request remains unchanged and unsent. Earlier `04E60E`
parent return is observed without full prospective transfer proof. Full record
parent return, persistence, full initialization, IRQ completion, audible audio,
normal boot, gameplay, saves, VMU and mobile/production remain open.
Goal classification: **progress**, original G1 unresolved.


## 2026-10-06 — Checksum and original record copies,203215

**Latest full-startup checkpoint — 20261006-203215: checksum and three record copies verified.**

Original `08A394` passes **17,920 isolated cases**. Its reflected16-bit
checksum reads record offsets4..1183 (1,180 bytes), with initialFFFF,
polynomial8408 and finalXORFFFF. Four ordered pushes write sixteen stack bytes;
matching pops restore R10–R13 and SP, with R13 restored in the RTS delay slot.
Cases cover stack/input overlap, independent P1/P2 aliases, four byte
alignments, IRQ masks, both T values, both physical FPU banks and all-even
paths that preserve R1. Whole16MiB RAM,69CPU and25TMU fields match. Branch-
dependent modeled cycles range69643..82873; the captured startup record
returns **0xE534 in82,728 cycles**, PC/PR08A038. These are emitter cycles,
not a hardware timing measurement.

A separate connected **17,920-case caller proof** retains the same generated
checksum code and the original parent/three real `10E9FC` copies. The parent
writes the checksum, copies three1,184-byte records in original load/store
order and preserves each helper's extra terminal word read and ordered stack
writes. In4,519 modeled cycles it reaches genuine missing **`08BD14`**, with
PC08A074/PR08A07A/SPFFFAF8/SR40000100/R0=04E338. WholeRAM/CPU/TMU match;
no bulk copy, host initializer or missing-target stub was substituted.

**Twenty prospective live comparisons pass**, retaining all earlier startup
phases plus checksum entry/fullreturn and predicted next failure. Motionreturn
passes. The exact same binary passes two supplied-state scenes, ten external
audits, release corruption controls and six unchanged reference images.
Raw execution retains intentional exit1 at the missing target, without timeout.
Only08A394 added; full cap850 retained. Measured closure849functions/
83476instructions/2387external records; isolated10functions/625instructions/
2external records required cap12 because ten would exhaust cap10.
Twenty full-run sources frozen; twelve runtime patches unchanged. Checkpoint
**574 identities**, all560 prior and118 reduction identities reverified;
SHA-256 `b43c97c3638762046162b0072c4cb32da8c9bfa93c7321cfab85e58806ec321b`.

Storage preservation verifies156 distinct completed diagnostic paths across175
sharing operations. All bytes/paths/owner/mode/timestamps/xattrs retained;
inode/birthtime changes declared, no content removed. Replacing only the two
capture references did not reclaim extents still held by their clone groups;
a fresh36-target group rebase includes exact matches and reclaims shared group
storage. An earlier unexecuted32-target plan remains preserved. Full-pass
available-byte changes total649,691,136, observations not attribution of all
space fluctuations. Prior560 and732 identities reverified after sharing.
Capacity guards remain enabled and passed before generation/build/execution.
All processes terminal; no Simulator launched or booted. Original inputs,
flash and root VMU unchanged.

**Next bounded experiment:** original `08BD14`, four instructions/no calls.
Literal08BD90 resolves756C64; one16-bit flag is cleared in the RTS delay slot.
Source predicts five modeled cycles, return08A07A, R1flagpointer/R2zero,
unchangedT and other CPU/FPU. Then original parent08A07A predicts eight cycles:
R1=1184, SP advances1184, savedPR is popped, RTS restores R14 in its delay;
finalSPFFFFA0/PC-PR04E338/savedR14=0. POP_PR costs the emitter default one cycle.
Prove wholeRAM/69CPU/25TMU, flag guards/aliases and saved-frame restoration,
retaining the enclosing04E318 continuation. These predictions remain unexecuted;
fullrecord-parent return is not yet verified. Isolated11fits12; full850 would
exhaust cap850, propose852 only if measured. Existing08BD1C is a distinct entry.
One classification batch used; execute before broader inventory. Exact contract:
`artifacts/recovery-aot/20261006-203215/next-startup-preflight.json`.

Fresh G1 remains runtime0/1, generated guest sources0/1474, headers0/177,
contracts0/4, title providers38/39, FFmpeg source10139/10139. Apple media
integration remains unverified and exact PALv1.003 descriptor is absent.
Exact bootstrap inventory/source-only request remains unchanged and unsent.
Earlier04E60E parent return remains observed without full prospective transfer
proof. Fullrecord-parent return, full initialization, IRQ completion, audible
audio, normal boot, gameplay, saves/VMU and mobile/production remain open.
Goal classification: **progress**, original G1 unresolved.


## 2026-10-06 — Original record initializer return,204816

**Latest full-startup checkpoint — 20261006-204816: original record initializer returns.**

Original `08BD14` passes **7,168 isolated cases**: one16-bit flag clear in its
RTS delay slot, five modeled cycles, unchanged T and unrelated CPU/FPU. Cases
cover fourteen disjoint/overlapping positions, four byte alignments, independent
P1/P2 flag and stack aliases, literal self-overlap, savedPR/R14 overlap, guards,
IRQ masks, both T values and both physical FPU banks. Whole16MiB RAM,69CPU and
25TMU fields match. The original `089F20` epilogue then executes eight cycles:
R1=1184, SP advances1184, savedPR is popped, and R14 is restored in RTS delay.
Overlap cases preserve the effects of the flag write on later frame reads.
For the captured state, full record-parent return is **PC/PR04E338,
SPFFFFA0, R14zero**. POP_PR costs the emitter default one cycle.

**Twenty-three prospective live comparisons pass**, including new flag entry/
return and full record-parent return, all earlier phases and predicted next
failure. The enclosing `04E310` block04E338 then executes four cycles and
rejects genuine missing **`011238`**, with PC04E338/PR04E33E/SPFFFFA0/
SR40000100. `04E318` is another block of the same owning ELF function;
the initial review at that block address found no ELF function and performed
no disassembly. Actual map/registration resolved owner04E310 before review.
Original record fields, checksum and three real copies remain connected;
no host initializer, bulk copy or missing-target stub replaced them.

Motionreturn passes. The exact same binary passes two supplied-state scenes,
ten external audits, release corruption controls and six unchanged images.
Raw execution retains intentional exit1 at the missing target, without timeout.
Only08BD14 added; full cap852 required because850 would exhaust cap850.
Measured closure850functions/83480instructions/2386external records;
isolated11functions/629instructions/1external record fits existing cap12.
Twenty full-run sources frozen; twelve runtime patches unchanged. Checkpoint
**569 identities**, all574 prior and100 reduction identities reverified;
SHA-256 `f4914532835c28c2d6356c00763154bc4b03cd41cc5d315026abf2e164d80fd2`. Earlier04E60E parent return remains observed without a full
prospective transfer proof; the new full-return claim is specifically089F20.

Storage preservation verifies169 distinct completed diagnostic paths, including
31 test-produced16MiB oracle RAM captures from passed terminal reductions,
56 baseline captures,61 compiled outputs and21 raw startup captures. Oracle
captures were compared against native execution before eligibility; prediction-
only output, original game inputs, sources, assets and saves were excluded.
Every byte/path/owner/mode/timestamp/xattr retained; inode/birthtime changes
declared, no content removed. All574 prior identities reverified after sharing.
Full-pass available-byte changes total1,163,522,048; samples and unrelated
space fluctuations are not attributed to that sum. Capacity guards retained
and passed before generation/build/execution. All processes terminal; no
Simulator launched or booted. Original inputs, flash and root VMU unchanged.

**Next bounded experiment:** original `011238`,66 instructions. It saves
R14/PR, allocates an80-byte frame and copies sixteen ordered bytes from112E43
into SP+56. Actual gate164AE0 is1 and descriptor164ADC iszero, so source predicts
the complete early-return branch in48 modeled cycles, R0zero, PC/PR04E33E,
restoredSPFFFFA0/R14zero/T0. Also test the zero-gate path, which must reject
genuine missing606C9C in48 cycles at blockPC0112C0/PR0112CA/SPFFFF50, without
substituting a provider; later010FCC remains absent. Aliased writes can change
the gate, so preserve live reads, stack/string order, guards, all registers and
wholeRAM/CPU/TMU. Caller04E310 and its already-registered tail10EFBC remain
present; inspect the tail contract before predicting a further boundary.
These predictions are unexecuted. Isolated12 would exhaust cap12/propose14
only if measured; full851fits852 unless measured transitive closure differs.
One classification batch used; execute before broader inventory. Exact contract:
`artifacts/recovery-aot/20261006-204816/next-startup-preflight.json`.

Fresh G1 remains runtime0/1, generated guest sources0/1474, headers0/177,
contracts0/4, title providers38/39, FFmpeg source10139/10139. Apple media
integration is unverified and exact PALv1.003 descriptor remains absent.
Exact bootstrap inventory/source-only request remains unchanged and unsent.
Full initialization, IRQ completion, audible audio, normal boot, gameplay,
saves/VMU and mobile/production remain open. Goal classification: **progress**,
original G1 unresolved; R3/C8 remains partial.

## 7 October bounded record-loop experiment

The last full-startup result remains235341/38 comparisons. The separate original
10B906 predicate and actual10B992 first-call prefix pass; later missing10A326 is
observed only. See STATUS.md and the155-identity record-loop-prefix checkpoint.
Production sources were not changed and full startup was not rebuilt. The native
commands/cache identities and frozen test sources are in the experiment reports.
Eight exact redundant generated-image shares preserve237 diagnostic paths and
reverify3748+155 identities. Unit400MiB storage guard is met; full1536MiB guard
is still unmet. Fresh source/PAL checks exit2; original G1 stays unresolved.

## 7 October record serialization and command prefixes

Isolated original10A326/10A2DA/6067AA and64A734/664286 experiments pass, with
40 serializer and48 command controls. Three prospective helper phases and the
26-cycle command-prefix rejection check wholeRAM/CPU/TMU. Missing6642C0 is real;
worker/transport/helper/parent returns remain unverified. The151-identity command
checkpoint keeps full-startup235341/38 unchanged. No production-core edits or
full rebuild. Mach-O inspection through explicit CLT selection confirms macOS,
ARM64, minimum13.0/SDK27.0; global otool had failed at the existing Xcode license
boundary. Unit400MiB guard met; full1536MiB unmet. Source/PAL audits still exit2.


## 2026-10-07 — Original record worker and four-record parent return

**Previous bounded recovery — 7 October 2026: original worker and four-record loop return.**

Original `6642C0` now calls the real registered `663550` and returns. Its
source-derived **21-cycle entry** matches all 16 MiB RAM, 69 CPU fields and
25 timer fields, including the 24-byte frame, header words 4/0 and 48-word
packet buffer. The surrounding original `10B992` four-record loop also returns
in a separate fresh native process. Its checked endpoints restore the caller
stack/registers, retain mode22, increment the counter to1 and release the lock
through the original caller. All real bindings remain; no stub, lock reset or
forced completion was used.

Both isolated experiments disable device clocks. **Worker and parent returns
are observed; complete post-return RAM/CPU/TMU and packet delivery are not
verified.** This is not a normal-startup or gameplay pass. The last full-startup
checkpoint remains **235341 / 38 comparisons**, unchanged and not rerun.

The new checkpoint binds **95 identities**, SHA-256
`5f7a82d4da3263c30d7b984b16d008c7a30eb5ed96ba735d9297115e4c95e8ff`.
Final verification passes all 3,748 full-startup, 155 prefix, 151 command and
95 worker identities plus 237 preserved diagnostic paths. The executable is
ARM64/macOS, minimum13.0, SDK27.0, verified with explicit CLT selection.
All experiments are terminal; no Simulator is booted. Available capacity at
verification is 458,915,840 bytes: unit400MiB met, full1536MiB unmet.

Next derive the real `663550` request-path and complete original `10B992`
return expectations from source and initial state, then verify them with the
exact native binary. Account for packet/request records, counters, frames and
ordered live reads; observed dumps must not become expected output. Preserve
all38 full-startup expectations and measure closure before integration or cap
changes. Native execution resets the classification budget; at most two new
static batches may precede the next experiment.

Fresh source audit still reports runtime0/1, guest0/1474, headers0/177,
contracts0/4, title38/39 and FFmpeg10139/10139. The supported PAL descriptor is
absent. The exact source-only acquisition request remains current and unsent.
Original G1, normal boot, gameplay, audio, saves and mobile acceptance remain
unresolved. R3/C8 partial; goal turn: **progress**.
Evidence: `artifacts/recovery-aot/20261007-startup-record-worker/`, including
`final-verification.json` and `next-record-return-preflight.json`.

Reproduction runner: `work/dreamcast-recovery/run-startup-record-worker.py`.
It emits only the original24-instruction worker at cap1, links seven previously
verified emitted objects plus the bound cached runtime/provider catalog, and
runs worker then parent sequentially. It does not regenerate the full image
or rerun full startup. Executed sources are frozen under the artifact directory.
Use a fresh output directory and recheck capacity before another build.


## 2026-10-07 — Request return reproduction

`work/dreamcast-recovery/run-startup-record-request-return.py` builds only the
new oracle `tests/recovery/startup-record-request-return.cpp` and links the
previously verified native bodies/runtime. It verifies the prior95 identities
and all cached/input hashes; no guest regeneration or production core edit.
The original worker and real request handler pass16 controls and the captured
556-cycle full return on first build/run. Sources are frozen under
`artifacts/recovery-aot/20261007-startup-record-request-return/verified-source/`.
Use a fresh output directory for a justified new run. The independent queued
packet contract is verified; device delivery/full parent/startup are not.
Storage recovery preserves277 diagnostic paths and restores the unchanged
400MiB unit and1536MiB full-build guards; recheck before any build.
