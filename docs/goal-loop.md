# SonicPad goal-based loop

**10 October 2026 — review handoff supersedes the experiment queue.**
Chris requested wrap-up, private push/merge and an independent direction review.
Start with [HANDOFF-REORIENTATION.md](HANDOFF-REORIENTATION.md). The local `work/`
directory is now absent, including the prepared next-run script and experimental
runtime checkout. Historical checkpoints retain their recorded hashes but have
missing dependencies. Freeing disk space alone cannot resume the prior command.
Do not automatically extend the recovery queue; assess viability against the PRD.
The historical entries below are preserved, not current instructions.

**Blocked audit — 9 October 2026.** The previous goal turn made concrete
progress by preparing and testing the AICA verifier. This turn's revalidation
found no newly available source paths and no remaining useful preparation
before native comparison. The full-run disk guard has remained unmet across
at least three consecutive goal turns: latest free space 2,857,533,440 bytes,
required 2,952,790,016 bytes (95,256,576-byte shortfall). There is no live
process/session to wait on, no booted Simulator, and no next-run output directory.
All 1,657 previously missing source paths and the supported PAL GDI remain absent.
The exact bootstrap inventory and source-only request are already complete in
`BOOTSTRAP-BLOCKERS.md`; the request remains unsent. This is a blocked goal,
not completion and not a claimed build/runtime failure.

Resume when a fresh space check passes, then run
`python3 work/dreamcast-recovery/run-original-startup-live-movie-video-payload.py`
once and apply the prepared AICA companion/final verification. More free space
reopens this recovery experiment; it does not by itself resolve G1. Preserve
all existing evidence and do not lower the guard or delete material to force it.

**Current checkpoint — 9 October 2026: video-copy code compiled; native verification awaits space.**

Three original video-copy bodies (187 instructions, 32 new entries) compile;
the startup observer passes its syntax check. The prepared manifest has 2,671
MOVIE entries; the latest executed manifest remains 2,639. The frozen 3,319-cycle
prediction remains pending native comparison. No decoded movie or gameplay.

The AICA companion verifier is now prepared and regressed against two sealed
native captures (119 cycles/no mixer frame and 5,962 cycles/one mixer frame).
All 28 corrupted/unsupported cases are rejected. It checks timer bytes,
instruction count, clock fraction, mixer accounting and stable exception/line/
halt state; it does not model ARM registers or prove audio quality. Timer
overflow is explicitly outside its scope. The pending video-copy prediction
freezes 374 AICA steps, one silent mixer frame, accumulator 233 and final
fraction 193574400. Native comparison remains pending.

Verifier: `tests/recovery/verify-aica-silent-window-20261009.py`.
Preparation evidence:
`artifacts/recovery-aot/20261009-video-payload-aica-verifier-preparation/aica-verifier-checkpoint.json`,
SHA256 `847981bacd71e423a11475870132ea876e8a579d487116522ca3bf7225a43dd1`.
After a successful native capture, run the verifier with `--cycles 3319`,
`--before docs/artifacts/recovery-aot/20261009-original-startup-live-movie-video-payload/original-startup-video-payload-live-entry`
and `--after docs/artifacts/recovery-aot/20261009-original-startup-live-movie-video-payload/original-startup-video-payload-live-return`.
Compare its timer prediction to the frozen `expected-aica-timers.bin` as well.
This is a companion check, not a substitute for the runner's CPU/RAM/TMU,
PREF, unchanged-device, prefix and final checkpoint verification.

Prepared runner: `work/dreamcast-recovery/run-original-startup-live-movie-video-payload.py`.
It retains original startup, normal activation, immediate-parent capture sharing,
and the 2,952,790,016-byte free-space guard. Verify the normal-RAM PREF counter
increment, 374 AICA steps and one silent mixer frame as well as CPU/RAM/TMU;
complete AICA timer/instruction-count verification after capturing the run.
Do not substitute the earlier short-window zero-frame check.

Preparation checkpoint (380 identities):
`artifacts/recovery-aot/20261009-live-movie-video-payload-preparation/preparation-checkpoint.json`,
SHA256 `2ccf85143c8018b116753a85e2c71699287794586e1066fc4ff7e7109d6f3880`.
Compiled-body checkpoint (31 identities):
`artifacts/recovery-aot/20261009-live-movie-video-payload-bodies/video-payload-bodies-checkpoint.json`,
SHA256 `cb5be46b8855c237ff95ada90beeb685266cf078a6a426ec4288ec409051aec2`.
These establish preparation only; the observer has not been linked or executed.

Fresh source audit `artifacts/bootstrap-input-audit-20261009-video-preparation.json`
confirms 1,657 missing source paths and absent supported PAL GDI; all 10,139
FFmpeg files match their pinned identities, but Apple media compilation is open.
The scoped upstream metadata refresh in
`artifacts/source-availability-refresh-20261009-video-preparation.json` finds
no new release or source archive; the Katana repository endpoint returns 404.
G1 remains unresolved and the source-only request remains unsent.

Latest local free-space check: 2,856,026,112 bytes, below the full-run guard.
No new game or Simulator was launched. Continue focused work or resume this
runner after a fresh passing space check. Preserve existing evidence.

**Previous checkpoint — 9 October 2026: stream advances to the first real video payload.**

Original wrapper `8CED1DD0` now executes (26 instructions/four new entries;
2,639 MOVIE entries total). The frozen 119-cycle prediction matches all 69 CPU
fields, full 16 MiB RAM and TMU state. Slot-zero cursor advances `8C900000`→
`8C900800`; its consumed-byte counter becomes 2,048. Return PC/PR `8CED1D4C`,
stack `8CEE2138`, clock 799,306,518, r0=0. All 1,501 earlier captures remain
identical. AICA timers/instruction count verify across 13 steps (accumulator
59→72), with no new IRQ or mixer frame; full ARM register behavior remains open.

A separate post-run 2,645-cycle CPU/RAM/TMU audit matches the next padding
sector and first video packet. The stream cursor reaches `8C901000`, consumed
count 4,096, and packet type 1 selects missing video-payload callback `8CED1FBE`.
Guard PC/PR `8CED1F44`/`8CED1F54`, stack `8CEE20D0`, clock 799,309,163. Actual
packet ID `0xE0`, payload pointer `8C90101E`, payload length 2,018; its prefix is
`00 00 01 B3` (sequence header). Payload SHA256:
`70c5bb9f96c2966103297c935b887bbf3285779d0a09bb30fc03eb5b8e1af333`.
The full 65,536-byte input and 462,848-byte original disc payload remain intact.
The post-run audit is not a frozen prediction or full device-state proof;
earlier long IRQ/discovery audits remain open. No movie frame has been decoded.

Next video path is reviewed and predicted, but not natively executed:
`8CED1FBE` with shared tail `8CED2048`, copy selector `8CED20C4`, and halfword
copy `8CED6778` (187 instructions/32 entries across three new bodies). Feature
index 5 is enabled; target stream slot is 1. The frozen prediction copies 2,018
bytes to `8C910000`, commits its cursor at `8C9107E2`, and returns r0=1 at
`8CED1F54` after 3,319 cycles. It models one normal-RAM PREF as the runtime's
cacheless hint; native verification must separately verify its `pref_calls`
increment, and account for AICA mixer-frame boundaries over this longer interval.
Retain normal module activation and the shared-tail ownership when adding bodies.

Next closure inventory:
`work/dreamcast-recovery/live-movie-video-payload-closure-inventory.json`
(SHA256 `9ded1714dc22a4cecf86371019ba82e3bfaf99048f377a608510252052e34780`).
Next prediction: `artifacts/recovery-aot/20261009-live-movie-video-payload-prediction/prediction-checkpoint.json`,
SHA256 `bbb22181bff48552bd7d115e3510bfcb7670053adaf5ca4b42283728ebb95af4`
(21 identities). Its model is `tests/recovery/live-movie-video-payload-model-20261009.py`.
This prediction is pending native comparison; do not report the video copy as passed.

Passed final verification:
`artifacts/recovery-aot/20261009-live-movie-stream-advance-final-verification.json`.
Saved verifier: `work/dreamcast-recovery/verify-live-movie-stream-advance.py`.
Verified checkpoints under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-movie-stream-advance/live-movie-stream-advance-checkpoint.json`:
  `673b81137bd8cf15e0aef4f8420823f192bc6c56d1ff8096de9066cf45b8793c`
  (29,886 identities).
- `20261009-live-movie-stream-advance-prediction/prediction-checkpoint.json`:
  `b9208647a2e1a97e1dfeaa2b08cc9f0737b6e277ee24f8221a1aba49872cf150` (15).
- `20261009-live-movie-stream-advance-bodies/stream-advance-bodies-checkpoint.json`:
  `4db3fc9e230a2a033843267ed506abf26dbe0c541d84df25fed3e24e400ecdb5` (17).
- `20261009-live-movie-video-packet-boundary-audit/boundary-audit-checkpoint.json`:
  `ac389330d3f5b24ea546328d995e088c36a7ee4c2f70bf0aa9fda50855fa3c73` (32).

Capture helpers share 2,037,781,152 logical bytes and write 71,565,672 bytes.
Preserve immediate-parent sharing and existing evidence. Native run terminal;
no Simulator. Final verifier saw 2.89 GB free; after preparing the next prediction,
2,859,282,432 bytes remain, below the full-run guard of 2,952,790,016 bytes.
Do not attempt another full native capture until a fresh disk check passes.
Focused source/model work can continue above its 419,430,400-byte guard.
No deletion, source substitution, runtime promotion or publication was performed.
Default emitter, runtime candidate and pins unchanged. All 1,657 G1 source paths
and supported PAL GDI remain absent; source-only request unchanged and unsent.
No decoded movie, normal boot, gameplay, audio quality or device acceptance.
Goal active: verified stream progress and the next frozen model were completed.

**Previous checkpoint — 9 October 2026: first padding payload consumed; one sector accounted for.**

Three original MOVIE bodies now execute: consumer `8CED1EF8`, metadata query
`8CED7878`, and padding callback `8CED2044` (93 instructions, 20 new entries;
2,635 MOVIE entries total). The consumer is explicitly bounded at `8CED1F7A`:
65 instructions/14 entries, with original calls and unknown-target guards intact.
No adjacent callback expansion or runtime substitution.

The frozen 118-cycle prediction matches all 69 CPU fields, full 16 MiB RAM and
TMU state. The selected original padding callback returns one; the consumer
stores 2,015 consumed payload bytes and returns r0=0, PC/PR `8CED1EA4`, stack
`8CEE2110`, clock 799,306,369. All 1,461 earlier captures remain identical.
AICA timers/instruction count verify across 13 steps (accumulator 42→55), with
no new IRQ or mixer frame; full ARM register behavior/audio quality remain open.
The 65,536-byte worker input and complete 462,848-byte disc payload retain their
verified hashes.

An independently authored post-run 30-cycle CPU/RAM/TMU audit then matches the
caller return and total-byte accounting: instance `8CEE2210`+40 becomes 2,048.
This is the first complete sector's headers plus padding; it is not a decoded
movie frame. The audit ends at missing stream-advance wrapper `8CED1DD0`, called
by BSR at `8CED1D48`. Guard PC/PR `8CED1D42`/`8CED1D4C`, stack `8CEE2138`, clock
799,306,399, IRQ count 1,235. Full device-state coverage of that continuation and
earlier long discovery intervals remains open.

Next: model the stream cursor update, then integrate original `8CED1DD0`
(26 instructions/four entries). It calls retained `8CED9C48` with instance
`8CEE2210`, slot index 0 and byte count 2,048. Slot `8CEE2438` is active;
buffer state `8CEE2448` has cursor `8C900000`, limit `8C910000`, capacity 65,536
and consumed count zero. Retained lock/unlock calls are `8CED1BCC`/`8CED1BE4`.
The initial inventory mislabeled the zero slot index as a pointer; preserve it
as superseded and use `work/dreamcast-recovery/live-movie-stream-advance-inventory-repair1.json`
(SHA256 `9acb84e2b2182c851a04a9ab40ac459457aa2a3c012522097d1b95e7581b341f`).
The wrapper has not executed; cursor advancement is not yet proven.

Passed final verification:
`artifacts/recovery-aot/20261009-live-movie-packet-consumer-final-verification.json`.
Saved verifier: `work/dreamcast-recovery/verify-live-movie-packet-consumer.py`.
Checkpoints under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-movie-packet-consumer/live-movie-packet-consumer-checkpoint.json`:
  `2d85254cd8db702bef68dd7fb030360d8d19fcf4326126fb6c12917b7633aea1`
  (28,310 identities).
- `20261009-live-movie-packet-consumer-prediction/prediction-checkpoint.json`:
  `513134010065f70e0abeecbe497897719562abcbd85480e15933df92e0a85b14` (16).
- `20261009-live-movie-packet-consumer-bodies/packet-consumer-bodies-checkpoint.json`:
  `65d2e75c44af5e13747e17d5bf30f90771853ad3196e16897dd159f7b765ef9a` (31).
- `20261009-live-movie-stream-advance-boundary-audit/boundary-audit-checkpoint.json`:
  `aab59407990bc4ab7d7dbdb1902b46ccbfc8c6608b4ea27b080f1f937855525d` (13).

Capture helpers share 1,982,992,696 logical bytes and write 71,565,672 bytes.
Keep the immediate-parent reference and shared hardware headers. Native run
terminal; no Simulator. Final free space 3.10 GB; recheck the 2.95 GB full-run
guard. No prior evidence was deleted or changed. Default emitter, runtime
candidate and pins unchanged; candidate unpromoted. All 1,657 G1 source paths
and supported PAL GDI remain absent; source-only request unchanged and unsent.
No decoded movie, normal boot, gameplay, audio-quality or device acceptance.
Goal active; this turn makes measured progress.

**Previous checkpoint — 9 October 2026: system and padding-packet headers verified.**

Two more original MOVIE bodies now run through normal module activation:
`8CED6C88` (457 instructions/59 entries) and `8CED7024` (993 instructions/155
entries), bringing the MOVIE total to 2,615. Two separate frozen predictions
match all 69 CPU fields, full 16 MiB RAM and TMU state:

- System header: 257 cycles, consumes 15 bytes, returns at `8CED696C`, clock
  799,305,872. Eight fields at `8CEE6FCC` become `[9,9183,1,0,0,1,1,1]`;
  the classifier sees the following packet and returns `0x40000`.
- First padding-packet header: 129 cycles, consumes six header bytes, returns
  at `8CED69A0`, clock 799,306,121. Packet ID is `0xBE`, type is 3, payload
  length is 2,015. This check does not claim that its payload was consumed.

All 1,421 earlier captures remain identical in the latest run. Both the
65,536-byte worker buffer and full 462,848-byte disc payload retain their
verified hashes. AICA timers/instruction count verify across 29 steps for the
system interval and 14 for the packet interval; neither interval adds an IRQ
or mixer frame. Full ARM register behavior/audio quality remain open. The
120-cycle interval between these two checks remains discovery, including its
mixer-frame boundary; do not treat the two frozen checks as continuous coverage.

A further 130 discovery cycles after packet-header return reach missing
consumer `8CED1EF8`, called by BSR at `8CED1EA0`. Guard PC/PR:
`8CED1E90`/`8CED1EA4`; clock 799,306,251; IRQ count 1,235. Instance `8CEE2210`,
payload pointer `8C900021`, available length 65,503, consumed-output pointer
`8CEE2118`, stack `8CEE2110`. This continuation is not fully independently modeled.

Next: model and integrate consumer `8CED1EF8`, metadata query `8CED7878`, and
selected padding callback `8CED2044`. The consumer's explicit first RTS boundary
is `8CED1F7A` (65 instructions); the broad analyzer report expands adjacent
callbacks to 498 instructions and must not be treated as the bounded function.
Its query resource is `8CEE6FB0`; callback table `8CEDC8AC` selects slot 3,
`8CED2044`, which is an original two-instruction return-one helper. The query
has 26 instructions/five blocks and calls retained helpers `8CED7AC4`,
`8CED79B2`, `8C10E958`. All three new symbols remain unlinked. Static pointer
literals match the original MOVIE image and captured RAM; no pointer patch is
required. Source closure inventory:
`work/dreamcast-recovery/live-movie-packet-consumer-closure-inventory.json`
(SHA256 `203274ba31fd3c94d82222b5f538004ae50207ef31ed35a862e4492dd2a51389`).

Passed final verifiers under `artifacts/recovery-aot/`:

- `20261009-live-movie-worker-system-final-verification.json`; native checkpoint
  `20261009-original-startup-live-movie-worker-system/live-movie-worker-system-checkpoint.json`:
  `89c172319b490d72186333b2e16c41e453ff97d4e791910d41a250fd166a6aa6`
  (25,274 identities).
- `20261009-live-movie-worker-packet-final-verification.json`; native checkpoint
  `20261009-original-startup-live-movie-worker-packet/live-movie-worker-packet-checkpoint.json`:
  `74681ccfdf0587a2b307d9405fe6af16c98a76d39bea7a9d151ba3a6ce3195df`
  (26,770 identities).

Each final verifier pins the corresponding prediction and body checkpoints.
Saved verifier scripts are `work/dreamcast-recovery/verify-live-movie-worker-system.py`
and `work/dreamcast-recovery/verify-live-movie-worker-packet.py`. The first system
model draft lacked immediate XOR handling and stopped before producing a
prediction or native execution; preserved repair1 supplies the correct immediate
operand semantics. Native comparisons use only the sealed repaired prediction.

Latest capture helpers share 1,928,204,240 logical bytes and write 71,565,672
bytes. Preserve the immediate-parent capture reference/shared hardware headers.
Native runs are terminal; no Simulator. Free space at final verification: 3.12 GB;
recheck the 2.95 GB full-run guard. No prior evidence was deleted or changed.
Default emitter, runtime candidate and pins unchanged; candidate unpromoted.
G1 still lacks all 1,657 source paths and the supported PAL GDI. Source-only
request unchanged and unsent. No decoded movie, normal boot, gameplay, audio
quality or device acceptance. Goal active; this turn makes measured progress.

**Previous checkpoint — 9 October 2026: first real movie pack header parsed and verified.**

Three original MOVIE functions (`8CED690A`, `8CEDA88C`, `8CED69B2`; 485
instructions, 72 new entries, 2,401 total) now classify and parse the first
pack header from the real worker buffer. The frozen 231-cycle prediction
matches all 69 CPU fields, full 16 MiB RAM and TMU state. It returns r0=1,
PC/PR `8CED6898`, stack `8CEE20E0`, clock 799,305,521. Classification is
`0x10000`, consumed length is 12 bytes and the three pack fields are `[1,0,9183]`.
All 1,341 earlier captures remain identical. The 65,536-byte worker buffer and
full 462,848-byte disc payload remain byte-identical to their verified inputs.
AICA timers and instruction count verify across 26 steps (accumulator 446→472),
with no new IRQ or mixer frame. Full ARM register behavior/audio quality remain open.

The next 94 cycles advance the buffer to `8C90000C`, classify `00 00 01 BB` as
`0x20000`, and reach missing system-header parser `8CED6C88` via BSR at
`8CED6968`. A separately authored post-run instruction audit matches all 69 CPU
fields, RAM and TMU state across that interval; it is not a frozen prediction
or a full device-state audit. Guard PC/PR `8CED6960`/`8CED696C`, stack `8CEE20C4`,
clock 799,305,615, IRQ count 1,235. Inputs: resource `8CEE6FCC`, buffer
`8C90000C`, remaining length 65,524, consumed-output pointer `8CEE20E4`.

Next: model and integrate the original system-header parser `8CED6C88` through
normal MOVIE activation, preserving all unknown-target and retirement guards.
Its reviewed 457 instructions/59 blocks match MOVIE and captured RAM; its two
calls both target retained classifier `8CEDA88C`. The captured header length
field is 9. This body has not executed. Inventory:
`work/dreamcast-recovery/live-movie-worker-system-inventory.json`.

Final verification passed:
`artifacts/recovery-aot/20261009-live-movie-worker-pack-final-verification.json`.
Saved verifier: `work/dreamcast-recovery/verify-live-movie-worker-pack.py`.
Checkpoints under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-movie-worker-pack/live-movie-worker-pack-checkpoint.json`:
  `02931d5b8d559640672dedb920cc875b7a2da4c3d23ec3a8e0f9432df8e92cb0`
  (23,818 identities).
- `20261009-live-movie-worker-pack-prediction/prediction-checkpoint.json`:
  `59c360c4297477895b8bd72b3d41a36be0ae43597e6b9a67d2faf3d06cd16e6e` (14).
- `20261009-live-movie-worker-pack-bodies/worker-pack-bodies-checkpoint.json`:
  `5d55f430f6758959b29bf2229ac59c8f01a96ad7b565add6e289c49693f989ab` (31).
- `20261009-live-movie-worker-system-boundary-audit/boundary-audit-checkpoint.json`:
  `a806d73e8a46148d6f32dd2af31ae701478c26fe95394f30f28f87a6c69e355e` (15).

Capture helpers share 1,818,627,328 logical bytes and write 71,565,672 bytes.
Preserve the immediate-parent capture reference and shared hardware headers.
No prior evidence was deleted or changed. Free space at final verification:
3.69 GB; recheck the 2.95 GB full-run guard. Native run terminal; no Simulator.
Default emitter, experimental runtime and pins unchanged; runtime unpromoted.
G1 still lacks all 1,657 required source paths and the supported PAL GDI;
source-only request unchanged and unsent. No decoded movie, normal boot,
gameplay or device acceptance. Goal active; this turn makes measured progress.
Earlier long IRQ/discovery audits remain open.

**Previous checkpoint — 9 October 2026: movie loop tail verified; worker receives real stream data.**

Nine original MOVIE bodies (193 instructions, 36 new entries; 2,329 total)
complete the first main-loop tail. The frozen 851-cycle prediction matches all
69 CPU fields, full 16 MiB RAM and TMU state; PC/PR is `8CEB0304`, stack
`8CFFFF5C`, clock 799,201,094. All 1,301 earlier captures remain identical.
Counter `8CEDCF4C` advances from zero to one. The actual object, service and
resident-record scans run; their inactive branches remain untested. AICA timers
and instruction count verify across 96 steps; full ARM register behavior and
audio quality remain open.

A further 104,196 discovery cycles and two interrupts reach missing worker
parser `8CED690A`, called by BSR at `8CED6894` (guard PC `8CED6886`, PR
`8CED6898`, clock 799,305,290). The retained r1 value `8CED6848` is the already
linked parent, not the missing target. The discovery interval is not an
independent full-state proof.

The actual worker input at `8C900000` contains 65,536 bytes identical to the
start of the disc-verified movie payload; SHA256:
`a2ec7d6d6a6734420e58fd4b346a4f52b4fa8254a72608615df05fbba5ffeaf5`.
All 462,848 payload bytes remain intact. Source review of classifier `8CEDA88C`
identifies the input prefix `00 00 01 BA` and predicts flag `0x10000`, selecting
pack-header parser `8CED69B2`. Those classifier/parser bodies have not run yet.
Next: model the original parser chain and its SH4 bit operations, then integrate
needed bodies through normal MOVIE activation and verify the first pack return.
Reviewed inventories: `work/dreamcast-recovery/live-movie-worker-parser-inventory.json`
and `work/dreamcast-recovery/live-movie-worker-pack-inventory.json`.

Fresh hardware capture headers extend the existing live-byte comparison and
APFS sharing mechanism to VRAM/TA vectors. Preserve the immediate-parent
`SONICPAD_RECOVERY_CAPTURE_REFERENCE` override in subsequent runners. This run
shares 1,755,450,412 logical bytes and writes 79,954,132 bytes through the helpers;
all shared outputs verify against their parent. No prior evidence was changed
or deleted, and capture files never supply guest state. Free space at this
checkpoint update: approximately 3.91 GB; recheck the 2.95 GB full-run guard.

Final verification passes in
`artifacts/recovery-aot/20261009-live-movie-main-tail-final-verification-repair1.json`.
The original verifier report is preserved but superseded: a variable name reuse
serialized storage metrics in its identity-count field. The fresh repair reruns
all checks and asserts the identity-count schema and total of 22,504.
Checkpoint identities:

- Native: `f7a63c9267465f2bb64867a9dea44fceb129df7ab958a4ccb4230da7bc512b08`
  (22,398 identities), directory `20261009-original-startup-live-movie-main-tail`.
- Prediction: `8404f4d4881e244f57f1c3b0c6dff5055e35c053fc46458477b6ee88a72bd560`
  (33 identities), directory `20261009-live-movie-main-tail-prediction`.
- Bodies: `5b823a3c3395955c885f3cbf3c7cc22b9d96bba3c687c8a5296676432d031e2f`
  (73 identities), directory `20261009-live-movie-main-tail-bodies`.

Saved verifier: `work/dreamcast-recovery/verify-live-movie-main-tail-repair1.py`.
The final verification recorded no active native run or booted Simulator.
Default emitter and pins remain unchanged; experimental runtime is unpromoted.
G1 remains unresolved: 1,657 required source paths and the supported PAL GDI
are absent. The source-only request remains unchanged and unsent. No decoded
movie, normal boot, gameplay, audio-quality or device acceptance. Goal active;
this checkpoint makes progress, with earlier long IRQ/discovery audits still open.

**Previous checkpoint — 9 October 2026: object status verified; capture sharing restored.**

Original MOVIE status getter `8CECF4F0` is integrated through module activation
(17 instructions, five new entries; 2,293 total). Its non-null path matches the
frozen eight-cycle prediction for all69 CPU fields,16MiB RAM and TMU state.
It returns status1 from object `8CEDD114`+8, PC/PR `8CEB02E6`, stack `8CFFFF5C`,
clock799,200,230. All1,261 earlier captures remain identical; the462,848-byte
movie payload still matches the independent disc hash. One AICA clock step,
timers and instruction count verify; no new IRQ or mixer frame. Full ARM
register path and audio quality remain open.

Continuation runs13cycles and stops at the existing module guard: caller
`8CEB02FE`, PR `8CEB0304`, requested target in r2=`8CED0914`, clock799,200,243.
It stores observed status1 in `8CECF02C`. This13-cycle continuation is discovery,
not a full independent CPU/device-state proof. Source review identifies the
loop-tail chain `8CED0914`→`8CED5464`, which walks16 callback slots at `8CEFB2C4`.
Only slot0 is nonzero: target `8CECFC58`, argument0. That callback invokes
`8CECFBA8`, `8CED59DE`, `8CED09BE`, and conditionally the retained `8CECF91C`.
The32-slot scan inside `8CECFBA8` has only its first slot nonzero (object
`8CEDD114`, tested field at offset4). Its first required call is `8CECFAE4`.
The service flag at `8CEFB54C` is0; `8CED59DE` calls `8CED56C0`/`8CED56D8`.
`8CED09BE` forwards to resident `8C65FEDE`. These are reviewed dependencies,
not executed tail results. Inventory:
`work/dreamcast-recovery/live-movie-main-tail-inventory.json`.

Next: review the actual first scan/query path and model the loop-tail callback
chain, then integrate only needed original bodies through normal MOVIE
activation. Preserve retirement and unknown-target guards. Do not skip the
scan or replay guest state.

Storage fix: the existing `capture-storage.hpp` already compares live memory
before APFS cloning and verifies the output afterward. The runner had inherited
a stale reference from8October. Set `SONICPAD_RECOVERY_CAPTURE_REFERENCE` to the
verified immediate parent; keep this override in future runner copies. This run
clones188 byte-identical captures /1,180,565,504 logical bytes and writes seven
new captures /54,788,096 bytes through that helper. Saved verifier checks all
cloned files against the pinned parent. No prior evidence is deleted or changed,
and no file supplies guest state. Remaining direct VRAM captures total
545,259,520 bytes in `startup-hardware-capture-20261009.hpp`; if more storage
work is needed, reuse the same live-byte comparison helper in a fresh header,
without editing sealed inputs.

One native run terminal; no booted Simulator. Free space at final audit:3.89GB.
Recheck the2.95GB full-run guard before another capture. Default emitter unchanged;
experimental runtime unpromoted. Earlier long IRQ/discovery audits remain open.
Previous/current goal turns make progress; goal active. G1 still lacks1,657
required source paths and the supported PAL GDI. Source-only request unchanged
and unsent. No decoded movie, normal boot, gameplay or device acceptance.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-main-status-final-verification.json` (passed).
- `20261009-original-startup-live-movie-main-status/live-movie-main-status-checkpoint.json`:
  `b91b8c6c5f77eb25882251b8ac069c33dbbda99f71f865a4f70bebd5660eb556`
  (21,003 identities).
- `20261009-live-movie-main-status-prediction/prediction-checkpoint.json`:
  `bebf8cb0efabcf67c039320a6340306e2ab3dd69f77df7d8fa38180b9d788042`.
- `20261009-live-movie-main-status-bodies/main-status-bodies-checkpoint.json`:
  `3e6d61eb92a2c04098920e4d36332d16819b0ba13d138f81702d9114c443df49`.

Saved verifier: `work/dreamcast-recovery/verify-live-movie-main-status.py`.

**Previous checkpoint — 9 October 2026: main-state query return verified.**

Three original MOVIE bodies now execute: `8CEB0430`, `8CECFEC8`, `8CED4846`
(342 instructions, 69 new activation entries; 2,288 total). Shared tail
`8CED48E0` retains its earlier frame-service-instance registration; the new
body keeps the original internal branch without registering that entry twice.
No resident-registration bypass or module-guard relaxation.

The actual first path matches the frozen 321-cycle prediction for all 69 CPU
fields, 16 MiB RAM and TMU state, returning to PC/PR `8CEB02D8`, stack `8CFFFF5C`,
clock 799,200,214. All 1,221 earlier capture files remain identical. The query
reads values `FFFFFFFF`/1 from instance `8CEE2210`; its wrapper clamps the
negative numerator to0 and stores denominator1. Globals `8CECF034`/`8CECF038`
become0/1; derived value `8CECF03C` and main-state `8CECF044` remain0. Return r0=0.
The original positive-divisor arithmetic paths for1 and100 are modeled; no
synthetic division result or guest-state replay is used.

The next eight cycles reach the status-method call from `8CEB02DC`, PR
`8CEB02E6`, clock 799,200,222. A separate source-derived saved-state audit of
those seven instructions verifies all CPU fields, RAM and TMU state through
that stop. Their words match both MOVIE image and live RAM. This later audit
is distinct from the frozen pre-execution321-cycle prediction.

The requested target is `8CECF4F0`, from object `8CEDD114`, vtable `8CEDCEE0`,
slot32; its generated symbol is absent. Original review has17 instructions,
five blocks, two calls, with opcode bytes matching live RAM. The actual object
is non-null and its status at offset8 is1. That path branches to `8CECF50A` and
returns the status; null-object error calls are not on the captured path.
Next: model/integrate this original status getter through MOVIE activation,
then follow the main-loop continuation. Inventory:
`work/dreamcast-recovery/live-movie-main-status-inventory.json`.

The 462,848-byte payload retains the independent disc hash. The321-cycle
prefix verifies36 AICA clock steps, timers and instruction count, unchanged
peripheral captures, no mixer frame and no IRQ (count1,233). Full ARM register
path/audio quality remain open; the eight-cycle caller audit covers CPU/RAM/TMU,
not complete device state. Earlier longer IRQ/discovery intervals remain open.
Default emitter unchanged; experimental runtime unpromoted. One native run is
terminal, no booted Simulator. Free space at final audit:4.62GB. Recheck the
2.95GB full-run guard before more native capture work; no cleanup authorized.
Previous/current goal turns make progress; goal active.

G1 still lacks1,657 required source paths and the supported PAL GDI. Source-only
request unchanged and unsent. No decoded movie, normal boot, gameplay or device
acceptance is claimed.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-main-state-final-verification-repair1.json` (passed).
- `20261009-original-startup-live-movie-main-state-repair1/live-movie-main-state-checkpoint.json`:
  `9d6c3cda867414e05ba67aecfc0c5cb3c7436c4b085df32b8ecb17afa0c2f28a`
  (19,667 identities).
- `20261009-live-movie-main-state-prediction/prediction-checkpoint.json`:
  `b7e7f228a76b3dd3a2558473b7a009f39bb58b5c07efec6773f2497141be2e73`.
- `20261009-live-movie-main-state-bodies/main-state-bodies-checkpoint.json`:
  `b7bbfe1103d19f5b2713e6b31fd511aaa3395dd77e9ed5ec26742bd85127aada`.

Saved verifier: `work/dreamcast-recovery/verify-live-movie-main-state-repair1.py`.
The first integration draft failed compilation on a duplicated harness observer,
before guest execution; it and its failed report are preserved. The first final
verifier ran the state checks but serialized the prefix count as5 after a local
variable was reused. Its repaired successor reran all checks and records1,221;
the original report is preserved and superseded.

**Previous checkpoint — 9 October 2026: waiting thread restored; main callback verified.**

The original resident resume trampoline `8C5FFB60` (eight instructions) now
enters the retained context switcher. All 69 CPU fields, full 16 MiB RAM and
TMU state match the independent 209-cycle save/restore prediction. It restores
PC/PR `8CECFAA0`, SR `40000100`, and stack `8CFFFF10`, at clock 799,185,717.
Current context changes `8CEDE190`→`8CEDE188`; the outgoing context's saved stack
becomes `8CEE2030`. All 1,141 earlier captures remain identical.

The pending Holly bit `1000` is enabled at priority4. BL blocks it during the
context switch; generated code ticks before the final RTE restores SR. The
next block delivers the IRQ at clock 799,185,720, and the observed handler
returns to `8CECFAA0` at 799,193,093. The full 12,579-cycle continuation after
the restore is discovery, not an independent CPU/device-state proof.

The next missing target `8CED0908` is now integrated through normal MOVIE
activation, together with `8CED55DA` and `8CECFC92` (31 original instructions,
eight new entries; 2,219 total). This chain walks a 16-slot callback table at
`8CEFB3C4`; only slot0 is nonzero, target `8CECFC92`, argument0. The complete
142-cycle return matches all 69 CPU fields, RAM and TMU state. It clears frame
counter `8CEDCF44` from2 to0 and returns to `8CEB0264`, stack `8CFFFF5C`, clock
799,198,438. All 1,181 earlier capture files remain identical.

Continuation advances another 1,455 cycles to a module guard at `8CEB02D4`,
PR `8CEB02D8`, clock 799,199,893. Source identifies the requested target as
`8CEB0430`, with r4=0. Its generated symbol is absent from the linked executable;
the combined guard alone does not distinguish inactive from unregistered.
Review finds 265 original instructions, 55 blocks and ten call sites, with all
instruction bytes matching the MOVIE image and captured RAM. Its first virtual
call uses object `8CEDD114`, vtable `8CEDCEE0`, slot36, target `8CECFEC8` (also
absent). That method first calls `8CED4846`. The captured main-state globals
`8CECF030` through `8CECF050` are all zero.

Next: review the query chain beginning `8CECFEC8`→`8CED4846`, model the actual
first path through `8CEB0430`, and add the needed MOVIE bodies through its
activation manifest. Preserve unknown-target and retirement guards. Inventory:
`work/dreamcast-recovery/live-movie-main-next-inventory.json`. No guest state
replay or synthetic branch skips. Recheck disk space before the next native run.

Both exact prefixes preserve the 462,848-byte movie payload and captured
peripherals. AICA accounting verifies23 and16 clock steps respectively, timer
state and instruction counts, with no new mixer frame or IRQ within either
prefix. Full ARM-register path and audio quality remain unverified. Later
continuations and earlier long IRQ intervals remain independently unmodeled.
Default emitter unchanged; experimental runtime unpromoted. Two sequential
native runs are terminal, no booted Simulator. Final free space: 6.48 GB.
Previous/current goal turns make progress; goal remains active.

G1 still lacks 1,657 required source paths and the supported PAL GDI. Source-only
request unchanged and unsent. No decoded movie, normal boot, gameplay or device
acceptance is claimed.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-main-resume-final-verification.json` (passed).
- `20261009-original-startup-live-movie-main-resume/live-movie-main-resume-checkpoint.json`:
  `25655b2f2707f155a85db6ce3bbab58cf501e17911f12d0f08ce7eb165cf27a6`
  (18,367 identities).
- `20261009-live-movie-main-resume-prediction/prediction-checkpoint.json`:
  `ce0d951505c23cfdb18ef172d777357c63cb0ae2450ecc4751d3f4afd7b7ec9f`.
- `20261009-live-movie-main-resume-bodies/main-resume-bodies-checkpoint.json`:
  `e63abca229d71299d86ec30d04e673c8d604325eb23748adc4bbbf129aadc4d2`.
- `20261009-live-movie-wake-resume-final-verification.json` (passed).
- `20261009-original-startup-live-movie-wake-resume/live-movie-wake-resume-checkpoint.json`:
  `307508f97c7994f78a130cddfeef97a2ac7f0933fd556a961c04f9a99b8463d4`
  (17,107 identities).
- `20261009-live-movie-wake-resume-prediction-repair1/prediction-checkpoint.json`:
  `5e851588d6f90ee16a8cf02abf6844db76e5644c95160c11146f0c34fb139f04`.
- `20261009-live-movie-wake-resume-bodies/wake-resume-bodies-checkpoint.json`:
  `d1225c7b13955168346c03f28ce9f4650b15956c03f88364dc1f6f064c3313a0`.

Saved verifiers: `work/dreamcast-recovery/verify-live-movie-wake-resume.py` and
`work/dreamcast-recovery/verify-live-movie-main-resume.py`.

**Previous checkpoint — 9 October 2026: saved interrupt-frame wake verified.**

Two original resident bodies, `8C5FFCFC` and `8C5FFD1E` (38 instructions,
4 resident entries), now execute the frame-two wake. All 69 CPU fields, full
16 MiB RAM and TMU state match the independent 40-cycle prediction. Return
clock is 799,185,182; all 1,101 earlier capture files are identical.

The saved interrupt-frame PC at `8C896A98` changes from `8CED9C48` to
`8C5FFB60`, and its SR at `8C896A9C` changes from `40000001` to `50000001`.
The requested context becomes `8CEDE188`; original PC/SR are preserved in
`8C5FFD88`/`8C5FFD84`. The normal IRQ return subsequently reaches missing
`8C5FFB60`, clock 799,185,508, SR `50000001`, stack `8CEE2134`.
That later 326-cycle interval is observed, not independently modeled.

The 462,848-byte movie payload retains its independently verified disc hash.
The wake prefix verifies five AICA clock steps, timer state and instruction
count, with no new mixer frame or IRQ; the IRQ count stays 1,232. Full ARM
register-path and audio-quality checks remain open. MOVIE registration stays
at 2,211 entries; module activation/retirement guards are retained.

Next: model and integrate the eight-instruction original resume trampoline
`8C5FFB60`, which reloads saved SSR/SPC and jumps to the already linked context
switcher `8C5FFB70`. Use runtime values at `8C5FFD84`/`8C5FFD88`, not the zero
literals in the original image. Current context is `8CEDE190`, requested context
`8CEDE188`, and the requested saved stack is `8CFFFE0C`. Its saved target is
`8CECFAA0` with SR `40000100`; these are inspected inputs, not an executed
resume result. Holly pending0 is `1000`: audit interrupt delivery around RTE
before reusing the earlier no-pending-interrupt context model.

Saved verifier `work/dreamcast-recovery/verify-live-movie-frame-wake.py` passed.
Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-frame-wake-final-verification.json`.
- `20261009-original-startup-live-movie-frame-wake/live-movie-frame-wake-checkpoint.json`:
  `c923e8c45960673b73ed2020cccb4e16dffd08cbfc5d67b9bb0082c0cd65d2bb`
  (15,891 identities).
- `20261009-live-movie-frame-wake-prediction/prediction-checkpoint.json`:
  `3c49e710815984d402e4a9828d27e964c796f8d9996952d87903694eba574a41`.
- `20261009-live-movie-frame-wake-bodies/frame-wake-bodies-checkpoint.json`:
  `d926f77b9f3e07cd0172dae91bda3d4eb08cf3b2296a781a140f269d4490dde0`.

Default emitter unchanged; experimental runtime unpromoted. Native run terminal,
no booted Simulator. Free space at final verification: 10.39 GB. Earlier long
IRQ/discovery intervals remain open. This goal turn makes progress; goal active.
G1 still lacks 1,657 required source paths and the supported PAL GDI. Source-only
request unchanged and unsent. No decoded movie, normal boot, gameplay or device
acceptance is claimed.

**Previous checkpoint — 9 October 2026: movie frame service runs; transferred data is committed.**

The first MOVIE frame service now executes through normal module activation.
Nine original MOVIE bodies (263 instructions, 46 new entries; 2,211 total) match
a source-derived 349-cycle prediction for all 69 CPU fields, 16 MiB RAM and TMU
state. Return clock 795,859,803; 1,021 earlier captures are identical. Frame and
notification counters advance 0→1. This resolves the previous missing frame
callback without weakening the runtime's module guard.

Continuation reaches completed-read processing. A further original resident
helper `8C66C834` (22 instructions) splits the buffer descriptor. The entire
completion path through the enclosing record return matches an independent
468-cycle CPU/RAM/TMU prediction, clock 799,181,043; 1,061 earlier captures are
identical. Resource `8C8AA8F8` now has 462,848 available bytes, previously zero.
The record's completed-byte count becomes 462,848 and its read-in-flight flag
clears. The completed span is `8C983000`/462,848 bytes; the remainder is
`8C9F4000`/zero bytes. The payload retains the independently verified disc hash.
This establishes data availability in the resource buffer, not movie decoding.

The next actual stop is `8C5FFCFC`, called from `8CECFC36`, PR `8CECFC3E`, stack
`8C896930`, at clock 799,185,142. Frame count and wake threshold are both2;
requested context is `8CEDE188`. Its original helper forwards to `8C5FFD1E`.
The reviewed path patches the saved interrupt return: frame top `8C896AA0`,
PC slot `8C896A98` currently `8CED9C48`, SR slot `8C896A9C` currently `40000001`.
The original replacement PC is `8C5FFB60`, with SR OR mask `10000000`. All three
generated bodies are absent from the linked executable. The two reviewed wake
bodies have 38 original instructions, all matching live RAM.

Next: model the wake helpers and their writes to this actual saved frame, then
integrate the missing original bodies and observe the interrupt-return path.
Preserve saved CPU/banked-register/context semantics; do not replay fixture state
or skip the wake. Inventory: `work/dreamcast-recovery/live-movie-frame-wake-inventory.json`.

**Verification scope:** exact checks cover the first frame-service return and
the read-completion return. The intervening 3,320,772-cycle continuation and the
later 4,099-cycle discovery are not independently modeled end to end. Earlier
long IRQ audits also remain open. The completion prefix verifies 53 AICA clock
steps, timers and instruction count, unchanged exception counters and peripheral
captures; no new IRQ or mixer frame. Full ARM-register and audio quality remain
open. Default emitter unchanged; runtime candidate unpromoted. Two sequential
native runs, now terminal; no booted Simulator. Free space at final audit: 11.83 GB.
Previous/current goal turns are progress; the goal remains active.

G1 still lacks 1,657 required source paths and the supported PAL GDI. Source-only
request unchanged and unsent. No decoded movie, normal boot, gameplay or device
acceptance is claimed.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-read-completion-final-verification.json` (passed).
- `20261009-original-startup-live-movie-read-completion/live-movie-read-completion-checkpoint.json`:
  `3f051b5a3592cff44b9dadf467117b6091f4b8ef39d7457a8ac9e4a9f5723277`
  (14,713 identities).
- `20261009-live-movie-read-completion-prediction/prediction-checkpoint.json`:
  `34cf632ed898a0c6ad9e85b9a52d3e1073f22cbf20a47d485b42ce8dea0e0d55`.
- `20261009-live-movie-read-completion-body/read-completion-body-checkpoint.json`:
  `c6e19ae17e6aa77d90bd4ee4c72e7781bc9db6ef8571e294b6ae0b798ae1cf0e`.
- `20261009-live-movie-frame-service-final-verification.json` (passed).
- `20261009-original-startup-live-movie-frame-service/live-movie-frame-service-checkpoint.json`:
  `0f4273030f6175c5ee698bd0a3243fd220a5df5d0875a33bc722880c530a6a96`
  (13,577 identities).

Saved verifiers: `work/dreamcast-recovery/verify-live-movie-frame-service.py` and
`work/dreamcast-recovery/verify-live-movie-read-completion.py`.

**Previous checkpoint — 9 October 2026: owner service verified; MOVIE frame callback reached.**

Four original resident functions now execute: `660754`, `6601A8`, `660562` and
`65F4A0` (469 instructions, 121 resident entries). The first owner-service path
matches all 69 CPU fields, 16 MiB RAM and TMU state across 545 cycles, ending at
795,853,711. All 981 earlier captures are identical. The task-state query returns
1, so initialization remains pending; the common tail updates the last timer
sample to 3,058,811. Owner state remains 1. Captured flag byte114=1 skips the
resource-starvation check; counters at offsets104/106 remain zero.

Continuation reaches the next MOVIE callback, requested target `8CED5518` in r3,
from PC `8C649872`, PR `8C649878`, stack `8C89694C`. Runtime stops at its existing
"Inactive or unregistered code module target" guard, clock 795,859,454. This later
5,743-cycle interval is discovery, not independent full CPU/device verification.
The requested generated body is absent from the linked executable. The activation
log records 2,165 MOVIE entries; no direct module-active/entry snapshot was taken,
so do not claim that the combined error alone proves which guard predicate failed.

Fresh source review identifies a 16-slot callback table at `8CEFB344`, with one
nonzero target: `8CECFBDC`. Its frame-notification path reaches `8CED38BA` →
`8CED478E`, which services one instance at `8CEE2210` through `8CED47D8` and
`8CED47F8`; the instance frame-handler pointer is `8CED14C8`. All reviewed
instruction bytes match the original MOVIE image and live RAM. Missing generated
bodies are recorded in the next inventory. The entire mutable MOVIE memory range
need not equal the original image; instruction-byte equality is the source check.

Next: review `ED482C` and `ED4956`, model the first frame-service path, then add
only the needed original MOVIE bodies to the activation manifest. Preserve module
retirement and unknown-entry rejection. Do not bypass the module guard with
resident registration. The first callback starts with frame count0/threshold2;
its notification flag is set, so the notification chain must be modeled.

The 462,848-byte movie payload still matches its independently verified disc
hash. Owner-prefix accounting verifies 61 AICA clock steps with no new mixer frame
or IRQ and unchanged captured peripherals; full ARM-register and audio quality
remain open. Default emitter unchanged; runtime candidate unpromoted. One native
run, now terminal; no booted Simulator. Free space at verification: 14.71 GB.
Previous/current goal turns are progress; the goal remains active.

G1 remains unresolved: 1,657 required source paths and supported PAL GDI remain
absent. Source-only request unchanged and unsent. No decoded movie, normal boot,
gameplay or physical-device acceptance. Prior longer IRQ/discovery audits remain
open; the exact owner-prefix check does not resolve them.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-owner-final-verification.json` (passed).
- `20261009-original-startup-live-movie-owner/live-movie-owner-checkpoint.json`:
  `28625b244627729d870b8829766e70516c8054cbc825599f62f15414cda29a19`
  (12,465 identities).
- `20261009-live-movie-owner-return-prediction/prediction-checkpoint.json`:
  `6da36dd84c839adb6dc765f59528820d12fe92d9da926486a4bbb12da8135955`.
- `20261009-live-movie-owner-bodies/owner-bodies-checkpoint.json`:
  `54a9188417a7b1084bcea1b10079973f75c43681e32f45371ff1811d0d6b0c51`.

Saved verifier: `work/dreamcast-recovery/verify-live-movie-owner.py`.
Next inventory: `work/dreamcast-recovery/live-movie-frame-service-inventory-repair1.json`,
SHA-256 `8d13acf272693c7da6152c26e7670d752e81888bbd60290eece5fbc6f3aad248`.
Use repair1: it distinguishes a missing generated body from uncaptured module
active/entry state. Original inventory is retained without being promoted.

**Previous checkpoint — 9 October 2026: movie data reaches RAM.**

Original startup now reads 226 sectors from the owner CDI at FAD 292,408 into
`8C983000`. All 462,848 transferred bytes match independent extraction from the
disc image, SHA-256 `7ae67ec4a965b09419162db0c586563cacc7083ed8126c9df11674546347e301`.
GD sector count advances 314→540. This is actual payload transfer, not merely a
queued request. Movie decoding, normal boot and gameplay are still unproven.

Two sequential native runs integrate three original resident functions (311
instructions total): task service `8C661490`, record dispatcher `8C65FED0` and
record service `8C65FD66`. The task status path exactly matches its source-derived
98-cycle prediction, ending at 795,843,780; 901 earlier captures are identical.
The record path matches all 69 CPU fields, 16 MiB RAM and TMU state through the
first read-callee entry at `8C604DC4`: 269 cycles, clock 795,844,374, 941 earlier
captures identical, request 226 sectors, destination `8C983000`.

The read path continues to missing owner-service function `8C660754`, caller PC
`8C64849E`, PR `8C6484A2`, stack `8C896928`, record `8C6774F0`. Final clock is
795,853,166. This later 8,792-cycle interval has exact payload verification, but
its entire CPU/device state is not independently modeled. The earlier long
polling/IRQ interval remains open too. Do not generalize the prefix checks into
complete interrupt or startup correctness.

Next: owner state byte1=1 selects missing `8C6601A8`; its dispatcher also has a
common tail at missing `8C660562`. Fresh review of `8C660754` matches all 38
original instructions in live RAM (15 blocks). Review the selected handler and
tail, then predict and integrate the needed original path. Source inventory:
`work/dreamcast-recovery/live-movie-owner-source-inventory.json`.

The record prefix independently verifies 30 AICA steps, timer and instruction
count updates, unchanged exception counters and peripheral captures. No new IRQ
or mixer frame in that prefix. Full ARM register-path and audio quality remain
open. MOVIE entries remain 2,165. Default emitter unchanged; experimental runtime
unpromoted. Both native processes are terminal; no booted Simulator. Free space
at final verification: 16.24 GB. Previous/current goal turns are progress.

G1 remains unresolved: all 1,657 required source paths and supported PAL GDI
remain absent. Source-only request unchanged and unsent. No production foundation
change or gameplay/device acceptance is claimed.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-record-final-verification.json`: exact prefix and disc bytes.
- `20261009-original-startup-live-movie-record/live-movie-record-checkpoint.json`:
  `cec8c8704ad20f607c5b4451a18a3d3c6d6e0809e8986ec286270253e3f475cf`
  (11,403 identities).
- `20261009-live-movie-record-request-prediction/prediction-checkpoint.json`:
  `d710b87abf259481c40993d7ee65d9725fdb5b1ce7c212243aea750177ded603`.
- `20261009-live-movie-record-bodies/record-bodies-checkpoint.json`:
  `66285f550ef14b2829a5b73a1cfd35185fd79b6f927ff2850129efc2b01b6876`.
- `20261009-live-movie-task-final-verification.json`: preceding 98-cycle task return.
- `20261009-original-startup-live-movie-task/live-movie-task-checkpoint.json`:
  `adea2dc34b0a1b55faab3733a8e9dff46f79bd12d6d2fb0c1846836caeb5ea86`
  (10,385 identities).

Saved verifiers: `work/dreamcast-recovery/verify-live-movie-task.py` and
`work/dreamcast-recovery/verify-live-movie-record.py`.

**Previous checkpoint — 9 October 2026: scheduler empty request and return verified.**

Two original resident functions, `8C65F8C8` and `8C65F4E8`, now execute in the
isolated recovery build (108 instructions; MOVIE entries remain 2,165). The first
4 KiB request finds zero available resource bytes and takes the original empty
return path. All 69 CPU fields, 16 MiB RAM and TMU state match the frozen
source-derived prediction across 498 cycles, ending at clock 795,843,555.
All 861 earlier captures are identical. The generated dispatcher preserves its
backward branch targets. No artificial payload or guest-state replay was added.

Continuation reaches missing original function `8C661490`, called by `8C6615A2`
at PC `8C6615C8`, PR `8C6615CC`, resource/task record `8C8A8C90`, stack
`8C896910`. Final clock is 795,843,682. This later 127-cycle discovery and the
prior longer polling/IRQ interval remain outside independent full-state proof.
Next: review `8C661490` against BOOT and captured RAM, identify the live branch
and callees, then predict and integrate only the needed original source closure.

Scheduler accounting confirms 56 AICA clock steps, no new mixer frame or IRQ,
and the recorded unchanged peripheral captures. Full ARM register-path and audio
quality remain open. One native run, now terminal; no booted Simulator. Saved
verification passed. Default emitter unchanged; experimental runtime unpromoted.

G1 remains unresolved: the 1,657 required source paths and supported PAL GDI are
still absent. Source-only request unchanged and unsent. No decoded movie, normal
boot, gameplay or device acceptance. Free space at verification: 19.11 GB.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-scheduler-final-verification.json` (passed).
- `20261009-original-startup-live-movie-scheduler/live-movie-scheduler-checkpoint.json`:
  `944f62d853fdd014322df379eaf09d55d320ed9aae5772da8c198c39dc7f2666`
  (9,409 identities).
- `20261009-live-movie-scheduler-return-prediction/prediction-checkpoint.json`:
  `5cf85f7bf4a13f6cae4eafca3598199d5a46d9b5198a43c977218829b72f9247`.
- `20261009-live-movie-scheduler-bodies/scheduler-bodies-checkpoint.json`:
  `b615be12f8482568c0f092f7172de3ba448add705cd5762638b4fbbea27dcb07`.

Verification: `work/dreamcast-recovery/verify-live-movie-scheduler.py`.

**Previous checkpoint — 9 October 2026: polling crossed; actual scheduler dependency reached.**

Original startup proceeds beyond the movie worker polling loop. Two original
state-2/readiness bodies add 86 instructions and 31 MOVIE entries (2,165 total).
The first complete state-2 repeat matches a frozen source-derived prediction:
all 69 CPU fields, 16 MiB RAM and TMU state, 5,962 cycles, ending at clock
792,963,072. The retained control matches too. All 821 earlier captures remain
identical. Observation of the second state-2 entry does not stop the guest.

Bounded discovery then reaches an actual new missing function. Progress captures
show 400 state-2 entries, unchanged 314 disc-sector reads and no worker yield flag.
A graphics interrupt is observed at clock 795,839,078, with interrupted PC
`8CED2164`; original scheduler callbacks subsequently reach missing `8C65F8C8`.
Final clock 795,843,057 is 2,885,947 cycles beyond the first state-2 entry.
Failure PC `8C65F92E`, PR `8C65F932`, stack `8C896918`, task record `8C8A7B90`.
Holly IRQ count rises 1,228→1,229. Neither the 10-million-cycle guard nor the
45-second native timeout is reached. One native invocation; no Simulator.

**Verification scope:** exact independent CPU/RAM/TMU and device checks cover
only the first 5,962-cycle repeat. Later polling, interrupt handling and scheduler
state are captured discovery evidence; their full state is not independently
verified. Do not use the discovery endpoint as proof of complete IRQ correctness,
normal boot or decoded media. First-repeat accounting verifies 673 AICA
instructions, sound timers and one mixer frame. Full ARM register-path and audio
quality remain open. First-repeat peripheral captures and relocation/alias counts
are unchanged. Default emitter unchanged; experimental runtime unpromoted.

The native run and captures completed, but the runner's final reporting code
accessed `.stderr` on a loop variable holding a Path. Preserve its failed report.
A separate saved-evidence verifier rechecks the model/control, every prefix and
device assertion, and the actual missing-target log. No native rerun was needed.
Use the final verification below as the scoped authority.

Next: source-derived scheduler dispatch `8C65F8C8` (10 instructions/four blocks).
Its backward branches require explicit original targets, not truncated fallthrough.
Live task byte0=1/byte1=1 selects missing state-1 body `8C65F4E8`; the other seven
records are inactive. Fresh source reviews match the dispatcher bytes in RAM.
State-1 resolves owner `8C8DA42C`, resource object `8C8AA8C8`, table `8C67FF74` and
existing methods `6621F8` (request), `662384` (return), `6622CA` (release).
Its first request size is 4,096 bytes. Review this body and its live branch/callee
closure, make a prediction from the captured scheduler context, then integrate
only missing original bodies. Preserve the open longer-interval IRQ audit.

G1 remains unresolved: all 1,657 required source paths and supported PAL GDI are
absent. Source-only request unchanged and unsent. No decoded movie, normal boot,
gameplay, audio-quality or device acceptance. Final audit: 20.46 GB free, no native
process or booted Simulator. Previous/current turns are progress; goal active.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-state-two-final-verification.json`: scoped exact repeat,
  retained control, prefix/device checks, actual scheduler failure, sources/pins.
- `20261009-original-startup-live-movie-state-two/live-movie-state-two-checkpoint.json`:
  `e464d00f9ec901ede55a653256177b85656c390706b7fc5b7828e878991ba439`
  (8,471 identities; original runner report remains failed).
- `20261009-live-movie-state-two-prediction/prediction-checkpoint.json`:
  `9662c7845ae38091ba34f979bb4028df525475921e595e56962a41c8d57f2b3c`
  (128 identities).
- `20261009-live-movie-state-two-control/control-checkpoint.json`:
  `8bdb96951629468dacb79705a243dad2ca6e94e832c4fa2a1200fdb5509fd72e`
  (129 identities).

Verification script: `work/dreamcast-recovery/verify-live-movie-state-two.py`.
Next source inventories:
`work/dreamcast-recovery/live-movie-scheduler-inventory.json`
(`26c84502fce6c1d9a128ca372dd90b24273acfbd46fd5c08879e39f77307bc9f`)
and `work/dreamcast-recovery/live-movie-scheduler-state-one-inventory.json`
(`be1bbc8bd7bd88f0c50405c36833596025c94e88384b5a51375571ca07e84ef4`).

**Previous checkpoint — 9 October 2026: original movie service pass verified.**

Original startup now executes the complete service pass from provider `8CED1C18`
through the packet/parser and later slot paths to missing state-2 handler
`8CED39D2`. All 69 CPU fields, 16 MiB of RAM and TMU state match a frozen,
source-derived 3,709-cycle prediction; the model also matches the retained
control. All 801 earlier captures are identical. Final clock: 792,957,110;
caller PC `8CED3962`, PR `8CED3966`, instance `8CEE2210`, stack `8CEE217C`.

Fifty-five original bodies add 2,522 instructions. Fifty-three MOVIE bodies add
465 entries (2,134 total); two resident helpers handle resource return and state.
The original code returns the slot-three resource reservation: remaining capacity
0x12000 and reserved bytes zero. Packet/movie payload remains empty. No fixture
state is replayed and no decoded movie or normal boot is claimed.

The first attempt stopped before guest execution because a new helper-availability
check preceded the helper's existing startup registration. A fresh observer moves
that check immediately after `resident_state_request_register` and its paired
registration. The corrected native run passes. Both attempts are retained;
no guest source, runtime semantics or frozen prediction changed. New manifest
arrays use local aliases to avoid collisions with earlier manifests.

Device accounting verifies 418 AICA instructions, one mixer frame and sound timer
updates; the nonzero-frame counter stays zero. Exception counters are unchanged,
with no new IRQ/fault. Full ARM register-path and audio-quality acceptance remain
open. AICA RAM/slots, flash, VRAM, GD/G2, MMIO, SQ/TA and PVR state are unchanged.
Scoped relocation/alias counts remain 1→1. Default emitter unchanged and runtime
candidate unpromoted. No Simulator; native processes are terminal. Final audit:
21.78 GB free. Previous/current turns are progress; goal remains active.

Next: state-2 handler `8CED39D2` and readiness helper `8CED3A2E`, two absent bodies,
86 instructions and 31 MOVIE entries. The retained 5,962-cycle repeat uses an
artificial second-entry stop: useful as a control, **not** a new missing function
or proof of startup advancement. Rebase it on live state, inspect the worker's
flag/timing/yield path and use bounded execution to find the next actual missing
startup dependency. Do not mistake an empty polling loop for decoded media.

G1 remains unresolved: all 1,657 required source paths and supported PAL GDI are
absent. Exact source-only request unchanged and unsent. No movie decoding, normal
boot, gameplay, audio-quality or device acceptance.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-service-pass-final-verification.json`: state/control,
  resource reservation, prefix, device accounting and source/pin/process checks.
- `20261009-original-startup-live-movie-service-pass-repair1/live-movie-service-pass-checkpoint.json`:
  `b6d82ae8be43a53a5f9d46749faca8c5c843eafdba17447aa084999316328141`
  (7,572 identities).
- `20261009-live-movie-service-pass-prediction/prediction-checkpoint.json`:
  `9ebeead68aec2c2f4aeb06acb01a74db1d518fd53cc1bf232d486d60da7dca8f`
  (83 identities).
- `20261009-live-movie-service-pass-control/control-checkpoint.json`:
  `1700e440d8f57290a1bd359e452bd820e4212ddd9991ddea31ee8895470c3a12`
  (84 identities).
- Failed preflight: `20261009-original-startup-live-movie-service-pass/live-movie-service-pass-checkpoint.json`,
  `7809a2af2b73ece3d362be202a6a2035707e6736b013a96100faa27e596a1490`.

Verification script: `work/dreamcast-recovery/verify-live-movie-service-pass.py`.
Next inventory: `work/dreamcast-recovery/live-movie-state-two-inventory.json`,
SHA256 `31c3b4582116377a22f44da62d16b58e5ee7a45375b9e06f6f66c3a630ce71b4`.

**Previous checkpoint — 9 October 2026: first original movie worker iteration completed.**

Original startup completes the first movie worker iteration and reaches the next
state-2 service pass. The original path copies metadata, requests 65,536 bytes,
receives zero available bytes, performs the empty copy/release/commit, returns
through the worker and dispatches service again. No movie payload is consumed.

All 69 CPU fields, 16 MiB of RAM and TMU state match the frozen source-derived
1,700-cycle prediction. The combined model also matches the retained control.
All 781 earlier captures are identical. Final clock: 792,953,401. Next actual
missing function: service provider `8CED1C18` (caller PC `8CEDA5CA`, PR `8CEDA5DA`,
stack `8CEE2160`). Twenty-two original bodies add 782 instructions; 17 MOVIE
bodies add 86 entries (1,583→1,669), and five bodies are resident functions.

The first offline model rejected the ring divisor because its inherited guard
allowed only the time-conversion divisor. A fresh repair permits exactly the two
reviewed positive divisors, 100 and 0x71000; retained and live predictions agree.
The first build failed before guest execution because two manifest array names
collided with earlier resource-reset names. A fresh observer uses scoped aliases
for those two arrays. Original generated sources/manifests, runtime semantics and
frozen predictions remain unchanged. The corrected build ran natively once.

Device checks account for 192 AICA instructions and one mixer frame, with the
nonzero-frame counter remaining zero. Sound timers, rational clock conversion,
instruction count and frame accumulator match; exception counters are unchanged,
with no new IRQ/fault. Full ARM register-path and audio-quality acceptance remain
open. AICA RAM/slots, flash, VRAM, GD/G2, MMIO, SQ/TA and PVR state are unchanged.
Scoped relocation/alias counts remain 1→1. Default emitter unchanged and runtime
candidate unpromoted. Native runs are terminal; no Simulator was used.

Next: original slot service provider/handlers beginning at `8CED1C18`. The next
inventory verifies six absent bodies, 137 instructions and 25 MOVIE entries.
Rebase its retained 143-cycle path onto the new live capture; inspect following
packet/state-2 dependencies for a useful combined test. Do not import retained
fixture state or stop on an artificially registered helper.

G1 remains unresolved: all 1,657 required source paths and the supported PAL GDI
are absent. Source-only request unchanged and unsent. No decoded movie, normal
boot, gameplay, audio-quality or device acceptance. Final audit found 23.11 GB
free and no native recovery process or booted Simulator. Previous/current turns
are progress; goal remains active.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-worker-iteration-final-verification.json`: exact state,
  retained control, preserved prefix, device accounting and source/pin/process checks.
- `20261009-original-startup-live-movie-worker-iteration-repair1/live-movie-worker-iteration-checkpoint.json`:
  `939fc293a8731fe23eff6ac84406c95eb321d8adc26def723ddafd62a092bff6`
  (6,597 identities).
- `20261009-live-movie-worker-iteration-prediction/prediction-checkpoint.json`:
  `c0f9736931d418d6e24e4e1ad0c2c109703145c17173387eeab197b3944383dd`
  (51 identities).
- `20261009-live-movie-worker-iteration-control-repair1/control-checkpoint.json`:
  `41d74ce2353d4258d26d21bfd3973ab1d49397c113e57835bebcf2ffaafef821`
  (52 identities).
- Compile-only failed attempt: `20261009-original-startup-live-movie-worker-iteration/live-movie-worker-iteration-checkpoint.json`,
  `d364cdb524fd3c5894e5a12516a744ea6e9b0ccab52f3c8ff494f6562e6b27d5`.

Verification script: `work/dreamcast-recovery/verify-live-movie-worker-iteration.py`.
Next inventory: `work/dreamcast-recovery/live-movie-service-slots-inventory.json`,
SHA256 `0168812d21b37dc787d968e9684df18f63ce37dfd736ba1428db4a99439f3f89`.

**Previous checkpoint — 9 October 2026: original movie worker processing reached.**

Original startup now executes the movie worker entry, its first service pass,
summary/time conversion and the start of instance processing. The instance at
`8CEE2210` changes from state 1 to state 2. All 69 CPU fields, all 16 MiB of RAM
and TMU state match a frozen, source-derived 567-cycle prediction. The same model
also matches the retained control. All 761 earlier capture files are identical.
Final clock: 792,951,701; next missing function: metadata query `8CED4508`.

Nine original bodies add 509 instructions: eight MOVIE bodies and one resident
time-conversion body. MOVIE registration grows from 1,459 to 1,583 entries.
Existing timer and integer-division helpers are verified registered before
startup. No fixture data is replayed; original code performs the state changes.

The first native attempt hit an obsolete observer assertion requiring worker
entry `8CECF9F2` to remain absent. Its evidence is preserved. A fresh observer and
runner move those guards to actual missing target `8CED4508`; the second attempt
passes. Runtime semantics and the frozen prediction are unchanged. An inventory
command initially hit the full-Xcode license gate; rerunning with the existing
Command Line Tools resolved that tooling invocation without changing the license.

AICA advances 64 instructions; rational clock conversion, sound timers and
instruction count match, with unchanged exception counters and no new IRQ/fault.
Full ARM register-path and audio-quality acceptance remain open. AICA RAM/slots,
flash, VRAM, GD/G2, MMIO, SQ/TA and PVR state are unchanged. Relocation/alias counts
remain 1→1. No Simulator was used; native invocations were sequential and are
terminal. Default emitter unchanged; experimental runtime unpromoted.

Next: integrate original metadata query/copy and its status/transfer helpers,
rebasing the retained prediction onto the new live capture. The verified next
inventory has four absent bodies and 21 MOVIE entries; check reused helpers'
actual registration before running. Consider batching the following buffer query
only after its live path is independently predicted.

G1 remains unresolved: all 1,657 required source paths and the supported PAL GDI
are still absent. Source-only request unchanged and unsent. No decoded movie,
normal boot, gameplay, audio-quality or device acceptance. Final audit found
24.36 GB free, no native recovery process and no booted Simulator. The previous
status-only turn was no progress; this turn advances the authoritative live state.
Goal remains active.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-worker-batch-final-verification.json`: CPU/RAM/TMU,
  retained control, preserved prefix, device accounting, source/pin/process checks.
- `20261009-original-startup-live-movie-worker-batch-repair1/live-movie-worker-batch-checkpoint.json`:
  `32b6e6c5929c1b60ac3b4ac63ff16b0bbdb6ab4e5f02860a8a0038dba5726dfa`
  (5,715 identities).
- `20261009-live-movie-worker-batch-prediction/prediction-checkpoint.json`:
  `aadb952b5183d1ffa21c3c3b8c5dbe129226d5bb86cbbbf0628d597db11a54a8`
  (25 identities).
- `20261009-live-movie-worker-batch-control/control-checkpoint.json`:
  `18960b25ae27e19a63913ebb6dfa2979f0b54c1db20a7cc0f961348785c665e1`
  (26 identities).
- Failed guard attempt: `20261009-original-startup-live-movie-worker-batch/live-movie-worker-batch-checkpoint.json`,
  `bd5bf07e2dc119ee54539b972fd8d3e063e4dca49102f52adcd6045f7158ed61`.

Executable handoff: `work/dreamcast-recovery/verify-live-movie-worker-batch.py`.
Next inventory: `work/dreamcast-recovery/live-movie-worker-metadata-inventory.json`,
SHA256 `4d6adb8880689596346b2cdda0974a3a24b4e7666f01ec2143054be339eb585e`.

**Previous checkpoint — 9 October: original movie worker context reached.**

Original startup now executes the movie frame callback and transfers into the
movie worker's saved context. The callback's 26-cycle prefix matches frozen
CPU/RAM/TMU predictions and preserves 721 earlier captures. The following
229-cycle entry/save/restore sequence also matches exactly, preserving 741
captures. Total new live progress: 255 cycles; final clock792,951,134.

Original code saves caller context8CEDE188 with stack8CFFFE0C, selects worker
context8CEDE190 and restores stack8CEE21B8. The next missing entry is8CECF9F2;
worker instructions have not executed. One MOVIE body adds35 instructions and
seven entries (1452→1459); three resident bodies add199 instructions. The existing
5FFA24 store helper is reused through `movie_slot_mark_register`.

Two verification issues were resolved without changing runtime semantics. The
callback runner incorrectly required AICA timers to remain static; saved-evidence
verification proves its three ARM instructions and timer updates exactly. The
first context attempt stopped before any guest instruction because a linked
helper was not registered. A fresh source/runner calls its existing registration
function, and the subsequent full startup run passes. Preserve the failed
artifacts and use the final verification reports below as acceptance authority.
Linked symbols alone do not prove runtime registration.

The context handoff advances AICA26 steps; rational clock conversion, sound timers
and instruction count match, with no faults or new IRQ. Full ARM register-path
and audio-quality acceptance are not claimed for that interval. AICA RAM/slots,
VRAM, SQ/TA packets, flash, MMIO, GD and G2 are unchanged. Scoped relocation/alias
counts remain1→1; the earlier vector relocation remains. All runs were sequential
with no Simulator. Default emitter unchanged; experimental runtime unpromoted.

Next: inspect original worker entry8CECF9F2 and step8CECF91C, compose a live
prediction from the restored context and add only missing original bodies.
Verified entry inventory: `work/dreamcast-recovery/live-movie-worker-entry-inventory.json`
(18 instructions, six MOVIE entries; live EDCF40 flag0). Check both linked source
and actual registration for helpers. No fixture data replay.

All1,657 source paths and supported PAL GDI remain absent. G1 unresolved, source
request unsent. No decoded movie, normal boot, gameplay, audio quality or device
acceptance. Final verification found25.68GB free, no native recovery process or
booted Simulator. Previous/current turns are progress; goal remains active.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-frame-callback-final-verification.json` and
  `20261009-live-movie-context-final-verification.json`: exact live comparisons,
  retained controls, device accounting, source absence, pins and process checks.
- `20261009-original-startup-live-frame-callback/live-frame-callback-checkpoint.json`:
  `a335b1250afb2b7aba807a1ed67eee5231b6f88807663b86fc50fadd1080424e` (4,081 identities;
  runner's static-AICA assertion failed, resolved by the saved verification).
- `20261009-original-startup-live-movie-context-repair1/live-movie-context-checkpoint.json`:
  `ffceae3cc7b6d402ee3bf56e164b14abdbbaa7adfe651e54121fa4766dc16f49` (4,879 identities).
- `20261009-live-movie-context-prediction/prediction-checkpoint.json`:
  `891cb6ca2869e36c000b159012152c715349564c15562adeea58de58b1b8a9b1` (22 identities).
- `20261009-live-movie-context-retained-control/retained-control-checkpoint.json`:
  `0784a84ad882d727ac6f4debf9dee53db5b2288f181b88e305febf53fd109731` (23 identities).

Next inventory SHA256: `6649a1fb8233da98341c7cb2fcc96d337391370cbb3124405870a6f2e19879d7`.

**Previous checkpoint — 9 October: live graphics prefix and interrupts verified.**

The live graphics prefix now passes a retrospective, source-derived interrupt
model. All 69 CPU fields, all 16 MiB of RAM, 8 MiB of VRAM, TMU state, store queues
and four graphics packets with source provenance match exactly. The model runs
2,304 cycles after the 182-cycle method return (2,486 total). It derives the two
interrupts from generated packet completion events and the live IRQ masks.

The extra 1,345 cycles are fully accounted for: handler spans 397+916 and repeated
pre-interrupt block charges 23+9. Every one of the former 149 RAM differences is
attributed to an original instruction write; no memory ranges are excluded.
The old runner's `frame_acceptance=false` remains immutable; the new scoped
verification supersedes that unresolved frame-prefix comparison, not game acceptance.

The original capture omitted the PVR current parameter pointer. A fresh observer
adds that field and related PVR/DMAC state to separate files. After one compile
repair (`dmac_dmatcr2` field spelling), one native replay reproduced all 705 old
capture files byte-for-byte. Model repairs handled an empty diagnostic string and
the previously uncaptured PVR register. Failed attempts remain preserved. No guest
source or runtime behavior changed; no Simulator was launched.

Next: integrate original callback `8CECFA68`, one body/35 instructions/seven
MOVIE entries. Verified inventory `work/dreamcast-recovery/live-frame-callback-inventory.json`
finds no linked symbol overlap; timer helper60634E exists, service5FFAE0 does not.
Live EDCFB0 flag is0. Rebase the retained 26-cycle callback prefix on the current
capture, freeze CPU/RAM/TMU predictions, then add only original object/entries at
normal MOVIE activation. Do not replay the retained fixture's module data.

G1 remains unresolved: all 1,657 missing source paths and the exact PAL GDI are
still absent; source request unsent. Default emitter and pinned references match;
experimental runtime remains unpromoted. No decoded movie, normal boot, gameplay,
audio-quality or physical-device acceptance. Final audit found 28.13 GB free and
no native recovery process or booted Simulator. Previous/current turns are progress;
goal remains active.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-frame-irq-final-verification.json`: exact comparisons, IRQ timings,
  all 149 byte-write attributions, pins, G1 absence and process/storage checks.
- `20261009-original-startup-live-stream-ready-irq-capture-repair1/live-stream-ready-irq-capture-checkpoint.json`:
  `a36135a4156215e09a4f9013c67608ff756d37c592ca349e30fca08a0ff55408` (3,310 identities).
- `20261009-live-movie-frame-irq-model-repair2/movie-frame-irq-model-checkpoint.json`:
  `e476e2525d4c84205939e632cf000ac35fd48e112f05813c8bcfe8d413a5a38a` (41 identities).

Next callback inventory SHA256: `4ec4c783713d0e9bcea09d47f961716495ad456aec5b3229c3b689d504a0efd1`.

**Previous checkpoint — 9 October: stream-ready return verified; interrupt audit open.**

Original startup now completes the stream position/start and returns from the
movie resource method to `8CEB0254`. The 182-cycle return matches the frozen
CPU/RAM/TMU prediction exactly; all 627 earlier capture files remain identical.
Ten original bodies add 119 instructions, with MOVIE activation 1429→1452.

The following graphics path emits all four expected packets with matching source
PCs and store queues, then reaches missing callback `8CECFA68` at
PC `8C645D98` / PR `8C645DA0`. It takes 2,486 cycles in total, including two
hardware interrupts absent from the 959-cycle uninterrupted frame reference.
**Frame acceptance remains false:** 149 RAM bytes, saved interrupt registers and
TMU timing differ. The two observed interrupt spans account for 1,313 of the
1,345 extra cycles; the remaining 32 cycles and every RAM difference need a
source-derived explanation. Matching graphics packets alone does not clear this.

Next: audit the interrupt handlers against the saved captures before integrating
the movie callback. No additional native run is needed merely to inspect this
evidence. No decoded movie, normal boot, gameplay, audio-quality or device proof.
G1 still lacks all 1,657 inventoried source paths and the supported PAL GDI; the
source-only acquisition request remains unsent. Default emitter unchanged;
experimental runtime remains unpromoted. Saved audit found 29.29 GB free and no
native recovery process or booted Simulator. Goal remains active.

Evidence: `artifacts/recovery-aot/20261009-live-stream-ready-saved-audit.json`;
`artifacts/recovery-aot/20261009-original-startup-live-stream-ready/live-stream-ready-checkpoint.json`
SHA256 `4dd57c72f00f24498d81c10c131a9fc8de1845a7cf7ea19dd1c536a812e24867`
(3,272 identities). The checkpoint's `passed` records the method return and bounded
discovery; consult the report's explicit `frame_acceptance=false` for frame status.

**Previous checkpoint — 9 October:** Original startup now finds `SLH448_1.SFD` in its loaded directory (index1738),
creates file descriptor8C6994F4 for3,119,104 bytes, and allocates stream8C8A8010.
Resident65FB12 returns ECF37A after16,845 live cycles. Another15 cycles publish
the stream at outerEDD114+52 and prepare position226. Full CPU/RAM/TMU matches
the return and endpoint; all589 earlier captures remain identical. Final clock
792,948,393; next missingED0960, PCECF3C4/PRECF3CE, r4=8C8A8010/r5=E2.

The preceding live159-cycle batch completed original resource reset, inactive
stream/object checks and IRQ-mask restoration. It preserved570 earlier captures
and reached resident65FB12 with filenameEB8B84/buffer8AA8F8. Both startup runs
and both composed models passed without native repair, run sequentially with no
Simulator. Resource preparation added seven bodies/264 instructions and68 MOVIE
entries (1361→1429); file/stream creation added three resident bodies/148
instructions. Existing provider, filename-copy and allocator helpers were reused.

No fixture data replay. Scoped relocation/alias counts remain1→1; earlier vector
relocation remains. AICA advances18 steps in preparation, then1,903 steps/four
mixer frames in file creation, without faults. IRQ count stays1,226 and critical
nesting0; AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 unchanged. Default emitter unchanged;
FPU/audio candidate unpromoted.

This verifies directory lookup, descriptor and stream creation. Movie contents
have not been read or decoded; no normal boot, gameplay, audio quality or device
acceptance. All1,657 source paths and supported PAL GDI remain absent; G1 remains
unresolved and source request unsent.30.49GB free at final verification; no native
process or booted Simulator.

Next original stream position/start and resource-method return: verified inventory
`work/dreamcast-recovery/live-stream-ready-inventory.json`. Extract182 remaining
cycles from the retained197-cycle model, excluding the15-cycle prefix already
executed. After returnEB0254, the original six-cycle caller reaches already-linked
608DF0. Both live timing flags893A58/893A50 are0. Trace this helper/provider path
and find the actual next missing boundary before execution; do not intercept a
registered helper as missing. Let original code write ECF040 without data replay.
Previous/current goal turns are progress; goal remains active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** Original indexed queries now return slot7 handle8C8A8C90 and slot3 handle8C6774F0.
The outer setup returns EDD114 to EB0174 after181 live cycles, clears its16-word
work table, and original entry code publishes the handle at ECEFE8. Another165
cycles select `SLH448_1.SFD` at8CEB8B84. Full CPU/RAM/TMU matches both return and
endpoint; all532 earlier capture files remain identical. Final clock792,931,374;
next missingECF35E, PCEB023C/PREB0254, r4=EDD114/r5=EB8B84.

Four original bodies add84 instructions and13 entries at normal MOVIE load,
activation1348→1361. No fixture data replay. The outer return observer uses
actualECF0F8 entry, verified from caller literalEB01F4 and original callEB0170.
Scoped relocation/alias counts remain1→1; the earlier vector relocation remains.
AICA advances39 steps without faults; IRQ count1,226 and critical nesting0.
AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 stay unchanged.

Default emitter unchanged; FPU/audio candidate unpromoted. File selection does
not prove file opening or playback. No decoded movie, normal boot, audio quality,
gameplay or device acceptance. All1,657 source paths and supported PAL GDI remain
absent; G1 unresolved and source request unsent.32.67GB free at final verification;
no native process or booted Simulator.

Next integrate original resource preparation/open wrapper, candidate159 cycles to
resident65FB12. Verified sources and symbol audit:
`work/dreamcast-recovery/live-resource-open-inventory.json`. Seven objects have
no linked overlaps; bothECF35E and65FB12 are absent. Rebase the retained model on
live arguments/slots and verify inactive drain/stream/object branches, IRQ mask
restoration and original resource initialization before one startup run. A retained
17,201-cycle method fixture is not live hardware or playback proof.

The preceding user status turn was no progress; this turn completes a verified
live integration. Goal remains active with an executable next step.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** Original output setup now returns to ECF1F6 in live startup after all eight
settings, scalar FPU and buffer checks. Full CPU/RAM/TMU matches846cycles to
return, then189 through wrapper, existing allocator/initializer and descriptor
setup. All494 earlier capture files stay identical. Pool slot4 becomes8C8AA8F8
with buffer8C983000/size71000; next missingED45C8 at PCECF21E/PRECF258 through r3.
Final clock792,931,028. Four new bodies add164 instructions; existing allocators
are reused, with no artificial stop at their registered entries.

Independent rational arithmetic checks live inputs2/2/1000/0.5: FLOAT/FADD values
are exactly representable, FTRC truncates4000.5 to4000, and the non-decrement branch
is taken. Actual return FR2/3/4=3F000000/457A0000/457A0800, FPUL/MACL=4000. This
exact-input check is not general FPU/vector/exception correctness. All eight mode
writes are verified.32 original entries extend MOVIE activation1316→1348 at load;
no fixture data replay. Scoped relocation/alias counts remain1→1 (earlier vector
relocation remains). AICA advances117 steps without faults; IRQ count1,226,
critical nesting0, AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 unchanged.

Default emitter unchanged; FPU/audio candidate unpromoted. No decoded movie,
normal boot, audio quality, gameplay or device acceptance. All1,657 source paths
and PAL GDI remain absent; G1 unresolved, request unsent.33.81GB free; no native
process or booted Simulator. Next indexed queries, outer return and movie-entry
resource selection: `work/dreamcast-recovery/live-outer-entry-inventory.json`.
Rebase the181-cycle remainder after the already executed71-cycle field prefix,
then the retained165-cycle entry path. Audit linked coverage/real callback entry,
verify live handles/slots, and freeze predictions to the actual next boundary.
Let original code publish the outer handle; do not replay module data words.
Previous/current goal turns are progress; goal remains active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** Creation now binds callbackED07FC/argument0 and returns handle8CEE2210 to ECF1B0
in live startup. The outer routine publishes the instance at EDD144 and performs
original stream-size computation, including original integer division. Full
CPU/RAM/TMU matches44cycles to creation return plus158 to missing output setup
ED0620. All456 earlier capture files stay identical. Final clock792,929,993;
PCECF1E4/PRECF1F6, r1=ED0620. One new body adds13 instructions.

Four original validity entries extend MOVIE activation1312→1316 at its original
load. No fixture data replay. The creation observer uses actual03F8 entry verified
from live literalECF2B8. Relocation/alias counts remain1→1 (the earlier vector
relocation remains). AICA advances23 steps without faults; IRQ count1,226 and
critical nesting0; AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 unchanged. Default emitter
unchanged; FPU/audio candidate unpromoted. No decoded movie, normal boot, audio
quality, gameplay or device acceptance. All1,657 source paths/PAL GDI remain
absent; G1 unresolved, source request unsent.34.94GB free; no native process or
booted Simulator.

Next integrate original output setupED0620. Retained candidate846cycles to
ECF1F6 includes eight settings, scalar FPU and buffer checks. Verified inventory:
`work/dreamcast-recovery/live-output-setup-inventory.json`. Audit linked coverage,
rebase live input and independently check scalar FPU bits. Predict the caller
past its12-cycle wrapper and already registered allocator662106 using actual
pool occupancy; do not treat that allocator as missing. Freeze return/endpoint
predictions before integration. Previous/current turns are progress; goal active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** The original MOVIE instance constructor now returns handle8CEE2210 to caller
ED05C4 in live startup. Remaining dispatcher slots6/7, initializer and constructor
tails match82 cycles; original callback-binding preparation matches20 more.
Full CPU/RAM/TMU matches the actual return and endpoint; all418 earlier captures
remain identical. Two new bodies add10 original instructions. Final clock
792,929,791. Next missingED1BAA is direct BSR opcodeB04A at ED1B12, PRED1B16;
r4=8CEE2210, r5=ED07FC, r6=0. Callback validity/binding completion remains open.

Three original slot entries extend MOVIE activation1309→1312 at its normal load;
no fixture data replay. Constructor observer uses actual3DC2 entry verified from
live call literalED06E0. Scoped relocation/alias counts stay1→1; earlier vector
relocation remains. AICA advances11 steps without faults; IRQ count1,226,
critical nesting0, AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 unchanged. Default emitter
unchanged; FPU/audio candidate unpromoted. No decoded movie, normal boot, audio
quality, gameplay or device acceptance. All1,657 source paths and PAL GDI absent;
G1 unresolved, request unsent.35.91GB free; no native process or booted Simulator.

Next integrate ED1BAA, remaining creation tail and outer stream-size path. Verified
inventory: `work/dreamcast-recovery/live-create-stream-return-inventory.json`.
Retained candidate44cycles to ECF1B0 plus158 to missingED0620 must be rebased and
its actual linked coverage audited. Extract the44-cycle remainder after the
already executed20-cycle binding prefix; preserve live module data/stack. Freeze
return/endpoint predictions before integration. Previous/current turns are
progress; goal active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** The MOVIE setup branch now returns to its dispatcher in live startup. Five
bodies/132 instructions match133 cycles to EDA5DA, then the existing dispatcher
matches39 cycles through empty slots4/5 to missing slot6 handlerED511A. Full
CPU/RAM/TMU matches both return and endpoint; all380 earlier capture files remain
identical. Mode15/value2 and requested object state1 are verified. Final clock
792,929,689; PCEDA5CA/PREDA5DA, r3=ED511A, r4=8CEE2210, slot index6.

Seventeen original mode entries extend load-time MOVIE activation1292→1309,
with no fixture data replay. Relocation/alias counts stay1→1; the known earlier
interrupt-vector relocation remains. AICA advances20 steps without faults; IRQ
count stays1,226 and critical nesting0. AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 unchanged.
Default emitter unchanged; FPU/audio candidate unpromoted. No decoded movie,
normal boot, audio quality, gameplay or device acceptance. All1,657 source paths
and supported PAL GDI remain absent; G1 unresolved, source request unsent.
36.92GB free; no native process or booted Simulator.

Next integrate the remaining dispatcher/instance return fromED511A, rebasing
retained slots6/7 and saved frames. Verified inventory includes the creation
callback tail: `work/dreamcast-recovery/live-instance-return-inventory.json`.
Audit existing bodies/module entries and predict through the caller to the next
actual missing target before integration. Preserve active module data and live
slots. Previous/current goal turns are progress; goal remains active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** Handler setup now returns to MOVIE in live startup, then the original state update
runs. Three bodies/36 original instructions match97 modeled cycles:75 to ED13E4,
then22 to missing648548 at PCED1666/PRED13EA through r3. Full CPU/RAM/TMU matches
both the return capture and endpoint. All342 earlier capture files stay identical.
Channel8C8A7B90 is enabled and binds handler8C8AA8C8; outer flags1/3 and MOVIE
state1 are verified. IRQ mask restores15→0, critical count1→0, IRQ count remains
1,226. Final clock792,929,517. No decoded movie or gameplay yet.

Five original MOVIE status entries extend actual load-time activation1287→1292;
no fixture data-word replay. Scoped relocation/alias counts remain1→1 (the earlier
interrupt-vector relocation remains). AICA advances11 steps without faults; AICA
RAM/SQ/flash/VRAM/MMIO/GD/G2 stay unchanged. Default emitter remains unchanged,
FPU/audio candidate unpromoted. Original G1 still lacks1,657 source paths and PAL
GDI; request unsent.38.66GB free; no native process or booted Simulator.

Next batch resident state request/query and MOVIE mode setup from648548 to EDA5DA;
retained candidate133cycles must be rebased on live inputs. Verified inventory:
`work/dreamcast-recovery/live-mode-return-inventory.json`. Audit linked bodies and
active module entries, model return plus the subsequent dispatcher boundary, then
freeze predictions before integration. Preserve actual MOVIE data/occupied slots.
Previous/current goal turns are progress; goal remains active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** The original idle reset now completes in live startup: record status reset, buffer
clear, both child initializers, critical exit and caller re-entry. Two batches
(88+304cycles) add nine bodies/268 original instructions. Full CPU/RAM/TMU matches
frozen source predictions;304 then323 earlier capture files stay identical.
Final clock792,929,420. Next missing65F4A4 at PC64817E/PR648186 through r3, with
channel8C8A7B90 and handler8C8AA8C8. No decoded movie or gameplay yet.

Scoped relocation counters and alias counts remain1→1 in both segments; whole
startup retains the known earlier interrupt-vector relocation. AICA advances44
steps and one mixer frame without faults, IRQ count stays1,226, and AICA RAM/SQ/
flash/VRAM/MMIO/GD/G2 are unchanged. The reset release/re-entry matches the source
model, ending at critical nesting1; pending-interrupt coverage remains open.

Next integrate handler setter, channel enable/setup return and MOVIE state update
from65F4A4, candidate97cycles to648548. Retained input inventory:
`work/dreamcast-recovery/live-setup-return-inventory.json`. Audit already-linked
bodies and current MOVIE registration, rebase live inputs and freeze predictions.
Do not replay old fixture activation words. Default emitter unchanged; FPU/audio
candidate unpromoted. All1,657 missing source paths/PAL GDI still absent; G1
unresolved, request unsent.39.52GB free; no native process or booted Simulator.
Previous/current goal turns are progress; goal remains active.

[STATUS.md](STATUS.md) records hashes and limits.

**Previous checkpoint — 9 October:** Original MOVIE setters, handler allocation/binding and reset entry now match the
frozen185-cycle model in live startup: full CPU/RAM/TMU, with285 earlier capture
files identical. Four new bodies/112 instructions reuse two existing allocator
bodies (six reached bodies/170 instructions). Next missing65F238 at PC6481BA,
PR6481C2 through r2; clock792,929,028, critical nesting1. AICA advances21 steps,
no faults; IRQ count remains1,226. No decoded movie or gameplay yet.

The first link failed on duplicate allocator bodies; repair reuses the matching
already-registered objects. The next run reached the expected endpoint but its
whole-startup zero-relocation assertion was wrong: both parent and current logs
retain one early interrupt-vector relocation8C000600→8C662518. Saved-capture
verification passes without another native run. No new template match occurred;
the counter does not count cached-alias use. Earlier unqualified “no template
fallback” wording must be read as segment-specific, not whole-startup proof.

Next bounded step: source-derived first reset path65F238→65F23E, candidate88cycles.
Verified retained input inventory: `work/dreamcast-recovery/live-first-reset-inventory.json`.
Audit existing symbols/registrations, rebase live state and freeze predictions
before integration. Default emitter unchanged; FPU/audio candidate unpromoted.
All1,657 missing source paths and PAL GDI remain absent; G1 unresolved, request
unsent.41.26GB free; no native process or booted Simulator. Goal remains active.

[STATUS.md](STATUS.md) records evidence and limits.

**Previous checkpoint — 9 October:** Both buffer queries and the original resident constructor now return in live
startup. Handle8C6774F0 returns to MOVIE ED144C; full CPU/RAM/TMU matches444cycles.
IRQ mask restores15→0 with no pending Holly event or unexpected interrupt; count
remains1,226. All247 earlier capture files are identical. The next8 MOVIE cycles
also match rebased CPU/RAM/TMU expectations, reaching648342 at PCED145C/PRED1464
through r3. Final clock792,928,843. No state reset or template fallback; ARM scheduling
remains enabled. Next batch the two setters, MOVIE continuation, allocator and
binding/reset prefix from648342. Verified six-body/170-instruction inventory:
`work/dreamcast-recovery/live-movie-handler-inventory.json`; earlier185-cycle path
to65F238 must be rebased on live occupancy and IRQ state. Preserve live MOVIE
activation; do not replay old fixture data words. Default unchanged; FPU/audio
candidate unpromoted. All1,657 source paths/PAL GDI absent; G1 unresolved, request
unsent.41.78GB free; no native process or booted Simulator. No decoded movie, normal
boot, audio-quality or gameplay proof. [STATUS.md](STATUS.md) has hashes and limits.
Goal active; previous/current turns are progress.

**Previous checkpoint — 9 October:** Five retained stereo bodies (166 original instructions) now run in live startup.
Both pan callbacks and the stereo return to660A18 match the source model; full
CPU/RAM/TMU at the next helper matches1,508cycles. All209 earlier capture files are
identical. Four command38 packets advance queue4→8/index44→48; original pan inputs
−15/+15 yield−127/+127. AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 unchanged; ARM scheduling
advances170 steps without faults. Clock792,928,391; next66C37C at PC660A20/PR660A2E,
r11=66C37C. Next integrate both buffer queries and resident return to MOVIE, with
live IRQ-mask restoration and saved frame verified. Inventory:
`work/dreamcast-recovery/live-resident-return-inventory.json` (two new bodies,
166instructions). Earlier444-cycle partial replay is not live return proof.
An ambiguous extraction marker failed before compilation; corrected to a unique
source prefix without changing recovered code. Model/native runs then passed.
Default unchanged; FPU/audio candidate unpromoted. All1,657 source paths/PAL GDI
absent; G1 unresolved, request unsent.42.49GB free; no native process/booted Simulator.
No decoded movie, normal boot, audio-quality or gameplay proof. [STATUS.md](STATUS.md)
has hashes and limits. Goal active; previous/current turns are progress.

**Previous checkpoint — 9 October:** Four retained bodies (88 original instructions) now complete both activation-command
submissions, service constructor return and outer record initialization in live
startup. Full CPU/RAM/TMU matches the frozen471-cycle model; all171 earlier capture
files are identical. Command46 queue count2→4, index42→44. Service66B91C returns
handle8C8DA98C to6609F4, and outer object8C8A8C90 retains it. AICA RAM/SQ/flash/VRAM/
MMIO/GD/G2 unchanged; ARM scheduling advances53 steps without faults. Final clock
792,926,883; next missing6618D6 is a direct BSR at660A14 (opcodeB75F), PC660A10/PR660A18.
Next batch retained stereo setup from6618D6 through66C37C, rebasing source models
on live state. Verified five-body/166-instruction object/model inventory:
`work/dreamcast-recovery/live-stereo-inventory.json`. Earlier1,508-cycle partial
replay had inactive devices; it is not live proof. Default unchanged; FPU/audio
candidate unpromoted. All1,657 source paths/PAL GDI absent; G1 unresolved, request
unsent.43.17GB free; no native process or booted Simulator. No decoded movie, normal
boot, audio-quality or gameplay proof. [STATUS.md](STATUS.md) has hashes and limits.
Goal active; previous/current turns are progress.

**Previous checkpoint — 9 October:** Four retained channel bodies (457 original instructions) now execute in live
startup. The channel-pair constructor returns to66BBB8 and the caller reaches
missing66D184 at PC66BC14/PR66BC1A through r9. Full CPU/RAM/TMU matches the frozen
910-cycle source model; all152 earlier capture files are identical. Both command38
packets are queued (count0→2, index40→42). AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 remain
unchanged while ARM scheduling advances103 steps without faults. Final clock
792,926,412. Queue insertion is not firmware consumption or audio-quality proof.
Next batch retained66D184/66EE7C/66D190/6608BC to service return and record init,
with predictions rebased on live state. Verified object/model inventory is
`work/dreamcast-recovery/live-service-return-inventory.json`. Earlier471-cycle
partial replay had inactive devices and must not substitute for live evidence.
Default unchanged; FPU/audio candidate unpromoted. All1,657 source paths/PAL GDI
absent; G1 unresolved, request unsent.43.73GB free; no native process or booted
Simulator. No decoded movie, normal boot or gameplay. [STATUS.md](STATUS.md)
has hashes and limitations. Goal active; previous/current turns are progress.

**Previous checkpoint — 9 October:** The saved live second-buffer and four-allocation segment now matches the retained
source-derived model for full CPU, main RAM, AICA RAM and TMU over 2,185 cycles.
The same model also passes the sealed earlier replay. No guest startup rerun was
needed; endpoint remains missing 66CFC4 at PC66BBA0/PR66BBB8, clock 792,925,502.
The initial model driver failed compilation because nested includes redefined main;
a fresh driver uses the exact sealed model prefix, with the failed attempt preserved.
Next integrate the retained channel-pair/count/callback/field bodies as a batch,
rebasing expectations on live state and keeping ARM/device scheduling enabled.
Default unchanged; FPU/audio candidate unpromoted. All 1,657 missing source paths
and PAL GDI remain absent; G1 unresolved, source-only request unsent. 44.31 GB free;
no native process or booted Simulator. No decoded movie, normal boot or gameplay.
[STATUS.md](STATUS.md) records proof and limits. Goal active; verified progress.

**Previous checkpoint — 9 October:** original 65E470/66AF40 buffer return now
passes in live startup with ARM scheduling enabled. Full CPU/RAM/TMU/AICA matches
the frozen 1,331-cycle model, including 113 main-RAM stores and two sound words.
All 114 earlier capture files are unchanged; no template fallback or AICA fault.
A premature dependency assertion failed before startup and was corrected in fresh
source after confirming the original registration table; failed evidence preserved.
Startup additionally reaches 66CFC4 at PC66BBA0/PR66BBB8, final clock 792,925,502.
Next verify the captured later 2,185-cycle second-buffer/allocation segment against
a model rebased on live state, then batch retained 66CFC4 dependencies. That later
CPU/RAM transition is currently observed, not fully verified; no rerun needed to
audit existing captures. [STATUS.md](STATUS.md) has hashes and exact scope.
Default unchanged; audio/FPU candidate isolated. All 1,657 source paths/PAL GDI
absent; G1 unresolved, acquisition request unsent. 44.39 GB free; no native instance
or booted Simulator. No decoded movie/normal boot/gameplay. Goal active; progress.

**Previous checkpoint — 9 October:** seven retained constructor/resource bodies
now execute their reached path in live startup. All CPU fields, 31 RAM stores and
TMU match a prospective integer model over 307 cycles; the model also matches the
sealed earlier replay. All 95 previous live capture files are unchanged. Resources
8C8EE064/8C8EE06C have IDs 15/14; actual missing target is 65E470 at PC66B9EE,
PR66BA16 with original SPRS arguments. Clock 792,921,986; original IRQ restoration
still ahead. Default unchanged; audio/FPU candidate unpromoted. Next integrate
retained 65E470/66AF40 and sound-word writes with predictions rebased on live RAM,
stack and AICA state. Preserve original device/ARM scheduling and five historical
G2 DMA runs; do not inherit the old empty-hardware fixture. [STATUS.md](STATUS.md)
has exact checkpoints and limitations. 2,086 live and 535 replay identities pass.
All 1,657 source paths/PAL GDI absent; G1 unresolved, request unsent. 44.83 GB free;
no native instance or booted Simulator. No decoded movie/normal boot/gameplay.
Goal active; latest turn is progress.

**Previous checkpoint — 9 October:** retained math and channel bodies now run
in original live startup. The complete 65EB00 return matches a frozen rational
CPU/RAM/TMU model over 284 cycles with outputs 1CA6/F32D. Retained 66B3CC/65F346 then
complete channel initialization and return to the actual saved PR 647F74, matching
an independent 75-cycle integer model. Earlier 38/57 prefix capture files remain
identical. A preflight using the partial-replay PR647F18 failed before native work;
its record is preserved and a fresh source-checked live-frame prediction passes.
Final real missing target is 65F214 (PC647F7A, PR647F84, r2), at 792,921,679 cycles.
Next integrate retained constructor dependencies from 65F214 to 66093E in a batch,
using this live state and fresh predictions. [STATUS.md](STATUS.md) has exact hashes.
4,040 sealed identities verified; audio/FPU candidate remains isolated, default
unchanged. Broader vector/FPU/geometry gates remain open. All 1,657 source paths and
supported PAL GDI absent; G1 unresolved, request unsent. 45.12 GB free; no native
instance or booted Simulator. No decoded movie/normal boot/gameplay. Goal active;
latest turn is progress.

**Previous checkpoint — 9 October:** isolated audio-domain candidate fixes
host gain/status leakage while preserving guest arithmetic. Negative controls show
32 gain differences and 64 each register/mixer status leaks; all are zero after
scoping AICA register writes and mixing. 64 audio controls (16 volumes, four frames,
normal/unwind), existing dispatch controls and renderer cases pass. No PCM difference
was observed in the fixture, so this is not an audible-quality claim. Live startup
now captures gain exactly 1; every other captured file is unchanged from the prior
candidate. Scalar CPU/RAM/TMU prediction still passes to missing 11152C. General
FPU/vector/geometry and earlier guest RAM/MMIO differences remain unvalidated.
Candidate and patch 0014 remain isolated; default unchanged. Next integrate retained
11152C/1118DC/111E38 into the same live path, using the independent full-rate model
rebased on live entry; the prior 24-control partial replay is not live proof.
[STATUS.md](STATUS.md) records exact inputs, hashes and limits. All 1,657 source paths
and supported PAL GDI absent; G1 unresolved, source request unsent. 45.58 GB free;
no native instance or booted Simulator. No decoded movie/normal boot/gameplay.
Goal active; latest turn is progress.

**Previous checkpoint — 9 October:** live startup FPU A/B completed. Default
reproduces the nearest-rounding defect; isolated candidate matches the independent
toward-zero product/ratio and full CPU/RAM/TMU transition over 25 cycles of original
65EB00. Both stop at missing 11152C. Candidate remains unpromoted. Earlier startup
A/B differs in four XF registers, 143 RAM bytes, one SQ byte, 28 MMIO values and
printed AICA master gain. Next audit host floating-environment ownership and these
changes before promotion, then continue the isolated live path using retained
11152C. General vector/geometry gates remain open. Candidate uses strict cached
objects/archive; 19 integer units retain their prior verified objects. Final
verification checks 1,731 default and 1,961 candidate identities. See [STATUS.md](STATUS.md)
for checkpoint paths and limitations. All 1,657 source paths/PAL GDI still absent;
G1 unresolved, request unsent. 45.70 GB free; no native instance or booted Simulator.
No decoded movie, normal boot or gameplay. Goal active.

**Previous checkpoint — 9 October:** retained integer dependency batch now runs
in the original startup runtime. Nineteen retained units add21 bodies/961 instructions,
bringing this experimental closure to1079 bodies and1287 MOVIE entries. All70 new
module addresses are distinct; seven resident registration functions are reused.
Original live clocks/IRQ/drive state preserved. FromED8536 to missing65EB00 adds
2,820 cycles, ending792,921,210. Full CPU/RAM/TMU/SQ matches the retained rate-pool
prediction. Prefix CPU/TMU/RAM/AICA-RAM/flash matches prior full-startup capture
except bound-dispatchPC; entry SQ was not captured for the new label. Final SQ is
verified exactly. Runner postcheck failed on absent entry SQ file; separate final
verifier passes all available authoritative evidence without a native rerun.
AICA advances318 scheduled steps and one mixer frame; no audio/playability claim.
AICA RAM,flash,VRAM,MMIO andGD state unchanged across this batch; IRQ count1226.
Next actual missing65EB00 has captured FPSCR40001, inputs500/44100 and output
pointers894930/894932. Its ARM64 rounding defect and isolated candidate are already
reproduced. Next compare bounded original65EB00 arithmetic in this live prefix
against the existing rational predictions, keeping candidate/runtime identities
explicit and stopping at original missing11152C. Do not label default arithmetic
correct or silently promote0013. Keep general FTRV/FIPR and geometry gates open.
Fresh source board-join-integer-integration-20261009.cpp; runner
run-original-startup-integer-integration.py. Frozen inventory/CP paths inSTATUS.
All1657 source paths/PAL GDI absent; G1 unresolved.46.02GB free; no native process/
Simulator. SunPad/default source unchanged; no decoded movie/normal boot/gameplay.
Goal active. [STATUS.md](STATUS.md) records hashes and limitations.

**Previous checkpoint — 9 October:** original startup hardware capture verified.
A fresh observer reuses the unchanged default full-startup closure and stops at the
same missingED8536, PCED2F7C/PR ED2F82/r2ED8536. All six prior CPU/RAM/TMU,
AICA-RAM,flash and SQ captures are byte-identical. New evidence records145 hardware
fields,1,940 MMIO words,two disc tracks,GD state,AICA ARM/timers/slots,G2,VRAM andTA.
Guest clock is enabled, host sync off,792,918,390 cycles and1,226 IRQs taken;
314 sectors/643,072 bytes read. This is the earlier original-startup boundary,
not the later isolatedEB0254 context and not gameplay/media playback proof.
Correction: older state.txt files already held Holly masks/pending and partial
AICA/G2/timer fields. Prior filename-only inventory understated available evidence.
The latest isolated replay still does not restore these values; zero masks there
are a fixture artifact. Preserve the full live runtime instead of guessing a restore.
Runner's post-execution target-string assertion failed because the exception is
generic; separate verifier confirms actual PC/PR/r2 and byte-identical captures.
No native rerun or game/runtime change. Checkpoint and failed report preserved.
Next integrate retained integer MOVIE dependencies beginning ED8536 into a fresh
full-startup observer/link, using exact existing manifests and checkpoints. Its
buffer-acquire object is source-verified with96 connected/9 activation controls,
but unpromoted. Batch actual reached dependencies up to the separate FPU decision;
do not silently replace the default runtime with the candidate. See
[RECOVERY-STATE-CAPTURE.md](RECOVERY-STATE-CAPTURE.md) for exact paths/hashes.
All1657 source paths/PAL GDI absent; G1 open.46.14GB free; no native process/Simulator.
Default startup/SunPad and FPU promotion status unchanged. Goal active.

**Previous checkpoint — 9 October:** a complete empty state2 repeat is now verified.
ED39D2(46 instructions, correcting earlier47) and ED3A2E(40) add86 instructions;
814 MOVIE entries/40 original words. Original readiness checks return0 for slots6/7,
state remains2, and worker returns to ED39D2 after5,962 more modeled cycles,
13,152 total. Full CPU/RAM/TMU matches; graphics unchanged, no payload/decode/frame.
A test-only observer delegates the first state2 entry unchanged and throws before
the second; it never changes guest state. Stop is diagnostic, not missing code.
One native comparison failed only on dispatch-PC expectation; bound dispatch sets
PC toED39D2 before observer, unlike an unbound-call stop. Fresh repair passes; old
failure preserved. Two earlier reference-model failures lacked already reviewed
DIV0U and the resource0x71000 divisor; repaired in fresh scripts, no runtime change.

**Next action changes:** do not extend the empty loop or simply enable clocks on
this partial replay. Hardware audit proves all nine replay Holly masks are0 and
that the fixture does not restore original MMIO/PVR state, GD-ROM runtime state,
AICA control/ARM state or clock phase. Original CPU/RAM/TMU/SQ/flash/AICA-RAM proof
remains valid; it cannot establish normal boot. Capture the missing runtime state
from the existing original startup prefix at a matching checkpoint, preserving
original scheduling, before clock-enabled continuation. See
[RECOVERY-STATE-CAPTURE.md](RECOVERY-STATE-CAPTURE.md) for exact evidence and path.
All1657 source paths/PAL GDI absent; G1 unresolved.46.25GB free; no native process/
Simulator. Default startup/SunPad unchanged, FPU candidate unpromoted; goal active.

**Previous checkpoint — 9 October:** slot3 resource reservation, zero-byte copy,
unused-reservation return and state/error checks now return to dispatcher. Slots6/7
complete their original flag checks; all six populated command2 callbacks return,
and the nine-slot dispatcher finishes this captured common service pass. Four
native runs match full frozen CPU/RAM/TMU at6,772/6,877/7,024/7,190 modeled cycles.
Sixteen new bodies add630 original instructions;783 MOVIE entries start with40
original words. Original resource8C8AA8C8 free count0x12000→0→0x12000 is preserved;
no payload copied/consumed. Retained648240 object is explicitly registered, not relinked.
Actual next missingED39D2 is confirmed by original callsiteED3962/PR ED3966 and
frozen prediction. This47-instruction state2 routine first callsED3A2E; inspect and
batch its original dependencies, checking retained bindings before generation.
Continue with fresh runner from run-movie-slot-seven-service.py, originalEB0254→
realRTE→same worker; never resume bookkeepingPC. All four native runs pass.
One final-verifier assertion incorrectly expected an address in generic exception
text; preserved and repaired to check original callsite/state. No native/game change.
Graphics/MMIO/TA/SQ unchanged; no decoded media, presented frame, normal boot or
gameplay. Deviceclockoff; FPU candidate unpromoted. All1657 source paths/PAL GDI
remain absent; G1 unresolved. 46.41GB free; no native process/Simulator.
Default startup/SunPad unchanged. [STATUS.md](STATUS.md) has hashes; goal active.

**Previous checkpoint — 9 October:** slot2 ring selection, empty packet handling
and zero-byte consumption now return to dispatcher; slot3 selects its empty buffer
and reaches actual missingED0F76. Three native runs match whole CPU/RAM/TMU at
5,284/5,608/6,059 modeled cycles. Nineteen bodies add1,099 original instructions;
685 exact MOVIE entries start with40 words. No failures; retained helpers reused.
Ring1 descriptor+28 cursor0→8C910000→0 follows original selection/consume; byte
counts stay0. Slot3 source8C971000,length0. Graphics/MMIO/TA/SQ unchanged;
no decode/frame/boot/gameplay; deviceclockoff, FPU candidate unpromoted.
NextED0F76 uses resource8C8AA8C8/vtable8C67FF74+24→retained6621F8 mode0.
Source-only219cycle prediction reachesED662C at6,278 total; not native yet. It
reserves0x12000 before zero copy to8CBB6040, making free0 temporarily. Preserve
original reservation return via vtable+28=662384. Copy109instructions and return98
reviewed. Reuse existing lock/remainder helpers; captured divisor0x12000. Check
bindings and batch the original path; never invent bytes or skip reservation effects.
Fresh runner from run-movie-slot-three-buffer.py; originalEB0254→RTE→same worker,
never bookkeepingPC ED0F58. All1657 source paths/PAL GDI absent; G1 unresolved.
46.82GB free; no game/Simulator. Default startup/SunPad unchanged. [STATUS.md](STATUS.md)
has sealed evidence; goal active.

**Previous checkpoint — 9 October:** original completion/fallback, post-parser
metadata and three-channel scan now return slot1 to dispatcher and reach slot2
callbackED25AE. Three native runs match frozen whole CPU/RAM/TMU at3,969/4,386/
4,750 modeled cycles. Ten new bodies add346 instructions;465 exact MOVIE entries
start with40 words. Existing ring/IRQ/copy/validation helpers reused; retained
ED44FC was already linked and now has its exact activation entry. No failures.
Metadata remainsFFFFFFFF, counters0, slots6/7 query0; no media bytes/decode/frame/boot.
Graphics/MMIO/TA/SQ unchanged; deviceclockoff; FPU candidate unpromoted.
Next ED25AE(28), ED313E(12), ED25E6(40) reuse activeED44FC/EDA274. Source-only
prediction adds59cycles toED26AE,4,809 total; not yet native. ED26AE and ED3194
reviewed; former first reuses ring reader then callsED9D2E. Verify retained bindings.
Fresh runner from run-movie-channel-service.py; originalEB0254→realRTE→same worker,
never bookkeepingPC EDA5CA. Batch original dependencies; never inject media bytes.
All1657 source paths/PAL GDI absent; G1 unresolved.47.12GB free; no game/Simulator.
Default startup/SunPad unchanged. [STATUS.md](STATUS.md) has sealed evidence; goal active.

**Previous checkpoint — 9 October:** original slot0 enable, slot1 ring read and
empty-parser return match full CPU/RAM/TMU through3,624/3,817/3,879 modeled cycles.
Ten new bodies add447 instructions;393 exact MOVIE entries start with40 words.
Retained ring/IRQ helpers and parser validationED7AC4 are reused. Six of nine slots
have command2 callbacks; first two entered. Input8C900000,length0; parser returns0,
outputs0, no bytes consumed. Graphics/MMIO/TA/SQ unchanged; no decode/frame/boot.
One duplicate registration-symbol link failure preserved; fresh repair uniquely
names the unused new registration function, no guest change. Separate lookahead
assertion failure corrected saved-r13 expectation to actual stack return value.
Next originalED21AA(31 instructions) and ED1EC6(9) reuse activeEDA274. Frozen
source-only prediction adds90cycles, returns empty packet loop then reachesED21E8;
3,969 endpoint remains unexecuted. Native actual missingED21AA, PCED1E4E/PR ED1E56.
Use fresh runner from run-movie-parser-empty-repair1.py, unique generated symbols,
originalEB0254→realRTE→same worker runtime. Never resume bookkeepingPC or inject
payload. ED21E8/ED2334 reviewed; verify retained dependencies before extension.
All1657 source paths/PAL GDI absent; G1 unresolved.47.43GB free; no game/Simulator.
Default startup, SunPad and unpromoted FPU unchanged. [STATUS.md](STATUS.md) has
sealed evidence; goal active.

**Previous checkpoint — 9 October:** original zero-byte copy/release/commit now
returns through stream status2 and completes the first worker iteration. The same
native dispatch enters the second service pass, reaches state2 common handler and
retained EDA59E dispatcher, then stops at actual missingED1C18 (slot0/command2).
Frozen full CPU/RAM/TMU predictions match2,642/2,789/3,481 modeled cycles. Nine
new bodies add336 instructions;313 exact MOVIE entries start with40 words.
One preflight rejected duplicate linkage of retained repair1 dispatcher; preserved,
then fixed by reusing linked original and activating its seven entries. No guest fix.
Buffer payload/counters remain empty; graphics/MMIO/TA/SQ unchanged. No decoded
movie, presented frame, normal boot or enabled-device-clock claim.
Next reviewED1C18(15 instructions)→EDA244(8): first prefix12cycles/two stack stores,
r4=instance,r5=slot0,r6=1. Setter writes slot+8=1, not media bytes. Reuse retained
handler dispatch and inspect all nine callback slots for command2 to batch the
next original path sensibly; do not invent a missing retained-helper boundary.
Continue EB0254→realRTE→same worker runtime, never bookkeepingPC EDA5CA.
All1657 source paths/PAL GDI absent; G1 unresolved.47.73GB free; no game/Simulator.
Default startup, SunPad and unpromoted FPU unchanged. [STATUS.md](STATUS.md) has
sealed evidence; goal active.

**Previous checkpoint — 9 October:** metadata/status, circular-buffer query and
resource request now execute through2,488 modeled cycles, matching frozen whole
CPU/RAM/TMU state. Thirteen original bodies add446 instructions. Metadata64-byte
copy passes; destination buffer has65,536 bytes free at8C900000. Resource record
8C8AA8F8 returns source8C983000 and length0 because its captured available count
is0. This is an empty startup queue, not missing-disc proof or a completed read.
Three passing fixtures; one preserved timer-prediction failure corrected in a
fresh repair: STS MACL costs1, unlike STS PR2. No game/runtime change.268 exact
MOVIE entries start with40 words; original helpers reused. Graphics/MMIO/TA/SQ
unchanged; no decoded video, presented frame or normal-boot evidence.
Next originalED5240 has99 instructions; captured length0 takes7-cycle early return,
then original caller8cycles reaches resource slot32 target6622CA. Check binding
and source before executing; nonempty FPU/SQ copy remains unproven. Follow original
queue completion/producer path, never inject bytes or skip empty results.
Continue EB0254→realRTE→worker in one runtime, not bookkeepingPC ED08B8.
All1657 source paths/PAL GDI absent; G1 unresolved.48.03GB free; no game/Simulator.
Default startup, SunPad and unpromoted FPU unchanged. [STATUS.md](STATUS.md) has
sealed evidence; goal active.

**Previous checkpoint — 9 October:** original aggregate serviceED3D10, time
conversion60635E and instance processingECF874→ECF75C now run to missingED4508.
Three frozen CPU/RAM/TMU predictions match native endpoints at1,472/1,758/1,781
modeled cycles. Four new bodies add267 instructions;216 exact MOVIE entries
start with40 replay words. AggregateEE6EF0 becomes1; active outer+72 becomes1.
Original IRQ helpersED1BCC/ED1BE4 are verified, linked and activated. Correction:
the previous isolated runner did not link them, despite the earlier loop note.
Original division helpers are reused; captured divisor100 is independently modeled
with bounded non-restoring arithmetic. Graphics/MMIO/TA/SQ unchanged; no media
read/decode, presented frame or normal-boot proof. FPU candidate unpromoted.
Next25-instruction ED4508 validates the instance then copies64 metadata bytes;
reuse retained ED1BAA and resident10E958, checking linkage AND activation first.
Continue EB0254 through realRTE then worker in the same runtime; never resume
bookkeepingPC ECF75C. All1657 source paths/PAL GDI absent; G1 unresolved.
48.42GB free; no game/Simulator. [STATUS.md](STATUS.md) records evidence; goal active.

**Previous checkpoint — 9 October:** original worker stepECF91C and service
ED38C4→ED3900→ED39AE execute through the captured state1/command4 path. The sole
movie instance8CEE2210 changes state+64 from1 to2; command+68 remains4. Two native
root/worker dispatches in one runtime match frozen whole CPU/RAM/TMU predictions
at1,263 then1,338 modeled cycles. Graphics/MMIO/TA/SQ remains unchanged. Four
original bodies add224 instructions; exact158-entry activation starts with40 words.
Clock storeCFAC joins the prior later writes in any successor activation audit.
Next actual missingED3D10 is a78-instruction aggregate routine. Reuse existing
original ED1BCC/ED1BE4 IRQ-mask helpers and manifests (18/16-cycle contracts),
available as retained objects but not linked or activated by that runner. Do not manufacture a missing
helper boundary or confuse their historical video-geometry filenames with behavior.
Continue original EB0254 root, real RTE, then worker dispatch; no bookkeeping-PC
resume. No movie decode/presented frame/normal boot; candidate unpromoted.
All1657 source paths/PAL GDI absent, G1 unresolved.48.71GB free; no game/Simulator.
[STATUS.md](STATUS.md) records evidence; goal active.

**Previous checkpoint — 9 October:** original context entry/save/restore now
transfers through RTE into movie workerECF9F2. Worker dispatch reaches missing
ECF91C in1,226 modeled cycles fromEB0254, matching frozen whole CPU/RAM/TMU
expectations; graphics/MMIO/TA/SQ remains unchanged. Four original bodies add217
instructions. Captured context saves both FPU banks and alternate GPRs, publishes
old stack8CFFFE0C, selects8CEDE190 and restores stack8CEE21B8/SR40000001/FPSCR0.
No context substitution or dummy return. One missing-registration preflight is
sealed; retained5FFA24 was already linked, and original movie_slot_mark_register
fixes the fixture. Two native dispatches use one runtime for final worker entry.
Exact96-entry MOVIE activation starts with40 replay words; include actual later
ECF040/CF8C/CF90/CF3C writes if making successor activation snapshots.
Next reviewedECF91C has107 instructions. CapturedCF38=1 selectsED38C4 after two
original clock calls; derive prefix and verify binding before execution. Continue
from verified root/real context transfer; never resume bookkeepingPC ECFA0A.
No movie decode/presented frame/normal boot; isolated FPU candidate unpromoted.
All1657 source paths/PAL GDI absent, G1 unresolved.48.92GB free; no live game or
Simulator. [STATUS.md](STATUS.md) records evidence; goal active.

**Previous checkpoint — 9 October:** captured frame prefix is independently
state-checked, and original movie callback `ECFA68` now executes to missing
resident `5FFAE0`. Offline bounded reference matches all CPU/RAM/TMU/SQ/packet
state for959 modeled cycles/931 executed instructions. A diagnostic PREF-source
PC expectation was corrected in a fresh, sealed repair; no guest/runtime change.
One native call fromEB0254 through the new35-instruction/7-entry callback reaches
5FFAE0 in985 modeled cycles, matching a frozen whole-state prediction. Exact90-entry
MOVIE activation starts with40 replay words. Callback storesCF8C=0, CF90=002E802C;
root also writesECF040=FFFFFFFF. Retain these writes in successor replay audit.
Graphics/DMA/Holly state remains byte-identical to the prior limited graphics audit.
No presented frame, movie decode or normal-boot claim. This proves only captured
flag0 and the isolated candidate path, not every callback/context branch.
Next recover22-instruction5FFAE0 and review its transfer to175-instruction5FFB70:
IRQ state, SSR/SPC, register/FPU-bank saving and RTE must stay real, never bypassed.
Continue from verified root to preserve graphics and nested call state.
[STATUS.md](STATUS.md) records hashes. All1657 source paths/PAL GDI absent; G1
unresolved. Default startup/SunPad/FPU unchanged;49.37GB free, no Simulator;
goal active.

**Previous checkpoint — 9 October:** retained movie entry `EB0254` now reaches
its first real frame-loop callback, `ECFA68`, through `608DF0` and `645C40`.
Three bounded native calls (discovery and two diagnostic captures) stop at the
same unregistered callback: PC `8C645D98`, PR `8C645DA0`, target `8CECFA68`.
CPU/RAM/timer/device captures repeat exactly. This is discovery evidence;
independent whole-frame CPU/RAM/timer verification remains open.
A separate source-derived audit validates all four32-byte graphics packets,
51 graphics fields and30 MMIO fields: two SQ packets, one64-byte CH2 transfer,
two list-end events and DMA completion. No vertices, triangles or presented frame;
no media decoding. Devices are clock-disabled, but synchronous DMA does execute.
Next validate the complete executed frame prefix before adding originalECFA68.
Do not resume these snapshots with blank TA/DMA/Holly state, or treat repeatability
as correctness. Exact83-entry activation starts with40 words; ECF040 store adds41st.
[STATUS.md](STATUS.md) records sealed evidence and the preserved compile failure.
All1657 source paths/PAL GDI remain absent; G1 unresolved. Default startup and
unpromoted FPU unchanged; no Simulator;49.50GB available; goal active.

**Previous checkpoint — 9 October:** the entire captured original `ECF35E`
resource method now returns to movie-entry caller `EB0254` in17,201 modeled cycles.
Six integrated controls match an independently verified whole CPU/RAM/AICA/timer/
device endpoint. It includes reset, actual directory/file open, stream creation,
name copy, stream setup and instance flags.49 checks pass, including27 activation
checks; ten original bodies add119 instructions. Exact95-entry activation replays40
module words. This remains isolated candidate evidence; no media read/decode or
normal boot. Source review corrects the earlier positioning label:226 is stored
atstream+44, current file position0 is queried; no seek is executed by this path.
Next retainedEB0254 predicts6cycles and one store ofFFFFFFFF atECF040 before
already-registered608DF0. It creates a41st module data word. Review the real timing/
provider path before executing; captured893A58/893A50 flags are0. Never intercept
this retained target as missing. [STATUS.md](STATUS.md) records evidence.
All1657 missing paths/PAL GDI absent; G1 unresolved. Default twelve patches,
startup1058/504state/412device, SunPad and unpromoted FPU unchanged. Storage49.72GB;
no native process/Simulator; goal active.

**Previous checkpoint — 9 October:** original resident movie open now looks up
SLH448_1.SFD, allocates filesystem descriptor8C6994F4, constructs stream8C8A8010,
copies/pads its filename and returns to MOVIE `ECF37A` in16,845 modeled cycles.
Eight integrated controls match whole CPU/RAM/AICA/timers/devices. Directory
entry1738 has3,119,104 bytes; no media read/decode/playback is claimed.37 checks pass,
including9 activation checks; three original bodies add148 instructions. Dirty
file/stream records and filename padding pass. Existing filesystem and string
helpers are reused. One missing-copy-registration fixture failure is sealed;
repair uses original registration with no interception or replacement.
Following MOVIE continuation executes15cycles, publishes the stream atouter+52
and reaches actual missingED0960 with position226 (captured0x71000 shifted11).
Next unexecuted ED0960 prefix predicts6cycles/two stack stores to65FF48; confirm
real registration and generated PC. [STATUS.md](STATUS.md) records evidence.
All1657 missing paths/PAL GDI absent; G1 unresolved. Default twelve patches,
startup1058/504state/412device, SunPad and unpromoted FPU unchanged. Storage49.95GB;
no native process/Simulator; goal active.

**Previous checkpoint — 9 October:** original `ECF35E` movie-resource preparation
now executes continuously through reset, IRQ save/restore, empty drain, instance
validation and retained resource initialization to resident open target `65FB12`.
The159-cycle run passes24 state controls plus9 activation checks;112 checks pass
across four fixtures. Seven original bodies add264 instructions. Exact72-entry
MOVIE activation replays40 data words. Dirty flag/object/resource fields and IRQ
masks0/5/15 pass; filename SLH448_1.SFD and resource handle8C8AA8F8 reach the target.
File-open success/playback remains unproven. One stale-registration assertion
failure is sealed; corrected fixture passes without runtime/source substitution.
Next original65FB12 has an unexecuted11-cycle/four-stack-store prediction to605416,
filename r4, r5=0, return65FB26. Check existing registrations and generated PC before
execution. [STATUS.md](STATUS.md) records evidence. All1657 missing paths/PAL GDI
absent; G1 unresolved. Default twelve patches, startup1058/504state/412device,
SunPad and unpromoted FPU unchanged. Storage50.23GB; no native process/Simulator;
goal active.

**Previous checkpoint — 9 October:** connected movie-entry continuation
`EB0174` stores the real handle, sets option3/value1, clears the captured outer
flag and dispatches resource `SLH448_1.SFD` to actual unregistered `ECF35E`.
One uninterrupted call takes165 modeled cycles; six integrated controls match
independent whole CPU/RAM/AICA/timer/device endpoint.58 checks pass, including36
activation checks; three original bodies add53 instructions. Exact119-entry
activation starts with39 module data words; the real ECEFE8 handle store creates
the40th. Successor fixtures replay40. No interception/fallback; playback unproven.
Next originalECF35E has an unexecuted11-cycle/five-stack-store prediction to
ECF450, returnECF372, resource handle8C8AA8F8. Confirm generated bookkeeping PC
and real registration before execution; do not resume saved caller PC EB023C.
[STATUS.md](STATUS.md) records hashes and boundaries. All1657 missing paths/PAL
GDI absent; G1 unresolved. Default twelve patches, startup1058/504state/412device,
SunPad and unpromoted FPU unchanged. Storage50.56GB; no native process/Simulator;
goal active.

**Previous checkpoint — 9 October:** connected `ECF1F6` wrapper/allocation/outer
setup returns handle `8CEDD114` to `EB0174` in370 modeled cycles. Six integrated
controls match independent endpoint and whole CPU/RAM/AICA/timers/devices.57 checks
pass, including27 activation checks; one original query body adds31 instructions.
Pool slot4 initialization, dirty record/fields, original32-byte descriptor copy,
slot7/3 handle reads and working-table clear execute without interception/fallback.
Exact37-entry activation replays39 data words; complete movie entry/playback open.
Next retained `EB0174` has frozen11-cycle prefix: captured stackflag1 selects
`ECFFB8`,r4=8CEDD114,r5=1,returnEB0214. It writes handle to `8CECEFE8`, adding a40th
module data word after execution. Use original movie-entry manifest and exact
39-word activation first; successor replay must include the new store. Unexecuted;
verify real target registration. Generated stopPC is blockEB020C, not JSR EB0210.
[STATUS.md](STATUS.md) records evidence. All1657 missing paths/PAL GDI absent;
G1 unresolved. Default twelve patches, startup1058/504state/412device, SunPad and
unpromoted FPU unchanged. Storage50.87GB; no native process/Simulator; goal active.

**Previous checkpoint — 9 October:** complete original `ED0620` output setup now
returns to outer `ECF1F6` in846 modeled cycles. Six uninterrupted full-state
controls match an independently verified endpoint.78 checks pass, including36
activation checks; two original bodies add51 instructions. Eight settings,
exact captured4000.5→4000 scalar FPU path, instance/buffer checks and dirty flag
clearing pass. Exact62-entry activation replays39 module words; no interception
or fallback. General FPU correctness, promotion, normal boot/playback remain open.
The following outer/ED0854 wrapper executes12 cycles to retained allocator662106:
r4=8C983000,r5=00071000,r6=0,returnECF1FC. Slots0–3 are occupied; frozen92-cycle/
ten-store prefix predicts slot4 before retained662194, unexecuted. Reuse allocator
and initializer registrations and continue original caller; no regeneration needed.
[STATUS.md](STATUS.md) records hashes/scope. All1657 missing paths and supported
PAL GDI absent; G1 unresolved. Default twelve patches, startup1058/504state/
412device, SunPad and isolated FPU unchanged. Storage51.14GB; no native process
or Simulator; goal active.

**Previous checkpoint — 9 October:** outer `ECF1B0` stores the real instance and
executes stream-size/division/caller continuation in158 modeled cycles to `ED0620`.
Its14-cycle output prefix reaches `ED436A`; the valid-instance mode8/value0 setter
then returns to `ED063A` in85 cycles.43 controls pass, including27 activation checks
and dirty-slot clearing. Two original bodies add113 instructions; later output
FPU/other settings and setter error paths remain untested. Two frozen-PC expectation
failures are preserved; generated unresolved calls retain block-entry PC, confirmed
before repair. Inspect generated PC assignments before predicting future stops.
Whole CPU/RAM/AICA/timers/devices match; no interception/fallback;39 module words.
Next execute remaining output setters via existing `ED436A`: mode1/value1 call
preparation is frozen at five cycles/no stores excluding callee, returning `ED0642`;
then mode0/value0 and mode23/value4 precede FPU work. Combine manifests and derive
FPU results from actual inputs. [STATUS.md](STATUS.md) records evidence.
All1657 missing paths and supported PAL GDI remain absent; G1/playback unresolved.
Default twelve patches, startup1058/504state/412device, SunPad and isolated FPU
unchanged. Storage51.41GB; no native process or Simulator; goal active.

**Previous checkpoint — 9 October:** connected MOVIE creation tail `ED05C4`
validates the real instance, binds callback `ED07FC` and returns handle `8CEE2210`
to outer routine `ECF1B0` in64 modeled cycles. Four integrated host/guest controls
match complete CPU/RAM/AICA/timers and inactive devices.47 checks pass, including
27 activation checks and valid/invalid-field/null validity branches. One original
body adds13 instructions. Exact37-entry activation replays39 audited data words;
no interception/fallback. This connects the tail, not full constructor/normal boot.
Next execute retained `ECF1B0`: frozen27-cycle/five-store prediction publishes the
handle at `8CEDD144`, pushes four output pointers and calls `ED01BC`, unexecuted.
Verify and reuse original registration. [STATUS.md](STATUS.md) records evidence.
All1657 missing paths and supported PAL GDI remain absent; playback/G1 unresolved.
Default twelve patches, full startup1058/504state/412device, unpromoted FPU and
SunPad unchanged. Storage51.71GB; no native process or Simulator; goal active.

**Previous checkpoint — 9 October:** the remaining captured MOVIE dispatcher
returns, then separate initializer/constructor tails return real handle `8CEE2210`
to `ED05C4`.83 checks pass, including45 activation checks; two original bodies
add10 instructions. One uninterrupted dispatcher invocation `EDA5DA`→`ED3EDC`
takes107 modeled cycles with six integrated full-state controls and52-entry exact
activation. Initializer success/failure and five-cycle constructor return pass.
No interception/fallback;39 audited module data words. This is isolated candidate
continuation evidence, not a connected full constructor, normal boot or playback.
Next execute retained `ED05C4`: frozen nine-cycle/no-store prediction reaches
`ED1AFA` with real handle, callback `ED07FC` and zero third argument; unexecuted.
Verify actual registration before expanding. [STATUS.md](STATUS.md) records hashes.
All1657 missing paths and supported PAL GDI remain absent; G1 unresolved.
Default twelve patches, full startup1058/504state/412device, unpromoted FPU and
SunPad unchanged. Storage51.91GB; no native process or Simulator; goal active.

**Previous checkpoint — 9 October:** the recovered MOVIE setup branch now returns
to dispatcher `EDA5DA`. One uninterrupted `ED136A` invocation takes789 modeled
cycles; six integrated controls match complete CPU/RAM/AICA/timers and inactive
devices.82 checks pass, including36 activation checks. Five original bodies add
132 instructions. The captured state request returns; mode15/value2 validation,
mode-table writes and retained handler refresh/install return through the caller.
Exact41-entry MOVIE activation replays39 data words; no interception or fallback.
Next execute retained `EDA5DA` with its existing dispatcher manifest: frozen
39-cycle/no-store prediction skips empty slots4/5 and reaches slot6 target
`ED511A`, unexecuted. Verify actual target registration before expanding.
[STATUS.md](STATUS.md) records evidence. Long state-change paths, other validator
modes, playback, normal boot and G1 remain open. Default twelve patches, full
startup1058/504state/412device, isolated/unpromoted FPU and SunPad unchanged.
All1657 missing paths and supported GDI remain absent. Storage53.40GB; no native
process or Simulator; goal active.

**Previous checkpoint — 9 October:** object setup now returns to MOVIE with the
channel enabled and saved IRQ mask restored. Original `ED161A` then updates the
movie state and reaches actual missing resident `648548`. One uninterrupted
`ED136A` invocation takes656 modeled cycles; six integrated controls match full
CPU/RAM/AICA/timers and inactive devices.62 checks pass, including18 activation
checks. Three original bodies add36 instructions. Dirty channel state and both
zero-argument branches pass. Exact22-entry MOVIE activation replays39 data words;
no interception or fallback. A filename collision was repaired before build;
all300 identities of the older checkpoint were restored and reverified.
Next execute original `648548`: frozen eight-cycle/three-store prefix predicts
`648240`, unexecuted. Verify actual target registration before expanding.
[STATUS.md](STATUS.md) records evidence. Playback, normal boot and G1 remain open.
Default twelve patches, full startup1058/504state/412device, isolated/unpromoted
FPU and SunPad unchanged. All1657 missing paths and supported GDI remain absent.
Storage53.92GB; no native process or Simulator; goal active.

**Previous checkpoint — 9 October:** the captured outer reset now returns after
both record resets, buffer clearing and two child initializers. Original critical
exit restores the saved IRQ mask; the caller then enters its next setup section.
One uninterrupted MOVIE `ED136A` call reaches actual missing `65F4A4` in559 modeled
cycles. Six integrated controls match complete CPU/RAM/AICA/timers and inactive
devices;80 controls total pass. Six original bodies add150 instructions. Dirty
buffers/children and nested critical exit pass without interception or fallback.
Next execute original `65F4A4` and existing caller `648186`: frozen nine-cycle,
two-store prediction reaches `65F4B8`, unexecuted. Verify real target registration
before expanding. [STATUS.md](STATUS.md) records immutable evidence. This is the
captured idle path; active transitions, playback, normal boot and G1 remain open.
Default twelve patches, full startup1058/504state/412device, isolated/unpromoted
FPU and SunPad unchanged. All1657 missing paths and supported GDI remain absent.
Storage54.38GB; no native process or Simulator; goal active.

**Previous checkpoint — 9 October:** MOVIE `ED136A` now executes allocation,
binding and the idle record reset in one uninterrupted 255-cycle invocation,
reaching actual missing `65F23E`. Six integrated controls match complete CPU/RAM/
AICA/timers and inactive devices; 38 controls total pass. Three original bodies
add 118 instructions. Both status-bit outcomes and reset arguments pass; original
IRQ save/restore helpers execute. The outer critical section remains active at
nesting1. No interception or signature fallback. Next execute original `65F23E`;
its frozen four-cycle/no-store tail jump predicts `66107A`, unexecuted. Verify
actual target registration before expanding. [STATUS.md](STATUS.md) records
hashes and scope. Active-record reset paths, outer reset completion, playback,
normal boot and G1 remain open. Default twelve patches, full startup1058/504state/
412device, isolated/unpromoted FPU and SunPad are unchanged. All1657 missing paths
and supported GDI remain absent. Storage55.17GB; no native process or Simulator;
goal active.

**Previous checkpoint — 9 October:** MOVIE `ED136A` now executes allocation,
constructor return, object binding and reset entry in one uninterrupted167-cycle
call. Six integrated controls match complete CPU/RAM/AICA/timers and inactive
devices;42 controls total pass. Four original bodies add165 instructions. The
64-slot pool chooses actual slot3, skips an extra occupied slot and returns null
when full. Dirty-record initialization and retained critical-entry behavior pass.
No interception or signature fallback. Next execute original `65F238`; its frozen
four-cycle/no-store tail jump predicts `660FA6`, unexecuted. Check actual target
registration before expanding. [STATUS.md](STATUS.md) records hashes and scope.
Reset completion, playback, normal boot and G1 remain open. Default twelve patches,
full startup1058/504state/412device, isolated/unpromoted FPU and SunPad unchanged.
All1657 missing paths and supported GDI remain absent. Storage55.52GB; no native
process or Simulator; goal active.

**Previous checkpoint — 9 October:** the MOVIE creation wrapper now returns to
`ED136A` after both original setters. Six uninterrupted `ED144C` controls take26
modeled cycles and match complete CPU/RAM/AICA/timers. The parent handler then
executes9 cycles to actual missing `662106`. This checkpoint adds52 passing
controls, including18 activation checks, and two original bodies/five instructions.
Exact MOVIE activation replays only39 verified data words; no helper interception
or signature fallback. Header/link command repairs and a preflight-only pool
assumption failure are preserved. Next execute original `662106`: slots0–2 are
occupied, so the frozen79-cycle/ten-store prediction allocates slot3 before
`662194`, unexecuted. Check that helper's actual registration before expanding.
[STATUS.md](STATUS.md) records evidence. Playback, normal boot and G1 remain open.
Default twelve patches, full startup1058/504state/412device, isolated/unpromoted FPU
and SunPad remain unchanged. All1657 missing paths and supported GDI remain
absent. Storage55.94GB; no active native process or Simulator; goal active.

**Previous checkpoint — 9 October:** the recovered resident constructor now
returns its real handle to MOVIE `ED144C`. One uninterrupted `647F74` invocation
returns at7,165 modeled cycles; six integrated controls match complete CPU/RAM/
AICA/timers and inactive-device invariants. This checkpoint adds70 controls and
two original bodies/166 instructions. Both buffer queries return the original
addresses and sizes; invalid indices preserve output slots. Existing division,
IRQ restoration and exact scalar unit/zero tail paths pass without interception
or template fallback. Normal boot/playback/G1 remain open. Next execute existing
MOVIE `ED144C` with exact module activation and39 verified data stores; its frozen
eight-cycle/no-store prefix predicts `648342`, unexecuted. Check actual target
registration before expanding. [STATUS.md](STATUS.md) records hashes. Default
twelve patches, full startup1058/504state/412device, isolated/unpromoted FPU and
SunPad are unchanged. All1657 missing paths and supported GDI remain absent.
Storage56.30GB; no active native process or Simulator; goal active.

**Previous checkpoint — 9 October:** original stereo setup now returns through
both pan callbacks. One uninterrupted `647F74` invocation reaches `66C37C` at
6,721 modeled cycles; six integrated controls match complete CPU/RAM/AICA/timers
and inactive-device invariants. This checkpoint adds 66 passing controls and five
original bodies/166 instructions. Retained division, callback and queue helpers
execute without interception or template fallback. Negative/center/positive pan,
truncation and channel1 upper clamp pass; queue count becomes8/write index48.
Playback, outer-constructor completion and full startup remain open. Next execute
original `66C37C` through retained `66E930`; the frozen29-cycle/eight-store prefix
predicts `65E640`, unexecuted. Check actual registration before classifying it.
[STATUS.md](STATUS.md) records immutable evidence. Default twelve patches, stable
full startup1058/504state/412device, SunPad and G1 remain unchanged. All1657 missing
paths and supported PAL GDI remain absent. Storage56.69GB; no active native process
or Simulator; goal active. Older entries below are historical.

**Previous checkpoint — 9 October:** the original service constructor now returns
its handle and the outer record initializer preserves it. One uninterrupted
`647F74` invocation reaches `6618D6` at5,213 modeled cycles; six integrated controls
match complete CPU/RAM/AICA/timers and device invariants. This checkpoint adds52
controls and four original bodies/88instructions. Both caller iterations submit
command46; queue wrap, byte sign extension/sentinel handling and all23 initializer
writes are checked. The outer constructor, playback and full startup remain open.
Next execute original `6618D6`:104 instructions match BOOT/live RAM; frozen15-cycle/
four-store prefix predicts `66C1AA`, unexecuted. Check actual registration before
classifying that target. [STATUS.md](STATUS.md) records hashes and a preserved
instruction-count guard failure. Default12patches, stable full startup1058/504state/
412device, SunPad and G1 gaps remain unchanged. All1657 missing paths and supported
GDI remain absent. Storage57.14GB; no active native process or Simulator; goal
active. Older entries below are historical.

**Previous checkpoint — 9 October:** the original channel-pair constructor now
returns and its caller stores the handle. One uninterrupted `647F74` invocation
reaches `66D184` at4,742 modeled cycles; six integrated controls match complete
CPU/RAM/AICA/timers and device invariants. This checkpoint adds36 passing controls
and two original bodies/215instructions. Both original callbacks enqueue command38;
queue wrap and the field setter's127 sentinel are covered separately. This is
command submission with devices frozen, not playback or full startup. Next execute
original `66D184`: three instructions predict a four-cycle tail jump to `66EE7C`,
PR and RAM preserved, unexecuted. Check actual target registration before expanding.
[STATUS.md](STATUS.md) records evidence and preserved test/preflight failures.
Default12patches, stable full startup1058/504state/412device, SunPad and G1 gaps
remain unchanged. All1657 missing paths and supported GDI remain absent. Storage
57.55GB; no active native process or Simulator; goal active. Older entries historical.

**Previous checkpoint — 9 October:** original channel allocation/count setup now
executes from `647F74` to `66F0B4` in one uninterrupted 4,060-cycle invocation.
Six integrated controls match complete CPU/RAM/AICA/timer state and device
invariants. This checkpoint adds 22 controls and two original bodies/242
instructions. The captured allocator starts with all four third-pool slots free;
a later-slot control also passes. Counts one/two are tested; the integrated path
uses the original count two. No helper interception or template fallback occurs.
Next execute original `66F0B4`: 165 instructions match BOOT/live RAM; frozen
152-cycle/21-store prediction prepares command38 for retained `65E1A4`, unexecuted.
[STATUS.md](STATUS.md) records hashes and corrected timing breakdowns. Default
12 patches, full startup1058/504 state/412 device, SunPad and G1 gaps remain
unchanged. All 1657 missing paths and supported GDI remain absent. Storage58.07GB;
no active native process or Simulator; goal active. Older entries are historical.

**Previous checkpoint — 9 October:** the original caller at `647F74` now reaches
`66CFC4` through both buffer bindings and four retained record allocations in one
uninterrupted call. Six integrated controls pass at 3,832 modeled cycles; complete
CPU/RAM/AICA/timer state matches the separately checked chain. This checkpoint adds
18 controls and two original bodies/178 instructions. The explicit original
`66AF40` writer preserves original return addresses; no signature relocation or
helper interception occurs. These are idle-device memory writes, not playback.
Next execute original `66CFC4`: its 162 instructions match BOOT/live RAM; a frozen
108-cycle/nine-store prefix predicts the call to retained `65E770`, not yet run.
[STATUS.md](STATUS.md) records evidence and two preserved test failures. Default
12 patches, full startup 1058/504 state/412 device, SunPad profile and G1 gaps are
unchanged. All 1657 missing source paths and supported PAL GDI remain absent.
Storage 58.31GB; native runs terminal; no Simulator; goal active. Older entries
below are historical.

**Previous checkpoint — 8 October:** the original caller at `647F74` now reaches
buffer constructor `65E470` in one uninterrupted316-cycle invocation. Six integrated
controls pass; complete CPU/RAM/timers and device invariants match the independently
checked steps. Seven new original bodies/590instructions and130controls were added
this checkpoint. Both channel resources are actually allocated; IDs15/14 and first
buffer arguments `52535053,15,001FBFC0,4000` are verified. No enclosing-constructor
return, full-startup expansion or media playback is claimed. Next execute original
`65E470`: the captured SPRS prefix predicts50cycles/seven frame stores to literal
`66AF40`; not executed. Check actual registration first. [STATUS.md](STATUS.md)
records hashes and scope. Default12patches, full startup1058/504state/412device,
SunPad profile and G1 gaps unchanged. Storage55.47GB; native runs terminal; no
Simulator; goal active. Earlier checkpoint statements below are historical.

**Previous checkpoint — 8 October:** original channel constructor `65F364` now
returns in one uninterrupted native call: six controls, 1750 modeled cycles,
record `8C8A7B90`, actual return `8C647F74`. Complete CPU/RAM/timers and device
invariants match the independently checked continuation chain. Two new original
leaves add 20 instructions; this checkpoint adds 54 controls including the work
return. Captured guest toward-zero mode only for the whole constructor; both host
rounding modes and three generated FPU modes pass. Isolated candidate only;
default twelve patches and full startup1058/504state/412device remain unchanged.
Next execute original `647F74`: nine cycles/one store to literal target `65F214`
are predicted, not executed; check the actual target inventory first.
[STATUS.md](STATUS.md) records evidence, two fixture repairs and a corrected
return-address label. G1/boot remain open. Storage56.15GB; no live native process
or Simulator; goal active. Older checkpoint and storage holds below are historical.

**Previous checkpoint — 8 October:** original65EB00 and its original math chain
return in24 controls at284 modeled cycles, with exact CPU/RAM/timers and output
halfwords1CA6/F32D. Separate wrapper/classifier/polynomial controls pass; existing
1117D8 is used. Allocator continuation64890C passes8 controls/63cycles and returns
894900 to66B13C. Next execute that original work-record continuation:31cycles/nine
stores to65F3DE are predicted, not executed. [STATUS.md](STATUS.md) records hashes,
failed inventory/preflight evidence and scope. Default12patches unchanged; isolated
FPU candidate only. Full startup1058/504state/412device remains unchanged; G1 and
normal boot stay open. Storage available; all native runs terminal; goal active.
Earlier holds are historical.

**8 October 2026 — blocked on storage.** The focused probe guard has held
across three consecutive goal turns. The prior turn and this turn produced no
new execution result; no live process exists to classify as a verified wait.
Latest available: 400,900,096 bytes, below the unchanged 419,430,400-byte guard.
Full capture still requires 2,952,790,016 bytes. No build/output directory was
created, no evidence was deleted, and the prepared probe inputs remain intact.
No further downstream source expansion or speculative runtime edit is justified
before this experiment. Resume with the native probe command below after storage
is available. G1 remains unresolved; original missing-source inventory and unsent
source-only acquisition request remain in BOOTSTRAP-BLOCKERS.md. Goal blocked,
not complete. Earlier active-status entries below are historical.

**Previous checkpoint — 8 October:** resident rate pool/clear pass12controls,
29/297/314cycles to65EB00.21new bodies/961instructions/919controls beyond saved
1058-function compilation; actual full1049/484state/392device unchanged. Source
audit finds missing ARM64 host rounding/DN synchronization. Exact nearest/zero
predictions differ; prepared native diagnostic is unexecuted. Probe guard held at
393916416bytes, below419430400; no build started. Recheck storage, then run the
probe before any deeper callee review or arithmetic/runtime claim. [STATUS.md](STATUS.md)
records exact evidence, limits and command. G1/boot open; goal active.

**Previous checkpoint — 8 October:** channel pool, three getters and work-record
prefix pass108controls.20new bodies/907controls beyond compiled1058 integration;
latest actual full1049/484state/392device unchanged. Next6488AE with existing clear:
unexecuted29/297/314cycle predictions to65EB00. Wrapped first descriptor preserved;
channel path uses normal sizes. Free471666688bytes;recheck focused guard/full reserve.
[STATUS.md](STATUS.md) records exact evidence and next action. G1/boot remain open;
goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** resident pool/clear and three descriptor
creations execute;140new controls,799beyond compiled1058 integration. Captured
wrapped lengthFFFFF800 preserved; descriptor creation is not a host allocation
of that length. Next65F364 prefix predicts42cycles/eight stores to6624D8, unexecuted.
Latest actual full1049/484state/392device unchanged. Free694738944bytes;full capture
awaits reserve. [STATUS.md](STATUS.md) records source evidence and next action.
G1/boot remain open; goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** fourth dispatcher, handler, setup/copy and
creation prefix pass164controls. Actual next dependency is resident8C647E7C;
206original instructions match BOOT/live RAM. First call10EF3C is already registered;
43cycle prediction is unexecuted.12module bodies/659controls beyond compiled1058
integration; latest actual full1049/484state/392device unchanged. Free637820928bytes;
full capture awaits reserve. [STATUS.md](STATUS.md) records exact evidence/next action.
G1/boot remain open; goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** buffer binding and the third startup handler
return in native tests;175new controls,495across nine buffer bodies beyond the
saved1058-function integration. Latest actual full remains1049functions/484state/
392device;504/412 full capture still awaits reserve. Next execute retained
EDA5DA dispatcher:21cycle/unchanged-RAM prediction to fourth handlerED1326 is
unexecuted. [STATUS.md](STATUS.md) records failures, identities and exact full-run
commands. G1/boot remain open; goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** buffer configuration,link setup and allocator
return in native units;190new controls,320across six buffer bodies beyond the
saved1058-function integration. Existing nine-body compiled/baseline evidence
unchanged; full504/412 capture awaits reserve,latest actual full484/392.
Next ED7E54 binding prefix predicts9cycles/four stores to missingED86AE;all
managed entries checked. [STATUS.md](STATUS.md) records identities and exact
full-run/audit commands. Boot/G1 remain open;goal active.
Older entries below are historical.

**Previous checkpoint — 8 October:** whole resource initializer and wrapper return
in full startup:484state/392device checks,472unchanged captures and same-binary
baselines;1049functions. Next execute ED7A2C callback-binding prefix:11cycles/four
frame stores to missingED7AC4 predicted but unexecuted. All managed manifests
checked. [STATUS.md](STATUS.md) records scope and identities. Second-handler/
instance/create/boot/G1 remain open;storage reserve met,goal active.
Older entries below are historical.

**Previous checkpoint — 8 October:** resource search,47-word clear,activation and
retained internal79A8 return execute in full startup:473state/381device checks,
464unchanged captures and same-binary baselines;1046functions. Next execute actual
missingED7B82. [STATUS.md](STATUS.md) records the repaired unit activation gap:
internal managed entries must be checked alongside root inventory. Resource/
wrapper/handler/instance/create/boot/G1 remain open; storage reserve met, goal
active. Older entries below are historical.

**Previous checkpoint — 8 October:** first record handler returns; second handler
prefix and pointer-clear helper execute in full startup:465state/373device checks,
459unchanged captures and same-binary baselines;1043functions. Next execute ED7ADC:
4cycle prefix/onePRstore to missingED7B0C predicted but unexecuted.
[STATUS.md](STATUS.md) records identities and scope. Handler/instance/create/boot/G1
remain open; storage reserve met, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** original tail jump and record dispatcher
execute in full startup:460state/368device checks,457unchanged prior captures,
same-binary baselines;1040functions. Next execute original ED1C36 return-zero
leaf and retained dispatcher:3cycle return/24cycle nextED2444 predicted but
unexecuted. [STATUS.md](STATUS.md) records identities and the corrected BRA timing
expectation. Instance/create/boot/G1 remain open; storage reserve met, goal active.
Older entries below are historical.

**Previous checkpoint — 8 October:** original table initializer and ancestor
frames return in full startup:458state/366device checks,446unchanged prior captures
and same-binary baselines;1038functions. Next execute ED3FEA:4cycle tail jump to
missingEDA59E with r5=3 and preserved PR/SP/RAM predicted but unexecuted.
[STATUS.md](STATUS.md) records identities and scope. Instance/create/boot/G1 remain
open; storage reserve met, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** original EDA486 returns through retained
helpers in full startup:447state/355device checks,440unchanged prior captures
and same-binary baselines;1036functions. Next execute EDA530 with canonical
sharedA54C:33cycle return/37cycle missingA4CC predicted but unexecuted. Its entry
selects the second table slot. [STATUS.md](STATUS.md) records identities and scope.
Instance/create/boot/G1 remain open; storage reserve met, goal active. Older
entries below are historical.

**Previous checkpoint — 8 October:** internal branch helper and second selector
execute in full startup:441state/349device checks,435unchanged prior captures
and same-binary baselines;1035functions. Canonical sharedA54C retained. Next
execute EDA486 with retained helpers/sharedA57C:99cycle whole return/107cycle
missingA530 predicted but unexecuted. [STATUS.md](STATUS.md) records identities,
PR-expectation repair and scope. Instance/create/boot/G1 remain open; storage
reserve met and goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** descriptor prefix and selector execute
in full startup: 436 state / 344 device checks, 432 unchanged prior captures and
same-binary baselines; 1033 functions. Next execute EDA42C with its internal A57C
entry: entry9 / return29 / next missing A524 at37 cycles predicted but unexecuted.
[STATUS.md](STATUS.md) records identities and the repaired stale record-tail audit.
Instance/create/boot/G1 remain open; storage reserve met, goal active. Older
entries below are historical.

**Previous checkpoint — 8 October:** nine table-record calls execute through
the original caller in full startup: 433 state / 341 device checks, 414 unchanged
prior captures and same-binary baselines; 1031 functions. The corrected overlap
fixture passes 41 controls. Next execute EDA388: saved 14-cycle / four-frame-store
prefix to EDA518 predicted but unexecuted. [STATUS.md](STATUS.md) records identities
and scope. Instance/create/boot/G1 remain open; storage reserve met and goal active.
Older entries below are historical.

**Previous checkpoint — 8 October:** the original stream initializer returns,
then the next initializer prefix executes in full startup: 415 state / 323
device checks, 413 unchanged prior captures and same-binary baselines; 1030
functions. Next execute reviewed EDA362 (19 instructions): 34-cycle return /
41-cycle second entry predicted but unexecuted. [STATUS.md](STATUS.md) records
identities and open gates. Instance/create/boot/G1 remain open; storage reserve
is met and the goal remains active. Older entries below are historical.

**Previous checkpoint — 8 October:** three indexed records and a small record
execute in full startup:405state/313device checks,396unchanged captures and same-
binary baselines;1028functions. Next execute46-instruction ED9978 type2 record;
47cycle first return/53cycle retained ED9918 entry predicted but unexecuted.
[STATUS.md](STATUS.md) records corrected MUL timing/padding audit, identities and
open gates. Connect remaining original helpers toward stream return. Stream/
instance/create/boot/G1 remain open; disk reserve met, no live native process/
Simulator, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** stream prefix/eight offsets/original resident
remainder execute in full startup:397state/305device checks,391unchanged captures,
same-binary baselines;1026functions. Next execute81-instruction ED9864 indexed
record initializer;77cycle first return/86cycle second entry are predicted only.
[STATUS.md](STATUS.md) records identities, repaired alias expectation and gates.
Complete stream/instance/create/boot/G1 remain open; disk reserve met, no live
native process/Simulator, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** five handlers/seven-word clear/complete rate
return execute in full startup:392state/300device checks,386unchanged captures,
same-binary baselines;1024functions. Next execute79-instruction ED97B0 prefix;
16cycles/five frame stores to missingED984E predicted but unexecuted.
[STATUS.md](STATUS.md) records identities, harness repairs and remaining gates.
Stream/instance/create/boot/G1 remain open; disk reserve met, no live native
process/Simulator, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** second fields/internal clear/copy/rate entry
execute in full startup:387state/295device checks,377unchanged captures and same-
binary baselines;1022functions. Next execute19-instruction ED4A54 handler installer
with retained five-call path;20cycle first return/25cycle second entry predicted.
[STATUS.md](STATUS.md) records scope/identities. Instance/create/boot/G1 remain
open; disk reserve met, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** original slot clear/copy/first field helper
execute in full startup:378state/286device checks,368unchanged captures and same-
binary baselines;1020functions. Next execute ED3F22 with retained fill;55cycle
return/60cycle registered internalED1A94 entry are predicted but unexecuted.
[STATUS.md](STATUS.md) records scope/identities. Instance/create/boot/G1 remain
open; disk reserve met, no live game/Simulator, goal active. Older entries below
are historical.

**Previous checkpoint — 8 October:** original instance guard/pool selectEE2210
in full startup:369state/277device checks,363unchanged captures and same-binary
baselines;1018functions. Next execute ED3E54 with registered ED67F8 clear and
resident copy10E964; first-call13cycle/16store-byte prediction is unexecuted.
[STATUS.md](STATUS.md) records scope and identities. Instance/create/boot/G1 remain
open; disk reserve met, no live game/Simulator, goal active. Older entries below
are historical.

**Previous checkpoint — 8 October:** both descriptor initializers return in full
startup:364state/272device checks,359unchanged prior captures and same-binary
baselines;1015functions. Next execute ED3DC2; its7cycle/12store-byte prefix to
missingED3DFE is predicted but unexecuted. [STATUS.md](STATUS.md) records identities
and scope. Complete create/boot/G1 remain open; disk reserve met, no live
game/Simulator, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** original movie-create prefix, capacity check
and descriptor copy execute in full startup:360state/268device comparisons,
348unchanged prior captures, same-binary baselines;1013functions. Next missing
ED2538 is a reviewed25-instruction leaf; predict26cycles and36ordered store bytes
toED05B2. Execute that saved prediction before deeper review. [STATUS.md](STATUS.md)
records scope and identities. G1/boot remain open; disk reserve met, no live
game/Simulator, goal active. Older entries below are historical.

**Previous checkpoint — 8 October:** setup binding/validation returns in full
startup:349state/257device comparisons,343unchanged prior captures and same-binary
baselines;1012functions. Next execute reviewed ED03F8, whose first callee ED01BC
is already registered. Prospective first-call boundary:28cycles/48stack-store bytes.
No fake helper returns. [STATUS.md](STATUS.md) records exact identities and scope.
G1 and boot/gameplay remain open; disk reserve met, no live game/Simulator, goal active.
Older entries below are historical.

**Previous checkpoint — 8 October:** disk hold cleared after Chris freed space.
Full startup now passes344state/252device comparisons,338unchanged prior captures
and same-executable baselines; it stops atED1AFA. That body's prefix separately
passes45controls and reaches ED1B5E. Next execute the reviewed three-instruction
ED1B5E leaf and original ED1B54 return;4/10cycle predictions are unexecuted.
[STATUS.md](STATUS.md) records identities and evidence. G1 and boot/gameplay remain
open; goal active. Earlier storage-hold entries below are historical.

**Historical execution hold — 7 October 2026:** recovery integration is ready but blocked on
storage. Available 2,771,390,464 bytes; required 2,952,790,016; short
181,399,552 bytes. This hold has recurred across three turns. Exact sharing
is complete for the inspected diagnostics; candidate old binaries belong to retained
checkpoints and were not deleted. Add disk space (1 GiB additional headroom preferred)
to resume the prepared command below. G1 separately remains blocked on matching
source material and the supported PAL dump; the exact unsent request is in
[BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md). No native process or Simulator is live.
See `artifacts/recovery-aot/20261007-movie-guard-final-storage-blocker/audit.json`.

**Previous checkpoint — 7 October:** 148 controls verify the signature guard,
descriptor initializer return and retained setup call in isolated native tests.
Combined integration preflight and compiler syntax pass;344state/252device checks
are prepared but **not executed**. Latest full run remains339state/247device atED38A0.
Unit next targetED1AFA. Disk reserve remains short by180,797,440bytes at checkpoint;
88additional diagnostic paths were shared without deleting contents. See
[STATUS.md](STATUS.md) for checkpoint, exact command, storage guard and unchanged G1.
Older notes below are historical.

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

**Previous recovery — 7 October2026: record serialization/command prefixes, R3/C8 partial.**
Original10A2DA returns in19,029cycles after40 packing/overlap controls. Actual
10A326 route1 passes three whole-RAM/CPU/TMU phases at42/19071/19081cycles.
Real64A734/664286 pass48 controls and captured26cycle zero-lock/argument forwarding,
then reject missing6642C0. Worker/command/transport/helper/parent returns unproved.
New151-identity checkpoint;3748+155+151 IDs and237 paths reverified. Both new native
binaries target macOS13/SDK27 ARM64; CLT selection preserves the existing Xcode
license block. Full-startup235341/38 unchanged/not rerun. Next real6642C0 has24
instructions,24-byte frame and registered663550 child; preserve lock80 and packet.
One classification batch used. Unit400MiB guard met/full1536MiB unmet. Source/PAL
still missing; G1 and gameplay unresolved. See [STATUS.md](STATUS.md).

**Previous bounded recovery — 7 October2026, R3/C8 partial; G1 unresolved.**
Original10B906 passes64 controls/four captured record inputs,13cycles. Actual
10B992 gate2/mode22/16-byte frame and first real predicate entry/return pass two
whole-RAM/CPU/TMU phases at21/34cycles. Real callees reach missing10A326; later
state only observed, four-record parent return unproved. Full-startup235341/38
checkpoint unchanged, not rerun. New155-identity checkpoint and3748 prior IDs
reverify;237 diagnostic paths preserved after eight exact emitted-image shares.
Unit400MiB guard restored; full1536MiB guard unmet. Next actual10A32647instruction/
204byteframe path selects10A2DA then6067AA from record bytes1/1/0/1. One new
classification batch used. Source/PAL audits still exit2; source-only request
unchanged/unsent. All terminal/no Simulator. See [STATUS.md](STATUS.md).

**Previous full-startup checkpoint: 20261006-235341, R3/C8 partial; G1 unresolved.**
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

Operating loop for the private, gated Apple port of **Sonic Adventure** using SARecomp/Katana-derived code. Written **5 October 2026**. Requirements and source findings live in [`prd.md`](prd.md); read both before acting.

**2026-10-05 recovery amendment:** Chris authorized an alternative-source
feasibility loop after source acquisition failed. Follow [RECOVERY-LOOP.md](RECOVERY-LOOP.md)
for isolated R0–R5 experiments. This supersedes the blanket stop on replacement
runtime investigation below within that bounded scope. It does not clear G1,
select a production core, or lower the original gameplay/physical acceptance gates.

**Current state: original G1 Katana/PAL unresolved; R3/C8 partial.**
The authoritative current recovery result and next step are the latest checkpoint above. Original G1 and production acceptance remain unresolved.

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
- **Checkpoint lineage.** Verify the parent checkpoint digest; hash current consumed inputs, outputs and newly shared files. Preserve frozen versions of changed sources. Label inherited historical identities separately from freshly verified identities. Recheck unrelated archived artifacts only when an integrity concern warrants it; keep current runtime, capture and regression coverage explicit.
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
