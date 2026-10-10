# SonicPad status

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

**Previous recovery — 9 October 2026: movie callback and worker context handoff verified.**

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

**Previous recovery — 9 October 2026: graphics interrupt differences resolved.**

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

**Previous recovery — 9 October 2026: stream-ready return verified; graphics interrupt audit open.**

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

**Previous recovery — 9 October 2026: live movie file descriptor and stream creation verified.**

Original startup now finds `SLH448_1.SFD` in its loaded directory (index1738),
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-resource-open/live-resource-open-checkpoint.json`:
  `d1ccfc1d95294f0b474fbcaa514f991d3a51820d8fd7f72de2a1261a15b62c74` (3,098 identities).
- `20261009-live-resource-open-prediction/prediction-checkpoint.json`:
  `ee4075265ab0415554584ee1f60a476c97310ccdec84a64df89b02d4bc58b4da` (332 identities).
- `20261009-original-startup-live-file-open/live-file-open-checkpoint.json`:
  `10727149301af1ce7c635abd4b7e55f8a5db2fca9b9e511a9f5c6b2bdd63f60a` (3,178 identities).
- `20261009-live-file-open-prediction/prediction-checkpoint.json`:
  `f5928493e174d93d65ab447cc2163c57ed6fbcdfd97d963634531946989e9f2c` (346 identities).
- `20261009-live-resource-open-final-verification.json` and
  `20261009-live-file-open-final-verification.json`: CPU/RAM/TMU, resource reset,
  IRQ mask, directory identity/name/length, descriptor/stream fields, source
  absence, pins, process and storage checks.
- Models `tests/recovery/live-resource-open-model-20261009.cpp` and
  `tests/recovery/live-file-open-model-20261009.cpp`; runners
  `work/dreamcast-recovery/run-original-startup-live-resource-open.py` and
  `work/dreamcast-recovery/run-original-startup-live-file-open.py`.

The file-open symbol audit initially compared every old linked object, including
already-present archived helpers. It was narrowed to the three absent resident
objects before any link attempt; no native failure or source replacement occurred.
Retained captures match before live prediction. Source-only acquisition remains
separate from this experimental recovery path. Next inventory SHA256:
`451f013b79285f70afc4d71281d8e5fe9ec169ac491425a0e2466ec2fdcb905b`.

**Previous recovery — 9 October 2026: live indexed queries and movie-file selection verified.**

Original indexed queries now return slot7 handle8C8A8C90 and slot3 handle8C6774F0.
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-outer-entry/live-outer-entry-checkpoint.json`:
  `527ecf6201fd17674c42af0747a1132f116f634f8a55b808d80fbb84f0cfbdca` (3,035 identities).
- `20261009-live-outer-entry-prediction/prediction-checkpoint.json`:
  `5d659a577602dc7dce58ea2b31c45efb56bdb4608d67270885fad2eb364ab867` (356 identities).
- `20261009-live-outer-entry-final-verification.json`: return/endpoint CPU/RAM/TMU,
  slots7/3, original handle publication, resource filename, worktable, source
  absence, pins, process and storage checks.
- `20261009-live-outer-entry-verifier-failure.json`: post-run filename slice was
  one byte short. Fresh `verify-live-outer-entry-repair1.py` checks the terminated
  name in saved captures; no native rerun or sealed evidence modification.
- Model `tests/recovery/live-outer-entry-model-20261009.cpp`, predictor
  `work/dreamcast-recovery/predict-live-outer-entry.py`, and runner
  `work/dreamcast-recovery/run-original-startup-live-outer-entry.py`.

The composed model reproduces four retained stages before live prediction.
It excludes the71-cycle outer-fields prefix already executed. The native run
passes without repair. Next inventory SHA256:
`bbc643cc2a4ebfdcdfd65f4202a6b4d2b621ac3a7aedc1438627b9bcd49e50c8`.

**Previous recovery — 9 October 2026: live output setup and slot4 allocation verified.**

Original output setup now returns to ECF1F6 in live startup after all eight
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-output-setup/live-output-setup-checkpoint.json`:
  `3ba44e641f9af4a17b80ff718770363f7b5457f3aa70f4ef39853ff935e2d941` (2,950 identities).
- `20261009-live-output-setup-prediction/prediction-checkpoint.json`:
  `42c3e51dbc05e19833f0d7bc519fe8142b06169d6ad47211237a52b80d3e57e0` (365 identities).
- `20261009-live-output-setup-prediction/scalar-fpu-oracle.json`:
  `7ea2b4c4ad1197d53107ac0dc9481ef665fe9d6b887e0d20d7c7d3831b1c7628`.
- `20261009-live-output-setup-final-verification.json`: full return/endpoint,
  independent FPU bits, all eight mode writes, slot4 contents/binding, source
  absence, pins, process and storage verification.
- Model `tests/recovery/live-output-setup-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-output-setup.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-output-setup.py`.

The composed model reproduces five retained stages before live prediction. It
runs no guest code. New original bodies are outputED0620, setterED436A,
tailED2EC0 and wrapperED0854. Existing validator/slot routines, buffer helpers,
allocator662106/initializer662194 and32-byte copy execute naturally. Symbols
have no overlaps; original module entries are appended with uniqueness checks.
The independent FPU oracle reads initial live arguments and original literals,
uses exact fractions to derive binary32 bits, and is frozen before native execution.
Both model and one original-startup run pass, with no repair.

Mode writes:8→0,1→1,0→0,23→4,45/44/42→4000,14→1. The allocator scans actual live
occupied slots0–3 and initializes slot4; it is not intercepted. The outer object
stores the new handle and copies the descriptor before the next query.

Next inventory SHA256:
`eea2e9a31b9a1e634f251697dc143554db84dfae6af4504adc14c1643d04d50f`.
Captured output configuration and successful allocation do not prove alternate
output/error paths, exhausted-pool handling, playback or production source closure.

**Previous recovery — 9 October 2026: live creation return and stream sizing verified.**

Creation now binds callbackED07FC/argument0 and returns handle8CEE2210 to ECF1B0
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-create-stream/live-create-stream-checkpoint.json`:
  `7645c5155546b53d309e46addbb0006aee76c620c34163e3779acda0be0dfcae` (2,867 identities).
- `20261009-live-create-stream-prediction/prediction-checkpoint.json`:
  `81f42a78fa7ba2b7768032165d91aa7a9c96fc6bd16cdd53d5b037f8ed804983` (324 identities).
- `20261009-live-create-stream-final-verification.json`: full return/endpoint,
  callback fields, instance publication, source absence, pins and process checks.
- Model `tests/recovery/live-create-stream-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-create-stream.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-create-stream.py`.

The model extracts the remaining44cycles after the previously executed20-cycle
binding prefix from the retained64-cycle creation model. Both creation-return
and outer-stream retained captures match before live prediction. The independent
stream model uses widened integer carry/borrow for SH-4 division; original guest
division code executes in the native run. The checkerED1BAA is the only new body;
existing callback setter, caller, stream helper and division bodies are reused.
Symbol audit confirms output setupED0620 is absent. Model and one native run pass.

Next inventory SHA256:
`894f6aff04ce90d3a6887bf6f63324ab02d6ae2c42a94de5347715c64e06e956`.
Earlier output FPU evidence covers exact inputs2/2/1000/0.5 and scalar4000; verify
current inputs rather than assume that evidence applies. The retained output
wrapper's allocator is already linked and has live slots0–3 occupied; its next
allocation must be modeled, not fabricated or treated as a missing helper.
General validity/stream paths, FPU/vector accuracy and physical acceptance remain
open. Production G1 is unchanged.

**Previous recovery — 9 October 2026: live MOVIE instance returns to creation caller.**

The original MOVIE instance constructor now returns handle8CEE2210 to caller
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-instance-return/live-instance-return-checkpoint.json`:
  `1e908cacd7550302f9618bc5fdf5b291bcdb8d0630ddb79bbba8555ffefa368d` (2,794 identities).
- `20261009-live-instance-return-prediction/prediction-checkpoint.json`:
  `62ae0f8326e95b973859929d885ac99d71b59a9a008a5d77f5962ebd8813d86b` (357 identities).
- `20261009-live-instance-return-final-verification.json`: full return/endpoint,
  direct-BSR target decode, source absence, pins, process and storage verification.
- Model `tests/recovery/live-instance-return-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-instance-return.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-instance-return.py`.

The composed source model reproduces five retained stages before predicting live
inputs. Original ED511A/ED178C and retained indexed-query/dispatcher/initializer/
constructor bodies run; existing ED1AFA then reaches its actual missing checker.
The callback argument is prepared, not yet bound. The constructor return observer
uses the real entry verified in original caller code and current live literal;
no observer repair or repeated native run was needed. All model/native checks pass.

Next inventory SHA256:
`b664a0cbc62ea33eb070eebd5c9e4a253575c018c6556a57e669941366ca8c29`.
It verifies the retained validity/checker, connected creation tail and repaired
outer-stream checkpoint. Subsequent stream arithmetic and output setup remain
unproven live. Captured successful initialization and current slot layout do not
prove failure branches or alternate layouts. Broader FPU/vector and physical
acceptance remain open; production G1 is unchanged.

**Previous recovery — 9 October 2026: live MOVIE mode setup returns to dispatcher.**

The MOVIE setup branch now returns to its dispatcher in live startup. Five
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-mode-return-repair1/live-mode-return-checkpoint.json`:
  `9ffc3f4fbdd0d339909ea1f5dae872bb01d411a345048d7e344775b3bad32e38` (2,718 identities).
- `20261009-live-mode-return-prediction/prediction-checkpoint.json`:
  `d9359bcbb7bf08fc6c827e810eb8f262abe6a2495c6ccc892d82eecca3f0592f` (373 identities).
- `20261009-live-mode-return-final-verification.json`: return/endpoint CPU/RAM/TMU,
  mode/state stores, source absence, pins, process and storage verification.
- Model `tests/recovery/live-mode-return-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-mode-return.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-mode-return-repair1.py`.

The source-derived model first reproduces seven retained stages, then predicts
live state without guest execution. Three new MOVIE manifests contain17 original
entries, appended at original activation with uniqueness checks. New resident
state-request/query routines and original mode-install/slot/validator bodies
execute; retained refresh/installer and dispatcher code are reused.

The first run reached the expected endpoint and passed endpoint comparisons but
lacked the requested intermediate return capture: its observer was attached to
internal continuationED136A. Current slot3 table8CEDC770+0C actually dispatches
ED1326. A fresh observer/runner/output targets that entry; all439 captures shared
with the first run are identical, and the additional return capture passes.
Game code and frozen predictions did not change. Preserved failure checkpoint:
`20261009-original-startup-live-mode-return/mode-return-observer-failure-checkpoint.json`,
`3c2e71a8dc43ed02ee156b9c468df95fed716c1dcf9b7b3c20c218b21905a0d6` (2,699 identities).
Observer explanation: `work/dreamcast-recovery/live-mode-return-observer-repair.json`.

Next inventory SHA256:
`898e22f9697e8306504ab1ea634d0de842187c7897c06afd909fce033bd3f04c`.
Retained candidates are82cycles to ED05C4 and a further64 to ECF1B0; actual linked
coverage and caller continuation still require audit. Only state1's short path
and validator mode15/value2 are covered here; long state paths, other validator
callbacks, pending-interrupt behavior and general FPU/vector accuracy remain open.

**Previous recovery — 9 October 2026: live setup return and MOVIE state update verified.**

Handler setup now returns to MOVIE in live startup, then the original state update
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-setup-return/live-setup-return-checkpoint.json`:
  `78244f2d837c3fd7643121f6f48fcd034bf7510fbd8b2b127a9f985ffede3c41` (2,637 identities).
- `20261009-live-setup-return-prediction/prediction-checkpoint.json`:
  `18fab4439bfe60d3a53787f4515eb5c2527bec421276a673866ab5500292e1b8` (335 identities).
- `20261009-live-setup-return-final-verification.json`: return/endpoint CPU/RAM/TMU,
  channel binding/flags, movie state, IRQ restoration, sources, pins and process checks.
- Model `tests/recovery/live-setup-return-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-setup-return.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-setup-return-repair1.py`.

The composed model first reproduces all four earlier retained stages. Native
integration adds original65F4A4/65F4B8/ED161A; symbol audit found no overlaps.
The five ED161A entries are appended only during the original MOVIE load, with
address-uniqueness checks. The model does not execute guest code. The live run
keeps ARM scheduling enabled and does not reset guest state.

A preflight lookup found that the aggregate prediction checkpoint omits the
original module-manifest file. The fresh repaired runner authenticates it directly
through its original status-toggle checkpoint; no compilation/native execution
occurred before that correction. Record:
`20261009-live-setup-return-preflight.json`. The model and one native run pass.

Next inventory SHA256:
`c73dfcad810a3ab82fb3c31b673a87de9ef77b0717f2f858a8ca8b045a53ce11`.
State-request long paths and validator mode5/6 callback branches remain open.
No pending-interrupt, general FPU/vector/geometry, decoded movie, normal boot,
audio quality, gameplay or device acceptance claim. G1 remains unresolved.

**Previous recovery — 9 October 2026: original idle reset completes in live startup.**

The original idle reset now completes in live startup: record status reset, buffer
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-first-reset-repair1/live-first-reset-checkpoint.json`:
  `f219eb8d5dd2e951cfa2401b022c5ea1c96e8ba5060bbcf19ede4148d40039dd` (2,505 identities).
- `20261009-live-first-reset-prediction/prediction-checkpoint.json`:
  `c3d14a64d291e7e95e2135b359cead5091af3c9036b7287a357357aa3452b3e0` (316 identities).
- `20261009-original-startup-live-reset-completion/live-reset-completion-checkpoint.json`:
  `a25f89781dfbdd40a8e3712d79c080116fd1158f6655a8b68f26e595ba388d00` (2,559 identities).
- `20261009-live-reset-completion-prediction/prediction-checkpoint.json`:
  `46e8df15ea86560489df8eba106dcc7d6891a49f45559d0420c9be58abb7ce51` (377 identities).
- `20261009-live-reset-completion-final-verification.json`: full identity/model,
  source-absence, pinned-reference, process and storage verification.

Both models first reproduce all retained stages (three and eight), then predict
live inputs without executing guest code. Symbol audits show no collisions for
the nine objects. An inherited assertion expected65F238 to remain absent and
stopped the first integration before guest execution; repaired in fresh source/
runner/output paths. Preserved failed checkpoint:
`20261009-original-startup-live-first-reset/first-reset-registration-failure-checkpoint.json`,
`d33ad7566c9eb4de868c74fd458ece772f1dcfd75bd245db7cf7361a9e24c975`.
Both subsequent live integration runs pass. Model/runner names are
`live-first-reset-model-20261009.cpp`, `live-reset-completion-model-20261009.cpp`
under `tests/recovery/`, and `run-original-startup-live-first-reset-repair1.py`,
`run-original-startup-live-reset-completion.py` under `work/dreamcast-recovery/`.
Next inventory SHA256:
`d2fea1470c620f38471f1f5b581eafc5cbfcfd81af382ee3b5c9fa84918e535d`.

Only the captured idle/status-matching path is established. Active transitions,
general FPU/vector/geometry behavior, decoded movie, normal boot, audio quality,
gameplay and device acceptance remain unproven. Production G1 remains unchanged.

**Previous recovery — 9 October 2026: live MOVIE handler and reset entry verified.**

Original MOVIE setters, handler allocation/binding and reset entry now match the
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

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-movie-handler-saved-verification.json`: all saved CPU/RAM/TMU,
  hardware invariants,285 prefix files, pins and source absence verified.
- `20261009-original-startup-live-movie-handler-repair1/live-movie-handler-counter-failure-checkpoint.json`:
  `78b214665bd61c7cdc360a2d030c3afcaa014b2fe228d87c8fccfbf57f6c6bbf` (2,452 identities).
  Failed original assertion/report preserved; separate verification is authoritative
  for this segment's acceptance.
- `20261009-live-movie-handler-prediction/prediction-checkpoint.json`:
  `1540ffe186b19a2241552b259910b93180ec9f0e5c5fed3afe06db7fa128a1e8` (367 identities).
- `20261009-original-startup-live-movie-handler/live-movie-handler-link-failure-checkpoint.json`:
  `65bab80028886af657bb8deb8c6d84e73a47ba8629e78279ae525acca7bf70e2`.
- Verifier `work/dreamcast-recovery/verify-live-movie-handler-saved-repair1.py`.
  Initial verifier needed the original audio-source checkpoint, because the aggregate
  checkpoint includes its object but does not transitively include its source.
- Next inventory SHA256 `12b2f5eb56ca3e159cc349b5d91d9dc753bfc7e61daf46f2361c20a04256704d`.

**Previous recovery — 9 October 2026: original resident constructor returns to MOVIE in live startup.**

Both buffer queries and the original resident constructor now return in live
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
boot, audio-quality or gameplay proof. Goal active; previous/current turns are progress.

Two retained bodies add166 original instructions: channel query66C37C and buffer
query65E640. The composed model passes four earlier stages (29+61+278+76cycles)
then predicts actual live inputs. Original getter, division, IRQ restore6607B8,
scalar parent tail and saved frames execute. Separate capture at ED144C proves the
real return before the caller continues. All main RAM and timer state match.
AICA RAM/SQ/flash/VRAM/MMIO/GD/G2 remain unchanged; audio processor stays active
without faults. The mask-restoration result is for this no-pending-interrupt state,
not general preemption coverage. Broader floating-point/vector accuracy stays open.

The additional eight-cycle caller segment uses the sealed source-derived MOVIE
continuation, rebased on live return registers/stack. No RAM writes occur; full
CPU and TMU match at the actual missing648342 with r5=60. Native startup was not
repeated for that saved-capture verification. Model build and full startup passed.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-resident-return/live-resident-return-checkpoint.json`:
  `c74b1a20f20fa980fd525d33642ce28a0735c981fd9a5a6856f47d32cb74ae39` (2,396 identities).
- `20261009-live-resident-return-prediction/prediction-checkpoint.json`:
  `7ebbda7562d1c90732eca152564fd685c9fd066179d0eb9b197b7aa42dbd4429` (313 identities).
- `20261009-live-resident-return-final-verification.json`: also verifies296 retained
  post-resident identities, full return/continuation, source absence and pinned refs.
- Model `tests/recovery/live-resident-return-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-resident-return.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-resident-return.py`.

Next inventory SHA256:
`365f9f6cab46438583030ec64579bfe586e3f8bcedb4519574c717efc9004de4`.
It identifies rate/flag setters648342/648412, allocator662106, initializer662194,
setup64816A and reset6481A8 plus retained MOVIE continuation models. Current active
MOVIE already reaches ED145C correctly. Do not apply the earlier isolated fixture's
39 data-word activation replay to live startup. Check current pool occupancy, saved
PR and original critical-section nesting; the retained scan/clear paths are inputs
to a new prediction, not a reason to edit live guest data. Normal boot, decoded
movie, audio quality, gameplay and physical acceptance remain unproven. G1 unchanged.

**Previous recovery — 9 October 2026: original stereo setup completes in live startup.**

Five retained stereo bodies (166 original instructions) now run in live startup.
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
No decoded movie, normal boot, audio-quality or gameplay proof. Goal active; previous/current turns are progress.

The composed model matches five prior retained stages before application to actual
live input: outer/count prefixes27cycles, count forwarding664, pan adapter103,
first forwarding270, second pan/return444. Existing original signed division and
callbacks execute unchanged. The real stereo return has a separate full-CPU capture
comparison; endpoint comparison covers all main RAM and timers. No forced return,
state reset or relocated/template fallback occurred. Command queue insertion remains
distinct from firmware consumption or sound-quality acceptance.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-stereo/live-stereo-checkpoint.json`:
  `31d481b2ae8527fa19e3baff6e4bb0d1e547d5fa4c6617146c73148f65d48c94` (2,329 identities).
- `20261009-live-stereo-prediction/prediction-checkpoint.json`:
  `78926b09b739cd76f8eb0925b27191ed47314ae56e15551afb7efa95085cd538` (325 identities).
- `20261009-live-stereo-final-verification.json`: full prediction, both pan values,
  four command packets, source absence, pinned references and process hygiene pass.
  IRQ1,226 and gain1 unchanged; original service/outer handles preserved.
- `20261009-live-stereo-model-extraction-preflight.json`: extraction guard and
  consequent missing-model preflight failure, both before build/native work.
- Model `tests/recovery/live-stereo-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-stereo.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-stereo.py`.

Next inventory SHA256:
`d562a6aa3a6fa7a22106c0100f034fc5986db797b39ce32b74fbd5275335a520`.
It identifies channel-query66C37C and buffer-query65E640 objects plus retained query/
parent return models. Earlier chain29+61+278+76=444cycles returns ED144C, including
original IRQ restoration and a scalar parent tail. Check actual saved PR, incoming
scalar/stack, pending interrupts and live hardware before accepting that prediction.
Do not replace the original interrupt path or import inactive-device assumptions.
General FPU/vector/geometry correctness, media decode, normal boot, gameplay and
physical acceptance remain unresolved; production foundation/G1 remain unchanged.

**Previous recovery — 9 October 2026: service construction returns its handle in live startup.**

Four retained bodies (88 original instructions) now complete both activation-command
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
boot, audio-quality or gameplay proof. Goal active; previous/current turns are progress.

The composed model matches all three earlier retained stages: both start-command
submissions281cycles, configuration/service return139cycles, and record initialization/
caller51cycles. Actual incoming live CPU/RAM/TMU then supplies the frozen prediction.
The real service return has a separate captured full-CPU comparison at6609F4;
the subsequent handle store and complete record state are covered at the endpoint.
Model build and native startup both passed on their first attempt. No dependency
return substitution, state reset or relocated/template fallback occurred.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-service-return/live-service-return-checkpoint.json`:
  `2f76d9db35858ac9810a82c8a88760d1ffc553909f349ab406a74632b11b42e8` (2,257 identities).
- `20261009-live-service-return-prediction/prediction-checkpoint.json`:
  `c1043a3e176e250f0a742e29dcbf38ae3dfc4f851560f999e9f3f1cfe734bedd` (299 identities).
- `20261009-live-service-return-final-verification.json`: full prediction, service
  handle, command46 packets at8C8D8CC0/8C8D8CDC, source absence, reference pins and
  process hygiene pass. IRQ1,226 and gain1 remain unchanged.
- Model `tests/recovery/live-service-return-model-20261009.cpp`; predictor
  `work/dreamcast-recovery/predict-live-service-return.py`; runner
  `work/dreamcast-recovery/run-original-startup-live-service-return.py`.

Next inventory SHA256:
`1d1469c6976f95061d67286b7a9c494c0b2568da298297a09faadf26c0193673`.
It identifies outer/count adapters, count-forward, pan adapter and pan-forward
objects, plus the retained continuation models. Expected earlier chain is
27+664+103+270+444=1,508cycles to66C37C. Preserve actual queue count4/index44,
service handle, stack and live device state. Command insertion does not establish
firmware consumption or audible output. General FPU/vector/geometry, normal boot,
media decode, full gameplay and physical acceptance remain open. G1 stays unresolved.

**Previous recovery — 9 October 2026: channel-pair construction and both queued commands verified live.**

Four retained channel bodies (457 original instructions) now execute in live
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
Simulator. No decoded movie, normal boot or gameplay. Goal active; previous/current turns are progress.

The composed source model first matched all five retained stages (pair prefix,
count prefix, first callback, second callback, field/pair/caller tail), then froze
its prediction from the real incoming live state. This turn's model build and
full startup run both passed on their first attempt. No guest state or dependency
return was replaced. Exact root return66BBB8 and no relocated fallback are asserted
in the live observer. The experiment preserves the existing active device runtime.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-channel-pair/live-channel-pair-checkpoint.json`:
  `d1ec48d40d7076a26ed8844e21e649461e313f571f87412e60d44a4efc6b850d` (2,185 identities).
- `20261009-live-channel-pair-prediction/prediction-checkpoint.json`:
  `1b6957d755b1f182f6c5c802d35bc711f2c775d1c3cda3badc6a371a76b831d8` (322 identities).
- `20261009-live-channel-pair-final-verification.json`: full end-state prediction,
  two command38 packets at8C8D8C88/8C8D8CA4, prior captures, source absence, reference
  pins and process hygiene verified. IRQ count1,226 and gain1 remain unchanged.
- Model `tests/recovery/live-channel-pair-model-20261009.cpp` with recorded retained
  source fragments; predictor `work/dreamcast-recovery/predict-live-channel-pair.py`.
  Runner `work/dreamcast-recovery/run-original-startup-live-channel-pair.py`.

Next inventory SHA256:
`7f8f07d25a1074050c60e9131fc4734cece0448e863bcf3aa920bf0867d0e7da`.
It verifies retained channel-start wrapper/body, configuration and record-init
objects/models, four bodies/88instructions. Original partial replay reached6618D6
via281+139+51cycles. Recompute from current live inputs before integration; preserve
actual saved PR, queue occupancy and all device state. General FPU/vector/geometry
accuracy, media decode, audio playback, normal boot and physical acceptance remain
unvalidated. This result changes neither the production foundation nor G1.

**Previous recovery — 9 October 2026: both buffer setups now verified in saved live startup.**

The saved live second-buffer and four-allocation segment now matches the retained
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
Goal active; verified progress.

Evidence under `artifacts/recovery-aot/`:

- `20261009-live-buffer-pair-model-repair1/model-checkpoint.json`:
  `78886c624a828f87857faeacc1eb1fc4532a50a37f0f554ac2a0fb2391a4db95` (294 identities).
- Failed compile preserved at `20261009-live-buffer-pair-model/model-failure-checkpoint.json`:
  `dbe2b527782310373262078e9e0bbef97cbadf0bba2e52748637b76d3c0dceb8` (281 identities).
- `20261009-live-buffer-pair-final-verification.json`: both model comparisons pass;
  also rechecks all 2,136 original live checkpoint identities and source absence.
- Driver `tests/recovery/verify-live-buffer-pair-model-repair1.cpp`; runner
  `work/dreamcast-recovery/verify-live-buffer-pair-model-repair1.py`.
  Extracted header is asserted byte-identical to the sealed model prefix.

The second buffer writes 001F7F80 at AICA address000141F0 and00004000 at000141F4;
the following four allocations and actual stack/register state all match. This
closes the explicitly open later-segment check from the previous checkpoint.
It does not extend execution, validate ARM instructions independently, or establish
audio quality. No guest state, production runtime or prior evidence was modified.

Retained next-batch reference:
`20261009-startup-resident-service-pair-complete-repair1/service-pair-complete-checkpoint.json`
(`b542108bbaf45b33f1ff3f7c72367f3bfb032fe2513c7fa6dd67fea43f1b953b`).
That partial replay returned the pair and reached66D184, with device clocks inactive;
its result must be verified anew against the live incoming state and active devices.

**Previous recovery — 9 October 2026: original buffer return and audio-memory writes verified live.**

Original 65E470 and explicit 66AF40 now execute in the live startup with the original
PIO write chain. The first complete buffer return at 66BA16 matches a frozen
source-derived model for all CPU fields, full main RAM (113 ordered stores), TMU
and full AICA RAM over 1,331 cycles. The only sound-memory changes are original
word writes 001FBFC0 at 000141F8 and 00004000 at 000141FC. No relocation/template
fallback occurred. All 114 earlier live capture files remain identical.

The ARM audio processor remains enabled and advances 150 scheduled steps during
this verified sequence. There are no additional AICA RAM changes; fault count
remains zero, IRQ count 1,226 and gain 1. SQ, flash, VRAM, MMIO, GD and G2 snapshots
are unchanged. Historical G2 DMA state is preserved. This verifies writes and
execution state, not audible playback or a separate ARM instruction oracle.

Startup then continues 2,185 more cycles through the second buffer and reaches
missing 66CFC4 at PC66BBA0/PR66BBB8 through r3. Final clock is 792,925,502. That
later capture has only two additional AICA word changes: 001F7F80 at 000141F0 and
00004000 at 000141F4. Its full CPU/RAM/TMU transition is still to be independently
verified; do not extend the first-return proof to that later segment.

The initial attempt stopped before original startup because the observer checked
66E97C before normal callback registration. The retained body and table entry were
present. A fresh source moves the assertion after normal registration; the failed
attempt and its report remain sealed. No game state or dependency return was altered.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-buffer-return-repair1/live-buffer-return-checkpoint.json`:
  `3078eed34807913271ca81741515f543954873a278802e6dfa52f0dba45382a2`.
- `20261009-original-startup-live-buffer-return/live-buffer-failure-checkpoint.json`:
  `459d82a14ea1034e63245a3ad8ffcb87141f3dd54e369f5139ba06380b1ea986`.
- `20261009-live-buffer-return-final-verification.json`: 2,136 live, 314 earlier
  replay and 1,939 failed-attempt identities verified. Independent model passes
  both the old replay and actual live return.
- Model: `work/dreamcast-recovery/predict-live-buffer-return.py`.
  Successful runner: `run-original-startup-live-buffer-return-repair1.py` in that directory.

Next model the captured later 2,185-cycle segment from the first live buffer return
through the second buffer and retained allocations. Reuse source expectations in
`tests/recovery/startup-resident-buffer-pair.cpp`, with actual incoming counters,
stack and RAM. Retained checkpoint
`20261008-startup-resident-buffer-pair/buffer-pair-checkpoint.json` is
`75404423560d26656079344d297e69572add84a16c665ba9d3736e37a3fa0cec`.
After verifying existing evidence, batch the retained dependencies beginning 66CFC4.
No native rerun is required just to verify the captured later segment.

Default source/emitter unchanged; audio/FPU candidate unpromoted. General FPU/vector
accuracy, decoded movie, normal boot, gameplay and device acceptance remain open.
All 1,657 missing source paths and supported PAL GDI remain absent; G1 unresolved,
source-only request unsent. 44.39 GB free; no native process or booted Simulator.
Goal active; latest turn is verified progress.

**Previous recovery — 9 October 2026: live constructor/resource batch reaches buffer creation.**

Seven retained original bodies (590 instructions in total) are linked into the
same live startup. Only their reached prefixes are claimed: the 133-instruction
second constructor and 393-instruction service constructor have later paths still
ahead. The independent integer model matches all CPU fields, 31 stores across the
full RAM image, and the 307-cycle TMU transition. It also independently reproduces
the sealed earlier replay. All 95 earlier live capture files remain byte-identical;
the new bound entry matches the preceding missing-call state except PC.

The original path validates two descriptors, saves/raises the interrupt mask,
selects real free pool entries without altering occupancy, and allocates resources
8C8EE064/8C8EE06C with IDs 15/14. It reaches missing 65E470 through r3 at
PC66B9EE/PR66BA16, with r4=52535053 (SPRS), r5=15, r6=001FBFC0 and r7=00004000.
Final clock is 792,921,986 cycles. Original IRQ restoration remains ahead.

Across the batch SQ, flash, AICA RAM, VRAM, MMIO and GD snapshots are unchanged.
AICA advances 35 scheduled steps, fault count stays zero, IRQ count stays 1,226 and
gain stays exactly 1. The original device clock remains enabled with host sync off.
These are runtime observations, not an independent ARM trace or audio acceptance.
Default source/emitter unchanged; the audio/FPU candidate remains isolated and
unpromoted. General FPU/vector/geometry and previous guest-state differences remain
open. No movie decode, normal boot, gameplay or physical-device proof.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-constructor-prefix/live-constructor-prefix-checkpoint.json`:
  `e0f774b08a5a7d8a0e9300fcc7eeb5d3eeab44b07941dc1bc76f502a8f205fd8`.
- `20261009-live-constructor-prefix-final-verification.json`: 2,086 live and
  535 retained-replay identities verified; prospective model passes both captures.
- Frozen seven-unit input list:
  `work/dreamcast-recovery/live-constructor-integration-inventory.json`.
  Model: `work/dreamcast-recovery/predict-live-service-prefix.py`.

Next integrate original 65E470 with retained 66AF40 and the original sound-word
write chain, then its reached constructor dependencies. Prior partial replay
`20261008-startup-resident-buffer-complete-repair1/buffer-complete-checkpoint.json`
is `8447bb3c1360bf54795121f5459d3eee888c4d86f8a550c4e3c3a0bed2910199`;
it covers 1,331 cycles and two AICA word stores. Rebase its RAM/stack/sound prediction
on the live snapshot and preserve active ARM/device scheduling. The live table at
8C7B0A20 has idle word 1, base A05F7800 and channels 0–3; G2 capture includes five
historical DMA runs and idle channel control fields. Do not reset those counters
or assume a fresh hardware fixture. Later retained buffer-pair evidence is available
but is not live proof.

All 1,657 missing source paths and supported PAL GDI remain absent; G1 unresolved
and source-only request unsent. 44.83 GB free; no native process or booted Simulator.
Goal active; this turn is verified progress.

**Previous recovery — 9 October 2026: complete rate calculation and channel initialization verified in live startup.**

Two sequential native integrations reuse retained original bodies. The first adds
11152C, 1118DC and 111E38 beside the existing 1117D8 reducer, allowing original
65EB00 to return to 64890C. A rational model frozen before execution predicts all
CPU fields, all RAM stores, both output halfwords (1CA6/F32D) and the 284-cycle TMU
transition. Native capture matches exactly. The 38 earlier prefix files are unchanged.
Startup continues to the real missing 66B3CC call through r2 at 65F3E6.

The second adds retained 66B3CC binding and 65F346 initialization. An independent
integer model from the actual captured live stack verifies their 75-cycle path
through the channel constructor return at 647F74, including all CPU/RAM/TMU and
record 8C8A7B90. Its 57 earlier capture files are unchanged, including the complete
rate return. Binding entry matches the prior missing-call state except bound PC.
An initial prediction preflight incorrectly retained the older partial-replay
return assertion 647F18. That failed before any native build/run; its source and
failure record are preserved. A fresh predictor uses the actual saved PR 647F74,
confirmed by original call 647F70 to 65F364. No runtime state was adjusted.

Across both verified sequences, SQ, AICA RAM, flash, VRAM, MMIO and GD snapshots
remain unchanged. AICA advances on its live clock (32 and eight scheduled steps);
this is not an independent ARM trace or audio acceptance. IRQ count remains 1,226,
AICA faults zero and gain exactly 1. Final startup reaches missing 65F214 at
PC647F7A/PR647F84 through r2, at 792,921,679 cycles: 469 beyond the original rate
entry. Default source/emitter unchanged; the audio/FPU candidate is unpromoted.
General vector/FPU/geometry accuracy and the earlier guest RAM/MMIO differences
remain open. No decoded movie, normal boot, gameplay or device acceptance.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-rate-return/live-rate-return-checkpoint.json`:
  `3222191a37ac29b54a38444dddb3301c557780d52881fcedb222e89d85947bb1`.
- `20261009-original-startup-live-channel-return-repair1/live-channel-return-checkpoint.json`:
  `cc75a93fa07dd63c28d5c34c59425b56423f310c1caf52def6f74ee8b56a0645`.
- `20261009-live-rate-channel-final-verification.json`: 4,040 sealed identities
  checked, both frozen predictions/returns verified and current G1/process audit.
- `20261009-original-startup-live-channel-return/preflight-failure.json` preserves
  the failed return-address assumption. Successful predictor is
  `work/dreamcast-recovery/predict-live-channel-return-repair1.py`.

Next batch retained constructor dependencies starting 65F214, whose four-cycle tail
reaches 66093E. Source unit checkpoint
`20261008-startup-resident-constructor-tail/constructor-tail-checkpoint.json` is
`814360b4b45de2799b3f1dd14195bf77f25d95da92fbbe1f1bb7d940e8eeba67`.
Reuse verified units and derive predictions from this live prefix rather than
assuming old partial-replay frames match. Keep original clocks/device state.
All 1,657 missing source paths and supported PAL GDI remain absent. G1 unresolved;
source-only acquisition request unsent. 45.12 GB free; no native process or booted
Simulator. Goal active; this turn is verified progress.

**Previous recovery — 9 October 2026: audio floating-point boundary corrected in an isolated candidate.**

The candidate's AICA register writer and mixer inherited guest rounding/status.
Patch `0014-arm64-fpu-audio-domain.patch` adds the existing host-environment scope
at these two boundaries, restoring the caller on normal return and unwind. The
change exists only in `work/dreamcast-recovery/arm64-fpu-audio-domain-candidate/`;
default source and the prior sealed candidate remain unchanged.

Synthetic public-runtime tests cover 64 combinations of caller rounding/denormal,
guest rounding/denormal and normal/unwind paths, with all 16 master volumes and
four mixed frames per control. Negative controls reproduce 32 gain differences,
64 register-status leaks and 64 mixer-status leaks. The scoped candidate has zero
of those differences/leaks and retains correct guest arithmetic after audio work.
The fixture has no PCM differences even before the fix; no audible-defect or audio
quality claim is made. Existing 64 dispatch controls and eight renderer cases pass.

A fresh sequential live-startup run confirms gain exactly 1 at integration entry,
65EB00 entry and missing 11152C. Every other captured file at those three stages
is byte-identical to the previous candidate; only the hardware-text gain field
changes. Original scalar arithmetic still matches the rational expectation, full
CPU/RAM/TMU and 25-cycle transition. Earlier four-XF/143-RAM-byte/one-SQ-byte/28-MMIO
A/B differences are unchanged. Their broader correctness, FTRV/FIPR, denormals and
exceptions remain open. Candidate remains unpromoted.

Evidence under `artifacts/recovery-aot/`:

- `20261009-fpu-audio-domain-boundaries/audio-domain-checkpoint.json`:
  `1ddc595451f6826b4d55698e624f9fb34b3608e2259ebfd91b1f55e84b1c3b3d`.
- `20261009-fpu-audio-domain-clean-status/audio-domain-checkpoint.json`:
  `0ccf090e0da7dce9af464d77c1ab48a85ef0af9194cdd4da1d28510384879592`.
  This fresh fixture clears status before mixing to expose previously masked leaks.
- `20261009-original-startup-live-fpu-candidate-audio/live-fpu-audio-checkpoint.json`:
  `e0583010b8d305098442444f9a9f5d1fdf12b7677efa60d21963815ea0e03fd5`.
- `20261009-original-startup-audio-domain-final-verification.json`: 3,973 sealed
  identities rechecked across four checkpoints, with all stage comparisons.

Next continue the isolated live path through the already retained 11152C wrapper,
1118DC classifier and 111E38 polynomial, keeping original 1117D8 reducer. Their
`20261008-startup-rate-complete/rate-complete-checkpoint.json` is
`c98e2227fc3f307b6106fb6178eed14da0912e7c6965fc89ad6135cb6393216e`.
It has 24 partial-replay controls and an independent 284-cycle full-rate model;
rebase predictions on the captured live entry and validate the actual return
before adding later dependencies. Do not treat partial replay as live integration.

All 1,657 missing source paths and supported PAL GDI remain absent. G1 unresolved;
source-only acquisition request unsent. 45.58 GB free, no native process or booted
Simulator. No decoded movie, normal boot, gameplay or physical-device acceptance.
Goal active; this turn produced a verified correction, not a repeated blocker.

**Previous recovery — 9 October 2026: live startup confirms the scalar FPU fix and exposes wider differences.**

Two sequential native runs execute original 65EB00 from the live startup and stop
at missing 11152C. The default reproduces nearest-rounding results despite guest
FPSCR 40001 requesting toward zero. The isolated candidate yields product 4544597a
and ratio 3d91e52f, matching the frozen rational expectations. Full CPU, RAM and TMU
transitions match each run's captured entry and its 25-cycle prediction. Entry and
final store queues are now captured. This confirms the bounded scalar correction;
it does not establish complete floating-point correctness.

The A/B comparison also finds earlier startup differences: four XF registers,
143 RAM bytes, one store-queue byte and 28 MMIO values. AICA master gain is printed
as 1 in default and approximately 0.9999999999999999 in candidate. This needs a
host floating-environment boundary audit before promotion; neither these changes
nor the general FTRV/FIPR/geometry behavior are cleared by the scalar result.
Guest clocks and IRQ count agree. Captured AICA RAM/ARM/timers/slots, flash, G2,
GD, VRAM and TA bytes agree across A/B. Hardware struct bytes are ABI-specific.

Candidate link replaces 226 direct cached generated objects and the generated
archive with strict versions and adds the candidate runtime/rate-prefix object.
The 19 newly integrated integer units retain their previously verified objects.
Default source and emitter remain unchanged; the candidate is not promoted.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-live-fpu-default/live-fpu-default-checkpoint.json`:
  `b1fb273d6da797ad596da06bccfcb9e59ba0c809465887c9ba1c7654b7f24b3a`.
- `20261009-original-startup-live-fpu-candidate/live-fpu-candidate-checkpoint.json`:
  `ea033908ebdcd086a9cb66b33e924e88bdcc41fef72894c51bbb994b29390d48`.
- `20261009-original-startup-live-fpu-final-verification.json`: 1,731 default and
  1,961 candidate identities checked, with full A/B field and byte differences.
- Fresh observer `tests/recovery/board-join-live-fpu-comparison-20261009.cpp`;
  runner and verifier `work/dreamcast-recovery/run-original-startup-live-fpu.py`
  and `verify-original-startup-live-fpu.py`.

Next audit the candidate's host floating-environment ownership and the changed
AICA gain/MMIO calculations. Then continue the isolated live path with retained
11152C and its reached dependencies, preserving original clocks and snapshots.
All 1,657 missing source paths and supported PAL GDI remain absent. G1 unresolved;
source-only acquisition request unsent. 45.70 GB free, no native process or booted
Simulator. No decoded movie, normal boot, gameplay or device acceptance. Goal active.

**Previous recovery — 9 October 2026: 21 retained routines integrated into live startup.**

The original startup now executes the retained integer chain fromED8536 through
buffer acquisition/binding, fourth-handler setup and resident resource/channel/
work/rate pools. Nineteen retained units supply21 bodies and961 original instructions.
The experimental closure is1079 bodies with1287 MOVIE activation entries, including
70 distinct new module addresses and seven resident registration functions.
No code was regenerated and the default runtime/emitter identity is unchanged.

The live path advances2,820 modeled cycles to792,921,210, reaching missing65EB00
atPC6488EC/PR64890C withr3=65EB00. Full CPU,16MiB RAM,TMU and store queues match
the previously frozen rate-pool endpoint. Inputs are500/44100, output pointers
894930/894932 and FPSCR40001. Original startup prefix state matches its prior capture
exceptPC, which bound dispatch changes toED8536. Prefix RAM,AICA RAM andflash match.
The new entry label did not save SQ, so no separate prefix-SQ claim is made.
The final SQ capture matches the retained prediction exactly.

Guest clocks remain enabled, host sync off, IRQ count1226. AICA advances318 scheduled
steps and one mixer frame. Across this batch AICA RAM,flash,VRAM,MMIO andGD state
remain unchanged. These hardware observations are not an independent ARM instruction
trace or movie/audio/gameplay acceptance. The capture runner's postcheck failed on
the absent optional entry-SQ file; its report remains preserved. A separate verifier
confirms the actual completed native path and the full endpoint without rerunning it.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-integer-integration/original-startup-integer-integration-checkpoint.json`:
  `053f450f7ddd70c0819a11a5eb34f54f3a3ec82b3b0f8933d6598bd1a0ea1b27`.
- `20261009-original-startup-integer-final-verification.json`:1,703 sealed identities
  and5,539 retained-unit identities rechecked; full endpoint and prefix checks pass.
- Frozen integration input list:
  `work/dreamcast-recovery/startup-integer-integration-inventory.json`.
  New source `tests/recovery/board-join-integer-integration-20261009.cpp`;
  runner `work/dreamcast-recovery/run-original-startup-integer-integration.py`.

Next65EB00 contains the already diagnosed ARM64 rounding boundary. Preserve this
live-runtime startup prefix for a bounded default/candidate comparison against the
existing rational arithmetic expectations, stopping at missing11152C. Candidate
`20261008-startup-movie-resident-rate-domain-candidate` has eight captured-prefix
controls, but remains separate from default source and production acceptance.
Do not substitute a computed result or promote0013 on this evidence alone. General
FTRV/FIPR, geometry and floating-environment ownership requirements remain open.
When cloning the runner, correct the entry-SQ capture/check mismatch in a fresh
source/output; do not overwrite this checkpoint or its failed postcheck report.

All1,657 missing source paths/PAL GDI remain absent, G1 unresolved and acquisition
request unsent.46.02GB free; no native test/game/Simulator. Pinned SunPad, default
source and FPU promotion status unchanged. No decoded movie, normal boot or gameplay.
Goal active.

**Previous recovery — 9 October 2026: original startup hardware captured without state changes.**

The fresh read-only observer reaches the existing full-startup missingED8536 at
PC8CED2F7C/PR8CED2F82, withr2=8CED8536. It preserves the original live runtime.
All six prior state/RAM/AICA-RAM/flash/SQ byte captures match exactly, including
CPU,TMU and the hardware fields already embedded in the state file.

New diagnostic evidence contains145 hardware fields,1,940 MMIO values,two disc
track records,GD state,AICA ARM context/timers/slots,G2 channels,VRAM andTA buffers.
The original startup has guest device clocks enabled with host sync off. At this
boundary:792,918,390 modeled cycles,1,226 interrupts taken,314 sectors and643,072
bytes read. Its masks are IRQ11 normal0x7F000,IRQ9 normal0x2807EC and error0xE.
The drive is mounted; AICA ARM fault count is0. These are startup observations,
not movie-decode/presentation or gameplay acceptance.

**Correction to the previous audit:** the older state.txt captures already include
Holly masks/pending, partial AICA/G2 state, timer fields and cycle counts. The
filename-only search missed embedded data. The conclusion that the isolated
fixture fails to restore those families remains valid, but acquisition was partly
available already. New capture fills additional diagnostic gaps at the earlier
ED8536 boundary; it cannot be transplanted into the laterEB0254 snapshot.
Raw diagnostic structs use the exact local C++ ABI and may contain padding. They
are not a portable restore format or a complete serialization of renderer caches.

The native run returned its expected missing-code failure. The runner then failed
a post-run assertion expecting ED8536 in the generic exception text. Preserve that
report; separate verification checks the captured PC/PR/r2 and all original bytes.
No second native execution was needed, and no game/runtime correction was made.

Evidence under `artifacts/recovery-aot/`:

- `20261009-original-startup-hardware-capture/original-startup-hardware-capture-checkpoint.json`:
  `4b0b17b5f9a6ce176edd2407e188ab6acc4c3d498217bf9217647b4078a54c73`.
- `20261009-original-startup-hardware-final-verification.json`:1,632 sealed
  identities checked; six prior captures identical; hardware and source checks pass.

Next preserve this live startup runtime while integrating already reviewed original
MOVIE integer dependencies fromED8536 onward. Start from the retained55-instruction
buffer-acquire object and exact manifest; verify actual linked/active targets and
batch the reached dependency chain. Keep the separate ARM64 FPU candidate decision
explicit. [RECOVERY-STATE-CAPTURE.md](RECOVERY-STATE-CAPTURE.md) records the handoff.
All1,657 missing source paths and supported PAL GDI remain absent; G1 unresolved,
source-only request unsent.46.14GB free; no native test/game/Simulator. Default
startup/SunPad and FPU status unchanged. Goal active.

**Previous recovery — 9 October 2026: empty repeat verified; hardware-state gap identified.**

Original state2 routineED39D2 and readiness queryED3A2E now execute. Both service
readiness flags are0, so instance state stays2. The same native worker completes
another empty pass and reaches ED39D2 again after5,962 added modeled cycles,
13,152 total. Full CPU/RAM/TMU matches an independent frozen prediction. Two new
bodies add86 instructions;814 MOVIE activation entries start with40 original words.
Correction: ED39D2 has46 reviewed instructions, not47 as earlier prose stated.

This endpoint uses an explicit test-only observer: first entry delegates to the
original body unchanged; second entry throws before game instructions. It is a
diagnostic stop, not a new missing function. Originalroot→RTE→worker remains intact.
No payload bytes consumed, decoded media, presented frame or normal boot. Graphics,
MMIO,TA,SQ,AICA,flash and VRAM remain unchanged from the partial replay baseline.

The next step changes because the replay fixture is incomplete for normal scheduling:
all nine Holly masks are0, and original MMIO/PVR, drive state, AICA controls/ARM and
clock phase were not restored. CPU/RAM correctness does not establish those families.
Do not blindly enable clocks or keep checking empty loops. Capture the missing
runtime state from the existing original startup prefix at an identified matching
checkpoint; [RECOVERY-STATE-CAPTURE.md](RECOVERY-STATE-CAPTURE.md) gives the path.
This is a fixture limitation, not a diagnosis of original-game hardware state.

Immutable evidence under `artifacts/recovery-aot/`:

- Passing `20261009-startup-movie-state-two-repeat-repair1/movie-state-two-repeat-checkpoint.json`:
  `174fff8753a49cd0c416ee39a408e6d3b4239147fefacd2052a6804d586110e0`.
- Preserved native comparison failure
  `20261009-startup-movie-state-two-repeat/movie-state-two-repeat-failure-checkpoint.json`:
  `3f5cae7ff51dd9e2261ae318f834056aeeba10fce5ccffdcd2a1228d9efe7872`.
  Bound dispatch setsPC=ED39D2 before observer; the initial prediction expected the
  prior unbound callsitePC. Fresh prediction/fixture repair fixes that expectation;
  no guest/runtime change. Two earlier source-only model failures lacked already
  reviewed DIV0U and divisor0x71000; both are preserved with fresh repairs.
- `20261009-movie-state-two-repeat-final-verification.json` verifies1,631 sealed
  identities,174 reviewed original instructions, native full state and graphics.
- `20261009-movie-empty-loop-hardware-audit.json`:
  `65eba8b59547a852917b7708c3cb963a940898d89ca10ce364372cc662b3f01f`.

All1,657 source paths/PAL GDI absent; G1 unresolved, source request unsent.
46.25GB free; no native test/game/Simulator. Default startup/SunPad unchanged;
FPU candidate unpromoted. Goal active.

**Previous recovery — 9 October 2026: common movie-service pass completes.**

Original slot3 transfer now reserves the full0x12000 resource region, follows its
zero-byte copy and returns the unused reservation. Its state/error checks return0.
Slots6 and7 then complete their original flag checks, and the nine-slot dispatcher
returns after all six populated callbacks. Empty slots4/5/8 are skipped by original
code. State2 processing now reaches the actual missing routineED39D2.

| Captured original path | New instructions | Added cycles | Integrated cycles | Next missing target |
|---|---:|---:|---:|---|
| Slot3 resource transfer, reservation return and postcheck | 355 | 713 | 6,772 | ED109E |
| Slot3 state/error checks and return | 85 | 105 | 6,877 | ED5040 |
| Slot6 service and flags | 102 | 147 | 7,024 | ED16DC |
| Slot7 flags and common dispatcher return | 88 | 166 | 7,190 | ED39D2 |

Sixteen new original bodies add630 instructions. All four native runs match frozen
whole CPU,16MiB RAM and TMU predictions. They verify captured branches, not all
branches. All use originalEB0254→realRTE→same worker, two native dispatches total.
Final bookkeepingPC8CED3962/PR8CED3966 identifies the reviewed original BSR to
ED39D2.783 exact MOVIE activation entries start with40 original data words.

Resource8C8AA8C8 free count changes0x12000→0→0x12000; write offset remains0.
OriginalED662C receives destination8CBB6040, source8C971000 and length0. No media
bytes are copied or consumed. Slot3 handle8C6774F0 reports state1 and error0.
The already linked648240 state-query object is reused and explicitly registered;
its original two-instruction body is not duplicated. Slot6's ED5116 is an original
two-instruction zero return, not an injected stub. Slots6/7 completion flags remain0.
Graphics/MMIO/TA/provenance/partial state,SQ,AICA,flash and VRAM are unchanged.
No decoded media, presented frame, normal boot or gameplay. Device clock remains
disabled; production/scheduling, nonempty decoding and candidate FPU promotion stay open.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-slot-three-transfer/movie-slot-three-transfer-checkpoint.json`:
  `9605103720e67ec1bc3425176e9708072a9228ce74eea86f774d0ab4b1d6d58f`.
- `20261009-startup-movie-slot-three-state/movie-slot-three-state-checkpoint.json`:
  `763656429854b6081bc6fb07818ffa0b94c322b560c9989c81334740dff268cb`.
- `20261009-startup-movie-slot-six-service/movie-slot-six-service-checkpoint.json`:
  `0a13dfd672ffc4e5ec4c821f7e04d1cffabf2a82f1f11122568b99f41bb8067e`.
- `20261009-startup-movie-slot-seven-service/movie-slot-seven-service-checkpoint.json`:
  `9a028f74a122ffcfb25c3e46b06f33d6239c9012835545f155f20f0cdaeec1e7`.

NextED39D2 is reviewed:47 instructions,16 blocks,four calls. It first snapshots
instance state+64 and command+68, then callsED3A2E; further paths may callED3A7E,
ED3BD0 andED405E. Review/batch original dependencies after verifying current bindings.
Use fresh runner from run-movie-slot-seven-service.py and preserve root/RTE/worker;
never resume bookkeepingPC or fabricate payload. This state2 path is not yet native.

`20261009-movie-slot-seven-service-final-verification.json` checks3,204 sealed
identities,300 historical identities and5,387 reviewed original instructions, plus
native predictions and graphics invariants. One verifier-only failure expected an
address in generic exception text; preserved in
`20261009-movie-slot-seven-final-verifier-failure.json`. Fresh repair verifies the
original callsite and exact CPU endpoint; no native checkpoint or runtime was changed.
All1,657 source paths and supported PAL GDI remain absent; G1 unresolved and source-only
request unsent. 46.41GB free; no native test/game/Simulator. Default startup/SunPad
unchanged; candidate remains isolated. Goal active.

**Previous recovery — 9 October 2026: slot2 completes; slot3 reaches resource transfer.**

Slot2 now follows its original ring snapshot, forward/reverse buffer-selection,
cursor update, empty packet handling, zero-byte consumption and service return.
Dispatcher then enters slot3; original buffer selection returns source8C971000,
length0 to its resource-transfer routineED0F76, now the actual missing target.

| Captured original path | New instructions | Added cycles | Integrated cycles | Actual missing target |
|---|---:|---:|---:|---|
| Slot2 enabled service, ring/cursor selection | 487 | 534 | 5,284 | ED29BA |
| Slot2 empty packet, zero consumption and return | 390 | 324 | 5,608 | ED0F04 |
| Slot3 enabled service and buffer selection | 222 | 451 | 6,059 | ED0F76 |

Nineteen new original bodies add1,099 instructions. Three native runs match frozen
full CPU,16MiB RAM and TMU predictions. This verifies their captured paths, not every
branch of their bodies. All continue originalEB0254→realRTE→same worker runtime,
with two native dispatches total. Final bookkeepingPC8CED0F58, PR8CED0F62;
r4=instance8CEE2210, r5=8C971000, r6=0. Exact685 MOVIE entries start with40
original audited data words. No new failures; every output/checkpoint is preserved.

Retained ring readers, IRQ helpers, indexed option and completion queries are reused.
Slot2 uses ring1 descriptor8CEE2770, base8C910000,size0x39000. Its descriptor+28
cursor changes0→8C910000 during selection, then returns to0 under the original
zero-byte consume routine. Produced/consumed counts and read/write pointers remain
unchanged. Slot3 uses ring2 and retains empty length0. Original metadata and packet
paths are executed; no payload bytes are invented or nonempty branches forced.
Graphics/MMIO/TA/provenance/partial state,SQ,AICA,flash and VRAM remain unchanged.
No decoded media, presented frame, normal boot or gameplay. Device clock remains
disabled; interrupt-driven production/scheduling and nonempty decoding stay open.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-slot-two-buffer/movie-slot-two-buffer-checkpoint.json`:
  `1e49cf3e0ef57eaa586bb4ced8faa8a0fca9a0767b641784e94cb1d9ea157b81`.
- `20261009-startup-movie-slot-two-return/movie-slot-two-return-checkpoint.json`:
  `cb09c96c26c733197e04705b08d8f455950b7eec767de411c3e3b89ccf37ea37`.
- `20261009-startup-movie-slot-three-buffer/movie-slot-three-buffer-checkpoint.json`:
  `cfe6a9e658a9bc5b334927bc34442ecd143f102343a734a76d0febad06e9e3e1`.

NextED0F76 calls resource8C8AA8C8's vtable+24, retained6621F8, in mode0. Its table
is8C67FF74; +28=662384 and +32=6622CA. Source-only prediction uses original lock,
remainder and unlock paths, with captured divisor0x12000. It adds219cycles to
missingcopyED662C at6,278 total, PCED0FC0/PR ED0FC8. Copy arguments are
r4=8CBB6040,r5=8C971000,r6=0. This endpoint is **not yet executed natively**.
Mode0 reserves the entire0x12000 free region before the copy, leaving free0/write
position0; do not mistake this intermediate reservation for consumed payload.
Follow original unused-reservation return via662384 afterward. ED662C(109
instructions,7 blocks) and662384(98,18) are reviewed; nonempty copy remains unproven.
`20261009-movie-slot-three-resource-prediction/prediction.json`, SHA
`c799e5497592af15dbc0f073ebacf6560e6436a8cf4c465218042fbf1c35d8bd`.
Verify actual bindings before extending. Fresh runner from run-movie-slot-three-buffer.py;
unique registration names, originalroot/RTE/worker, no bookkeepingPC resume.

`20261009-movie-slot-three-buffer-final-verification.json` checks2,244 sealed identities,
300 historical identities and5,228 original instructions, all native predictions and
graphics invariants. All1,657 missing source paths and supported PAL GDI remain
absent; G1 unresolved and exact source-only request unsent.46.82GB free; no native
test/game/Simulator running. Default startup/SunPad unchanged; FPU candidate remains
isolated and unpromoted. Goal active.

**Previous recovery — 9 October 2026: slot1 completes and dispatch reaches slot2.**

Original empty-packet handling now returns through the completion/fallback checks,
post-parser threshold and metadata queries, and three-channel metadata scan. Slot1
returns0 to the retained dispatcher; the next actual missing callback is slot2's
ED25AE. The captured parser metadata is still allFFFFFFFF and media counters remain0.
This extends verified original control flow; it does not establish media delivery.

| Captured original path | New instructions | Added cycles | Integrated cycles | Actual missing target |
|---|---:|---:|---:|---|
| Completion check and zero-length packet fallback | 40 | 90 | 3,969 | ED21E8 |
| Post-parser flags, threshold and metadata queries | 209 | 417 | 4,386 | ED2334 |
| Three-channel scan and return to slot dispatcher | 97 | 364 | 4,750 | ED25AE |

Ten new bodies add346 instructions. Three passing native fixtures match frozen full
CPU,16MiB RAM and TMU predictions. Each runs originalEB0254 through realRTE and
then the worker in the same runtime, with two native dispatches total. Final
bookkeepingPC8CEDA5CA, PR8CEDA5DA; r4=instance8CEE2210. Exact465 MOVIE entries
start with40 audited replay words. No failures this turn; all three checkpoints sealed.

Existing original EDA274, ED9BA4/ED9CF4, IRQ helpers, ED7AC4 and resident12/32-byte
copy helpers10E964/10E958 are reused. Original indexed option queryED44FC was already
linked; its one verified activation entry is now included. Slot0's processing flag1
is preserved. Slot1's other processing/completion flags remain0; the threshold check
does not call setterED2298. Channel scan sees unavailable(-1) metadata, keeps its
maxima0, and queries slots6/7; both return0. Options5/6 remain1. No substitute data,
forced success, new provider, callback interception or runtime change.

Graphics/MMIO/TA contents and provenance, partial state, SQ,AICA,flash and VRAM
remain unchanged. No payload consumed, media decode, presented frame or normal boot.
Device clock remains disabled; these bounded tests do not prove interrupt-driven
media production or scheduling. FPU candidate remains isolated and unpromoted.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-parser-fallback/movie-parser-fallback-checkpoint.json`:
  `dc1e08d599228f8179946f880ef8fa9ebebcf8619fbd13db2b2d0f5d6e95a018`.
- `20261009-startup-movie-post-parser/movie-post-parser-checkpoint.json`:
  `032baa12d09155ac97dbd0129655cee0342041f740b62d2c36b16b2237cbff4d`.
- `20261009-startup-movie-channel-service/movie-channel-service-checkpoint.json`:
  `111daf3e20547f123eb647fa9a2c88597067e9dba7e34ce0c7f40fbcc753ae24`.

Next recover ED25AE(28 instructions/8 blocks), ED313E(12/1) and ED25E6(40/9),
reusing already-active ED44FC and EDA274. Option5=1 and slot2 completion flag0
select the processing loop. Frozen source-only prediction adds59cycles and reaches
ED26AE at4,809 cycles; this endpoint is **not yet executed natively**.
`20261009-movie-slot-two-prefix-prediction/prediction.json`, SHA
`2c6742c32f7e0fcfcc9efb044407a406e3dae246f025f66b893dd5ef038e60ea`.
ED26AE and ED3194 are now reviewed. ED26AE first reuses the active ring readerED9BA4
then calls ED9D2E before deeper buffer selection; inspect existing objects/registration
before declaring any helper missing. Batch this next original path sensibly.
Use fresh files/output from run-movie-channel-service.py, unique generated symbols,
and original root/RTE/worker. Never resume bookkeepingPC EDA5CA or inject payload.

`20261009-movie-channel-service-final-verification.json` checks1,992 sealed identities,
300 historical identities and3,857 original instructions, all three new native state
predictions and graphics invariants. All1,657 missing source paths and supported PAL
GDI remain absent; G1 unresolved and exact source-only request unsent.47.12GB free;
no native test/game/Simulator running. Default startup and SunPad profile unchanged.
Goal active.

**Previous recovery — 9 October 2026: service slots and empty movie-parser return verified.**

The second worker pass now enables slot0, enters slot1 packet processing, reads the
original circular-buffer descriptor and calls the original parser. Captured input is
8C900000 with length0; parser handle8CEE6FB0 validates successfully. The parser
returns0 with both output words0. No bytes are consumed or decoded. This is further
original control flow recovered, not movie playback or a playable build.

| Captured original path | New instructions | Added cycles | Integrated cycles | Actual missing target |
|---|---:|---:|---:|---|
| Slot0 enable and slot1 service prefix | 137 | 143 | 3,624 | ED1D68 |
| Ring read and parser invocation | 213 | 193 | 3,817 | ED6848 |
| Parser empty-input return, with retained handle validation | 97 | 62 | 3,879 | ED21AA |

Ten new bodies add447 instructions. Three passing native fixtures match frozen full
CPU,16MiB RAM and TMU expectations. Each starts at originalEB0254, transfers through
realRTE and continues the same worker runtime, using two native dispatches total.
Final bookkeepingPC8CED1E4E, PR8CED1E56; r4=instance8CEE2210, r5/r6=0,
SP8CEE2110. Exact393 MOVIE entries start with the same40 audited data words.
Retained ED9CF4 and IRQ helpers are reused. Retained12-instructionED7AC4 is linked
from its verified binding-validation object and activated with its four exact entries.
It stores the parser handle at8CEFB8F0, already holding that value in this capture.

Nine-slot inventory finds six populated command2 callbacks: ED1C18, ED1CDA,
ED25AE, ED0F04, ED5040 and ED16DC. Only the first two have been entered in this
captured pass; later option-dependent handlers remain unexecuted. Slot0 writes1 to
8CEE2440, a processing flag, without filling the ring. Produced/consumed counts
remain0. Graphics/MMIO/TA packet contents, provenance, partial state, SQ,AICA,flash
and VRAM remain unchanged. Device clock is disabled; synchronous DMA from the
already-verified initial frame prefix is retained. No scheduling, media producer,
nonempty parsing/decoding, presentation or normal-boot proof is claimed.

One native-fixture build failed before execution: two generated objects shared an
unused registration symbol inherited from a runner template. Fresh repair gives the
new parser object a unique unmanaged registration name; original code and frozen
prediction are unchanged. Failure and repair are both sealed. A separate lookahead
prediction initially asserted r13==0 after the caller had restored saved r13. That
pre-output assertion failure is preserved; fresh prediction checks the actual return
value stored at handler SP. It is not a native execution failure or a guest fix.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-service-slots/movie-service-slots-checkpoint.json`:
  `eff6785150bae1da5222ac769c5e846e17ade33c9efaaef99a170e28fbec75a7`.
- `20261009-startup-movie-packet-read/movie-packet-read-checkpoint.json`:
  `75be4d68e33248c1f89bd851a50e9cc5e0dae4f6306c6c94c25fb474d9d55005`.
- Build failure `20261009-startup-movie-parser-empty/movie-parser-empty-failure-checkpoint.json`:
  `390959f9a94375b15f67bed8adc2b013cf0ccd113eca660b2b6da0b158caafa7`.
- `20261009-startup-movie-parser-empty-repair1/movie-parser-empty-checkpoint.json`:
  `e8c736ad38cc2a0de20b60930fd74d99f75b106c38d4612ca81a05c0382ccaf2`.

Next recover originalED21AA(31 instructions/6 blocks) and ED1EC6(9/3), reusing
already-active EDA274. Captured threshold atinstance+40 is0x800; ring completion
flag is0. Source-only lookahead predicts90 additional cycles: completion check0,
zero-length fallback consumes0, packet wrapper returns0, then handler calls actual
unboundED21E8. This3,969-cycle endpoint is **not yet executed natively**.
Prediction: `20261009-movie-parser-fallback-repair1-prediction/prediction.json`, SHA
`614dba7d5cffcd85ac832729329dcee78963eeb2fbef9b56f870f3e4331b88f0`.
ED21E8 and ED2334 are reviewed; inspect their retained dependencies before extension.
Continue original root/RTE/worker, never resume bookkeepingPC or inject media bytes.
Use `work/dreamcast-recovery/run-movie-parser-empty-repair1.py` as the next runner
base; give all new generated registration symbols unique names.

`20261009-movie-parser-empty-final-verification.json` rechecks2,469 sealed identities,
300 historical identities and3,424 original instructions, all passing predictions and
graphics state. All1,657 source paths and supported PAL GDI remain absent; exact
source-only acquisition request remains unsent. G1 unresolved.47.43GB free; no game,
native test or Simulator left running. Default startup, SunPad profile and unpromoted
FPU candidate status unchanged. Goal remains active.

**Previous recovery — 9 October 2026: first worker iteration completes; second service pass begins.**

Original code handles the empty resource result without bypassing it: zero-byte
copy, resource release and movie ring commit all return. Stream status query reads
stored state2 and does not close the stream. The worker finishes its remaining
instance scan and timing bookkeeping, returns toECF9F2, then enters another service
pass. Instance state2 selects common handlerED39A8 and retainedEDA59E; its first
command2 callbackED1C18 is now the actual missing target.

| Captured original path | New instructions | Added cycles | Integrated cycles | Actual missing target |
|---|---:|---:|---:|---|
| Zero-byte copy, release and commit dispatch | 231 | 154 | 2,642 | ED1C66 |
| Zero-byte ring commit and stream-status wrapper | 100 | 147 | 2,789 | 65FF06 |
| Status byte, first worker completion and next service dispatch | 5 | 692 | 3,481 | ED1C18 |

Nine new original bodies add336 instructions; retained47-instructionEDA59E is reused.
All three passing native fixtures match frozen complete CPU,16MiB RAM and TMU
expectations from originalEB0254 through realRTE and the same worker runtime.
The final prediction visits the remaining31 outer slots, completes the timing calls,
then follows the next service pass. Final PC8CEDA5CA, PR8CEDA5DA; r4=8CEE2210,
r5/r6/r7=0, command offsetr9=8. Instance state2/command4 and stream state2 remain.
Exact313-entry MOVIE activation starts with40 audited replay words. Original code
also updates worker timestampsCFA4/CFA8 and CFAC; include these in any successor
activation audit. No context snapshot substitution or guest callback interception.

Resource available count and movie ring byte count remain0. Empty commit preserves
the write pointer8C900000 and does not create data. Graphics/MMIO/TA packet contents,
provenance,partial state,SQ,AICA,flash and VRAM remain unchanged. Nonempty FPU/SQ
copy, media read/decode, presentation and normal boot remain unproven. Device clock
is still disabled; this is a bounded isolated candidate path, not scheduling proof.

One preflight failure is preserved: the runner attempted to add the retained
handler-dispatch repair1 object although that exact object was already linked.
Duplicate-link protection stopped before output/build/guest execution. Fresh repair
reuses the linked object and adds its seven original activation entries. The earlier
unrepaired directory contains identical object bytes but is not the selected input.
This was a fixture inventory mistake; no runtime or guest change was made.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-worker-empty/movie-worker-empty-checkpoint.json`:
  `2ebfcf7d3251750d4cd253a26f3e6bf50c05624797e4812e3037d3ef275db7e1`.
- `20261009-startup-movie-worker-commit/movie-worker-commit-checkpoint.json`:
  `65ea5ae46ae274d1e29e9a3f1abc3b6b58384fc8a76c4dda9a8da98ee741a4ed`.
- Preflight only `20261009-startup-movie-worker-repeat-preflight/movie-worker-repeat-preflight-checkpoint.json`:
  `2e560dea4a6e570dd1df1e0045d2907b5b6a508c5988106ff8131cf53b834efb`.
- `20261009-startup-movie-worker-repeat-repair1/movie-worker-repeat-checkpoint.json`:
  `3495c8d709c7facae859467244151f78774377c8fb38cd12142b88548570d75a`.

NextED1C18 has15 instructions/2 blocks, reachingEDA244 after12 modeled cycles
and two stack stores, with instance8CEE2210, slot index0 and value1. EDA244 has8
instructions and writes1 to instance+0x228+slot*0x328+8. It sets a slot field; it does
not fetch media. Both source reviews are under
`work/dreamcast-recovery/loader-review/startup-producers/ED1C18-20261009-movie-worker-service-provider/`
and `EDA244-20261009-movie-worker-service-mode/`. These prefixes are unexecuted.
Inspect command2 callbacks across the nine handler slots and reuse any existing
original objects/manifests before extending the bounded path. Continue toward
actual producer work; never inject bytes or reduce success to an empty worker loop.
Use original root/RTE/worker, not bookkeepingPC EDA5CA.

`20261009-movie-worker-repeat-final-verification.json` checks1,732 sealed identities,
300 historical identities and2,682 original instructions, all three new passing
predictions and unchanged graphics. All1,657 missing source paths and supported PAL
GDI/private source trees remain absent.47,730,970,624 bytes free; no native process
or booted Simulator. Default twelve patches,startup1058/504state/412device, SunPad
and unpromoted FPU unchanged. G1 unresolved; source-only acquisition request unsent.
Goal active; no decoded movie or playable game claim.

**Previous recovery — 9 October 2026: metadata and buffering reach an empty resource queue.**

The original movie worker now copies its64-byte metadata record, rechecks state,
queries destination capacity and calls the resource queue for up to65,536 bytes.
Native execution confirms that this captured resource queue returns **zero bytes**.
The destination has space; no input bytes have reached the queue in this snapshot.
This does not prove missing disc data, a permanent stall or successful movie decoding.

| Captured original path | New instructions | Added cycles | Integrated cycles | Actual missing target |
|---|---:|---:|---:|---|
| Metadata/status/aggregate and buffering prefix | 109 | 257 | 2,038 | ED0754 |
| Circular-buffer description with original IRQ helpers | 208 | 259 | 2,297 | 6621F8 |
| Resident resource request, IRQ save/restore and empty return | 129 | 191 | 2,488 | ED5240 |

Thirteen original bodies add446 instructions. Each passing native fixture starts
atEB0254, performs real context transfer, then dispatches the worker in the same
runtime. Frozen complete CPU,16MiB RAM and TMU state matches at each endpoint.
Graphics/MMIO/TA packet contents/provenance/partial state, SQ, AICA,flash and VRAM
remain unchanged.268 exact MOVIE entries start with40 audited replay words.
Retained ED1BAA validation and resident10E958 copy are reused; existing MOVIE IRQ
helpers and original10E7D8 signed remainder execute without interception.

Concrete buffer state: instance8CEE2210, descriptor8CEE2448, destination8C900000,
capacity0x10000. Resource8C8AA8F8 has available count+12=0, read offset+24=0,
base+28=8C983000, size+32=0x71000. Request mode1 clamps returned length to0;
IRQ mask restoration and retained signed remainder0/0x71000 match the prediction.
Final PC8CED08B8, PR8CED08C2, r4=8C900000,r5=8C983000,r6=0,SP8CEE2114.
The status remains instance2/command4 and aggregate1. Only captured branches pass.

One failed timer comparison is preserved: the first buffer model assigned2 issue
cycles to STS MACL instead of the emitter's default1. CPU/RAM/graphics already
matched; two STS MACL instructions accounted for the two-cycle mismatch. The fresh
repair derives this distinction from the unchanged candidate emitter, freezes new
expectations, and passes. No runtime or guest change was made. The failed result
file's partial success fields do not supersede its report's `passed:false`.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-worker-metadata/movie-worker-metadata-checkpoint.json`:
  `97b905cc4e72fa057f4cb7e64ebb776911ae93849c8d1471cc6f5681f6711f38`.
- Preserved failure `20261009-startup-movie-worker-buffer/movie-worker-buffer-failure-checkpoint.json`:
  `26575d3fb95af029baba456a212b7b807adcc38be9a6bac8f02a60a72df90c77`.
- `20261009-startup-movie-worker-buffer-repair1/movie-worker-buffer-checkpoint.json`:
  `77d32538a461933fc4c6748e48a0911ffdfc969fdb4aa530e6a18760b0263f98`.
- `20261009-startup-movie-worker-resource/movie-worker-resource-checkpoint.json`:
  `0d98868961d77bcd4ef65be9f7acc9724c6e5ea699d1c7dba3309d315e83675a`.

Next source review:
`work/dreamcast-recovery/loader-review/startup-producers/ED5240-20261009-movie-worker-buffer-copy/review/function.txt`.
This99-instruction/9-block routine returns immediately for captured length0
(7 modeled cycles), then callerED08C2 takes8cycles to resource vtable slot32,
target6622CA. This prediction remains unexecuted; verify its original registration
and review the target. Nonempty aligned FPU/SQ and unaligned copy paths remain open.
Follow the original queue completion and producer path to obtain data; never inject
bytes, suppress an empty result or claim playback from a successful zero-byte copy.
Continue original root/RTE/worker, not bookkeepingPC ED08B8.

`20261009-movie-worker-resource-final-verification.json` verifies2,166 sealed
identities,300 historical identities,2,329 original instructions, all three new
passing predictions and unchanged graphics. All1,657 missing source paths and
supported PAL GDI/private source trees remain absent.48,031,739,904 bytes free;
no native process or booted Simulator. Default twelve patches,startup1058/
504state/412device, SunPad and unpromoted FPU unchanged. G1 unresolved; source-only
acquisition request remains unsent. Goal active; no media decode/frame/normal boot.

**Previous recovery — 9 October 2026: worker reaches original movie metadata query.**

Three native fixtures pass frozen full CPU,16MiB RAM and TMU expectations, each
starting at originalEB0254 and dispatching the worker after realRTE in the same
runtime. The worker aggregates instance state, converts elapsed time, marks the
active outer record and enters original instance processing. Media read/decode,
presentation and normal boot remain unproven.

| Added original code | New instructions | Added cycles | Integrated cycles | Actual missing target |
|---|---:|---:|---:|---|
| ED3D10 aggregate with retained IRQ save/restore | 78 | 134 | 1,472 | 60635E |
| 60635E time conversion with retained division | 26 | 286 | 1,758 | ECF874 |
| ECF874 dispatch and ECF75C processing prefix | 163 | 23 | 1,781 | ED4508 |

Four bodies add267 instructions; only the captured paths are executed. Final exact
MOVIE activation has216 entries and40 starting replay words. Original code stores
aggregate1 at8CEE6EF0 and active flag1 atouter8CEDD114+72. Inner instance remains
state2/command4. Final PC8CECF75C, PR8CECF770; r4=8CEE2210, r5/SP=8CEE2134.
Recorded graphics/MMIO/TA packet/provenance/partial state and SQ match the prior
frame baseline; AICA,flash,VRAM unchanged. No native failures or guest substitutions.
Device clock remains disabled; existing synchronous graphics DMA is preserved.

The IRQ helpersED1BCC/ED1BE4 are original sealed objects/manifests, now verified,
linked and activated. The earlier loop description saying they were already linked
was incorrect for this isolated runner. Time conversion reuses registered original
10E8A0/10E730. Its independent prediction uses signed non-restoring arithmetic only
for the captured positive divisor100/M=0 case; no general division claim follows.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-service-summary/movie-service-summary-checkpoint.json`:
  `491007e78d45e057366aa3b76b1de313ad164d574a5db70a4944c18e3c6c7bd1`.
- `20261009-startup-movie-worker-time/movie-worker-time-checkpoint.json`:
  `dd66d334566d2200df74914d7bb2cb37b5d45248280cbefd697bf43afb9b7f62`.
- `20261009-startup-movie-worker-process/movie-worker-process-checkpoint.json`:
  `eaa228df68f1cd22ce146a105875781475b284405faf14857ce0df56a6659336`.

NextED4508 has25 instructions/5 blocks/3 external calls. It validates via retained
ED1BAA then copies64 bytes from instance+76 to caller stack via resident10E958.
Reuse `20261009-startup-movie-validity-return/wrapper.o` and its original manifest;
this runner already links that object but does not activate its four entries.
Checkpoint SHA256: `1bf0086bd53757a66b9b39a9a1f4e90328a1153a92a80db67ec828b97736de90`.
Check original helper binding rather
than manufacturing a stop at a retained target. The source review is
`work/dreamcast-recovery/loader-review/startup-producers/ED4508-20261009-movie-worker-metadata/review/function.txt`.
Predict its copy and ensuing caller branches before execution. Continue from the
verified root and real context transfer, never from saved bookkeepingPC ECF75C.

`20261009-movie-worker-process-final-verification.json` checks1,435 sealed identities,
300 historical identities and1,662 original instructions, all three new frozen
predictions, unchanged graphics, default emitter and1,657 absent source paths.
Supported PAL GDI/private source trees remain absent; exact source-only request
unsent.48,422,600,704 bytes free; no native process/Simulator. Default twelve patches,
startup1058/504state/412device, SunPad and unpromoted FPU unchanged. G1 unresolved;
goal active.

**Previous recovery — 9 October 2026: original worker service advances the movie instance state.**

The captured worker path now executes `ECF91C` and the original service chain
`ED38C4`→`ED3900`→`ED39AE`. The sole instance at `8CEE2210` has state1 and pending
command4; the service changes state+64 to2 while leaving command+68 at4. This is an
original state transition, **not decoded video or a presented frame**.

Four original bodies add224 instructions: worker107, service30, instance dispatch69,
state-one transition18. Their captured prefixes are verified as follows:

| Path from original EB0254 root and real context transfer | Added cycles | Integrated cycles | Boundary |
|---|---:|---:|---|
| Worker prefix, two original clock calls, capturedCF38=1 | 37 | 1,263 | MissingED38C4 |
| One-instance service and state1/command4 transition | 75 | 1,338 | MissingED3D10 |

Both native runs use two sequential dispatches in the same runtime. Complete CPU,
16MiB RAM and TMU state match expectations frozen before native execution. Full
recorded graphics/MMIO/TA packet contents/provenance/partial state and SQ remain
unchanged; AICA/flash/VRAM checks also pass. No runtime fix, stub, fallback or failed
native run this turn. Exact158-entry MOVIE activation replays the original40 data
words, then original code performs its stores. Any successor activation must also
account for ECF040, CF8C/CF90, CF3C and the worker's newCFAC timestamp store.

The worker captured its timing value through60634E twice and selected service
ED38C4 becauseCF38=1. The service table count atEE6F54 is1, baseEE6F58 is8CEE2210.
ED3900 takes state1; ED39AE recognizes command4 and returns2. Original caller writes
that value back to instance+64, advances its one-entry loop and callsED3D10.
Final PC=`8CED38F0`, PR=`8CED38F4`. Only this captured branch is proven; other
commands, multiple instances and later processing remain open.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-worker-step/movie-worker-step-checkpoint.json`:
  `01ade0f8583157585a929fbb6a04834ebb9ac3eb7253e781e96eb69c2f4e3a08`.
- `20261009-startup-movie-service-state/movie-service-state-checkpoint.json`:
  `2a27c299c2e03097f6fd512ed6d1cbf004deae8797b8435aa859d751a8ce7b34`.

Next recover78-instruction aggregate routineED3D10. Review:
`work/dreamcast-recovery/loader-review/startup-producers/ED3D10-20261009-movie-worker-service-child/review/function.txt`.
It saves the IRQ mask throughED1BCC, counts instances into three stack counters,
writes aggregate state atEE6DE4+10C and restores the mask throughED1BE4. Both
helpers already exist as original generated objects with independent18/16-cycle
contracts and manifests, historically named `movie-instance-video-geometry` and
`movie-instance-video-return`. Reuse their exact original activation entries;
no helper regeneration/interception is necessary. Their fixtures and reviews are
`tests/recovery/startup-movie-instance-video-geometry.cpp`,
`tests/recovery/startup-movie-instance-video-return.cpp`, and corresponding
ED1BCC/ED1BE4 review directories. Check actual bindings rather than linked symbols.
Model the aggregate return and original service/worker continuation before running;
use the verified root/context transfer, never saved bookkeepingPC ED38F0.

`20261009-movie-service-state-final-verification.json` checks900 sealed identities,
300 historical identities,1,282 original instructions, both new whole-state
predictions and unchanged graphics. All1657 missing source paths, supported PAL
GDI and private source trees remain absent.48,705,650,688 bytes free; no native
process or booted Simulator. Default twelve patches, startup1058/504state/412device,
SunPad and unpromoted FPU unchanged. G1 unresolved; exact source-only request unsent.
Goal active; playback and normal boot remain unproven.

**Previous recovery — 9 October 2026: real context switch reaches movie worker.**

Original context-switch entry `5FFAE0`, save/restore body `5FFB70`, getter `5FFA1C`
and worker `ECF9F2` now execute in the isolated ARM64 candidate. Four original
bodies add217 instructions. The complete captured path from `EB0254` through
`RTE` and the first worker branch reaches missing `ECF91C` in **1,226 modeled
cycles**, matching independently frozen CPU/RAM/TMU predictions. Existing graphics,
MMIO, TA packet/provenance, partial-packet and SQ state remains byte-identical.
No decoded movie, presented frame or normal-boot claim follows from this evidence.

| Captured step | Added cycles | Integrated cycles | Result |
|---|---:|---:|---|
| Context entry,22 original instructions | 29 | 1,014 | Actual missing5FFB70 |
| Context save,175-instruction body plus retained save helper | 111 | 1,125 | Actual missing5FFA1C |
| Two-instruction getter and original context restore/RTE | 89 | 1,214 | Transfer toECF9F2 |
| 18-instruction worker's first branch | 12 | 1,226 | Actual missingECF91C |

This is the original transition: SSR/SPC retain status/return address, IRQ mask
changes, alternate GPRs and both physical FPU register banks are saved, and the
caller stack8CFFFE0C is published at8CEDE188. Current context changes to8CEDE190,
whose stack pointer8CEE20B4 supplies the real restore frame. AfterRTE the worker
hasSP8CEE21B8, SR40000001, FPSCR0 and PC8CECF9F2. No replacement context or forced
return is injected. The worker's capturedCF40=0 branch setsCF3C=1 and callsECF91C;
final PC isECFA0A, PR ECFA10. Two explicit native dispatches use the same runtime
instance, preserving graphics and nested caller state. MOVIE activation has96
exact entries, starting from the same40 audited data words.

The save helper5FFA24 already existed as an original linked object, but was not
registered in this fixture. A preflight failed before guest execution; its source
and report are sealed. Fresh repair calls original `movie_slot_mark_register`.
No new helper body, callback interception, guest patch or runtime change was needed.
All four successful native fixtures match their frozen whole-state predictions.
Only the captured context/flag/bank configuration is proven, not arbitrary contexts.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-context-entry/movie-context-entry-checkpoint.json`:
  `afbbc510765632eef7f60c7f1ae5f36bad26bdf85408dd294c0785a4cbec8398`.
- `20261009-startup-movie-context-save/movie-context-save-failure-checkpoint.json`:
  `f8b9cdd7fff93951b5d932b6b8d53aea9419d3be38bdfc38c7acfc5dbe627a17`.
- `20261009-startup-movie-context-save-repair1/movie-context-save-checkpoint.json`:
  `edb8487cd324edf5aacb377ac492b9407fe97b37fb45595023cd9730c4950aff`.
- `20261009-startup-movie-context-restore/movie-context-restore-checkpoint.json`:
  `a5a6a32e4cf84cbec31558f628802a0fd4b4268110ecd494037a6fac8f0681a0`.
- `20261009-startup-movie-worker-entry/movie-worker-entry-checkpoint.json`:
  `4148f3de9c0d6ea4ee7a400dc633cae03b95c816a5930c7956ade9d2f0297899`.

Next recover reviewed107-instruction `ECF91C` worker step. Original review:
`work/dreamcast-recovery/loader-review/startup-producers/ECF91C-20261009-movie-worker-step/review/function.txt`.
CapturedCF38=1 selectsED38C4 after two calls to retained clock60634E. Derive and
freeze that prefix, verify actual registration/generated PCs, then continue the
real worker entry. Later scan loops can reachECF874; neither branch has executed.
Preserve RTE/context state and audited graphics. Never resume bookkeepingPC ECFA0A
as though it were the unexecuted callee. Future activation replay must account for
rootECF040, callbackCF8C/CF90 and workerCF3C stores in addition to initial40 words.

`20261009-movie-worker-entry-final-verification.json` rechecks2,133 sealed identities,
300 historical identities,1,064 original instructions, all four whole-state
predictions and unchanged graphics. All1657 source paths, supported PAL GDI and
private source trees remain absent.48,917,843,968 bytes available; no native process
or booted Simulator. Default twelve patches, startup1058/504state/412device,
SunPad and unpromoted FPU candidate unchanged. G1 unresolved; source-only request
unsent. Goal active; context/worker progress remains isolated from production boot.

**Previous recovery — 9 October 2026: frame state verified; original callback reaches context service.**

The original captured frame prefix from `EB0254` to movie callback `ECFA68` now
matches a bounded offline reference over **959 modeled cycles,931 instructions
and114 basic blocks**. The reference calculates expectations from original reviewed
instructions and initial state before reading native outputs. It matches all69 CPU
fields, all16MiB RAM,25 TMU fields, SQ bytes/counters, four packet contents and
packet provenance. It follows only the reviewed captured path; this is not general
SH-4 conformance or physical hardware timing evidence. The previous source-derived
51-field graphics/30-field MMIO audit remains valid.

The first reference run matched CPU/RAM/TMU/packets but expected the block address
for PREF provenance. Emitter/runtime source specifies the actual instruction PC.
The failed reference is preserved; a fresh repair changes only that diagnostic
expectation and passes. No native runtime, emitter or original code was repaired.

Original `ECFA68` is now emitted as35 instructions/seven exact activation entries.
One uninterrupted native call from `EB0254`, with90-entry MOVIE activation, runs
**985 modeled cycles** to actual unregistered resident `5FFAE0`. Full CPU/RAM/TMU
matches the frozen prefix expectation; graphics, sparse MMIO, TA packets, provenance
and partial-packet state match the preceding audited capture. No snapshot with
empty graphics state is substituted. Callback adds26 cycles, copyingCF90 toCF8C
and reading the original clock helper. CapturedCFB0=0 skips the optional timestamp
branch. End: PC=`8CECFA98`, PR=`8CECFAA0`, target r3=`8C5FFAE0`, r4=`8CEDE190`.
CF8C becomes0 andCF90 becomes002E802C. Root also writesECF040=FFFFFFFF. Start
activation with40 audited words; account for all three writes in successor replay.

Sealed evidence under `artifacts/recovery-aot/`:

- `20261009-movie-frame-reference/movie-frame-reference-failure-checkpoint.json`:
  `bde28bff0f7d5cbba2c4d59fbc285bc79a7445d13403d8a72072c1da8ab109c5`.
- `20261009-movie-frame-reference-repair1/movie-frame-reference-checkpoint.json`:
  `bead1e009ff7440ceb9d8eb9ec38cf1218d561c2685f57c0807b2c8417ee9250`.
- `20261009-startup-movie-frame-callback/movie-frame-callback-checkpoint.json`:
  `cebc8a15a7f4030894321345d52e7c2869d5765e1fe6a9ab0461f030adf5c957`.
- `20261009-movie-frame-callback-state-audit.json`:
  `82c2b8b9f423dd2eddfae9f28bb5b4620db57b8c150f66d74d1fd3dc67ea41cf`.

Next original `5FFAE0` is22 instructions: saveR0/R1/SR, publish oldSR, mask IRQs,
setSSR/SPC from savedSR/PR, publishr4, then jump to `5FFB70`. The latter has175
reviewed instructions including alternate GPR/FPU-bank saves, context-selection
calls, restored stack and RTE. Neither has executed in this path. Their reviews
are under `work/dreamcast-recovery/loader-review/startup-producers/` in
`5FFAE0-20261009-movie-frame-service/review/` and
`5FFB70-20261009-movie-frame-context/review/`. Model the real state transition and
verify existing registrations/generated bookkeeping before execution. Do not stub
the context service, skip bank state, or resume a saved bookkeeping PC mid-call.
Continue from the verified EB0254 root so TA/DMA/Holly and nested frames survive.

`20261009-movie-frame-callback-final-verification.json` checks902 sealed identities,
300 historical identities,935 source instructions, frame/callback predictions,
unchanged emitter and all1657 absent source paths. Supported PAL GDI/private trees
remain absent.49,365,659,648 bytes free; no native process or booted Simulator.
Default twelve patches, startup1058/504state/412device, SunPad and unpromoted FPU
candidate unchanged. No decoded movie/presented frame/normal boot. G1 unresolved,
exact source-only request unsent; goal active.

**Previous recovery — 9 October 2026: movie frame loop reaches its installed callback.**

Original movie-entry `EB0254` calls retained `608DF0`, then the original graphics
routines, and stops at unregistered MOVIE callback `ECFA68`. The callback was
installed at `8C8A723C`; it has not executed. Captured PC is `8C645D98`, PR
`8C645DA0`, r2=`8CECFA68`, r4=0, SP=`8CFFFF18`. This advances discovery beyond the
complete resource-method return. It does **not** prove a complete frame or playback.

Three bounded, sequential native calls use the same verified resource-method input:
initial discovery, call/packet provenance, then expanded graphics capture. All ten
shared CPU/RAM/timer/AICA/flash/VRAM/SQ/device/boundary files match byte-for-byte.
Eleven original calls are recorded. No generated game bodies were added. Original
83-entry MOVIE activation replays40 words; entry writes ECF040=FFFFFFFF as the41st.
One diagnostic fixture compile failed on an unqualified nested type; preserved
failure, fresh repair uses `sizeof(rt.pvr_ta_stream[0])`. No native failure or
runtime/emitter replacement was needed.

The source-derived graphics audit passes a deliberately limited contract:

- All128 bytes of four TA packets match original header/template/list-buffer data.
  PCs `6465AA` and `645CC8` submit two SQ packets; `654AD0` submits two through CH2.
- One64-byte DMA transfer reads `ACFEC480` into TA. Original source review plus
  runtime completion rules explain the51 captured graphics and30 MMIO fields.
- Two list-end events and CH2 completion produce pending Holly mask `00080280`;
  no IRQ is taken. Device clock is disabled, but synchronous DMA still operates.
- No vertex packets, decoded triangles or presented frames. AICA/flash unchanged;
  VRAM remains zero. No media decoding/playback established.

Complete CPU/RAM/timer correctness of the executed frame prefix is **still open**.
The second and third runs establish diagnostic repeatability, not an independent
whole-state oracle. Graphics discovery snapshots must not be resumed with fresh,
empty TA/DMA/Holly state. No full-startup promotion is authorized by these results.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-loop-discovery/movie-loop-discovery-checkpoint.json`:
  `886daea0a654d4ce52c95729d83c288af48eed80e648997096a713247dec6c8a`.
- `20261009-startup-movie-loop-provenance/movie-loop-provenance-failure-checkpoint.json`:
  `a150e9ffe3dca779de9703672c71e9358204c9046c052d18d4e7e089f442d1ab`.
- `20261009-startup-movie-loop-provenance-repair1/movie-loop-provenance-checkpoint.json`:
  `e066b12885c73f216eb52227de8be8ece2713bbfc018ba389ed8dca09667546f`.
- `20261009-startup-movie-loop-graphics/movie-loop-graphics-checkpoint.json`:
  `38dfc88b5db63b1925436c86e7539461f478052f9e081de7db90551413d0fc51`.

Graphics audit `20261009-movie-loop-graphics-audit.json` SHA-256
`b510c58dea63d10318840e54703f1267e34bb0768939cfa540dc97d5b0b4fda8`
checks1,356 original instructions against captured RAM/BOOT. Final verification
`20261009-movie-loop-graphics-final-verification.json` rechecks1,594 checkpoint
identities,300 historical identities,703 preceding-path instructions, audit inputs,
unchanged default emitter and all1657 absent source paths. Supported PAL GDI and
private source trees remain absent.49,504,546,816 bytes free; no native process or
booted Simulator. Default twelve patches/full startup1058/504state/412device,
SunPad and unpromoted FPU candidate remain unchanged. G1 unresolved; request unsent.

Next independent work: model the captured frame prefix through `608DF0`,
`646180`, `65A220`, `645C40`, `645B40`, `654908` and original copy/time helpers,
using recorded call history to delimit the executed branches. Match complete CPU,
RAM and TMU changes while retaining the audited graphics effects. Then recover
original `ECFA68`; review exists at
`work/dreamcast-recovery/loader-review/startup-producers/ECFA68-20261009-movie-loop-callback/review/function.txt`.
It calls the clock helper and `5FFAE0`; verify real registration before generation.
Do not intercept the installed callback or execute from bookkeeping PC `645D98`.
Goal active; this is isolated recovery progress, not production source closure.

**Previous recovery — 9 October 2026: complete captured resource method returns.**

The entire original `ECF35E` resource method now runs continuously from entry to
return `EB0254` in **17,201 modeled cycles**. Six integrated controls match the
independently verified whole CPU/RAM/AICA/timer/device endpoint. This connects
reset, directory/file opening, stream creation, filename copy, stream setup and
instance-state changes. The next original movie-entry loop has not executed.
No media bytes are read/decoded by this path; normal boot/playback remain unproven.

**49 checks pass** across three fixtures:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Post-open stream field, compatibility and start routines | 8+9 activation | 94; next `ED0700` |
| Post-open setup, instance flags and method return | 8+9 activation | 197; return `EB0254` |
| Entire captured resource method `ECF35E` | 6+9 activation | 17,201; uninterrupted return,95-entry activation |

Ten original bodies add119 instructions: stream wrappers `ED0960`9, `ED0972`2,
`ED0976`7; resident helpers `65FF48`3, `65FC82`15, `604C06`11; instance wrappers
`ED0700`14, `ED071C`14; state setters `ED3858`22, `ED4032`22. Original validity
checker `ED1BAA` is reused. Dirty stream, inner-state and outer-state fields pass;
all transitions preserve complete state/device expectations. The final integrated
controls cover host RN/RZ and three candidate FPU modes with captured guest RZ.
Exact original MOVIE activation replays40 data words; no interception/fallback.
Active-object, invalid-instance and error branches remain outside this path proof.

Source inspection corrects the previous speculative positioning description:
`65FF48` stores226 atstream+44, `ED0972` immediately returns0, and `65FC82` changes
stream byte1 to2, clears stream+52, queries current file position0 via `604C06`
and writes it to stream+12. No seek or read occurs here. Instance+68 is written3
then4; outer+68 is cleared and outer+8 becomes1 before the method returns.

A reused fixture-name guard stopped preparation before any write; a fresh name
was used. An inherited-header identity preflight needed the original validity
checkpoint and was corrected before output creation. All native fixture runs
passed; no runtime correction or failed execution checkpoint was needed this turn.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-stream-start/movie-stream-start-checkpoint.json`:
  `20640274353e48a31fd239c06f9dd77be3ae904abdeb68ddd3a16f9bc4b0783b`.
- `20261009-startup-movie-resource-ready-return/resource-ready-return-checkpoint.json`:
  `faf50b0b9e5cb45832abc052c37a6e3db6656e626f40dd20c51259233098149e`.
- `20261009-startup-movie-resource-method-complete/resource-method-complete-checkpoint.json`:
  `8d1cfc6c093a554d44e1fb616a74d16c4c8a3afc9aa9cefc318076835a63a100`.

Next execute retained movie-entry `EB0254`. The frozen prefix takes6 cycles,
writesFFFFFFFF to `8CECF040`, then calls already-registered `608DF0` with return
`EB025E`; generated bookkeeping PC is `EB0258`. The store creates a41st audited
module data word. Start activation with40, then account for the new word in any
successor capture. Review `608DF0` timing/provider behavior before execution;
current `893A58` and `893A50` flags are0. Do not replace/intercept a retained target
to manufacture a missing-call boundary. Prediction:
`20261009-movie-loop-entry-prediction/prediction.json`, SHA-256
`9cfeabd9224cad9cc9b8f5df22ea674c4a7cf3d1f96051acf415eb653985fc68`.

`20261009-resource-method-complete-final-verification.json` rechecks1,342 sealed
identities,300 historical identities,703 original BOOT/MOVIE instructions,
prediction inputs and unchanged default emitter. All1657 missing source paths,
supported PAL GDI and pinned private trees remain absent. Available49,717,100,544
bytes; no native process or booted Simulator. Default twelve patches, full
startup1058/504state/412device, SunPad and unpromoted FPU unchanged. G1 unresolved;
source-only acquisition request unsent. Goal active; new native evidence.

**Previous recovery — 9 October 2026: original movie file open returns a stream.**

One uninterrupted resident `65FB12` call now executes the retained filesystem
provider, directory search and file allocation, creates a movie stream, copies
and zero-pads its filename, and returns handle `8C8A8010` to MOVIE `ECF37A`.
It takes **16,845 modeled cycles**. Eight integrated controls match the independently
modeled whole CPU/RAM/AICA/timer/device endpoint. This verifies opening a file
against the captured directory and creating its stream record; no media bytes
have been read or decoded in these fixtures, and playback remains unproven.

**37 checks pass** across four fixtures:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Resident movie caller and retained filesystem open | 8 | 16,530; directory entry1738; next `65FA86` |
| Stream constructor and buffer-size query | 8 | 77; stream `8C8A8010`; return `65FB64` |
| Connected file open, stream and name-copy return, repair1 | 8 | 16,845; return `ECF37A` |
| MOVIE post-open publication/position call | 4+9 activation | 15; position226; actual missing `ED0960` |

Three original bodies add148 instructions: `65FB12`54, `65FA86`66, `6621A2`28.
Existing `605416`/`604214` provider dispatch, filename hashing/comparison/directory
lookup, descriptor initialization, size query and `11157C` bounded string copy
execute unchanged. The captured directory has2063 entries; search makes four hash
matches and selects one-based entry1738 for `SLH448_1.SFD`. Filesystem descriptor
`8C6994F4` records3,119,104 bytes. Stream slot0 takes that descriptor and resource
`8C8AA8F8`; its 32-byte name field contains the12-byte filename plus20 zeros.
Dirty descriptor/stream fields and name padding are checked. Full tables, missing
names, active-stream and error branches remain outside this captured-path proof.

The first connected fixture caught a missing filename-copy registration before
executing the game call. Its failure is sealed. The original `recomp_8C11157C`
object was already linked; repair1 invokes its original `movie_bounded_string_register`
and executes without interception. Original object/checkpoint identity verified:
`20261007-startup-movie-workarea/movie-workarea-checkpoint.json`, SHA-256
`303e3126730df998b6b12e6a461b5d8f8c0b55651487e364ab2e61a063a2f8df`.
One diagnostic `nm` invocation hit the full-Xcode license gate; rerunning through
CommandLineTools succeeded. No license/developer-directory setting was changed.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-movie-file-open/movie-file-open-checkpoint.json`:
  `f166d5450064af75755dbbb7ce21cd4f43f0f026d79ecb5bb2f9e564bce55948`.
- `20261009-startup-resident-movie-stream/movie-stream-checkpoint.json`:
  `bc8af2c6cb3346d2b2a4f308ede7f29964e9934f9a5be52e92b7bf3843266d8c`.
- `20261009-startup-movie-file-open-complete/movie-file-open-complete-failure-checkpoint.json`:
  `7859b2a50fd94520c73bb4e54119b41433ba9a4266ca38f0efed454867103f3c`.
- `20261009-startup-movie-file-open-complete-repair1/movie-file-open-complete-checkpoint.json`:
  `ec344eced300e5b61d9f25faa4135986231807b9e823e70f30b5fba180eae8a5`.
- `20261009-startup-movie-open-position/movie-open-position-checkpoint.json`:
  `e0a718ba8ddab0d8b7819c4be6074d1e67a60190c30c132dd61a7e505554fb67`.

The following original MOVIE continuation publishes the stream atouter+52 and
prepares position226 from captured field124=`0x71000`, shifted right11. Exact
36-entry activation replays40 audited module data words. The next `ED0960` wrapper
has an unexecuted six-cycle/two-stack-store prefix to resident `65FF48`, returning
`ED096A`. Confirm its actual registration and generated bookkeeping PC before
running. Prediction: `20261009-movie-position-wrapper-prediction/prediction.json`,
SHA-256 `9b55ba28ba34c181e1f7b18af06bad590a8e83b096d20eb61fcae51b27256438`.

`20261009-file-open-complete-final-verification.json` rechecks1,997 sealed
identities (including failure),300 historical identities,556 original BOOT/MOVIE
instructions, prediction inputs and unchanged default emitter. All1657 missing
source paths, supported PAL GDI and pinned private trees remain absent. Available
49,954,795,520 bytes; no native process or booted Simulator. Default twelve patches,
full startup1058/504state/412device, SunPad and unpromoted FPU unchanged. G1 and
normal boot/playback remain unresolved; source-only request unsent. Goal active.

**Previous recovery — 9 October 2026: movie preparation reaches resident open.**

One uninterrupted original `ECF35E` call now performs the captured resource reset,
saves/restores the interrupt mask, checks the empty drain, validates the instance,
reinitializes its resource through original resident `662194`, returns through
the caller and executes wrapper `ED094C`. It reaches actual unregistered resident
`65FB12` in **159 modeled cycles**. The filename is `SLH448_1.SFD` and the resource
handle is `8C8AA8F8`. The open routine has not executed; playback remains unproven.

**112 checks pass** across four fixtures:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `ECF35E` resource-method prefix | 4+9 activation | 11; five stack stores; next `ECF450` |
| `ECF450` reset, IRQ helpers and empty drain | 24+9 activation | 81; next inner reset `ED4084` |
| Connected method/reset/instance/resource preparation | 24+9 activation | 155; next wrapper `ED094C` |
| Connected preparation and open wrapper, repaired fixture | 24+9 activation | 159; actual missing resident `65FB12` |

Seven original bodies add264 instructions: `ECF35E`127, `ECF450`54, `ECF068`12,
`ECF080`11, `ECFAAE`27, `ED4084`30 and `ED094C`3. The compiled bodies include
unexecuted branches; coverage is the captured preparation path. Active drain,
non-default instance status, existing stream/object and error paths remain open.
Three IRQ masks0/5/15, host/guest rounding, dirty drain flag, outer state and resource
record fields pass complete CPU/RAM/AICA/timer/device comparisons. Exact72-entry
MOVIE activation replays40 audited data words. No interception or signature fallback.
This is still the isolated FPU candidate with inactive devices, not normal boot.

The first connected-wrapper fixture stopped at its own stale assertion that
`ED094C` should be unregistered after adding its original body. That failure is
sealed below. A new repair fixture requires the binding, then verifies the actual
unregistered resident target; it passes. Game code/runtime were unchanged by the
repair. Two runner preflight errors (parent path and missing inherited header
identity) were corrected before their output directories were created.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-resource-method/resource-method-checkpoint.json`:
  `a17980c0abaa8b9bc2ef488f73ab896fad53f2c181cede12e0164d09eadf4cc6`.
- `20261009-startup-movie-resource-reset/resource-reset-checkpoint.json`:
  `58586db64daaff3a596cc66e2a2669f1ebc7fbaeb284b3c57b870c66602b7392`.
- `20261009-startup-movie-resource-open-dispatch/resource-open-dispatch-checkpoint.json`:
  `8c35b031b61e21c3d67ea468c261c1acaca4bcb8521d31d5c6fd3e763f6c1ef2`.
- `20261009-startup-movie-resource-open-connected/resource-open-connected-failure-checkpoint.json`:
  `e3594cada3e4228580b29f078e4bbe7aa6797276a6bf82b21825e3dad10f0463`.
- `20261009-startup-movie-resource-open-connected-repair1/resource-open-connected-checkpoint.json`:
  `b479f76286a793f57af4dc4f6eecaff89c875a70ab495a33420de9c3db8fd72a`.

Next recover original resident `65FB12`. Its reviewed prefix predicts11 modeled
cycles, four stack stores and a call to `605416`, with filename in r4, r5=0 and
return `65FB26`. It has not executed. Inspect actual retained registrations and
generated bookkeeping PC before extending; do not replay saved wrapper PC `ED094C`
to substitute for the missing callee. Prediction:
`20261009-resident-movie-open-prediction/prediction.json`, SHA-256
`50567f944f9210ec02275ce43ad0b2a561ea401d9614361bd21ada816ccef759`.

`20261009-resource-open-final-verification.json` rechecks1,887 sealed identities
(including the failure),300 historical identities,325 original BOOT/MOVIE
instructions, prediction inputs and unchanged default emitter. All1657 missing
source paths, supported PAL GDI and pinned private trees remain absent. Available
50,226,814,976 bytes; no native process or booted Simulator. Default twelve patches,
full startup1058/504state/412device, SunPad and unpromoted FPU unchanged. G1 remains
unresolved; source-only acquisition request unsent. Goal active; new native evidence.

**Previous recovery — 9 October 2026: movie entry reaches resource dispatch.**

One uninterrupted `EB0174` invocation stores the real outer handle, sets the
child option, executes the selected setup path and dispatches `SLH448_1.SFD` to
original method `ECF35E` in **165 modeled cycles**. Six integrated controls match
the independently verified complete CPU/RAM/AICA/timer/device endpoint. The
method has not executed: this is resource selection, not file opening or playback.

All **58 checks** pass:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Retained entry handle store and option call | 4+9 activation | 11; next `ECFFB8`; module handle published |
| New `ECFFB8` and retained option helpers | 8+9 activation | 93; child flag3=1; dirty flag checked |
| Entry setup and resource dispatch | 4+9 activation | 61; next actual unregistered `ECF35E` |
| Connected entry continuation | 6+9 activation | 165; uninterrupted119-entry activation |

Three original bodies add53 instructions: `ECFFB8` (8), `EB03F2` (31) and
`ED0022` (14). The captured zero active-object path clears outer+92; the nonzero
branch remains untested. Exact original MOVIE activation starts with39 audited
data words. Execution writes `8CEDD114` at `8CECEFE8`, adding the40th word used
in successor activations. No interception, replacement helper or signature fallback.

At the stop, r4=`8CEDD114`, r5=`8CEB8B84` points to `SLH448_1.SFD`, PR=`8CEB0254`.
Generated bookkeeping PC remains caller block `EB023C`; call the next callee
explicitly rather than replaying this already-executed caller prefix.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-entry-handle/entry-handle-checkpoint.json`:
  `aa123b50f08c18a106e32b810c2028143d0b5cf9461156b1731113c63dd9c18e`.
- `20261009-startup-movie-entry-option/entry-option-checkpoint.json`:
  `ab1efeada1d5111a4c3ece8a477500e99a4c3c6400466a535b16b59580702a2a`.
- `20261009-startup-movie-entry-dispatch/entry-dispatch-checkpoint.json`:
  `9e5690f731cc901e26bd5d4575f35ea252fef74e16041edb14851a929a9da7e2`.
- `20261009-startup-movie-entry-connected/entry-connected-checkpoint.json`:
  `545e5a8f4f11da701aec72b9f17a55eb6967a82ba64a9ea5d1d4048748c90c6a`.

Next recover original `ECF35E`. Its reviewed prefix predicts11 modeled cycles,
five stack stores and a direct call to `ECF450`, return `ECF372`, with resource
handle `8C8AA8F8` saved on the stack. This prefix has not executed. Confirm its
actual generated bookkeeping PC and target registration before freezing that
part of the state;127 reviewed instructions match original MOVIE/current RAM.
Prediction: `20261009-movie-resource-method-prediction/prediction.json`, SHA-256
`5c5297f6ddb23782a98b86b3298bb2e2bafa024a723c1bfc61b427f6d8591177`.
A preparatory filename assertion counted one extra NUL; it was corrected before
prediction creation. No fixture/runtime failure or checkpoint mutation resulted.

`20261009-entry-connected-final-verification.json` rechecks1,443 sealed identities,
300 historical identities,546 original instructions, prediction inputs and unchanged
default emitter. All1657 missing source paths, supported PAL GDI and pinned private
trees remain absent. Available50,561,863,680 bytes; no native process or booted
Simulator. Default twelve patches, full startup1058/504state/412device, SunPad
and unpromoted FPU remain unchanged. Normal boot, playback and G1 are unresolved;
source-only request unsent. Goal active; new native evidence.

**Previous recovery — 9 October 2026: resource allocation and outer movie setup return.**

One uninterrupted invocation from `ECF1F6` now allocates and initializes pool
slot4, copies the descriptor, initializes the outer object, executes both indexed
queries, clears its working table and returns handle `8CEDD114` to movie-entry
caller `EB0174`. It takes **370 modeled cycles**; six integrated controls match
the independently verified whole CPU/RAM/AICA/timer/device endpoint. This connects
the resource/outer continuation, not complete movie-entry execution or playback.

All **57 checks** pass without repair:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Retained `662106` allocator and `662194` initializer | 8 | 106; slot4 handle `8C8AA8F8`, dirty record checked |
| Retained outer descriptor/field setup | 8+9 activation | 71; original32-byte copy; next `ED45C8` |
| Outer fields, both queries and return | 8+9 activation | 252; slots7/3 read; working table cleared; return `EB0174` |
| Connected wrapper/allocation/outer return | 6+9 activation | 370; uninterrupted return,37-entry module activation |

One original body adds31 instructions: indexed query `ED45C8`. The valid-instance
path reads slot7 handle `8C8A8C90` into outer+60 and slot3 handle `8C6774F0` into
outer+88. Its error path remains untested. Allocator slots0–3 stay occupied and
slot4 takes buffer `8C983000`, size `00071000`, zero third argument. Existing
allocator, initializer,32-byte copy and validity checker execute without
interception or replacement. Dirty-record and dirty-field checks verify writes.
Exact original MOVIE activation still replays39 audited data words. Normal boot,
full movie-entry progression and playback remain unproven.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-next-resource-allocation/next-resource-allocation-checkpoint.json`:
  `5839dbc0d7a4158a64e06b966e9741d26cc72ba555a9523cbc884395b7a58319`.
- `20261009-startup-movie-outer-fields/outer-fields-checkpoint.json`:
  `1dc26e09bceb06c530777f50e71f02caffc1b9fb14974806437454916c49a169`.
- `20261009-startup-movie-outer-return/outer-return-checkpoint.json`:
  `7e2e13eb351ed9fcc0ace88a5bce3f97069176046dfb086e2cdf94195c69fcdc`.
- `20261009-startup-movie-resource-outer-connected/resource-outer-connected-checkpoint.json`:
  `e9b2010417bad13bcfa90cba6df5bd74fc417b4c1721f7a8f3d881b31d1865e3`.

Next execute existing movie-entry continuation `EB0174` with its original
`movie-entry` manifest. Captured stack flag atSP+12 is1, selecting `ECFFB8` with
r4=`8CEDD114`,r5=1, return `EB0214`. Frozen11-cycle prediction includes a real
store of the outer handle at module address `8CECEFE8`; this adds a40th audited
module data word after execution. Activate the original image with39 replay words
before the prefix, then account for the new store in successor activation.
The next continuation has not executed; check real target registration first.
Generated unresolved-call PC is block entry `EB020C`, not JSR address `EB0210`.
Prediction: `20261009-movie-entry-handle-prediction/prediction.json`, SHA-256
`424c83c22ae91aa4f3f038403fac620bff2c923b25e74fbcb847f8434d7b8e72`.

`20261009-resource-outer-final-verification.json` rechecks1,413 sealed identities,
300 historical identities,694 original BOOT/MOVIE instructions against current
captured RAM, prediction inputs and unchanged default emitter. All1657 missing
source paths, supported PAL GDI and pinned private trees remain absent. Available
50,869,264,384 bytes; no native process or booted Simulator. Default twelve patches,
full startup1058/504state/412device, SunPad and unpromoted FPU remain unchanged.
G1 unresolved; source-only request unsent. Goal active; new native evidence.

**Previous recovery — 9 October 2026: complete original output setup returns.**

One uninterrupted `ED0620` invocation now executes all eight output settings,
the captured scalar FPU calculation, instance/buffer checks and final flag clear,
then returns to outer caller `ECF1F6` in **846 modeled cycles**. Six integrated
controls match the independently verified endpoint, complete CPU/RAM/AICA/timers
and inactive devices. This proves the captured output-setup path, not normal boot
or playback. The following original wrapper reaches retained allocator `662106`.

All **78 checks** pass without repair:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `ED0620` settings/FPU to tail | 24+9 activation | 762; eight settings, scalar value4000, next `ED2EC0` |
| `ED2EC0` and retained buffer helpers | 8+9 activation | 84; flag5/value0, dirty flag control, return `ECF1F6` |
| Complete connected output setup | 6+9 activation | 846; uninterrupted return,62-entry exact activation |
| Outer caller and `ED0854` wrapper | 4+9 activation | 12; actual unregistered resident allocator `662106` |

Two new original bodies add51 instructions:44 in `ED2EC0`,7 in `ED0854`. The
retained `ED8604` flag setter and `ED86AE` buffer check are reused from exact old
objects/manifests. Dirty output slots and child flag verify real writes. Exact
original MOVIE activation still replays39 audited data words. No interception,
replacement helper or signature fallback.

Captured FPU inputs are2,2,1000 and float0.5. The exact operations produce4000.5,
truncate to4000 and take the non-decrement branch. Expected FR bits are checked
independently: FR2=`3F000000`, FR3=`457A0000`, FR4=`457A0800`, FPUL/MACL=4000.
Host RN/RZ, guest RN/RZ, three candidate FPU execution modes and dirty output slots
pass. This exact-value path does not prove general FPU correctness or authorize
candidate promotion; other output/error paths remain open.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-output-connected/output-connected-checkpoint.json`:
  `63736573e06cc647b795444c38bb13edd05c2e0fb09923e51d107bc54dedc8a2`.
- `20261009-startup-movie-output-tail-return/output-tail-return-checkpoint.json`:
  `fb5b46f50bf6028aab0b2f189fdc4fe471690ef979228ea965595990e828950b`.
- `20261009-startup-movie-output-complete/output-complete-checkpoint.json`:
  `8bb0fc1a1c1d40d8a1aff795c1dc91b9717d0b6c1414eb02399c8c5afdce18df`.
- `20261009-startup-movie-next-resource-wrapper/next-resource-wrapper-checkpoint.json`:
  `000f2b02b1e7398b237c7f13fbd3bcb2c3208dc0d552c47ebc2c2dad32a914fd`.

Next execute retained `662106` and initializer `662194` for the new wrapper call.
Actual arguments are `r4=8C983000`, `r5=00071000`, `r6=0`, return `ECF1FC`.
Captured pool slots0–3 are occupied; the frozen92-cycle/ten-store allocator-prefix
prediction selects slot4. This allocation has not executed here. Reuse the existing
allocator/initializer registrations and continue the original outer caller; do
not regenerate them or assume the previous slot3 is still free.
Prediction: `20261009-next-resource-pool-prediction/prediction.json`, SHA-256
`e9ceda167f26b17f957b20b85f7b2cc6be078a197493fe82b885f3b589917b07`.

`20261009-output-complete-final-verification.json` rechecks1,416 sealed identities,
300 historical identities,464 BOOT/MOVIE instructions against captured RAM,
prediction inputs and unchanged default emitter. All1657 missing source paths,
supported PAL GDI and pinned private trees remain absent. Available51,139,325,952
bytes; no native process or booted Simulator. Default twelve patches, full startup
1058/504state/412device, SunPad and unpromoted FPU unchanged. G1 unresolved;
source-only acquisition request unsent. Goal active; native evidence advances.

**Previous recovery — 9 October 2026: outer stream sizes and first output setting execute.**

The outer movie routine stores the real instance, computes stream sizes through
original `ED01BC` and its original division helper, and reaches `ED0620` output
setup in **158 modeled cycles**. Output setup then reaches its first setter in14
cycles. Original `ED436A`, retained validity check, mode validator and slot write
return to `ED063A` in85 cycles, setting mode8/value0. A dirty-slot control proves
the destination is cleared. These are connected outer/stream and setter runs,
with output-prefix execution checked separately; normal boot/playback are unproven.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Outer `ECF1B0`, stream-size helper, caller continuation | 4+9 activation | 158; actual missing `ED0620` |
| Output setup `ED0620` prefix | 4+9 activation | 14; actual missing `ED436A` |
| Setter with validity/validator/slot helpers | 8+9 activation | 85; mode8/value0, dirty destination, return `ED063A` |

All43 checks pass after two expectation repairs. Both initial failures are sealed:
they expected the JSR instruction address while generated unresolved-call state
retains the basic-block entry (`ECF1E4` and `ED0620`). Source inspection confirmed
this before changing expectations; game code and default emitter were unchanged.
For future boundaries inspect generated PC assignments before freezing expected
state. Do not resume the stored block-entry PC as though the prefix had not run.

Two original bodies add113 instructions:74 in output setup,39 in setter. Only
the output prefix and valid-instance mode8 setter path execute here; later output
setters, floating-point calculation, null/error setter paths and tail call remain
open. Whole CPU/RAM/AICA/timer and inactive-device checks pass across host/guest
rounding modes. Exact MOVIE activation replays39 audited words; the final setter
combines27 entries. Original division runs without interception or fallback.

Immutable checkpoints under `artifacts/recovery-aot/` (failures explicitly named):

- `20261009-startup-movie-outer-stream-resume/outer-stream-resume-failure-checkpoint.json`:
  `a1ddff2bf917bcce6ff3f9cc7bff6237effe2f79f2b12283bd5497ea824223f2`.
- `20261009-startup-movie-outer-stream-resume-repair1/outer-stream-resume-checkpoint.json`:
  `88bdd95ac9749618197a9627b802c367aed900d2578043f6f51fbe85f560e629`.
- `20261009-startup-movie-output-setup-prefix/output-setup-prefix-failure-checkpoint.json`:
  `2c3447064d6b690849209864a7dd69b7bff29b7e40d5627a4b9a5416c5ba9495`.
- `20261009-startup-movie-output-setup-prefix-repair1/output-setup-prefix-checkpoint.json`:
  `6cd47ad73aff42f94ea1c97b59bdb7345df6dbdbcf9ef1ee4ea3351390500c2e`.
- `20261009-startup-movie-output-mode-eight/output-mode-eight-checkpoint.json`:
  `9ba26c4c2e299307645dbad62b3a3a43f05ce85f172c9cb5c32daad5b35004d4`.

Next execute `ED063A` through retained setter `ED436A` for mode1/value1, then the
following original mode0/value0 and mode23/value4 calls. The frozen call-preparation
prediction is five cycles/no stores, excluding the already recovered callee;
return `ED0642`. Combine the output and setter/helper manifests, then derive the
FPU phase from actual inputs. Do not treat this as another missing helper.
Prediction: `20261009-output-mode-one-prediction/prediction.json`, SHA-256
`c737edd13d9428a47ed5481b9073e2411d1d1566cd9db60e0af987e180effed3`.

`20261009-output-mode-eight-final-verification.json` rechecks1,734 sealed identities
including both failures,300 historical identities and436 original instructions,
plus prediction inputs and unchanged default emitter. All1657 missing source paths,
supported PAL GDI and pinned private trees remain absent. Available51,410,907,136
bytes; no native process or booted Simulator. Default twelve patches, full startup
1058/504state/412device, SunPad and unpromoted FPU candidate unchanged. G1 unresolved;
source-only request unsent. Goal active; native evidence changed the next action.

**Previous recovery — 9 October 2026: connected creation tail binds callback and returns.**

One uninterrupted invocation from `ED05C4` now validates the real instance, binds
callback `ED07FC` with argument0 at instance+0xCC/+0xD0, restores both saved frames,
and returns handle `8CEE2210` to outer routine `ECF1B0`. It takes **64 modeled
cycles** and matches complete CPU/RAM/AICA/timer and inactive-device expectations
across four host/guest rounding controls. This connects the creation tail, not
an uninterrupted full constructor or normal boot. Playback remains unproven.

All **47 checks** pass without repair:

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Retained caller and non-null binding prefix | 4+9 activation | 20; actual missing checker `ED1BAA` |
| Original `ED1BAA` validity check | 12+9 activation | 12 valid /11 invalid field /7 null; correct return and global store |
| Connected caller, check, setter and returns | 4+9 activation | 64; real handle returned to `ECF1B0` |

One new original body adds13 instructions and four activation entries. Its valid,
invalid-field and null-input branches all execute. Existing `ED1AFA` and `ED1B5E`
are reused, as is the creation caller. Connected activation combines37 entries
against the exact original MOVIE image, then replays39 audited data words.
Seven expected word stores include four stack saves, the validity global and
callback/argument slots. No helper interception or signature fallback.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-create-bind-resume/create-bind-resume-checkpoint.json`:
  `3531503485cb5f118d8003143be7c7bd7ebc5560ea430ca7ba7cb43ae6488743`.
- `20261009-startup-movie-validity-return/validity-return-checkpoint.json`:
  `1bf0086bd53757a66b9b39a9a1f4e90328a1153a92a80db67ec828b97736de90`.
- `20261009-startup-movie-create-tail-connected/create-tail-connected-checkpoint.json`:
  `f660beb10b335c5cbe0a2091638a7f051c991a580c0d7075fc826c55298f881a`.

Next execute retained outer continuation `ECF1B0`. Its frozen27-cycle prediction
stores the real handle at `8CEDD144`, saves four output pointers on the stack and
calls `ED01BC`, returning to `ECF1E4`. This next continuation has not executed;
verify and reuse original target registration before expanding. Prediction:
`20261009-movie-outer-resume-prediction/prediction.json`, SHA-256
`9ffa05295a9872087593a48087d1bcab00702d8657edf8b561518d9b4883e0b2`.

`20261009-create-tail-final-verification.json` rechecks1,040 sealed identities,
300 restored historical identities and458 original instructions against MOVIE/
captured RAM, plus prediction inputs and the unchanged default emitter. All1657
missing source paths, supported PAL GDI and pinned private trees remain absent.
Available51,711,504,384 bytes; no native process or booted Simulator. Default twelve
patches, full startup1058/504state/412device, SunPad and unpromoted FPU candidate
unchanged. G1 unresolved; exact source-only acquisition request remains unsent.
Goal active; previous/current turns produced new native evidence.

**Previous recovery — 9 October 2026: movie instance returns to its creation caller.**

The remaining captured dispatcher slots now complete, and the initializer and
instance-constructor tails return the real handle `8CEE2210` to caller `ED05C4`.
All **83 checks** pass. A single uninterrupted dispatcher invocation from
`EDA5DA` to `ED3EDC` takes107 modeled cycles and passes six integrated full-state
controls. Initializer success/failure and the final constructor return are
separate continuation tests; this does not claim an uninterrupted full constructor
or movie playback. Two original bodies add10 instructions; existing helpers and
caller bodies are reused without interception or signature fallback.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Dispatcher skips empty slots4/5 | 8+9 activation | 39 captured /48 with extra empty slot; reaches slot6 `ED511A` |
| Slot6 `ED511A` and dispatcher | 4+9 activation | 24; returns0 and reaches slot7 `ED178C` |
| Slot7 `ED178C`, retained `ED44FC`, remaining dispatcher | 8+9 activation | 44; zero/nonzero query result; returns `ED3EDC` |
| Connected remaining dispatcher | 6 | 107; uninterrupted return,52-entry exact activation |
| Initializer tail `ED3EDC` | 8+9 activation | 9 success /15 failure; handle or null, expected field clearing |
| Constructor tail `ED3DF6` | 4+9 activation | 5; returns real handle to `ED05C4` |

Whole CPU/RAM/AICA/timer and inactive-device comparisons pass. Exact original
MOVIE activation still replays39 audited data words. Slot8 is empty in the
captured state; the loop restores its original saved frame. The newly recovered
bodies are `ED511A` (2 instructions) and `ED178C` (8 instructions). The retained
indexed query and both return tails use their previously sealed code and manifests.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-next-dispatch/next-dispatch-checkpoint.json`:
  `c1dd734448627e8ee9a66ce16940e84b94c9d1b2b9a80227f5b727a92cbd4025`.
- `20261009-startup-movie-slot-six/slot-six-checkpoint.json`:
  `13a8cdde853bb0135c8916570342dc940b26d58e8fff491c0eced7b0512a87b7`.
- `20261009-startup-movie-slot-seven/slot-seven-checkpoint.json`:
  `bddd52af943f63df3738f959b92a299859b951ae25445d15a458f9b18990497b`.
- `20261009-startup-movie-dispatch-return-connected/dispatch-return-connected-checkpoint.json`:
  `5a2f8aa1cf50818714fe11683650c7452f7003e705b12f1984f8d065e395227a`.
- `20261009-startup-movie-initializer-return/initializer-return-checkpoint.json`:
  `d7f3f69d6b2ebd50236e364e520c48a34d598e6302e385303a561fd2fdabe40a`.
- `20261009-startup-movie-instance-return/instance-return-checkpoint.json`:
  `c9fdb8ecd07b51a126a14dc715e3dce8bebd95dda6c41dfd67ac4c6a16f47e31`.

Next execute retained caller `ED05C4` using the original `movie-create-prefix`
manifest. Its non-null branch has a frozen nine-cycle/no-store prediction to
`ED1AFA`, with `r4=8CEE2210`, `r5=8CED07FC`, `r6=0`, return `ED05E0`.
This next call has not executed here; verify actual registration before expansion.
Prediction: `20261009-movie-create-resume-prediction/prediction.json`, SHA-256
`fd52d45d8b936448d6b1a564deeb17bfdb1b6725ba44dbe25b33cb24567398c0`.

`20261009-instance-return-final-verification.json` rechecks2,027 sealed identities,
300 restored historical identities and397 original instructions against MOVIE/
captured RAM, plus prediction inputs, unchanged default emitter and all1657
missing source paths. Supported PAL GDI and pinned private source trees remain
absent. Available51,908,018,176 bytes; no native process or booted Simulator.
Default twelve patches, full startup1058/504state/412device, SunPad and unpromoted
FPU candidate unchanged. Normal boot/playback/G1 remain unresolved; exact
source-only request remains unsent. Goal active; next boundary is actionable.

**Previous recovery — 9 October 2026: movie setup branch returns to dispatcher.**

The recovered MOVIE setup branch now returns to `EDA5DA`. One uninterrupted
invocation from `ED136A` performs allocation, binding, resets, channel enable,
state handling and mode-table installation, then returns in **789 modeled
cycles**. Six integrated controls match independently checked complete CPU/RAM/
AICA/timers and inactive devices. Exact MOVIE activation combines41 entries and
replays39 audited data words. No helper interception or signature fallback.
This remains an internal continuation test, not normal boot or playback.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `648548` state request prefix | 4 | 8; next `648240` |
| `648240` getter and short caller return | 16 | 18; return `ED13EA`; signed bytes0/1/128/255 checked |
| MOVIE parent continuation | 4+9 activation | 3; next `ED1476` |
| `ED1476` mode-install wrapper prefix | 4+9 activation | 8; next `ED43F8` |
| `ED43F8` mode-slot prefix | 4+9 activation | 11; next `ED4460` |
| `ED4460`, slot/wrapper/parent returns | 8+9 activation | 85; mode15/value2, dirty table control, return `EDA5DA` |
| Connected setup branch | 6 | 789; uninterrupted return and full-state comparison |

All82 checks pass without repair. Five new original bodies add132 instructions:
44+2+14+34+38. The captured state1 uses the short `648548` path and writes the
requested byte at object+114; state3/4 transitions remain untested. Validator
mode15/value2 returns success; its mode5/6 callback branches remain untested.
The original mode slot writes2 at context+0x11C and calls the previously recovered
`ED4A64`; the wrapper installs the `ED14C8` descriptor through retained `ED4A54`.
Dirty mode/active-handler fields prove the writes. Final `r0=0`, PC/PR=`EDA5DA`,
outer critical count0 and saved IRQ mask restored. The broader dispatcher and
subsequent handlers remain open.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-state-request/state-request-checkpoint.json`:
  `4662a827a031ff7edfaf9739b194077fde822891fe9be39132646cc4941da393`.
- `20261009-startup-resident-state-query/state-query-checkpoint.json`:
  `9c31083dee5561956c2192d2cf3167e9d92a173c47cd1a8118b4fd0c043c6445`.
- `20261009-startup-movie-after-state/after-state-checkpoint.json`:
  `bb56258b60306c043bab2d3955ca67302f5b133fcbd9b834269f07b6bbf227d8`.
- `20261009-startup-movie-mode-install/mode-install-checkpoint.json`:
  `83a09dc72034497d0d88c17f19bb27d084ae687abc75c710ad86665bbef79185`.
- `20261009-startup-movie-mode-slot/mode-slot-checkpoint.json`:
  `7151afe2bbc519d217177e6179778ea495292b4459f08e74f131f6261f1892e0`.
- `20261009-startup-movie-mode-validation/mode-validation-checkpoint.json`:
  `c3c60fc02b116bde41153c7fb99608e915b5e943b9278dc824cfb30fefb580c1`.
- `20261009-startup-movie-setup-return-connected/setup-return-connected-checkpoint.json`:
  `c49c1bfcb35db21aa07bc046308cf4926d6ead26d2ac4467f3ec6321e215b350`.

Next execute the existing `EDA5DA` dispatcher continuation using its original
`movie-instance-handler-dispatch` manifest. Actual captured slots4/5 are empty;
slot6 has table `8CEDCA1C` and callback `8CED511A`. The frozen39-cycle/no-store
prediction preserves the frame and reaches that callback. It has not executed
in this checkpoint; check real target registration before expanding. Prediction:
`20261009-next-dispatch-prediction/prediction.json`, SHA-256
`00f3150d7b5459a2711ef3003edcf40072efa4f3b3a2682b07f30a9c59a9ec4c`.
Do not repeat the prior error of assuming the very next slot is occupied.

`20261009-setup-return-final-verification.json` rechecks2,358 sealed identities,
300 restored historical identities,286 original instructions against BOOT/MOVIE/
live RAM, prediction inputs, unchanged default emitter, all1657 missing source
paths and supported GDI absence. Available53,401,743,360 bytes; no active native
process or booted Simulator. Default twelve patches, full startup1058/504state/
412device, SunPad and unpromoted FPU candidate unchanged. G1 unresolved; exact
source-only request remains unsent. Previous/current turns made progress; goal active.

**Previous recovery — 9 October 2026: setup returns to MOVIE and updates state.**

Object setup now stores the handler, enables the channel through its original
initializer, sets the outer-object flags and returns to MOVIE `ED13E4` with the
saved IRQ mask restored. The movie then executes original `ED161A`, updates its
state and reaches actual missing resident `648548`. One uninterrupted invocation
from `ED136A` reaches this boundary in **656 modeled cycles**. Six integrated
controls match independently checked complete CPU/RAM/AICA/timers and inactive
devices; no helper interception or signature fallback. This is not playback or
normal-boot evidence.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `65F4A4` handler setter and caller | 8 | 9; two stores; dirty destination checked; next `65F4B8` |
| `65F4B8`, retained initializer and setup return | 8 | 66; return `ED13E4`, channel flags/sentinel and IRQ restore checked |
| MOVIE parent continuation | 4+9 activation | 4; actual unregistered `ED161A` |
| First connected setup sequence | 6 | 638; next `ED161A` |
| `ED161A` state update | 12+9 activation | 18 captured/18 early return/22 zero-argument continuation; next `648548` |
| Final connected sequence | 6 | 656; next `648548`, exact state comparison |

All62 checks pass. Three new original bodies add36 instructions:2 in `65F4A4`,
11 in `65F4B8`,23 in `ED161A`. The previous channel initializer `65F346` and
critical exit `65EACE` are reused. Dirty channel fields verify clearing, sentinel
writes and enable flags. MOVIE combines its existing17-entry handler manifest
with five entries for `ED161A`; exact image activation still replays only39 data
words. The state update writes its original argument, returns early for the
zero-argument/state1 condition, and otherwise forwards the stored object handle.
Resident status handling, playback and normal boot remain open.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-handler-setter/handler-setter-checkpoint.json`:
  `42bbe03b04d498a6ce9574abf75e87ff5878ea646ab2e27d8cf034741faacafb`.
- `20261009-startup-resident-channel-enable/channel-enable-checkpoint.json`:
  `f8df6bd8f38e29cfcb824f8f30ed8a4aa49909a52db394235a70fb1c49aa0a9c`.
- `20261009-startup-movie-after-setup/after-setup-checkpoint.json`:
  `643267120fea105d94c53a65ef00207ea8c9c015333da490a6b310eea29469a0`.
- `20261009-startup-movie-setup-connected/setup-connected-checkpoint.json`:
  `cb5c2c20e643441f9f2ab82eb8e42459daf54c89a72d935dceaae62cbcb518b5`.
- `20261009-startup-movie-status-toggle/status-toggle-checkpoint.json`:
  `224b17c8a81739db309e7042eaec8afad258c18681a13e6b5cde2191b8a9a0a3`.
- `20261009-startup-movie-status-connected/status-connected-checkpoint.json`:
  `5cae5c1617c5f71cba68d995ffbc74432b443a8f36f2840f56de19467401c731`.

During preparation, the name `channel-start` collided with an older experiment.
Its output-exists guard stopped the run before building. Both original C++ files
were restored from sealed copies; its runner was reconstructed from the old
repair version and matched its pinned SHA-256. All300 old checkpoint identities
verify. The new work uses `channel-enable` and a distinct registration symbol.
Details: `20261009-channel-enable-name-repair.json`. Future preparation must assert
all source/runner/output paths are absent before writing, including unique names
for temporary preparers. No native test required repair.

Next execute original `648548` from the final capture. Its reviewed first block
predicts eight cycles and three stack stores before `648240`; that callee is only
predicted and its registration must be checked before expansion. Preserve the
captured MOVIE state, enabled channel and AICA. Prediction:
`20261009-status-next-prediction/prediction.json`, SHA-256
`71252afa8251383bed844410f269070b971b17176bca8ec13160c29978910754`.

`20261009-status-connected-final-verification.json` rechecks1,982 new sealed
identities,300 restored historical identities,233 original instructions against
BOOT/MOVIE/live RAM, prediction inputs, unchanged default emitter, all1657 missing
source paths and supported GDI absence. Available53,919,080,448 bytes; no active
native process or booted Simulator. Default twelve patches, full startup1058/
504state/412device, SunPad and unpromoted FPU candidate unchanged. G1 unresolved;
exact source-only request remains unsent. Goal active.

**Previous recovery — 9 October 2026: outer reset returns; movie setup resumes.**

The captured outer reset now completes both record resets, channel/stream buffer
clearing and two child initializers, then returns through the original critical
exit. It releases nesting1 to0 and restores the saved IRQ mask. The caller then
enters its next setup critical section. One uninterrupted MOVIE `ED136A` call
reaches actual missing `65F4A4` in **559 modeled cycles**. Six integrated controls
match the independently checked complete CPU/RAM/AICA/timers and inactive devices.
Exact MOVIE activation still uses17 entries/39 audited data stores. No helper
interception or signature fallback.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `65F23E` forwarder | 4 | 4; next `66107A` |
| `66107A` second reset prefix | 8 | 36 captured/39 alternate; next `66184C` |
| `66184C` status query within second reset | 16 | 78 captured/81 already-active; return `6481CA` |
| Existing `6481CA` continuation | 4 | 4; next `65F4CE` |
| First connected MOVIE sequence | 6 | 341; next `65F4CE` |
| `65F4CE` channel reset prefix | 4 | 7; next `66B4BC` |
| `66B4BC` stream reset prefix | 4 | 7; next `6489A8` |
| `6489A8`, retained byte clear and both caller tails | 8 | 76; dirty-buffer control; return `6481D0` |
| Existing outer-reset tail and child initializers | 16 | 94 captured/80 nested; return `648178`, IRQ restore verified |
| Existing post-reset setup prefix | 4 | 34; next `65F4A4` |
| Final connected MOVIE sequence | 6 | 559; uninterrupted full-state comparison |

All80 controls pass without repair. Six new original bodies add150 instructions:
3+91+19+13+13+11. `66184C` returns byte1 bit1; second-reset return controls cover
status equal to requested0/1 with unrelated bits preserved. Transition branches
that change that status and null-record paths remain unverified. Original
`10EF3C` clears eight bytes; dirty-buffer controls prove writes. Existing
`662194` handles both child callbacks, including dirty-record controls. Existing
`65EACE` restores IRQs only when the critical count reaches zero; the nested
control keeps the incoming mask. The final capture is inside the caller's newly
entered critical section (count1), not the completed reset's section. Setup,
playback and normal boot remain unverified.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-reset-second/reset-second-checkpoint.json`:
  `9e949a662ea3f07a10f1d576ea06b07ee02bf5fbeeb252abce9cfe9c02c25523`.
- `20261009-startup-resident-record-reset-second/record-reset-second-checkpoint.json`:
  `87b6b1ab232c8ca24d2c58b2aa1508d908a39cee6a0568c8ff4402d3c83b623c`.
- `20261009-startup-resident-record-reset-second-complete/record-reset-second-complete-checkpoint.json`:
  `20ddb925d94e643ddacd1ba1efc8267743fb1c41a6577a13c7842311ac5af49a`.
- `20261009-startup-resident-reset-second-return/reset-second-return-checkpoint.json`:
  `d116ff089f22666582acd653e752102f0d6ac59054d92b662efeaae798f9d9fb`.
- `20261009-startup-movie-reset-second-connected/reset-second-connected-checkpoint.json`:
  `c34082e25360fcf0aff0bc0f744aa3dddd100321c9da57d91a62fc44745590e2`.
- `20261009-startup-resident-channel-reset/channel-reset-checkpoint.json`:
  `839bfadea795cc6ce94fd0413df253696024792f28cf6c8eb01ac953fc9fa354`.
- `20261009-startup-resident-stream-reset/stream-reset-checkpoint.json`:
  `a7b449393b29ae0967f11469aa5b4c7f58d326cd4b24cec49984fc42509da359`.
- `20261009-startup-resident-buffer-reset/buffer-reset-checkpoint.json`:
  `5f3bc5db3ac9808fda876bd7101fbc5ef133be4cab6fd959b13d90520b85c704`.
- `20261009-startup-resident-outer-reset-return/outer-reset-return-checkpoint.json`:
  `b60b7b3b8038923ddf477d96875558ba0c9317ec4dcd987b8fc62bf74671b3ee`.
- `20261009-startup-resident-post-reset/post-reset-checkpoint.json`:
  `ffb622f38f9d35dbe8c298f03ec68ce6b38ac4f39e8a82f5247f72ebb4f4604f`.
- `20261009-startup-movie-reset-complete-connected/reset-complete-connected-checkpoint.json`:
  `b86e34c2eb0b8e36df6c12397a6381729e98ceeb306cdb20c0b583e8528823c9`.

Next execute original `65F4A4` (two instructions) and existing `648186`. The frozen
prediction stores the handler at channel+8, returns, stores it at outer-object+20,
then reaches target `65F4B8` after3+6 cycles. That next target is predicted only;
check actual registration and execution before classifying it. Preserve the
current critical count, child/pool records and captured AICA. Prediction:
`20261009-post-reset-setter-prediction/prediction.json`, SHA-256
`232693a5a58bbf392f3a8ddc67307f576a9ddeba5da39daf42608cf08094fa86`.

`20261009-reset-complete-final-verification.json` rechecks3,614 sealed identities,
339 original instructions against BOOT/live RAM, prediction inputs, unchanged
default emitter, all1657 missing source paths and supported GDI absence.
Available54,382,944,256 bytes; no active native process or booted Simulator.
Default twelve patches, full startup1058/504state/412device, SunPad and unpromoted
FPU candidate unchanged. G1 unresolved; exact source-only request remains unsent.
Previous and current turns made native progress; goal active.

**Previous recovery — 9 October 2026: idle record reset returns in connected movie setup.**

One uninterrupted MOVIE `ED136A` invocation now runs allocation, binding, the
original reset forwarding leaf, idle record reset and caller continuation. It
reaches actual missing `65F23E` in **255 modeled cycles**. Six integrated controls
match the independently checked complete CPU/RAM/AICA/timer state; SQ, flash,
VRAM and inactive devices match. MOVIE activation still uses its17-entry manifest
and39 audited data stores. No helper interception or signature fallback.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `65F238` forwarding leaf | 4 | 4; tail jump to `660FA6`, unchanged PR/RAM |
| `660FA6` idle reset prefix | 8 | 40 captured/43 alternate argument; original IRQ save, next `661828` |
| `661828` query within complete record reset | 16 | 79 captured;90/107/82 other flag/argument combinations; return `6481C2` |
| Existing `6481C2` caller continuation | 4 | 5; next `65F23E` |
| Integrated MOVIE sequence | 6 | 255; one call with full state comparison |

Three new original bodies add118 instructions:3 in `65F238`,97 in `660FA6`,18 in
`661828`. All38 controls pass. The captured record remains `8C8A8C90`, byte5=0;
status byte1 is tested at0 and0x87 and argument r5 at0/1. The existing `6607A0`
and `6607B8` save/restore the incoming IRQ mask. The enclosing critical section
still has nesting1 and masked interrupts; its exit has not been reached. Null
records and active byte5=1 paths are not covered. Record reset return does not
establish outer reset completion, playback or normal boot.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-reset-forward/reset-forward-checkpoint.json`:
  `741cf529c0afd45831d0db907ccf745cacad2034f7343a174fa67cdfb839927d`.
- `20261009-startup-resident-record-reset/record-reset-checkpoint.json`:
  `03b70c0c8f832eead851cdac3cbf67705ff67ff5d2b2525f60e0b45491a128c5`.
- `20261009-startup-resident-record-reset-complete/record-reset-complete-checkpoint.json`:
  `64384c35699d49ecf328df067ba77c7a3cdebc5461f5bac46178a9f5f59c7aee`.
- `20261009-startup-resident-reset-return/reset-return-checkpoint.json`:
  `42d633ad261ced229f9f92f047e2a49b0643af51d5e19c34f4ff3cf11a08b360`.
- `20261009-startup-movie-reset-connected/reset-connected-checkpoint.json`:
  `34f5391a2c0e62edaa696545ee893e2d2707b66d2b5a9299b4b269d2472258ba`.

Next execute original `65F23E` from the integrated capture. Its three instructions
match BOOT/live RAM and predict a four-cycle tail jump to `66107A`, no stores
and unchanged PR. This leaf is unexecuted; check actual target registration before
classifying the callee. Preserve nesting1, pool occupancy and captured AICA.
Prediction: `20261009-reset-next-prediction/prediction.json`, SHA-256
`a411c25c701d3918c848e2edf462ee87cda806e801f01ebba14b31c312ce4e01`.

Review tooling initially wrote the `661828` review into a nested output directory;
the caller's file read failed. Original output is preserved, and `-repair1`
contains the byte-verified review and provenance. No native build/test failed.
`20261009-reset-connected-final-verification.json` rechecks1,623 sealed identities,
220 original instructions, prediction inputs, the unchanged default emitter,
all1657 missing source paths and supported GDI absence. Available55,173,586,944
bytes; no active native process or booted Simulator. Full startup1058/504state/
412device, default twelve patches, SunPad and unpromoted FPU candidate unchanged.
G1 unresolved; exact source-only request remains unsent. Goal active.

**Previous recovery — 9 October 2026: allocation and binding reach original reset path.**

One uninterrupted invocation from MOVIE `ED136A` now allocates the next object,
returns its constructor, stores the handles and enters the original reset path.
It reaches actual missing `65F238` in **167 modeled cycles**. Six integrated
controls match complete CPU/RAM/AICA/timers with the independently checked chain.
SQ, flash, VRAM and inactive-device invariants pass. Exact MOVIE activation uses
its existing17-entry manifest and39 audited data stores. No helper interception,
substituted return or template fallback. The reset path has not completed;
normal boot and playback remain unverified.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `662106` 64-slot constructor | 12 | 79 captured slot3;92 slot4;846 when full |
| `662194` initializer and constructor tail | 8 | 14; four field stores, dirty-record control, return `ED13B2` |
| MOVIE `ED13B2` handle binding | 4 | 17; three original handle stores, next `64816A` |
| `64816A` setup prefix | 4 | 8; three frame stores, next `6481A8` |
| `6481A8` reset prefix and retained critical section | 8 | 40 captured/24 nested; next `65F238` |
| Integrated `ED136A` sequence | 6 | 167; complete state matches at missing helper |

Four new original bodies add165 instructions:51 for `662106`,7 for `662194`,31
for `64816A`,76 for `6481A8`. All42 controls pass without repair. Captured pool
slots0–2 remain occupied; slot3 at8C8AA8C8 is allocated. An extra-occupied-slot
control and full-pool null return test the scan boundary. Initializer stores zero
to offsets12/20/24 and copies offset32 to16; dirty record words prove actual writes.
Original `65EAA8` is reused, not replaced. Captured nesting0 becomes1 and saves the
old IRQ mask; a separate nesting2 control increments without replacing it. Final
captured reset state is inside that critical section, awaiting its next helper;
interrupt restoration and reset completion are not claimed.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-handler-pool/handler-pool-checkpoint.json`:
  `7e6c236faecb645bacdcc52a6560a1093dd9225cc42605cfe4d0e1ffe81357f9`.
- `20261009-startup-resident-handler-init/handler-init-checkpoint.json`:
  `561e6e879935df96b73c23336cf9ac2d17aa60c0e60eead67203d4ff72d41c80`.
- `20261009-startup-movie-handler-bind/handler-bind-checkpoint.json`:
  `cd69b7ea2c483999ac4ec81dd348202a9f445b56fdce71e3fd6ef0faef4c46c5`.
- `20261009-startup-resident-object-start/object-start-checkpoint.json`:
  `3aca298a4501148fea5d322c8cfa32347fc8179e0ff63ff9273b0f185e4df072`.
- `20261009-startup-resident-object-reset/object-reset-checkpoint.json`:
  `23fc56a7ea31632ec02bf76108790c377ecd4a6003dd8d0e8cc28b8f7ca09790`.
- `20261009-startup-movie-handler-connected/handler-connected-checkpoint.json`:
  `5b61cb1f049dc66dbeddc7df9defabdf5c87e99de7f47c02a2aa61a57b4b9216`.

Next execute original `65F238` from the integrated capture. Its three instructions
match BOOT/live RAM and predict a four-cycle tail jump to `660FA6`, no stores and
unchanged PR. Check actual registration and signature fallback before classifying
that callee. Do not reset the captured critical count or occupied pool records.
This forwarding leaf has not executed.
Prediction: `20261009-reset-forward-prediction/prediction.json`, SHA-256
`dd1242fb860361b1cdc58cb352d9c95409cf7b6a32ebae00ca6bf49e41f2a6a3`.
The proof also rechecks retained critical-entry instructions against BOOT/live RAM.

`20261009-handler-connected-final-verification.json` rechecks1,948 sealed identities,
prediction inputs, unchanged default emitter, all1657 missing source paths and
supported GDI absence. Available55,516,393,472 bytes; no active native process or
booted Simulator. Full startup1058/504state/412device, default twelve patches,
SunPad and isolated/unpromoted FPU candidate are unchanged. G1 unresolved;
source-only request remains unsent. Goal active.

**Previous recovery — 9 October 2026: MOVIE wrapper returns; parent reaches next constructor.**

One uninterrupted invocation from MOVIE `ED144C` now returns the original resident
handle to `ED136A` in **26 modeled cycles**, after original timing and byte setters.
Six integrated controls match complete CPU/RAM/AICA/timers and inactive-device
invariants against the independently checked chain. The parent handler then runs
nine cycles to actual missing `662106`. Neither playback nor normal boot is proved.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Exact MOVIE activation and `ED144C` continuation | 4 +9 activation | 8; next `648342` |
| `648342` timing setter and caller | 8 | 8; values30/60, next `648412` |
| `648412` byte setter and wrapper return | 12 | 10; values0/1/255, returns `ED136A` |
| Integrated `ED144C` through wrapper return | 6 | 26; complete state matches |
| Parent `ED136A` continuation and activation | 4 +9 activation | 9; next `662106` |

Two new original bodies add five instructions:2 for `648342`,3 for `648412`.
All52 executed controls pass. The six-entry creation manifest and17-entry parent
manifest use their existing verified compiled objects. Original MOVIE bytes are
validated before exactly39 audited data-word stores restore the captured module.
Activation rejects inactive aliases, altered images, duplicate activation and
unregistered internal PCs; retirement removes the installed entries. No helper
interception, substituted return or signature fallback occurs. Captured guest
RZ, two host modes and three generated FPU modes cover the integrated return.

Two build failures are preserved: the inherited resident-only command first
omitted the MOVIE header path, then its compiled object. Repairs restored those
verified inputs; no game/runtime implementation changed. A later unexecuted pool
prediction assumed slot0 was free. Actual RAM has slots0–2 occupied. The failed
preparer and inventory are preserved in `20261009-movie-next-pool-prediction/`;
repair1 models the observed scan to slot3. No pool contents were changed.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-movie-post-resident/post-resident-build-failure-checkpoint.json`:
  `ed93542ce02b5c1b04b03c816a4cfd40d1add463220abfbce5d84c84a9258295`.
- `20261009-startup-movie-post-resident-repair1/post-resident-link-failure-checkpoint.json`:
  `51fb3b522c7c1e94412cbf3972ae2a54f5bcbf72fe6208e7077fdf12e043a433`.
- `20261009-startup-movie-post-resident-repair2/post-resident-checkpoint.json`:
  `bfe36f19d8edc1bec87467dc647185384c35e81ba9b5ee176281a90caf6da345`.
- `20261009-startup-movie-rate-setter/rate-setter-checkpoint.json`:
  `a396d955aec38a86e11e104dc2bd72d5758e00dfcd0b443c40534ce6f584fdd0`.
- `20261009-startup-movie-flag-setter/flag-setter-checkpoint.json`:
  `bf15d03e1846ffb04329a2e32c7a2f3e712a9b80d81c64ae59cf7c9c22e1da1e`.
- `20261009-startup-movie-wrapper-complete/wrapper-complete-checkpoint.json`:
  `889bd31c8bd05b17e02be155c34e17dd5c8523da0d56d7836f167a28a1070a82`.
- `20261009-startup-movie-handler-next/handler-next-checkpoint.json`:
  `3be286a91ba18fccbc14804a533b2363be854e28e27bf3c5cd61442173c01999`.

Next execute original `662106` from the final handler capture. All51 reviewed
instructions match BOOT/live RAM. The frozen prefix predicts79 cycles and ten
word stores: two frame stores and eight record stores, choosing slot3 at8C8AA8C8.
It calls `662194` with PR662162. Check actual registration and signature fallback
before classifying that helper. This constructor prefix has not executed.
Prediction: `20261009-movie-next-pool-prediction-repair1/prediction.json`, SHA-256
`631b9ad7c7d6b5bf22df7919eb551cb2959f62313b2742add42e3e38c833c4fb`.

`20261009-movie-wrapper-final-verification.json` rechecks2,125 sealed identities,
prediction inputs, unchanged default emitter, all1657 missing source paths and
supported GDI absence. Available55,944,364,032 bytes; no active native process or
booted Simulator. Stable full startup1058/504state/412device, default twelve
patches, SunPad and isolated/unpromoted FPU candidate are unchanged. G1 unresolved;
source-only request remains unsent. Goal active.

**Previous recovery — 9 October 2026: resident constructor returns to MOVIE.**

One uninterrupted invocation from `647F74` now returns the real object handle
`8C6774F0` to MOVIE `8CED144C` in **7,165 modeled cycles**. Six integrated controls
match complete CPU/RAM/AICA/timers against the independently checked steps. SQ,
flash, VRAM and inactive-device invariants pass. No helper interception, forced
return or signature fallback occurs. The whole invocation uses captured guest
toward-zero, both host rounding modes and three generated FPU modes. This proves
the recovered sequence from `647F74` through return, not normal boot or playback.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `66C37C` through retained resource getter | 8 | 29; both channel IDs, next `65E640` |
| `65E640` buffer query | 16 | 61 valid;53/34 invalid-index paths preserve outputs |
| Both query continuations and outer constructor return | 4 | 278; retained division/IRQ restore, returns `647F84` |
| Parent final continuation | 36 | 76 captured/74 nonnegative; returns MOVIE `ED144C` |
| Integrated `647F74` sequence | 6 | 7,165; same complete state at return |

Two new original bodies add166 instructions:47 for `66C37C`,119 for `65E640`.
All70 controls pass without repair. Captured SPRS buffer addresses are001FBFC0 and
001F7F80, both size4000. Indices16/255 follow original errors with no output writes.
The parent scalar tail is checked at source values−2048/0/2048, which yield exact
−1/0/+1 conversion inputs. The original coefficient3F599999 gives signed results
with magnitude below1 and truncation to zero. This narrow result does not close
general FPU/vector/exception gates. Original frame restoration and caller PR are
checked. No new default-runtime or production-foundation changes were made.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-channel-query/channel-query-checkpoint.json`:
  `c18ef4f61b70d819ae62f89453bcdc91709867055f8e24d5cd5d89633d801ede`.
- `20261009-startup-resident-buffer-query/buffer-query-checkpoint.json`:
  `a9bc058cde995ab7c4f2f54a22bfa1aae21bc7945ef0d1955600cbc55d462891`.
- `20261009-startup-resident-query-return/query-return-checkpoint.json`:
  `bba08032f20091c1c1b8cf64d802e715a4836a1a691128248adb64dbc79ab4fe`.
- `20261009-startup-resident-return/resident-return-checkpoint.json`:
  `074129143bccba501877f4e06f1e1289b53aa8e8ecd39dae7918309d639e196f`.
- `20261009-startup-resident-complete/resident-complete-checkpoint.json`:
  `f5d3a1dda05cefcb026bd7d2e32ba2fd6aaf05c1b752b7423b26d0138a3a0475`.

Next execute existing MOVIE `ED144C`, not another replacement resident constructor.
Use the six-entry `movie-instance-fourth-create` manifest and original MOVIE image
activation before replaying exactly39 known data words. The current final RAM was
checked against that activation contract. The original continuation predicts
eight cycles, no stores, PR `ED1464` and a call to literal `648342`. It has not
executed; check actual registration and dispatch before declaring the target absent.
Frozen prediction: `20261009-movie-post-resident-prediction/prediction.json`, SHA-256
`f6931e226977c3e4f1a2291af6826ffc8063280b8ced1640336705ffd615f907`.
The same proof rechecks original instruction bytes for division, IRQ restore,
resident parent and MOVIE wrapper against current RAM and original images.

`20261009-resident-complete-final-verification.json` rechecks1,564 sealed identities,
prediction inputs, unchanged default emitter, all1657 missing source paths and
supported GDI absence. Available56,302,714,880 bytes; no active native process or
booted Simulator. Full startup1058/504state/412device, default twelve patches,
SunPad and isolated/unpromoted FPU candidate are unchanged. G1 unresolved;
exact source-only request remains unsent. Goal active.

**Previous recovery — 9 October 2026: original stereo setup completes.**

One uninterrupted invocation from `647F74` now reaches `66C37C` in **6,721 modeled
cycles**, after service construction, record initialization, channel count and
both pan callbacks. Six integrated controls match complete CPU/RAM/AICA/timers
against the independently checked steps. SQ, flash, VRAM and inactive-device
invariants pass. No helper interception, substituted return or template fallback
occurs. Captured guest toward-zero, both host rounding modes and three generated
FPU modes are covered. The outer constructor, normal boot and playback remain open.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `6618D6` and `66C1AA` prefixes | 8 | 27; counts1/2, next `66D1F8` |
| `66D1F8`, retained count/callback/queue and caller | 4 | 664; next `66C0BC` |
| `66C0BC` through retained `10E67C` division | 20 | 103; pan −15/−1/0/1/15, next `66D1D8` |
| `66D1D8`, retained callback and adapter return | 24 | 270/278/280; both channels, signed pan and upper clamp |
| Caller, complete second pan and stereo-setup return | 4 | 444; returns `660A18`, caller reaches `66C37C` |
| Integrated `647F74` sequence | 6 | 6,721; same complete state at `66C37C` |

Five new original bodies add166 instructions:104 for `6618D6`,23 for `66C1AA`,
3 for `66D1F8`,33 for `66C0BC`,3 for `66D1D8`. All66 controls pass without repair.
Existing signed division, count setter, callback and queue/copy bodies are reused.
The signed pan conversion produces −127/+127 from −15/+15; ±1 checks truncation.
The second channel's +127 case exercises the original −2 intermediate clamp.
Canonical queue count advances4→8 and write index44→48. These are command-memory
writes with device clocks inactive; firmware consumption and playback are unproven.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-outer-channels/outer-channels-checkpoint.json`:
  `faeb9d038e8faf897794bf178ac5d856478c968cac777428b4e290b985a2dcfa`.
- `20261009-startup-resident-count-forward/count-forward-checkpoint.json`:
  `872d0cf89cf517e5e9677c3a233e7b95a694389f9e97e14147ff656dc1c5bc64`.
- `20261009-startup-resident-pan-adapter/pan-adapter-checkpoint.json`:
  `27e991f07f67e6e5df1cab00a7f762a3551560a7a47b8560de72c41fef7ddf88`.
- `20261009-startup-resident-pan-forward/pan-forward-checkpoint.json`:
  `74c9c80d66f215b6f268154e5b4a2cc57fbfd76d5deb700ce715d3829db94fec`.
- `20261009-startup-resident-pan-complete/pan-complete-checkpoint.json`:
  `4ea4bf769d49e89263e8435e7f8e5ed4f6ea2a9fd5fb73d1ab09fe3f4f14e044`.
- `20261009-startup-resident-stereo-complete/stereo-complete-checkpoint.json`:
  `e007f45b88fe68ef518ca45f4578a3a1bf146b3125eafb18f0b59d3a91acbf70`.

Next execute original `66C37C` from the integrated capture. Its47 instructions
match BOOT/live RAM. The already recovered `66E930` getter is the first callee;
do not replace it or classify it as missing. A frozen prediction covers18 prefix
cycles,3 getter cycles and8 caller cycles, with eight stores, before `65E640`.
Check the actual target map and signature fallback before expanding that target.
Prediction: `20261009-resident-channel-query-prediction/prediction.json`, SHA-256
`88420971a2d91105df13d5d4102812a05645ca928da8562c6a5d4ab2a3e8b50f`.
This next query prefix has not executed.

`20261009-stereo-complete-final-verification.json` rechecks1,891 sealed identities,
prediction inputs, unchanged default emitter, all1657 missing source paths and
supported GDI absence. Available56,686,391,296 bytes; no active native process or
booted Simulator. Stable full startup1058/504state/412device and default twelve
patches are unchanged. The FPU candidate remains isolated/unpromoted; no change
to SunPad controls or production foundation. G1 unresolved; source-only request
remains unsent. Goal active.

**Previous recovery — 9 October 2026: service constructor returns; outer record initializes.**

The original `66B91C` service constructor now returns its handle to `6609F4`,
and the outer constructor stores that handle before executing original `6608BC`.
A single uninterrupted invocation from `647F74` reaches `6618D6` in **5,213 modeled
cycles**. Six integrated controls match complete CPU/RAM/AICA/timers against the
independently checked steps; SQ, flash, VRAM and inactive-device invariants pass.
No helper interception, substituted return or template fallback occurs. Captured
guest toward-zero mode, both host rounding modes and three generated FPU modes
are covered. The outer constructor still has work remaining; this is not normal
boot, live audio or gameplay evidence.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| `66D184` tail + `66EE7C`, retained queue and both caller iterations | 12 | 123 normally,125 on wrap;281 combined to `66D190`; command46 queued twice |
| `66D190` + existing field setter, service return and outer handle store | 16 | 43/44 for tested byte values;139 combined to `6608BC` |
| Integrated `647F74` through service return | 6 | 5,162; complete state matches at `6608BC` |
| `6608BC` initializer and caller | 12 | 46 standalone,51 combined;23 original stores; next `6618D6` |
| Integrated `647F74` through initializer | 6 | 5,213; same complete state at `6618D6` |

Four new original bodies add88 instructions:3 for `66D184`,34 for `66EE7C`,6 for
`66D190`,45 for `6608BC`. Two original caller iterations submit command46 through
retained `65E1A4`/`10E964`; the actual code uses the same handle in both calls.
Canonical queue count advances2→4 and write index42→44. Queue insertion is checked
with device clocks inactive; firmware consumption and playback remain unproven.
The configuration wrapper truncates/sign-extends its input byte before the retained
field setter. Values127,0,255 cover the sentinel and signed input behavior.
The initializer's five byte stores and18 word stores preserve all other memory,
including the service handle. A separate A5-filled record control distinguishes
real clearing from pre-existing zeros; the canonical record is unmodified at entry.

The first generator run stopped before executing native code: its handwritten
instruction-count guard said33 for `66EE7C`, while both original review and analysis
show34. The failure and source are preserved. Repair corrected only that count
and result metadata; all five subsequent native runs pass without repair. No guest
or runtime implementation was changed. Integrated expected AICA now comes from the
direct parent's declared input, avoiding stale ancestor-path assumptions.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-channel-start/channel-start-failure-checkpoint.json`:
  `21ec623df6ca82bf54da6d2871863a6df7ffee6d11d1e8b0632f8358c1548186`.
- `20261009-startup-resident-channel-start-repair1/channel-start-checkpoint.json`:
  `b5df0730bc4a4fa1f2160d7a5f9bf0c650082345c94f83e8caf235f711432712`.
- `20261009-startup-resident-channel-configure/channel-configure-checkpoint.json`:
  `14d3eeace597614d879dc99d6a55638121010a140be5776d936b0ac45265ae45`.
- `20261009-startup-resident-service-complete/service-complete-checkpoint.json`:
  `73e8cb11488386f14680595507a80aa44fc9eaca8dfeb2820f342d5a62c7a1ff`.
- `20261009-startup-resident-record-init/record-init-checkpoint.json`:
  `0943db2865d8a3d38559f8aa0466a4641a60248c2cb6eb615203ee17be565d82`.
- `20261009-startup-resident-record-complete/record-complete-checkpoint.json`:
  `5c4a29bd71216102dc4fafabb6e2c0a7e3e8a638ae39cdd85eebefcdba910a50`.

Next execute original `6618D6` from the final integrated capture. Its104 reviewed
instructions, including internal continuation `661A0A`, match BOOT/live RAM. The
frozen two-channel prefix predicts15 cycles, three frame-word stores and one byte
store before `66C1AA`. It has not executed. Check the target map and signature
fallback before declaring the callee missing; retain original dependencies.
`20261009-resident-record-channels-prediction/prediction.json` SHA-256:
`7d44294a754e897580440fd3358f591e09aceddb077e437109ea1e19c13363ec`.

`20261009-record-complete-final-verification.json` rechecks1,823 sealed identities,
prediction inputs, unchanged default emitter, all1657 missing source paths and
supported GDI absence. Available57,138,155,520 bytes; no active native process or
booted Simulator. Stable full startup1058/504state/412device, default twelve
patches, isolated/unpromoted FPU candidate and SunPad profile remain unchanged.
G1 is unresolved; exact source-only request remains unsent. Goal active.


**Previous recovery — 9 October 2026: channel-pair constructor returns through both queued commands.**

One uninterrupted invocation from `647F74` now reaches `66D184` in **4,742 modeled
cycles**. The original `66CFC4` channel-pair constructor returns zero to `66BBB8`;
its caller stores the resulting handle and two-channel counts. Six integrated
controls compare complete CPU/RAM/AICA/timer state with the independently checked
steps. SQ, flash, VRAM and inactive-device invariants pass. No helper interception,
substituted return or template fallback occurs. The integrated path uses captured
guest toward-zero mode, both incoming host rounding modes and three generated FPU
modes. The enclosing service constructor, full startup and gameplay remain open.

| Original path | Controls | Modeled cycles / result |
|---|---:|---|
| Complete `66F0B4` channel0 callback, retained queue/copy | 8 | 257 normally,259 on ring wrap; result zero to `66F368` |
| Channel1 callback and enclosing count-setter return | 4 | 299 including caller to missing `66EEE0`; two commands queued |
| Integrated `647F74` through both commands | 6 | 4,616; same complete state at `66EEE0` |
| `66EEE0` field setter plus pair/caller continuation | 12 | 37 normally,36 for127 sentinel,126 combined to `66D184` |
| Integrated `647F74` through pair return | 6 | 4,742; same complete state at `66D184` |

This checkpoint adds36 controls and215 original instructions:165 for `66F0B4`,
50 for `66EEE0`. The two reached callbacks submit command38 using original
`65E1A4` and ordered `10E964` twenty-byte copies, including unchanged packet tails.
Canonical pending count becomes two and write index advances40→42. A separate
ring-wrap test advances255→0. This proves queue insertion with ARM7/device clocks
inactive; consumption, sound quality and playback are not established. Field
value zero produces byteFF; sentinel127 produces zero. Original pair tail stores
IDs15/14 and the per-channel flags before returning its actual frame.

Two native fixture failures and one integration preflight failure were preserved:

- Directly invoking the count-setter continuation returns to its caller; it does
  not automatically resume an unrelated host invocation. The first fixture expected
  a missing-helper exception too early. The repaired step verifies the exact return,
  then explicitly invokes the caller. The final integrated run uses one call.
- The next fixture passed CPU/RAM but failed timers. Recounting the original
  channel1 path gives a160-cycle prefix, not159, and299 combined cycles. The extra
  compare block and distinct packet arm explain the correction. No runtime or guest
  change was needed. Source trace: `20261009-second-callback-cycle-correction.json`,
  SHA-256 `2c1ed098e420fbb5eb8c08de80042cee9b4f3ab4769ef802a268aa8d17dd28cd`.
- Integration preflight rejected a stale ancestor AICA identity before creating
  output or compiling. Repair uses the direct parent's actual AICA input; evidence
  is `20261009-service-pair-integration-preflight.json`.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261009-startup-resident-channel-callback/channel-callback-checkpoint.json`:
  `98e2709e9f51fbec11d9b40d0da357f8279f9ad6af5519c9a434513372f01646`.
- `20261009-startup-resident-channel-count-return/channel-count-return-failure-checkpoint.json`:
  `8fa29ab9c250670a51db97db17869c0557adc984f42d430747c087a52b43fe3f`.
- `20261009-startup-resident-channel-count-return-repair1/channel-count-return-failure-checkpoint.json`:
  `dc00878ab7e43d5c5fba452ead1abf57ac7b11d484e3a46a32d2178d68987d11`.
- `20261009-startup-resident-channel-count-return-repair2/channel-count-return-checkpoint.json`:
  `3dec690735ef8b0c4ee7d389b40ce558588e3a58dfe572d0eb631fb674130234`.
- `20261009-startup-resident-service-commands-complete/service-commands-complete-checkpoint.json`:
  `8d8632d8efa5c4f7cfc5405de5f5a3c64b1be065a57b3651508291875fb1da46`.
- `20261009-startup-resident-channel-field/channel-field-checkpoint.json`:
  `0e9e86b118e82f445619474257aa1e3890d6104e4a261895eebe35e3db72bf5b`.
- `20261009-startup-resident-service-pair-complete-repair1/service-pair-complete-checkpoint.json`:
  `b542108bbaf45b33f1ff3f7c72367f3bfb032fe2513c7fa6dd67fea43f1b953b`.

Next execute original `66D184` using the integrated observed capture. Its three
instructions match BOOT/live RAM; the frozen four-cycle tail-jump prediction has
no RAM writes and preserves PR while targeting `66EE7C`. This has not executed;
check target inventory and signature fallback before calling the target missing.
`20261009-resident-channel-start-prediction/prediction.json` SHA-256:
`b0a0e4adacc48180efd681dfa2b5f230ed6e086b6be8eeb2a3cf0b92a39136ac`.

`20261009-service-pair-final-verification.json` verifies2,047 sealed identities,
prediction inputs, unchanged default emitter, all1657 missing source paths and
supported GDI absence. Available57,548,189,696 bytes; no active native process or
booted Simulator. Stable full startup1058/504state/412device, default12patches,
isolated/unpromoted FPU candidate and SunPad profile remain unchanged. G1 remains
unresolved; the exact source-only request is unsent. Goal active.


**Previous recovery — 9 October 2026: original channel setup reaches its sound-command callback.**

The original caller at `647F74` now reaches missing `66F0B4` in one uninterrupted
**4,060-cycle invocation**. Six integrated controls compare complete CPU/RAM/AICA/
timers with the independently checked continuation chain. SQ, flash, VRAM and
inactive-device invariants pass; no helper interception or signature fallback
occurs. The integrated path uses captured guest toward-zero mode, two incoming
host rounding modes and three generated FPU modes. This is still a constructor
prefix, not a whole-constructor return, live audio, full startup or gameplay.

| Original path | Controls | Result |
|---|---:|---|
| `66CFC4` with retained `65E770` allocator | 8 | 173/180 cycles for first/later free slot; next `66F2EC` |
| `66F2EC` active-channel prefix | 8 | 55 cycles; counts one/two; next callback `66F0B4` |
| Integrated original `647F74` | 6 | 4,060 cycles = 3,832 + 173 + 55; same complete state |

Two original bodies add242 instructions:162 for `66CFC4`,80 for `66F2EC`.
The tests establish the reached prefixes, not every compiled branch. The captured
third-pool flags were all zero; canonical execution allocates the first slot
without altering its incoming occupancy. A separate later-slot control deliberately
marks the first slot occupied. Channel IDs15/14, configured count two and active
count two flow from the prior real allocations into the original callback.

Source review corrected two timing descriptions before this execution:

- The previously unexecuted `66CFC4` prefix is106 cycles, not108. Static BRA costs
  one modeled issue cycle. The old prediction is preserved; repaired prediction
  SHA-256 is `6348b19a5923bf21a9c886dcae62f4a25692d0ac62df79f27e4f382d5b2d79bd`.
- Earlier caller sub-block counts contained offsetting errors: `64728A` is7
  cycles, `66BA34` is3, and `66BA28`/`66BAA8` are2 each. Their net correction is
  zero, so the verified2,185/3,832 totals and sealed captures are unchanged.
  `20261009-cycle-breakdown-correction.json` preserves the exact correction,
  SHA-256 `9b1d8d7718711807eed1d81fa7f5e7bf86a7ab5335cc8b46f4c798a05e24cb93`.

All three new native runs pass without repair. Immutable checkpoints under
`artifacts/recovery-aot/`:

- `20261009-startup-resident-channel-pair/channel-pair-checkpoint.json`:
  `2463203bac1862201b93e6eda453744b2e98d658ab4a8c05ce151f4d16e35b34`.
- `20261009-startup-resident-channel-count/channel-count-checkpoint.json`:
  `cdf0a89b251a861174afd69cb28c34079f297201789cb951eb43b2db66abaab5`.
- `20261009-startup-resident-service-channels-complete/service-channels-complete-checkpoint.json`:
  `5587cc427a7b5ae1f4569b80ddd8cbc4b18e65ae754c6e3814cd60faa90529d3`.

Next: execute original `66F0B4` using the integrated observed capture. Its165
instructions match BOOT/live RAM. The frozen channel0/count2/value0 prefix predicts
152 cycles/21 stores to existing `65E1A4`; command38 at `8C8EE108` has twelve bytes
`2600000020c0000000000000`. Prediction includes complete state/RAM hashes and ordered
writes, but has not been executed. Review/model the retained queue path before
accepting the callback return; do not replace it or label it missing.
`20261009-resident-channel-callback-prediction/prediction.json` SHA-256:
`fa6176cecb6302981a417b5b98c46b9914e0c75435b3c8fce0a20c42dcf68cab`.

`20261009-service-channels-final-verification.json` rechecks893 sealed identities,
prediction inputs, default-emitter identity, all1657 missing paths and supported
GDI absence. Available58,073,350,144 bytes; no active native process or booted
Simulator. Stable full startup1058functions/504state/412device and default twelve
patches remain unchanged. The FPU candidate remains isolated, SunPad profile
unchanged, G1 unresolved and exact source-only request unsent. Goal active.


**Previous recovery — 9 October 2026: both original buffer bindings and four allocations execute.**

One uninterrupted invocation from `647F74` now reaches missing original helper
`66CFC4` at **3,832 modeled cycles**. Six integrated controls cover the captured
guest toward-zero mode, two host rounding modes and three generated FPU modes.
Complete CPU, RAM, AICA memory and timer state match the separately checked chain;
SQ, flash, VRAM and inactive-device invariants also pass. No helper is intercepted,
no return is substituted, and the runtime reports zero signature-relocation matches.
This remains a constructor prefix, not whole startup, media playback or gameplay.

| Original path | Controls | Evidence |
|---|---:|---|
| `65E470` with explicit original `66AF40` and retained PIO chain | 8 | 1,331 cycles; return `66BA16`, result zero; two AICA word stores; channel IDs 15/14 and both guest/host rounding modes |
| Caller `66BA16`, second buffer and retained `647220` allocator | 4 | 2,185 cycles; two more AICA stores and four real allocations; next `66CFC4` |
| Integrated caller `647F74` | 6 | 3,832 cycles = 316 + 1,331 + 2,185; same full state as the independent steps |

Two new original bodies add 178 instructions: 153 for `65E470`, 25 for `66AF40`.
Compilation of the whole bodies does not establish every branch. Reached SPRS
paths write address/length pairs at AICA offsets `141F8/141FC` and `141F0/141F4`,
with buffer addresses `1FBFC0` and `1F7F80`, length `4000`. The native tests verify
these writes with ARM7/device clocks inactive; there is no live sound claim.
The existing record pool starts with slot zero occupied. Original allocation
reserves the next four records without manufacturing free slots. Captured AICA
memory is now an explicit input to continuation runs.

Two failures were preserved and resolved:

- The first prefix fixture expected `66AF40` to be absent. Runtime signature
  fallback instead selected existing template `66E97C`. Target-map absence alone
  is insufficient. Both original helper bodies were reviewed, and explicit
  original `66AF40` was emitted so original internal return addresses are retained.
  The runtime's global fallback behavior was not changed.
- The first full-buffer model overcounted one cycle per SR restoration. Original
  `STC SR` costs two modeled cycles and `LDC SR` four; the nine-instruction restore
  block totals 13. Correct totals are transfer 604, word helper 631, buffer 1,331.
  Full CPU/RAM already matched in the failed run; timer and AICA acceptance only
  follows from the repaired passing run. No guest/runtime repair was needed.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261008-startup-resident-buffer-bind/buffer-bind-failure-checkpoint.json`:
  `1177c7a2648ac71cc58ec24c61a60acdf8cd557174257e74b0e292471a22ab70`.
- `20261008-startup-resident-buffer-complete/buffer-complete-failure-checkpoint.json`:
  `d788e8f5248f2f1e0b50bb16e15012a970e937482fdf0cc6c2d2e22dbb8a933e`.
- `20261008-startup-resident-buffer-complete-repair1/buffer-complete-checkpoint.json`:
  `8447bb3c1360bf54795121f5459d3eee888c4d86f8a550c4e3c3a0bed2910199`.
- `20261008-startup-resident-buffer-pair/buffer-pair-checkpoint.json`:
  `75404423560d26656079344d297e69572add84a16c665ba9d3736e37a3fa0cec`.
- `20261009-startup-resident-service-buffers-complete/service-buffers-complete-checkpoint.json`:
  `9bf53e5dd489663488d065ce6320b17759e415f2389320508e4581e0ed3e7398`.

Next: execute original `66CFC4`, using the integrated observed RAM/state/timers and
AICA. Its 162 original instructions match BOOT and live RAM. The unexecuted
captured two-channel prefix predicts 108 cycles/nine stores to retained `65E770`.
Do not classify that retained helper as missing or intercept it. Prediction:
`20261009-resident-channel-pair-prediction/prediction.json`, SHA-256
`4b1017bc93ebfce95fde610aefc93ccce82d3073417f8b360abf7db8d047c0ff`.

Final verification `20261009-service-buffers-final-verification.json` rechecks
1,491 sealed identities, prediction inputs, all 1,657 missing source paths,
supported GDI absence and the unchanged default emitter. Available storage is
58,310,565,888 bytes; no native game/compile process or booted Simulator remains.
Default twelve patches, stable full startup 1058 functions/504 state/412 device,
SunPad single-stick profile and G1 status are unchanged. The FPU candidate remains
isolated and unpromoted. Exact source-only request remains unsent in
[BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md). Goal active; no boot/playability claim.


**Previous recovery — 8 October 2026: original constructor chain reaches buffer creation.**

Starting at the verified channel return `647F74`, a single continuous invocation
now reaches actual missing buffer constructor `65E470`. **Six integrated controls
pass at316 modeled cycles**. They use the original enclosing caller, tail jump,
second constructor, existing interrupt-mask helper, record pools, two real resource
allocations and original getters. Complete CPU/RAM/timer state matches the separately
checked chain. No helper is intercepted or its return substituted. SQ, flash,
sound RAM, VRAM and device invariants are checked. This integrated capture covers
the original guest toward-zero state, two incoming host rounding modes and three
generated FPU modes. It is a prefix, not an enclosing-constructor return or boot.

The nine new runs passed without a repair:

| Original path | Controls | Modeled cycles / reached boundary |
|---|---:|---|
| `647F74` caller | 8 | 9; actual next `65F214` |
| `65F214` original tail | 8 | 4; actual next `66093E`, PR preserved |
| `66093E` successful validation + retained `6607A0` | 24 | 55/62 for one/two descriptors; next `660916` |
| `660916` pool + caller | 20 | pool17/38/68, combined51/72; next `66B91C` |
| `66B91C` service constructor prefix | 8 | 20; next `66B8F6` |
| `66B8F6` pool + caller | 20 | pool16/37/67, combined34/55; next `66E902` |
| `66E902` resource allocation + caller | 20 | pool17/38/67, combined36/57; next `66E930` |
| `66E930` getter + second allocation/caller | 16 | getter3, combined39/100; next `65E470` |
| Integrated original `647F74` prefix | 6 | 316; exact same `65E470` boundary |

This checkpoint adds130controls and seven original bodies/590instructions.
The393-instruction service constructor is generated whole, but only its reached
prefix has execution evidence. No broad function-completeness claim follows from
its compilation. The second constructor's133instructions are subject to the same
limit. Pool controls include first/later free slots and a fully occupied pool.
All three canonical fixture pool occupancies were checked against their incoming
captures and were already zero; the canonical path did not manufacture free slots.
The existing interrupt-mask helper saves the incoming mask and raises it; tests
cover masks0/5/15. The later restoration remains ahead on this constructor path.

On the captured path the resource allocator reserves `8C8EE064` and `8C8EE06C`;
the original getter reads their IDs15 and14. The original caller passes
`r4=52535053` (SPRS), `r5=15`, `r6=001FBFC0`, `r7=00004000` to `65E470`.
These are argument and state results; there is no audio/media-playback evidence.

Immutable checkpoints under `artifacts/recovery-aot/`:

- `20261008-startup-resident-channel-return/resident-channel-return-checkpoint.json`:
  `17f83cb4abc253ea9dcb53bb7ddbc8e58e48052b169af0a8406faeafce90d35b`.
- `20261008-startup-resident-constructor-tail/constructor-tail-checkpoint.json`:
  `814360b4b45de2799b3f1dd14195bf77f25d95da92fbbe1f1bb7d940e8eeba67`.
- `20261008-startup-resident-second-constructor/second-constructor-checkpoint.json`:
  `7096d982e78e53c08f49df404b368705cf3f9cdcb3aa9e506bb8f723218881b6`.
- `20261008-startup-resident-second-pool/second-pool-checkpoint.json`:
  `bf3a4c66fd0f2c84214b0308722c1c75e040144bd3a01a7355171be9ce1f5b3a`.
- `20261008-startup-resident-service-constructor/service-constructor-checkpoint.json`:
  `175358e4a69a382efa1b23638bdc7ec83061d02427b93247d55c82db6aabb779`.
- `20261008-startup-resident-service-pool/service-pool-checkpoint.json`:
  `57f7daf19dd14f610d21f4d5abaab04b7445f8bef6d27bf48347ba3e0ffc23c2`.
- `20261008-startup-resident-channel-resource/channel-resource-checkpoint.json`:
  `436899e0498bc12956519c680a1e85974439377cdb7790ec7c00894d946ab125`.
- `20261008-startup-resident-resource-id/resource-id-checkpoint.json`:
  `e5bad1cd8e27c1b86c1cf6200be968d6b40b7e9bb96b71d57cc8c286600f0980`.
- `20261008-startup-resident-service-prefix-complete/service-prefix-complete-checkpoint.json`:
  `b768a5c50c1773465d4993a758fd249ea3c28cc35a6784ccf16255ebcdc755fd`.

**Next:** execute original `65E470` from the integrated capture in
`20261008-startup-resident-service-prefix-complete`. Review:
`work/dreamcast-recovery/loader-review/startup-producers/65E470-20261008-service-prefix-complete-next/function.txt`.
All153 original instruction bytes match BOOT and reached RAM. The captured global
at `67FF10` is `8C8D7680`, and its offset1C0 is14000; it is initialized, so do not
substitute an early error return. For the captured SPRS/ID15 path, the independent
prospective model predicts50cycles, seven frame stores, target literal `66AF40`,
`r4=000141F8`, `r5=001FBFC0`, `r14=8C8D7A5C`, `PR=65E5C2`, and missing-call
PC `65E5BA`. The prediction is **unexecuted**. Check the actual runtime's complete
registration inventory before classifying `66AF40` as missing.
Prediction `20261008-resident-buffer-bind-prediction/prediction.json`, SHA256
`655de7090597d7facc70dc6091d1fb06df3b07308414e3431f46b385ce639934`.
Proceed through original buffer binding and the constructor's remaining calls,
then verify its return and integrate the accumulated bodies into startup separately.

`20261008-service-prefix-final-verification.json` rechecks2627 sealed identities,
canonical pool occupancy and all1657 still-missing source paths. No missing input
has appeared, and supported PAL GDI remains absent. Free storage55,466,242,048bytes,
above unchanged guards. All native runs are terminal; no Simulator launched.
Default emitter and twelve patches remain unchanged; isolated FPU candidate only.
Full startup remains1058functions /504state /412device, with no new integration
claim from these units. Pinned SunPad single-stick profile is unchanged. G1, normal
boot, media playback, gameplay, saves and mobile/device acceptance remain open.
Exact source-only request remains unsent in BOOTSTRAP-BLOCKERS.md. Goal active;
both previous and current goal turns produced concrete native progress.

**Previous recovery — 8 October 2026: complete original channel constructor returns.**

Original `65F364` now executes as one uninterrupted call, including the three
getters, work/rate allocators, retained clears, complete original rate-math chain,
callback setter and channel initialization. Six controls pass at **1750 modeled
cycles**, return record `8C8A7B90` and actual return PC `8C647F74`. Complete CPU,
RAM and timer results match the independently checked continuation chain; SQ,
flash, sound RAM, VRAM and device invariants are preserved. No helper is intercepted
or its result substituted. This whole-call evidence covers the captured guest
rounding-toward-zero state, two incoming host rounding modes and all three generated
FPU modes. It does not claim whole-constructor nearest-rounding coverage.

The new steps are:

- Original work continuation `66B13C`: eight controls,31cycles,nine writes, returns
  to `65F3DE`, matching the prior prospective state and full RAM hash.
- Original channel continuation `65F3DE`: eight controls,7cycles,one write; actual
  missing helper `66B3CC` is confirmed by runtime inventory and execution.
- Original five-instruction `66B3CC`: installs the callback/context at work-record
  offsets136/140. Eight leaf and eight retained-caller controls pass; leaf6cycles,
  combined32/42cycles for one/two descriptors, actual next `65F346`.
- Original fifteen-instruction `65F346`: initializes eight channel fields. Eight
  leaf and eight retained-caller controls pass;16/33cycles, then activation and
  frame restoration return to `647F74`.
- Whole constructor: six controls,1750cycles. Total this checkpoint:54controls,
  two new original bodies/20instructions. These remain separate from full startup.

Two fixture failures are preserved. First, nested test-main macros caused a
compile failure; the repair copied the existing helper declarations into a new
fixture. Second, a hard-coded return label `647F18` failed the boundary assertion
before game execution. The original call at `647F70`, saved PR and observed state
all identify `647F74`; repair2 uses that address and passes. No runtime or game-body
change was required. The earlier initializer unit's descriptive result/prediction/
log label also said `647F18`; its computed expected state and native capture already
agree on `647F74`. The sealed files remain intact; explicit correction is recorded
in `20261008-channel-return-metadata-correction.json`.

New immutable checkpoints under `artifacts/recovery-aot/`:

- `20261008-startup-rate-work-return/work-return-checkpoint.json`:
  `8a0ea3ac0c4a48a753e41ec957e42351caac5e20b3e809047fee5e38c522da0d`.
- `20261008-startup-rate-channel-bind/channel-bind-checkpoint.json`:
  `7274222453845fc5801763efac5b53c9cd6b7a400d3cc648daef5256f759621b`.
- `20261008-startup-rate-callback-setter/callback-setter-checkpoint.json`:
  `167179e73dc36f67aca008722052c998e21334e8ebc83682ce9dd7861e6b2df3`.
- `20261008-startup-rate-channel-initialize/channel-initialize-checkpoint.json`:
  `4a975d8ad5485398ae152b916445e12d7d53e9768921b330313e8ad07c5b4b80`.
- `20261008-startup-rate-channel-complete/channel-complete-failure-checkpoint.json`:
  `f8f7c619edcf9bc5afa812f033713c5e2d33fd538c01049c9e1f1ede402834dc`.
- `20261008-startup-rate-channel-complete-repair1/channel-complete-failure-checkpoint.json`:
  `1d1cc08281aa8f171d47454beaf814a68557ab19b0afde19503fe132b8c7e677`.
- `20261008-startup-rate-channel-complete-repair2/channel-complete-checkpoint.json`:
  `820d15a12950a80601f086875ff3b6c655c4b168c4a1bc336b25ce1e5adf8e92`.

**Next:** execute existing original `647F74` using `resident_pool_register` from
`20261008-startup-movie-resident-pool/movie-resident-pool.o` (24 managed entries).
Start at the whole-constructor capture from `20261008-startup-rate-channel-complete-repair2`.
The successful prefix predicts9cycles, one pointer store at `r14+4`,
`r4=r14+24`, `r5=r12`, `PR=647F84`, and literal target `65F214`; the missing-call
PC would remain `647F7A`. Verify all actual registered/internal entries before
classifying that target as missing. Original/live prefix instruction bytes match.
Prediction `20261008-resident-channel-return-prediction/prediction.json`, SHA256
`29e7814b19dfa743696a5e18e43ed09761494572a43fd6c9b5a8431d0dada42b`, is unexecuted.
The original pool checkpoint is
`b26729c90f038dab54e66b2fda5f4d65bcde8abd2d753450d60ff66699790e1e`.
Continue toward the whole resident allocator return, then integrate and revalidate
startup under the isolated candidate; do not conflate unit work with full startup.

Final verification rechecked1883 sealed identities and all1657 missing source
paths: none appeared; the supported PAL GDI remains absent. Available storage is
56,150,745,088 bytes, above unchanged guards. No native process or Simulator remains
running. Default emitter and twelve-patch runtime are unchanged; broader FPU
promotion/renderer differences remain open. Full startup stays1058functions /
504state /412device. Pinned SunPad single-stick profile is unchanged. G1, normal
boot, media, gameplay, saves and mobile/device acceptance remain open. Exact
source-only request remains unsent in BOOTSTRAP-BLOCKERS.md. Goal active.

**Previous recovery — 8 October 2026: original rate calculation returns; allocator continuation returns its record.**

The floating-point blocker is cleared for the captured rate-calculation path.
Original `65EB00` executes its original wrapper `11152C`, classifier `1118DC`,
retained range reducer `1117D8` and polynomial `111E38`, then returns to `64890C`.
**24 complete-call controls pass at 284 modeled cycles.** Exact rational binary32
and square-root predictions match the complete CPU/RAM/timer state and the two
halfword stores: `894930=1CA6`, `894932=F32D`. Both rounding modes, both incoming
host modes, both context-loading methods and three generated FPU modes are
covered. The rounding modes differ internally even though these two final
halfwords agree. No helper return or coefficient is substituted.

The separate helper evidence passes16 wrapper-prefix controls,32 finite classifier/
reducer controls and96 polynomial-return controls. The classifier's first run
correctly stopped at an inventory assertion: `1117D8` was already registered.
The repaired test invokes that original body. Its normal finite division is
checked against an exact rational prediction; it is not a replacement implementation.
The polynomial performs the original six FMAC iterations, with predictions from
its original seven coefficients. Exceptional/nonfinite paths remain outside these
controls. Three new original bodies add147 instructions; the retained reducer
is not counted as new.

After the rate function returned, its original allocator continuation `64890C`
ran the retained8-byte clear and returned rate record `894900` to `66B13C`.
**Eight continuation controls pass at63 modeled cycles**, including dirty clear
bytes. This is an internal continuation from the captured return, not yet one
whole allocator invocation. This turn adds176 controls in total; they remain
isolated recovery evidence. Full startup remains1058 functions /504 state /
412 device comparisons. Later unit bodies have not been integrated into it.

The explicit domain-owned FPU candidate remains separate from the default twelve
patches. Prior strict scene, host-domain and cached-vector evidence remains valid;
general FTRV/FIPR hardware equivalence, exception/DN edges and global promotion
remain open. Production foundation and pinned SunPad single-stick profile are
unchanged. No Simulator was launched. All native runs are terminal, with storage
still above every unchanged guard.

New immutable checkpoints under `artifacts/recovery-aot/`:

- `20261008-startup-rate-math-wrapper/wrapper-checkpoint.json`:
  `d842249a0e0bd422d4c6ede4e4fb5b691b17f999f1ab49e8a5a1fdab666797f9`.
- `20261008-startup-rate-math-classify/classify-checkpoint.json` (inventory failure):
  `2461afa4fc385fcbda65a670a8a5a2924dfa46652fa2a5120c436c36778cb522`.
- `20261008-startup-rate-math-classify-repair1/classify-checkpoint.json`:
  `fd3eac9cdc4a2b14cc7d4fa15e399a06a84a262bfe4b0ab493e7049ac3eb16be`.
- `20261008-startup-rate-polynomial/polynomial-checkpoint.json`:
  `99a4796dffa71c0dbd821cc68c466cbdad6a6a0bb53909dcf6009b56232853f9`.
- `20261008-startup-rate-complete/rate-complete-checkpoint.json`:
  `c98e2227fc3f307b6106fb6178eed14da0912e7c6965fc89ad6135cb6393216e`.
- `20261008-startup-rate-pool-return/pool-return-checkpoint.json`:
  `f415ce63f861c75c65dfcaec513271dfcb9e5abecd0ed56982ccb847ce18dcd5`.

Two preflight-only script failures are preserved separately: relative checkpoint
keys, and a reserved Python keyword in diagnostic metadata. Each was repaired
once before native execution; no game/runtime repair was needed. Polynomial
checkpoint output keys are relative; resolve them against the repository root
when validating. Do not rewrite that sealed checkpoint.

**Next:** execute original managed continuation `66B13C` with the existing
`66B0F4` body, from the pool-return capture. Original/live instruction bytes match.
The prospective31-cycle, nine-store model returns work record `8DA42C` to
`65F3DE`; it has not run. Prediction:
`20261008-rate-work-return-prediction/prediction.json`, SHA256
`d91933ac0d982a0b4e0a4e5113d2934113a5dccd9b38d92133c45179c9a653c9`.
Check all registered/internal entries before treating a target as missing. Continue
the original caller chain, then integrate and revalidate the candidate startup
path separately from the unchanged default-runtime checkpoint.

G1 remains unresolved. The local Katana/generated/header/contract/provider and
supported PAL-input inventory is still open; recovery sources do not supply those
missing inputs. BOOTSTRAP-BLOCKERS.md retains the exact unsent source-only request.
Normal boot, media playback, gameplay, saves and mobile/device acceptance remain
unproven. Previous and current goal turns made concrete native progress; goal active.

**Previous recovery — 8 October 2026: ARM64 FPU ownership corrected in an isolated candidate; strict scene rebuild verified.**

Storage is available and the goal remains active. Candidate
`patches/recovery/0013-arm64-fpu-domain.patch` is separate from the default twelve
patches. The default emitter remains SHA256
`2938ffb672743140015577efc385d1a8809e2b7a06e39aed284b2a56d50704ec`.
No Simulator was launched; all native processes are terminal.

The actual candidate emitter now builds and emits a runtime byte-identical to its
staged source. Original `65EB00` passes eight rounding/context controls and reaches
real missing `11152C` after 25 modeled cycles. Two further defects were reproduced
and corrected: guest execution leaked FPCR/FPSR into host work, and cached vector
regions could inherit the wrong rounding mode after entry or an FPSCR write.
The latest candidate passes **64 host-state controls, eight renderer-invariance
cases, and 24 original-transform entry controls**. The intermediate candidate
failed 18 of the 24 transform controls; that failure is preserved.

The saved scene harness was rebuilt with strict floating-point flags across
**257 generated guest translation units**. One compile-command repair restored
an existing `record_counter_register` macro omitted by the rebuild script; the
other 256 successful outputs were reused. Both scenes return. Their six images
and both collision RAM dumps exactly match the preceding domain-owned candidate.
Selection, candidate geometry, terrain, update-state and model-release checks
pass (nine follow-up audits). This is strict compilation of saved generated
sources, not wholesale regeneration or normal boot.

The unchanged legacy geometry audit still fails because it expects nearest
translation. A read-only trace observes 2,799 collision transforms per scene:
2,787 source vertices and 12 model centers, all with FPSCR `00040001`. Every input
and identity-plus-translation matrix matches the original SBOARD source. The
separate candidate audit checks exact toward-zero translation consistency, the
existing centroid/radius tolerances, bounds, buckets and pool partition; both
929-triangle captures pass. One-ULP vertex and zero-radius corruptions are rejected.
No old reference or legacy checker was replaced. Collision-miss auditing remains
behind its legacy geometry dependency.

**Vector accuracy remains open.** The candidate translations fall within the
normal-input error envelope documented in the [Renesas SH-4 manual, sections
6.6.1 and 9.47](https://www.renesas.com/en/document/mas/sh-4-software-manual).
This is not bit-exact hardware evidence: 168 coordinate input pairs per scene
need more than 30 aligned magnitude bits. General staged FTRV/FIPR, exception and
denormal edge behavior, and changed scene pixels are not certified by these
checks. Images still differ from the old runtime by 17/1830/1855 bytes per trio.
Global candidate promotion remains gated; the focused scalar prefix may continue
using the explicitly isolated candidate and independent arithmetic expectations.

New immutable checkpoints under `artifacts/recovery-aot/`:

- `20261008-startup-movie-resident-rate-domain-candidate/rate-prefix-checkpoint.json`:
  `6b6d3c7d3d8b78c7d7f320b24d1d4e9e059ed8025950de7aa91bbac947ae44e4`.
- `20261008-fpu-domain-boundaries-v3/fpu-evidence-checkpoint.json`:
  `b664c8c9759e85d4cd4eb243f32f9aa41c3c69576de01f8ac5448758d87b1250`.
- `20261008-fpu-cached-entry-after/fpu-evidence-checkpoint.json`:
  `fcd105cf92589d93f6a2d1736b0db37a22919604f569f56011adad3edd066111`.
- `20261008-arm64-fpu-domain-baseline/fpu-evidence-checkpoint.json`:
  `ac3432265db5f0ede6dea2d58a1e220d226d6ae490ddea153ef2a67b1c4abe21`.
- `20261008-fpu-transform-trace/fpu-evidence-checkpoint.json`:
  `762edfcb1fbc9978f47e019cbdd0a82474faf47dd671ef647d3fe87d27205ffe`.
- `20261008-arm64-fpu-domain-strict/strict-compile-checkpoint.json` (link failure):
  `521a35a58a57f59428f6b3b81c2038788520a7b20d2ddb1371f3b185e5c8b3cd`.
- `20261008-arm64-fpu-domain-strict-repair1/strict-scenes-checkpoint.json`:
  `f234cb8a69385d5589d2a49b2ec99891a158e0739ef4510e59e254c570279169`.
  Native binary: `f866073ed8f51d36ee302e7e194340047f97bce8edbe0c309561d86c517fc212`.

**Next bounded experiment:** execute the original `11152C` wrapper prefix using
the latest candidate's captured `observed-ram.bin`, state and timers. The review
at `work/dreamcast-recovery/loader-review/startup-producers/11152C-20261008-domain-rate-next/`
verifies all instruction bytes against BOOT and live RAM. Its first five
instructions save PR, reserve four bytes and call `1118DC`; no return may be
substituted. Record prospective CPU/RAM/timer effects before native execution,
then review only the actually reached next body. The full startup remains
1058 functions / 504 state / 412 device comparisons, with 21 later bodies and
919 controls still separate. Candidate FPU controls are not added to that count.

G1 is still unresolved. `artifacts/bootstrap-missing-path-recheck-20261008-fpu-domain.json`
rechecks every previously missing path: no Katana/generated/header/contract/title
item appeared, and the supported PAL GDI remains absent. Present-source byte
verification is inherited from the prior full audit. The exact inventory and
unsent source-only request remain in BOOTSTRAP-BLOCKERS.md. Production foundation,
pinned SunPad profile, normal boot, playback, gameplay, saves and mobile/device
gates remain unchanged. This turn made native progress; earlier holds are historical.

**Previous recovery — 8 October 2026: storage cleared; full integration verified; ARM64 rounding defect reproduced and candidate tested.**

Cleanup restored roughly60GB before execution; 59,351,490,560 bytes remain at this
checkpoint. The goal is active again. No storage guard was lowered or evidence
deleted. All native runs are terminal; no Simulator launched.

The saved nine-body integration now executes in full startup: **1058functions,
504state/412device comparisons,483unchanged prior captures**. Both same-binary
scenes, ten external audits, release corruption controls and six unchanged images
pass. Next full-startup missing target is still `ED8536`, at421additional modeled
cycles. The21later bodies/919controls remain separate unit evidence, not integrated.
Full checkpoint `20261008-startup-movie-instance-video-complete-startup/`
`movie-instance-video-complete-startup-checkpoint.json`:
`3caf6b6413d09528fd9dc7026456d9c7f92eb73a0150fc1a4c2df0aa2ce6dac8`
(196883identities;7217freshly checked). Its runtime still has the known rounding bug;
unchanged baseline results establish compatibility, not general FPU correctness.

The native extracted-helper probe confirmed ARM64 FPCR remained0 after guest
FPSCR40001. Original66-instruction `65EB00` was then generated and linked against
the actual unchanged runtime. **Six diagnostic controls** reproduce the defect:
direct context assignment and `dc_write_fpscr` both leave arithmetic at nearest
rounding; two forced-host controls establish the toward-zero alternative. The
25-cycle prefix, six frame stores, complete CPU/RAM/timers match the independent
rational prediction and stop at real missing`11152C`. No helper return is invented.

Candidate `patches/recovery/0013-arm64-fpu-mode.patch` reads/writes ARM64 FPCR,
checks the actual host mode during arithmetic (including restored contexts), and
emits strict floating-environment pragmas for Clang ARM64 guest sources. It is
**not applied to the default source or twelve-patch runner**. Its separately
compiled full runtime and strict original prefix pass **eight controls** across
both guest/host rounding modes and direct/writer context loading. Product/ratio
now match requested zero rounding:4544597A/3D91E52F. Exception flags, denormal edge
semantics, general vector math and host-state ownership are not thereby proven.

**Broader candidate regression failed**, preserved without replacing references.
The existing geometry checker expects nearest translation. Both native scene
runs return, but its first geometry audit rejects triangle vertices; separate
exact-rational diagnostics find all929triangles in each scene match toward-zero
translation. This explains the narrow mismatch as a rounding hypothesis, not a
complete transform contract. All six candidate image hashes differ (17/4133/4149
bytes per repeated scene trio). Later external baseline audits were not completed.
Old cached full-scene guest objects retain their original compilation mode.

Evidence under `artifacts/recovery-aot/`:

- `20261008-resident-rate-fpu-probe/fpu-probe-checkpoint.json`:
  `3e7cde2592c41b3dc875a47024ed219b33a7ae60c9aac1c6b9107699520d996b`.
- `20261008-startup-movie-resident-rate-prefix-unfixed/rate-prefix-checkpoint.json`:
  `c29414de02514e17eb1fb23ed4a8cda2cc36f2989b7a72dc08145ce608b49c02`.
- `20261008-startup-movie-resident-rate-prefix-candidate/rate-prefix-checkpoint.json`:
  `552721f679e1ba915f6208b4a7a4312550f2db91801e4ba2722db2f6150d1e37`.
- `20261008-arm64-fpu-candidate-baseline/candidate-baseline-checkpoint.json`:
  `310eab8bd93d08bc98ac88399bc2dabfa44e30860fbf3b708faf47cff22c2ed4`.

**Next:** resolve the FPU migration before adding downstream callees. Build the
candidate emitter separately and verify its actual generated preamble; current
candidate-prefix test applied that preamble directly to old-generator output.
Test guest-entry/exit and host-renderer rounding ownership, then derive the actual
transform instruction contract and update only independently justified arithmetic
expectations. Renesas specifies staged approximate computation for FTRV/FIPR;
a general matrix-operation replacement is insufficient ([SH-4 manual, sections
6.6.1 and9.47](https://www.renesas.com/en/document/mas/sh-4-software-manual)).
Preserve old captures; do not label differing images correct or promote0013 until
the affected regressions have valid independent expectations. No candidate repair
attempt has yet followed the geometry failure.

G1 remains unresolved. Actual Katana/generated/header/provider/PAL gaps and unsent
source-only acquisition request remain in BOOTSTRAP-BLOCKERS.md; prior byte audit
is inherited, missing paths were checked by the full checkpoint. Production
foundation and pinned SunPad single-stick profile unchanged. Normal boot, playback,
gameplay, saves and mobile/device acceptance remain open. This resumed turn made
native execution and correction progress; goal active. Earlier holds are historical.

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

**Latest recovery — 8 October 2026: rate-record pool passes; ARM64 rounding audit prepared.**

Original resident `6488AE` and the retained 52-byte clear pass **12 native
controls** (8 ready, 4 full-pool returns). Captured boundaries at 29, 297 and 314
modeled cycles match the prior independent predictions for CPU, RAM and timers.
The next missing function is `65EB00`, with inputs 500/44100 and output pointers
`894930`/`894932`. ARM64/no-runtime-emission checks pass. No runtime/compiler change.
Checkpoint: `artifacts/recovery-aot/20261008-startup-movie-resident-rate-pool/`
`movie-resident-rate-pool-checkpoint.json`, SHA-256
`cd0f85c8d4e2125975b86cd240e507fe8646d63887a45855ee03b2cd4926344f` (292 identities).

**New source finding:** captured FPSCR is `40001`. The runtime helper synchronizes
rounding/denormal handling on MSVC/SSE2 but does nothing on ARM64. Its arithmetic
guard accepts this mode without synchronizing, and captured-context assignment
also bypasses the FPSCR writer. The identical helper appears in the cached runtime
and pinned emitter. This is a source finding; no native arithmetic mismatch or fix
has been demonstrated. Earlier transfer/bit-pattern controls do not establish
arithmetic correctness.

An exact rational model predicts the next 25-cycle prefix, six ordered frame
stores, complete modeled CPU/timers and RAM hash. Positive-finite binary32 results:

| Operation | Nearest/even | Toward zero requested by guest |
|---|---|---|
| literal × 500 | `4544597B` | `4544597A` |
| square root of 2 | `3FB504F3` | `3FB504F3` |
| product ÷ 44100 | `3D91E531` | `3D91E52F` |

Predictions are **unexecuted**, including the next call to `11152C`. Guest exception
flags and true hardware latency are not proven. Source audit and exact prediction:
`artifacts/recovery-aot/20261008-resident-rate-fpu-audit/source-audit.json`.
RM mapping is checked against Renesas's FPU guide sections 1.2.1 and 4.1; section
2.3 contains an inconsistent reversal and was not used:
[Renesas FPU guide](https://www.renesas.com/en/document/apn/sh-compiler-application-note-2-compiler-use-guide-coding-guide-using-fpu).

**Next:** run the prepared bounded native probe once free space reaches the
unchanged 419,430,400-byte focused guard:

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/run-resident-rate-fpu-probe.py
```

It tests the unchanged extracted helper bodies with strict rounding compilation
and known hardware controls. This is not linked full-runtime or generated guest
execution. If it reproduces the gap, make a separately identified ARM64 correction,
verify actual generated arithmetic and context-entry synchronization, then rerun
appropriate baselines before claiming integration. Existing generated units lack
explicit `-frounding-math`; a host-register change alone is insufficient evidence.
Do not expand downstream callees before the next native experiment.

The probe guard returned **storage hold**, 393,916,416 bytes available; no build
or output directory created. No evidence deleted. There are now **21 new bodies**
(12 module/9 resident), **961 instructions and 919 controls** beyond the saved
1058-function compiled integration. Latest actual full startup remains
1049 functions/484 state/392 device checks; prospective 504/412 remains unexecuted.
Full capture requires 2,952,790,016 bytes. Preserve separate old-runtime evidence
if the FPU correction changes its identity. No Simulator or native instance running.

G1 remains unresolved; exact Katana/generated/header/provider/PAL gaps and the
unsent source-only request remain in BOOTSTRAP-BLOCKERS.md. Prior source byte audit
is inherited. Production foundation and SunPad single-stick profile unchanged.
Normal boot, playback, gameplay, saves and mobile/device acceptance remain open.
This turn made native and source-audit progress; goal active.

**Previous recovery — 8 October 2026: channel getters and work-record creation prefix execute.**

Three native runs pass **108 controls**. Original65F364 selects an empty144-byte
record and reaches6624D8 in42cycles. The three original two-instruction getters
6624D8/DC/E0 return buffer/length/stride from the channel descriptor; retained
caller reaches66B0F4 after29cycles with arguments2/8CBC8040/2000/2020. Original
66B0F4 selects a172-byte record, calls the existing10EF3C clear and marks its active
halfword, reaching genuinely missing6488AE at32/900/905cycle boundaries.

The getter batch used two reviewed adjacent callees already resolved in65F364,
then ran them together; no further static expansion preceded execution. Tests
cover full/partly used pools, channel counts, zero/high-bit/wrapped field values,
aliases and unaligned access. Complete RAM/CPU/timer/SQ/device, independent source
reconstruction and ARM64/no-runtime-emission checks pass. No repairs or runtime/
compiler changes. No fabricated helper returns or omission of existing providers.

The earlier first-descriptor FFFFF800 length remains unchanged. This caller reads
the channel descriptor instead, whose length4000 and stride40 become2000/2020.
That does not resolve downstream use of the first descriptor, nor prove a guest
bug or a host allocation of the wrapped size.

Checkpoints (under `artifacts/recovery-aot/`):

- `20261008-startup-movie-resident-channel`:36controls,
  `d0d5edc6682ecdf68da16cd020bcae8a4769a4d2fd8f5037eeb61728c763cb73`.
- `20261008-startup-movie-resident-channel-getters`:36controls,
  `79dacf0e630b96e009c01f679ebe0e38fdc4cd20da7cc7f8f82b37a0ddf730a4`.
- `20261008-startup-movie-resident-work-pool`:36controls,
  `dda34b955060e7444b26dc9692d5e457c2f6cb2def6376b72805c9a615422ffc`.

**Next:** execute original60-instruction resident6488AE from work-pool
`phase2-expected-*`. Review matches BOOT/live instruction bytes. It selects an
empty52-byte record at894900 and uses already-registered10EF3C. Saved unexecuted
`next-startup-preflight.json` predicts29/297/314cycles: real52-byte clear followed
by next65EB00, arguments500/44100 and output pointers894930/894932. Base65EB00
registration is absent; verify actual runtime inventory. Recheck storage before
creating an output directory; retain guards and the real clear implementation.

There are20new bodies (12module/8resident),901instructions and907controls beyond
saved1058-function compiled video integration. Full startup evidence remains
1049functions/484state/392device; prepared504/412 integration is unexecuted. Full
board, compiled baseline and production foundation unchanged. Available at sealing
**471,666,688bytes**, close to focused419,430,400byte guard and below full
2,952,790,016byte reserve. No native game/test/compiler or booted Simulator remains.
No existing evidence deleted/compacted; unchanged new captures use verified sharing.

G1 remains unresolved: exact missing Katana/generated/header/provider inputs and
supported PAL dump remain in BOOTSTRAP-BLOCKERS.md with the unsent source-only
request. Source byte audit inherited. SunPad profile unchanged. Complete work/
channel/pool/handler/instance/create, normal boot/gameplay/media/saves/mobile/device
acceptance remain open. Previous/current turns progress; goal active. Execute saved
full video integration/audits/baselines once reserve passes before adding new bodies.

**Previous recovery — 8 October 2026: resident pool and three descriptor creations execute.**

Three native units pass **140 controls**. Original resident647E7C selects an empty
116-byte record and calls the existing10EF3C clear;43/631/667cycle boundaries
reach missing662106. Original662106 selects a48-byte descriptor from its64-record
pool and reaches662194 in40cycles. Original seven-instruction662194 initializes
that descriptor and returns. Retained662106/647E7C then create two more descriptors
and reach missing65F364 at244cycles beyond the initializer entry. No synthetic
return or omitted existing clear provider. All original resident instruction
bytes reviewed match BOOT and live RAM.

The captured first length calculation wraps: available80A0 minus two4040-byte
regions minus36 givesFFFFFFFC; original shifts produce **FFFFF800**. Native
execution and independent reconstruction both confirm it. The first descriptor
stores that value at+32 and+16; the following two store4000. These calls create
descriptors, not host allocations of the wrapped length. This observation is
preserved without clamping or labeling it a confirmed guest bug; downstream use
and full integration must resolve its significance.

Tests cover free/full pools, later free slots, pointer alignment, zero/two-channel
arithmetic, zero/normal/wrapped lengths, aliases, unaligned stacks and frame overlap.
72pool controls include48real-clear dispatches/24natural empty returns;36descriptor
controls include24ready/12full returns;32initializer controls check ordered fields.
All CPU/RAM/timer/SQ/device and ARM64/no-runtime-emission checks pass. Independent
reconstructions verify all captured phases and three descriptor returns. No repairs.

Checkpoints (under `artifacts/recovery-aot/`):

- `20261008-startup-movie-resident-pool`:
  `b26729c90f038dab54e66b2fda5f4d65bcde8abd2d753450d60ff66699790e1e`.
- `20261008-startup-movie-resident-descriptor`:
  `2fc0aea300bab5cebc5a2d9797a2b21454532f32d45f2b336fa454135da2916d`.
- `20261008-startup-movie-resident-descriptor-return`:
  `9a9ad4afb885eaedeeda615d667231c8f3280d34cdd0a060694fc717df65b387`.

**Next:** execute original110-instruction resident65F364 from descriptor-return
`phase2-expected-*`. It selects from eight144-byte records at8A7B90; all are empty.
Saved `next-startup-preflight.json` predicts42cycles/eight frame stores to6624D8,
with R4=8C8AA868, PR=65F3BC and SP=FFFE00. The base registration and linked symbol
are absent; verify actual runtime inventory and reached call before reviewing it.
This prediction is unexecuted. Preserve the wrapped first descriptor length.

15new bodies (12module/3resident),715instructions and799controls now extend beyond
the saved1058-function integration. They are not integrated full-startup evidence.
Compiled video baseline and actual full1049functions/484state/392device are unchanged;
prepared504/412 full run remains unexecuted. Available **694,738,944bytes**; focused
400MiB guard met, full2816MiB reserve unmet. One native process maximum enforced;
no native game/test/compiler or booted Simulator remains. Existing evidence was
not deleted/compacted. New captures use the existing verified sharing writer.

G1 remains unresolved; missing production Katana/generated/header/provider inputs,
supported PAL dump and exact unsent source-only request remain in BOOTSTRAP-BLOCKERS.md.
Prior byte audit is inherited. Production foundation, runtime/compiler, SunPad
profile and full board unchanged. Complete pool/handler/instance/create returns,
normal boot/gameplay/media/saves/mobile/device remain open. Previous/current turns
made progress; goal active. Run saved full video integration/audits/baselines once
reserve passes, using the exact sequence retained below before adding new bodies.

**Previous recovery — 8 October 2026: fourth handler setup returns; resident creation dependency reached.**

Four focused units pass **164 controls**. Retained dispatcher EDA5DA reaches
fourth record EE496C/handler ED1326 in21cycles with unchanged RAM. ED1326 uses
retained ED44FC: disabled cases return naturally; the captured enabled case
reaches ED13FC at31cycles. ED13FC performs the original resident28-byte copy,
initializes descriptor fields and returns. Its caller queries value60 and reaches
ED1438 at70cycles. ED1438 then reaches genuinely missing resident **8C647E7C**
in11cycles. Complete fourth-handler/instance/create return remains unproved.

Controls cover disabled/enabled branches, original setup error tails, source/
frame overlap, pointer and target aliases, unaligned access and retained dispatch.
All native runs passed full CPU/RAM/timer/SQ/device comparisons and ARM64/no-runtime-
emission checks. Independent reconstructions passed; no unit repair was needed.
One read-only nm command lacked the established CLT override and encountered the
unchanged Xcode license limitation; rerunning with the override succeeded. No
SDK/license/runtime/compiler change was made.

Checkpoints (under `artifacts/recovery-aot/`):

- `20261008-startup-movie-instance-fourth-dispatch`:25controls,
  `45215618cb38197dd0449df4f56bff3b97c3fb7c7f1a601476aba83676b7aee4`.
- `20261008-startup-movie-instance-fourth-handler`:57controls,
  `2ae7af557930e94de33031fdb4a45ce3c4cf8b4d1043ead7152a489a6187e38e`.
- `20261008-startup-movie-instance-fourth-setup`:57controls,
  `8bd63f3c063d0ee996c45276e15ce8884b1afb71a33716c1be502236eea77863`.
- `20261008-startup-movie-instance-fourth-create`:25controls,
  `1b473080476bd50b1a39f1479516b2298cb1b098a8f7b54371033413e668d48a`.

There are now12new module bodies/451instructions and659controls beyond the saved
1058-function integration (including25retained-dispatch controls). That compiled
integration and its baselines are unchanged; its full504/412 comparison remains
unexecuted. Latest actual full remains1049functions/484state/392device. Full board
source was freshly matched to the compiled report. Unit evidence is not boot.

**Next:** execute original206-instruction resident8C647E7C from fourth-create
`phase0-expected-*`. Its instruction bytes match the original BOOT and captured
RAM. The eight-record pool at6774F0 is empty; captured arguments areR4=2,
R5=8CBC8040,R6=80A0. Saved `next-startup-preflight.json` predicts43cycles/nine frame
stores to **already-registered resident10EF3C**, clearing116bytes at6774F0. Observe
that call and retain the real clear provider; do not treat it as missing or
fabricate a return. Bind new resident source to BOOT rather than MOVIE activation.
The following resident calls662106/65F364/65F214/648032 have not been executed.

Available at sealing: **637,820,928 bytes**; focused guard419,430,400 and full
capture reserve2,952,790,016. No native game/test/compiler or booted Simulator
is active. No old evidence deleted or compacted; new unchanged captures use
verified APFS sharing. Once reserve passes, execute the prepared video integration
and its exact audits/baselines/checkpoint sequence retained below before adding
these subsequent bodies.

G1 remains unresolved with exact inventory and unsent source-only request in
BOOTSTRAP-BLOCKERS.md. Prior source-byte audit inherited; missing production
Katana/generated/header/provider sources and supported PAL input not acquired.
SunPad profile and production foundation unchanged; normal boot/gameplay/media/
saves/mobile/device remain open. Previous and current turns made progress; goal active.

**Previous recovery — 8 October 2026: buffer binding and third startup handler return in native tests.**

Three new units pass **175 controls**. Original ED7E54 reaches ED86AE in nine
cycles, preserving its internal ED7ECC error entry. ED86AE validates the active
buffer, and original binding code installs callback ED3020 and owner EE2210 at
buffer8EEE8000+1C8/+1CC before returning. Original ED8604 then performs all three
flag calls; the original third handler returns to EDA5DA with R0=0 and SP=FFFE68.
The final flag/handler boundaries are37/129cycles. The instance and create
functions have not yet returned.

Tests cover null/invalid/valid validation, signed offsets, ordered global/status
overlap, wrapped flag indices, values, aliases, unaligned accesses and frame
argument overlap. All compare complete RAM/CPU/timers/SQ/device state. The flags
unit covers its valid-buffer route; its null/error routes were not executed.
The first binding harness failed compilation because two model store calls
omitted their RAM argument. Repair1 fixes those calls only; the original failed
compile and sources remain preserved. No game, runtime or compiler change.

Checkpoints (under `artifacts/recovery-aot/`):

- `20261008-startup-movie-instance-buffer-binding-repair1`:25 controls,
  `fd4093fd86a05fa4cfb760c31657e5f6b6e42079a602e91a60d75e57c96ff05a`.
- `20261008-startup-movie-instance-buffer-validate`:77 controls,
  `af58a2563780d3adca0763504037cc2eeb68860e14beda92ec7d95f7911eff76`.
- `20261008-startup-movie-instance-buffer-flags`:73 controls,
  `175d533e7c69cc52de6d24c224d00c751053585d3de2da2cddbbe6fcd31a5b98`.

Nine buffer bodies (302instructions) now pass **495 controls beyond the saved
1058-function compiled integration**. That integration still includes its own
nine bodies/485controls and baseline proof; the latest actual full startup is
unchanged at1049functions/484state/392device checks. The prepared504/412 full run
remains unexecuted. Full board source and production foundation are unchanged.
Do not promote these connected unit returns to full-startup or boot evidence.

Available at sealing: **944,312,320 bytes**. Full capture requires
**2,952,790,016 bytes**. No game/test/compiler or booted Simulator is active.
Existing evidence was not deleted or compacted. New captures use the existing
verified equality-only APFS writer; validation phases1/2 share one16MiB extent.

**Next:** execute retained dispatcher at EDA5DA from flags `phase1-expected-*`.
Saved `next-startup-preflight.json` predicts21cycles, unchanged RAM and the fourth
record EE496C reaching missingED1326. All compiled managed manifests were checked.
This continuation is unexecuted; review the next body only after observing it.
Once reserve passes, run the previously prepared nine-body video integration and
its exact audit/baseline/checkpoint sequence below, before integrating the nine
subsequent buffer bodies. Keep one native instance and one Simulator maximum.

G1 remains unresolved: missing Katana/generated/header/provider material and the
supported PAL dump remain inventoried in BOOTSTRAP-BLOCKERS.md with the unsent
source-only request. Prior source byte audit is inherited, not rerun this turn.
SunPad single-stick profile unchanged. Instance/create, boot/gameplay/media/saves,
mobile/device and production foundation gates remain open. Goal active; this
turn made measurable progress.

**Previous recovery — 8 October 2026: complete buffer configuration, links and acquisition return in native units.**

Four new units pass **190 controls**. EDA9FC reaches the original EDC458
four-word template copy, then finishes all six pointer/length entries and returns.
ED85A4 calls original EDA7F8 to initialize84bytes of fields, including raw float
bits. Original link setup and the buffer allocator both return successfully;
the third handler receives buffer8EEE8000 and reaches ED7E54 to bind its callback.
Final connected boundaries are50/65/75/84cycles. Tests cover copy overlap,
configuration branches, signed offsets, pointer wrap, both FR banks, signed zero,
NaN/subnormal bits, aliases, unaligned access and frame/global overlap. These are
unit results, not normal boot or rendering evidence.

Six original buffer bodies now pass **320 controls beyond the compiled build**.
They are not included in the saved1058-function integration, which still contains
nine new bodies with485controls and passed baseline scenes. Its board source is
unchanged. Full504state/412device predictions and their prepared audit remain
unexecuted below the capture reserve. The latest actual full startup remains
484state/392device and1049functions.

Checkpoints:

- Config prefix: `02c6f2651681065b359f697442047474c3a80e254415e0a5294b8fb68141bbfc`.
- Config return: `53feb6d2343e86bc38e8f695af1b2803fe454ca537b11c58329254f5ad8b7b7b`.
- Link prefix: `41c65177190c89f3bd6e77939fd7c750110076d1611b9d9b249bf76ad5b1eab1`.
- Whole links/acquire return: `09e97f9fbd7875ce391d4987a78e160081b4c5f5aedccd9022a182a318626f91`.
- Compiled/baseline integration: `83af9bc1d8ec0d42e8507e23d90ae0c9faf7a6cba5d7b08d1863f3223b1adb1a`.
- Last full: `2e6751cefa8e9e16fd2d59792411ce13fe6635b92826e3933426d00e1261aaa7`.

New units reuse the existing verified capture writer. Three identical new RAM
captures share48MiB through APFS clones; byte equality and distinct inodes were
checked. Existing evidence was not rewritten, deleted or compacted. Changed
captures still use normal verified writes. Runtime/compiler and full board did
not change.

Free space at sealing: **1,200,082,944 bytes**; full capture needs **2,952,790,016 bytes**.
No native game/test/compiler or booted Simulator remains active.

**Next:** execute original50-instruction ED7E54 buffer-binding prefix. Saved
unexecuted prediction:9cycles/four frame stores to missingED86AE. All cached
managed manifests checked for7E54,its internal7ECC error path,86AE and7EFA;
none are already registered. Preserve the internal error path rather than
creating a duplicate provider.

**Full-run sequence once the capture reserve passes:**

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools DCR_GPR_CACHE=0 DCR_EMIT_HOT_TRACE=0 python3 work/dreamcast-recovery/run-startup-movie-instance-video-complete-startup.py
python3 work/dreamcast-recovery/audit-loaded-movie-instance-video-complete.py
python3 work/dreamcast-recovery/audit-startup-movie-instance-video-complete-startup.py
DEVELOPER_DIR=/Library/Developer/CommandLineTools python3 work/dreamcast-recovery/baseline-loop-regression.py 20261008-startup-movie-instance-video-complete-startup
python3 work/dreamcast-recovery/checkpoint-movie-instance-video-complete-startup.py
```

Readiness audit `integration-readiness.json` in the compiled folder validates
20observer boundaries,1217managed entries and independent wholeRAM/timer
predictions; it does not replace the full run. First validate the saved nine-body
integration, then integrate the subsequent buffer bodies with their own evidence.

G1 source closure and supported PAL input remain unresolved. The exact inventory
and unsent source-only request remain in BOOTSTRAP-BLOCKERS.md; the prior source
byte audit is inherited. Production foundation and SunPad profile unchanged.
Third-handler/instance/create,normal boot/gameplay/media/saves/mobile/device
acceptance remain open. Goal active; previous and current turns made progress.

**Previous recovery — 8 October 2026: whole resource initializer and wrapper return; R3/C8 partial.**

Original ED7B82 writes three sentinel fields. The retained caller executes original
ED7B8C four times and original ED7BA0 once, filling the remaining resource fields.
Original ED7B3E restores its frame and returns the resource pointer; original
ED7ADC restores its frame and returns to second handlerED2464. That handler
reaches missing callback-binding routineED7A2C. Relative boundaries:
0/6/10/21/36/51/66/70/81/86/90/99. No fabricated helper returns.

**484 state /392 device comparisons pass; all472 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1049functions,
9886supplemental instructions,1170managed entries;6091continuations under the
unchanged8192limit. Module mutation remains39words/45changed bytes; compiled
instruction bytes unchanged.

Supporting units pass75controls:25sentinel,25four-block and25final-block/return
controls. These verify43ordered sentinel writes, aliases, unaligned input,
frame/destination overlap, signed final offset and actual ancestor-frame returns.
The first sentinel harness failed compilation because an old target-loop variable
remained after template editing. Repair1 fixes that control loop only; failed
compile evidence is retained. All subsequent native units/full startup passed;
no runtime/compiler or generated-guest-code change was needed.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-resource-complete-startup/`.
Units: `152a2dceb7d9fcdb1586b579a6e7b6cb728c9a74fd7b12c31417eb3b0993865a`,
`2f779b1ce40d34587a7bb457e276963e2b41600e4496798314231c93e5c03b21`,
`ea666fdaa609f8fbd8c692df88befe5b660c781df75217e9af48eaeace1c501e`.
Full checkpoint: `2e6751cefa8e9e16fd2d59792411ce13fe6635b92826e3933426d00e1261aaa7`
(192,629identities;6,728freshly hashed). At sealing2,998,091,776bytes were free;
all guards passed. No game/test/compiler or booted Simulator remained live.
No deletion or compaction. G1 missing paths freshly checked; source-byte audit
inherited by digest. Production source and supported PAL input remain absent.

**Next:** execute original36-instruction ED7A2C binding prefix. Saved unexecuted
prediction:11cycles/four frame stores to ED7AC4 on the captured nonzero-resource
path. All cached managed manifests checked:7AC4/7A74/79B2 absent. Verify null-resource
branch separately, then connect original validation/binding helpers. Resource
initializer/wrapper return are proven; complete second handler,instance/create,
normal boot/gameplay and G1 remain open. SunPad profile,production foundation and
runtime/compiler/twelve patches unchanged;source-only request remains unsent.

**Previous recovery — 8 October 2026: resource selection, clear and activation execute; R3/C8 partial.**

Original ED7ADC calls the ready-resource search ED7B0C. The live four-record table
selects EE6FB0. Original ED7B3E calls retained ED67F8 to clear47words, marks the
record active with value2, and calls retained internalED79A8 to clear three fields.
That helper returns. Full startup reaches missingED7B82 at153cycles beyond the
prior stop; boundaries0/4/23/29/37/137/143/149/153.

**473 state /381 device comparisons pass; all464 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1046functions,
9861supplemental instructions,1167managed entries;6091continuations under the
unchanged8192limit. Module mutation remains39words/45changed bytes; compiled
instruction bytes unchanged.

Supporting units pass131controls:25wrapper,73search and33initializer controls.
Search cases cover signed counts/strides, ready-entry positions, aliases and
32original empty-wrapper returns. Initializer tests cover unaligned data, live
fill-target aliases and frame/destination overlap. First full prediction failed:
the unit had omitted canonical internal79A8, already registered in full startup.
Repair1 adds its existing manifest to the unit, predicts its6cycle return and
captures the actual next helper. Failed full evidence is retained. Generated
initializer source/header/manifest are byte-identical; no runtime/compiler change.
The final checkpoint validator also caught a template typo in the module-size
assertion;184592bytes was restored, with failed validator snapshot preserved.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-resources-startup-repair1/`.
Units: `5a40b05028552a50a7e733f0e88c1ea16a970c4073827efe5f8867599294393e`,
`4b1176d9eb0def7bb868faa6faae554fbbca61ebe66595923d11710cbcd317f1`,
`4d38482be4c067b687feac4942557619d403a846a7c4771ffbb5c479b125b45d`.
Full checkpoint: `63e72a0234706297c264c7a072241fc38b4e46728c17f41ec506cb994498f3c0`
(188,804identities;6,621freshly hashed). At sealing3,976,478,720bytes were free;
all guards passed. No game/test/compiler or booted Simulator remained live.
No deletion or compaction. G1 missing paths freshly checked; source-byte audit
inherited by digest. Production source and supported PAL input remain absent.

**Next:** execute actual missingED7B82 and its retained caller. Use saved review
and preflight; verify every proposed helper against managed internal entries,
not just root-function inventory. Complete resource initialization, successful
wrapper/second-handler/instance/create return, normal boot/gameplay and G1 remain
open. SunPad profile, production foundation and runtime/compiler/twelve patches
unchanged; source-only request remains unsent.

**Previous recovery — 8 October 2026: first record handler returns and second handler setup reaches its next wrapper; R3/C8 partial.**

Original ED1C36 returns zero to the retained record dispatcher. The dispatcher
selects the second record's ED2444 handler. Its original frame setup calls ED24A2,
which clears the pointer atEE417C and returns. Full startup reaches missingED7ADC
at46cycles beyond the prior stop; boundaries0/3/24/38/42/46. All calls, delay slots,
returns and stores execute original code.

**465 state /373 device comparisons pass; all459 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1043functions,
9788supplemental instructions,1148managed entries;6091continuations under the
unchanged8192limit. Module mutation remains39words/45changed bytes; compiled
instruction bytes unchanged.

Supporting units pass91controls:33first-handler,25second-handler prefix and33
pointer-clear/continuation controls. Coverage includes register/flag/PR/SP
preservation, signed live offsets, unaligned data, entry/data/target aliases and
zero/nonzero prior pointer values. No failed experiment or test repair this turn.
No runtime/compiler changes or manufactured returns.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-handlers-startup/`.
Unit checkpoints: `1d1fa4c213807f581f07d3d97cf67f52996e6064a53bee188e9ef9a35f94a075`,
`8e100adccd2126e896dad47b6d54d84f5977c67a649d8dbc6134c24adda41871` and
`b7460de77856b11b2011490364c0de342a82084ac74d13a023603f3b84056567`.
Full checkpoint: `538b5fbc480a4d4293056cb6369c1888cfcad349239603a607917b48285cb83d`
(185,035identities;6,438freshly hashed). At sealing5,466,750,976bytes were free;
all storage guards passed. No game/test/compiler or booted Simulator remained
live. No deletion or compaction. G1 missing paths freshly checked; full source-byte
audit inherited by digest. Production source and supported PAL input remain absent.

**Next:** execute original14-instruction ED7ADC wrapper. Saved unexecuted prediction:
4cycles and one PR-frame store to missingED7B0C. Review that actual reached helper,
then connect original conditional-return paths. Complete second handler,
instance/create return, normal boot/gameplay and G1 remain open. Pinned SunPad
profile, production foundation and runtime/compiler/twelve patches unchanged;
source-only request remains unsent.

**Previous recovery — 8 October 2026: original tail jump and record dispatch reach the first handler; R3/C8 partial.**

Original ED3FEA executes its live target and delay-slot r5=3, preserving PR/SP/RAM.
Original EDA59E then pushes its seven-word frame and reads the first record's
handler table. Full startup reaches missing ED1C36 at37cycles beyond the prior
stop; boundaries0/4/37. The source-derived table and index select this target;
it is not inferred from the error's caller PC.

**460 state /368 device comparisons pass; all457 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1040functions,
9736supplemental instructions,1136managed entries;6091continuations under the
unchanged8192limit. Module mutation remains39words/45changed bytes, with compiled
instruction bytes unchanged.

Supporting units pass122controls:33tail-jump and89record-dispatch controls,
covering live target aliases, register/flag preservation, all nine record positions,
signed live offsets, unaligned data and eight original empty-list returns.
The first dispatcher test rejected a timing expectation: BRA costs1cycle in the
pinned emitter, not2. Repair1 changes only expected initial-block timing18,
first callback33 and empty-list return111. Failed evidence is preserved; generated
source/header/manifest are byte-identical across the repair. No compiler/runtime
or manufactured guest-return change.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-dispatch-startup/`.
Unit checkpoints: `54b901edf85d706c938e4346a94889c29b2b90afb7bf3dc7db9cef1c5f9750e5`
and `6fc25ade33ceb1314de345e4df21673551d92d10e7db4254c195013b45ac9f35`.
Full checkpoint: `9ce124b5f0cde73be3fa89b5a65949537cb1fcc4432c8fecb45cf837b1c1d228`
(181,407identities;6,342freshly hashed). At sealing6,069,211,136bytes were free;
all guards passed. No game/test/compiler or booted Simulator remained live.
No deletion or compaction. G1 missing paths freshly rechecked; full source-byte
audit inherited by digest. Production source and supported PAL input still absent.

**Next:** execute original ED1C36 (RTS with delay-slot r0=0) and retained EDA59E
continuation. Saved unexecuted prediction:3cycle return,24cycle second callback
atED2444; PR/SP/RAM preserved by the leaf. Review that next callback only after
native execution reaches it. Complete instance/create return, normal boot,
gameplay and G1 remain open. Pinned SunPad profile, production foundation and
runtime/compiler/twelve patches unchanged; source-only request remains unsent.

**Previous recovery — 8 October 2026: the whole table initializer returns to instance setup; R3/C8 partial.**

Original EDA530 selects the second table slot. Original EDA4CC then executes
retained helpers and returns through EDA42C, EDA388 and EDA304, restoring their
frames. The table initializer returns to ED3ED8 at165cycles beyond the prior
stop; instance setup reaches real missing ED3FEA at168cycles. Boundaries:
0/33/37/47/67/72/105/136/144/153/165/168. No manufactured helper returns.

**458 state /366 device comparisons pass; all446 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1038functions,
9686supplemental instructions,1128managed entries;6091continuations under the
unchanged8192limit. Module mutation remains39words/45changed bytes; compiled
instruction bytes unchanged.

Supporting units pass114controls:57third-selector and57third-branch controls,
covering signed literals, aliases, unaligned data, zero/nonzero branch paths,
selector frame/destination overlap and descriptor/destination overlap. Canonical
shared A54C/A57C providers remain unchanged; only1+8new managed entries are added.
The captured ancestor return chain verifies SP restoration through FFFE4C,
FFFE58,FFFE68 and FFFE84. No failed experiments or test repairs this continuation.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-table-return-startup/`.
Unit checkpoints: `4c60830940820e483c8728744322045baf4ebb331578973742a65b5eeb3fbd7a`
and `52ee817c81c9f4e2c799e22471588c35e389de457f3aad768ab1fbbb2d13eb0b`.
Full checkpoint: `2585e0a4c3e4e30dd18cb9aee8f0a5d52fa542ed4f63c504042c733ebd03497a`
(177,851identities;6,279freshly hashed). At sealing6,953,865,216bytes were free;
all storage guards passed. No game/test/compiler or booted Simulator remained
live. No deletion or compaction.

**Next:** execute original ED3FEA (3instructions,1block,1tail call). Saved
unexecuted prediction:4cycles to missingEDA59E, r5=3 from the delay slot,
PR/SP/RAM preserved. Verify the live target literal and original tail jump before
connecting the reached helper. Complete instance/create return, normal boot,
gameplay and G1 remain open. Pinned SunPad profile, production foundation and
runtime/compiler/twelve patches are unchanged. Source-only request remains unsent.

**Previous recovery — 8 October 2026: EDA486 returns through original helpers in full startup; R3/C8 partial.**

Original EDA486 executes retained A57C/EDA518 helpers and its own tail, restores
its frame and returns to EDA450 after99cycles beyond the prior stop. The retained
caller reaches real missing EDA530 at107cycles. Captured boundaries are
0/10/30/35/68/99/107; all six unit predictions match full startup.

**447 state /355 device comparisons pass; all440 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1036functions,
9604supplemental instructions,1119managed entries;6091continuations within the
unchanged8192limit. Module mutation remains39words/45changed bytes, with compiled
instruction bytes unchanged.

The connected unit passes57controls covering zero/nonzero descriptor paths,
aliases, unaligned data, signed live literals and actual descriptor/destination
overlap. Native whole returns are99cycles for the zero path and163for the nonzero
path. The new body adds8managed entries and retains canonical sharedA57C. No test
repairs or runtime/compiler changes in this continuation.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-next-branch-startup/`.
Unit checkpoint: `07e6af40d697f4c040a4d387730dae0e87a1127a41a271287423818fdbce2c8f`.
Full checkpoint: `6288c1055b9641f057a1878bce397746c6876385d78214ba5301d029bbbb7672`
(174,308identities;6,111freshly hashed). At sealing7,898,525,696bytes were free;
all storage guards passed. No native game/test/compiler or booted Simulator
remained live. No deletion or compaction.

**Next:** execute original EDA530 (30instructions,2blocks,no calls), adding only
its root and retaining canonical sharedA54C. Its entry sets r6=1 before the shared
tail, selecting the second table slot. Saved unexecuted prediction:33cycle return,
37cycle missingA4CC; original BSR@A460 sets PR=A464. Preserve original helpers and
caller. Complete instance/create return, normal boot/gameplay and G1 remain open.
Pinned SunPad profile, production foundation and runtime/compiler/twelve patches
are unchanged; exact source-only request remains unsent.

**Previous recovery — 8 October 2026: internal branch helper and second selector execute in full startup; R3/C8 partial.**

Original EDA42C calls its internal A57C entry and returns from that helper at
29 cycles. It then calls original EDA524; the selector returns at70cycles and
the original caller reaches missing EDA486 at74cycles beyond the prior stop.
Boundaries:0/9/29/37/70/74. All helper calls and returns execute original code.

**441 state / 349 device comparisons pass; all435 prior captures are unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1035functions,
9552supplemental instructions,1111managed entries;6091continuations within the
unchanged8192limit. Module mutation remains39words/45changed bytes; compiled
instruction bytes unchanged.

Supporting units pass130controls:73branch/internal-helper controls and57second-
selector controls. These cover descriptor branch priority, eight original all-zero
branch returns, signed literals, aliases, unaligned data and selector frame/
destination overlap. EDA524 shares the exact A54C tail with EDA518; only its root
is newly registered, preserving the canonical existing A54C provider. The legacy
generator log reports two CFG blocks as activation entries; the emitted manifest
and runtime checks prove one new registration. See `shared-entry-audit.json`.

One test expectation was repaired: BSR atEDA44C sets PR=EDA450, notEDA452.
Native comparison caught the wrong expected PR. The initial failed unit is
retained with `expectation-correction.json`; only the expectation changed, with
no runtime/compiler/generated-code change. Full startup confirms corrected PR.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-second-selector-startup/`.
Unit checkpoints: `b0488094c714726506f82e30191fc92358d90b833ad6c848a420896162859cb7`
and `3d5af7fb5da98b94c24695bb5ccd050d95858357463532fed4f2f798a851f2bd`.
Full checkpoint: `90578b7a154807614a7decbd95bb17d927fef5a2bafa34dedbc6ace2d6502951`
(170,905identities;6,080freshly hashed). At sealing8,585,629,696bytes were free;
all storage guards passed. No native game/test/compiler or booted Simulator
remained live. No deletion or compaction.

**Next:** execute original EDA486 (52instructions,9blocks,4calls), adding8managed
entries while retaining canonical sharedA57C. Its existing A57C/EDA518 helpers
and original tail give an unexecuted prediction:10/30/35/68cycle boundaries,
whole return99cycles and next missingA530 at107cycles. Test the captured zero
and alternate nonzero descriptor paths. No manufactured returns. Complete
instance/create return, normal boot/gameplay and G1 remain open. Pinned SunPad
profile, production foundation and runtime/compiler/twelve patches are unchanged.

**Previous recovery — 8 October 2026: descriptor branch and selector advance native startup; R3/C8 partial.**

Original EDA388 follows the captured nonzero descriptor to EDA518. The original
selector returns after 33 cycles; its retained caller reaches missing EDA42C.
From the previous full-startup stop, boundaries are 0 / 14 / 47 / 51 cycles.
The descriptor-prefix unit passes 137 controls, including all 16 descriptor
presence combinations, branch priority, aliases, unaligned input, live literals
and eight original all-zero returns. The selector unit passes 57 controls for
index/mode patterns, signed live offsets, aliases, unaligned data and actual
frame/destination overlap. All calls and returns use original generated code.

The full run's 436 predicted state and 344 device comparisons pass, with all
432 prior captures unchanged. Both exact-binary baseline scenes pass ten external
audits, corruption controls and six unchanged images. A historical extra
record-pattern assertion still expected every tail field to equal9; original
EDA518 now deliberately writes index0 to8CEE2758 and mode0 to8CEE4168. The repaired
audit derives both addresses from original signed literals and captured arguments.
The failed audit is retained; native execution and predictions are unchanged.
Full checkpoint: `76bded1c4fa4c230b9316a217317bbaf11aa9f6720386e0598b0dc60dd860a31`
(167,500 identities; 5,995 freshly hashed). At sealing, 9,530,527,744 bytes
were free and all storage guards passed. No game/test/compiler or booted
Simulator remained live. No deletion or compaction.

Closure is 1033 functions, 9457 supplemental instructions, 1097 managed entries
and 6091 continuations within the unchanged8192limit. Source evidence lives in
`artifacts/recovery-aot/20261008-startup-movie-instance-selector-startup/`.
Unit checkpoints: `3b91d35ce20a06ecd3fe4f2596f7b2c50e66c44c4fb7d9af25a6a275eb4d0649`
and `8d1d569b25ea383ccfb73804f67a13da12d6640a49fc2e1b2d77636c476fc47a`.
Complete instance/create return, normal boot, gameplay and G1 remain open.
Production foundation, SunPad profile and runtime/compiler/twelve patches are unchanged.

**Next:** generate EDA42C (65 instructions, 13 managed blocks, six call sites).
Its first callee A57C is an internal block of this same body, not a separate
missing source. Saved unexecuted predictions: internal entry9cycles, actual
internal return29cycles, then missing EDA524 at37cycles. Register every original
entry and preserve the real caller/helper path. No fake returns.

**Previous recovery — 8 October 2026: all nine table records initialize through the original caller; R3/C8 partial.**

Original EDA362 executes nine times through retained EDA304. First return at
34 cycles and second entry at 41 match the saved prediction. The ninth return
occurs at 362 cycles, followed by real missing EDA388 at 369 cycles. All nine
calls and returns execute original generated code.

**433 state / 341 device comparisons pass; all 414 prior named captures are
unchanged.** The exact ARM64 executable passes both baseline scenes, ten external
audits, release corruption controls and six unchanged images. Closure is 1031
functions, 9355 supplemental instructions and 1073 managed entries. Actual
continuations remain 6091 under the unchanged 8192 limit. Module mutation remains
39 audited words / 45 changed bytes; compiled instruction bytes are unchanged.

The focused unit passes 41 controls, covering value patterns, aliases, unaligned
data, signed live literals and source/destination overlap. The first fixture
labelled overlap did not overlap those regions: it placed the source at
`destination+0x3e8` while writing negative destination offsets. The stronger
fixture places the source at `destination-16`, where the first loop writes.
It passes with all 18 captured RAM/CPU/TMU/SQ phases byte-identical to the initial
run. Both runs are retained; count the 41 strengthened controls once. This was a
fixture improvement, with no generated-code or runtime fix.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-table-records-startup/`.
Strengthened unit checkpoint:
`876ac8f651d4b51f4078b615efeaf0b5272145c55bc198d88725d5066bc3f418`.
Full checkpoint: `25206d5753ab7c112a256c82d212b69b2aac47ae6aeaa3fbcfb8d34bea479f9a`
(164,153 identities; 6,062 freshly hashed). At sealing, 10,168,233,984 bytes
were free and all guards passed. No game/test/compiler or booted Simulator
remained live. No deletion or compaction was performed.

**Next:** execute original EDA388 (72 instructions, 22 blocks, ten call sites).
The observed nonzero descriptor selects a saved, unexecuted 14-cycle prefix
with four frame stores to missing EDA518. Test branch priority and the all-zero
return without supplying fake helper returns. Complete instance/create return,
normal boot, gameplay and G1 remain open. The exact source inventory and unsent
request in [BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md) are unchanged. Pinned
SunPad controls, production foundation and runtime/compiler/twelve patches are unchanged.

**Previous recovery — 8 October 2026: stream return and the next initializer prefix execute in full startup; R3/C8 partial.**

The previous milestone now proves the complete original ED97B0 stream return.
Original EDA304 then executes its 20-cycle prefix, saves seven frame words and
reaches the real missing helper EDA362. The focused prefix unit passes 41 controls
and matches the prior RAM, CPU, timer and store-queue predictions exactly.

The new full run passes **415 state / 323 device comparisons**, preserving all
413 prior named captures. Its exact ARM64 executable passes both baseline scenes,
ten external audits, release corruption controls and six unchanged images.
Closure: 1030 functions, 9336 supplemental instructions, 1070 managed entries;
6091 continuations within the unchanged 8192 limit. Module mutation remains
39 audited words / 45 changed bytes, with compiled instructions unchanged.
Runtime/compiler/twelve patches and the pinned SunPad single-stick profile are unchanged.

Evidence: `artifacts/recovery-aot/20261008-startup-movie-instance-table-startup/`.
Prefix unit checkpoint: `722900b8d4c134a29ea2347dcd32cf9fd71d36077fcae16b1c3b59abc9307b43`.
Full checkpoint: `4af112425585a8698e0eb6a127f6d67b88eb273e2aa7e1093dc1117311c9e259`
(160,676 identities; 5,727 freshly hashed). At sealing, 11,684,593,664 bytes
were free and all storage guards passed. No native game/test/compiler or booted
Simulator remained live. No deletion or compaction was performed.

**Next:** execute reviewed EDA362 (19 instructions, three blocks, no calls).
The saved prediction is a 34-cycle return after seven ordered stores, followed
by the retained caller's second entry at 41 cycles. These predictions are
unexecuted. Connect the original parent's nine calls and next actual helper;
no manufactured returns. Complete instance/create return, normal boot, gameplay
and original G1 source completeness remain unproved. The exact source inventory
and unsent acquisition request remain in [BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md).

**Previous recovery — 8 October 2026: the original stream initializer returns in full startup; R3/C8 partial.**

Original ED9978 completes the remaining type2 records. Retained ED9918 and ED9864
execute the remaining type1/indexed records, then ED97B0 restores its frame and
returns to ED3ECA after 358 cycles beyond the prior stop. The parent reaches
EDA304 eight cycles later. All calls and returns use original generated code.

The full run passes **414 state and 322 device comparisons**, preserving all
404 prior named captures. The exact ARM64 executable passes both baseline scenes,
ten external audits, release corruption controls and six unchanged images.
Closure: 1029 functions, 9289 supplemental instructions, 1063 managed entries;
6091 continuations within the unchanged 8192 limit. Module mutation remains
39 audited words / 45 changed bytes, with compiled instruction bytes unchanged.
The connected unit passes 57 controls including nonzero destination data,
live signed literals, aliases, unaligned data and source/destination overlaps.

Full checkpoint: `5217089bd97065292690b78e5848e63d9a3778eccddb9f379b13788bbcd11f27`
(157,522 identities; 5,722 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-stream-return-startup/`.
Unit checkpoint: `3680d1b53a69245a2d5ec9d505db15f42195242b98db7448bf00393f81e1e76f`.
At sealing, 12,180,344,832 bytes were free, all capacity guards passed and no game,
compiler or Simulator remained live. No files were deleted or compacted.

The next EDA304 prefix separately passes 41 controls and matches the saved
20-cycle / seven-frame-store prediction to missing EDA362. Its full-startup
integration is verified in the latest checkpoint above; complete instance/create return and boot remain
unproved. G1 source completeness remains unresolved, with the exact missing-source
inventory and unsent acquisition request in [BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md).
Production foundation, pinned SunPad controls and runtime/compiler patches are unchanged.

**Previous recovery — 8 October 2026: three indexed records and a small record execute in full startup; R3/C8 partial.**

Original ED9864 initializes three indexed stream records through the retained
caller. Original ED9918 then initializes the zero-length type1 record and clears
16words at48-byte intervals. The original caller reaches missing **ED9978 at
418cycles** beyond the previous stop. Relative boundaries:0/78/87/165/174/252/
258/412/418. All helpers and returns are original generated code.

**405 state /313 device comparisons pass; all396 earlier captures remain unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1028functions,
9243supplemental instructions,1059managed entries;6091continuations with unchanged
8192limit. Module data remains39audited words/45changed bytes; compiled instruction
bytes unchanged. Runtime/compiler/twelve patches unchanged.

Supporting units pass162controls:105indexed-record/caller and57small-record/caller.
They cover zero/nonzero lengths and flags, signed live literals, wraparound,
source/destination overlaps, aliases, unaligned data and original managed activation.
The small-record fixture was strengthened with nonzero destination words and
explicit16-word strided-clear checks; captured results remain byte-identical to
the initial passing run. First indexed-record RAM/CPU and second-call RAM/CPU
match the saved prospective expectations.

Two expectation corrections are preserved. The old77/86cycle prediction counted
MUL.L as one cycle; the pinned translator charges two. Native timer comparison
caught it. Corrected first-return/second-entry times78/87 pass; no compiler/runtime
change. An additional record-pattern audit incorrectly converted0x28000 to decimal.
It is163840, so the second remaining size is397312-163840=233472 (0x39000).
The repaired audit reads the validated original padding literals. All405full-state
comparisons had already passed before that failed extra assertion; native execution
was unchanged. See unit `timing-prediction-correction.json` and full-run
`record-pattern-audit-failure1.json`; failed files/runs remain retained.

Full checkpoint: `2d0dd2132b84a9e050b99a77814ea207383ebbf4a4325c578c69d58d4f4abccf`
(154,337 identities;5,658 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-records-startup/`.
Unit checkpoints: `e97af7b23bc8f94917a1572b2a28438a6914eae01ef70c3831620eb368fb37e6`
and `659cbfde375e0659d2a8b3f34846bff5eb1ebdaa66db24947a0b72974b94b66b`.
13,033,603,072bytes available at sealing; all capacity guards pass. No native
game/test/compiler or booted Simulator remains live. No deletion or compaction.

**Next:** execute reviewed ED9978 (46instructions,4blocks,no calls), the type2
record initializer. Saved unexecuted prediction:47cycle zero-length return to
ED981C,19ordered stores, then53cycles to already registered ED9918 at index5.
MUL.L's two-cycle cost is included. Exact RAM/CPU/timer/SQ predictions are saved
in `next-startup-preflight.json`. Test nonzero lengths, signed live strides and
ordered overlaps. Connect remaining original calls using verified ED9918/ED9864
through the stream epilogue before reviewing a new dependency. One classification
batch used. Stream/instance/create return and normal boot remain unproved.

G1 remains unresolved: runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39.
The8October source inventory is inherited by verified digest; missing paths and
supported PAL GDI absence were rechecked. Last FFmpeg byte audit10139/10139;
Apple media build unproved. Exact source-only request remains unsent in
`BOOTSTRAP-BLOCKERS.md`. Production foundation, playback/gameplay/audio/save/mobile/
device acceptance remain open. Repository safety/whitespace pass. Previous/current
turns made execution progress; goal active. Older entries are historical.

**Previous recovery — 8 October 2026: stream offsets and original remainder execute in full startup; R3/C8 partial.**

Original ED97B0 saves its frame and reaches ED984E in16cycles. ED984E builds
eight offsets from the real length array in54cycles. The retained caller invokes
the already compiled resident10E7D8 signed-remainder helper:65536 %2048 returns0
in90cycles. Its original caller then reaches missing **ED9864 at175cycles**
relative to the previous full stop. No helper return is synthesized.

**397 state /305 device comparisons pass; all391 earlier captures remain unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1026functions,
9114supplemental instructions,1046managed entries;6091continuations with unchanged
8192limit. Module data remains39audited words/45changed bytes; compiled instruction
bytes unchanged. Runtime/compiler/twelve patches unchanged.

Supporting units pass110controls:41stream-prefix and69offset/remainder/caller.
The prefix matches the saved16cycle RAM/CPU/timer/SQ prediction exactly. Controls
cover ordered frame/source/destination overlap, aliases, unaligned buffers,
integer wrap, signed remainder and zero-divisor behavior. The remainder model
uses widened arithmetic for instruction carries and separately checks the signed
mathematical result. A failed expectation preserved under the descriptor's first
run assumed the guest code alias remained in PC; the resident dispatcher normalizes
that address. Only the expected entry PC changed in repair1; runtime and original
code were unchanged. Captured leaf/remainder-entry/remainder-return/next-call
cycles are54/59/149/159 relative to the ED984E entry.

Full checkpoint: `53f29489cae421e0c7f610c7d55beb371f2349d6226ab76bebd259a014675f6a`
(151,174 identities;5,552 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-stream-startup/`.
Unit checkpoints: `430b4a0f48288ff3d164c54b7742854102ea4a5bdcf0489975a3447f81a89611`
and `7d3e6887ce6f96595ff0267dd4baf96a87c20ac6eaf99be06899dfb1a7e33d18`.
14,012,477,440bytes available at sealing; all capacity guards pass. No native
game/test/compiler or booted Simulator remains live. No deletion or compaction.

**Next:** execute reviewed ED9864 (81instructions,7blocks,no calls), which fills an
indexed stream record. Saved unexecuted prediction:77cycle first return toED97E8,
28ordered word stores, then86cycles to its second call. Separate exact RAM/CPU/
timer/SQ expectations cover both boundaries. Test zero/nonzero lengths and flags,
live signed literals, wraparound and ordered overlaps, then connect the retained
three-call path before reviewing the next new callee. One classification batch
since full execution. Complete stream/instance/create return and boot remain open.

G1 remains unresolved: runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39.
The8October source inventory is inherited by verified digest; missing paths and
supported PAL GDI absence were rechecked. Last FFmpeg byte audit10139/10139;
Apple media build unproved. Exact source-only request remains unsent in
`BOOTSTRAP-BLOCKERS.md`. Production foundation, playback/gameplay/audio/save/mobile/
device acceptance remain open. Repository safety/whitespace pass. Previous/current
turns made execution progress; goal active. Older entries are historical.

**Previous recovery — 8 October 2026: five handlers and complete rate initializer return in full startup; R3/C8 partial.**

Original ED4A54 installs all five callback slots and refreshes the selected
callback. Original ED46DE clears seven words; the retained ED4668 epilogue writes
integer defaults and exact -1.0 floating-point fields, restores its saved frame
and returns to ED3EBE. The original parent reaches missing **ED97B0**.
Relative to the previous full stop: first handler return20cycles, field entry126,
field return136, complete rate return196, next missing stream initializer203.

**392 state /300 device comparisons pass; all386 earlier captures remain unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1024functions,
9024supplemental instructions,1032managed entries;6091continuations with unchanged
8192limit. Module data remains39audited words/45changed bytes; compiled instruction
bytes unchanged. No runtime/compiler/twelve-patch change or manufactured return.

Supporting units pass138controls:69handler/caller and69field/rate-return.
They verify complete RAM/CPU/timers/SQ/devices, original managed activation,
selected callback indexes, live signed offsets/literals, aliases, unaligned data,
ordered overlaps, saved-frame overwrites and both floating-point register banks.
The saved20cycle first-handler-return RAM/CPU/timer prediction matches exactly.

Failed harness runs are retained. Handler fixes: captured timers reside in the
state file, a hex literal needed separation before addition, and a copied old
terminal assertion was stale. Rate-field fixes: another hex literal typo and
expected FPU storage must select FR or XF using FPSCR.FR. Generated original
bodies, runtime and compiler were unchanged. Passing runs use handler repair3
and rate-fields repair2; none of the failed runs is promoted to evidence.

Full checkpoint: `6b9a4b16f49223d86fc622fc58fff4359fe7a3a642fb08421f93593d4bbcd3b3`
(148,082 identities;5,477 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-rate-return-startup/`.
Unit checkpoints: `b831d51657fda8ba923a2ef5b7a7518636afceaa5f6262ae8da301350961c7e7`
and `1212c88d7f862ecf0ce4e0d7690d9a84ebcd60e4eedd4e0d71b04b322e4d168e`.
14,795,624,448bytes available at sealing; all capacity guards pass. No native
game/test/compiler or booted Simulator remains live. No deletion or compaction.

**Next:** execute reviewed ED97B0 (79instructions,11blocks,10calls). Its first
16cycles push five words, reserve32stack bytes and reach missing ED984E with
R4/R12=FFFE50 and R5=FFFED0. Exact RAM/CPU/timer/SQ and ordered-store predictions
are saved, unexecuted, in `next-startup-preflight.json`. Test descriptor/frame
overlaps, aliases, unaligned loads and state preservation before deeper review.
One classification batch used. Complete stream/instance/create return and normal
boot remain unproved. The already verified rate initializer is not playback.

G1 remains unresolved: runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39.
The8October source inventory is inherited by verified digest; missing paths and
supported PAL GDI absence were rechecked. Last FFmpeg byte audit10139/10139;
Apple media build unproved. Exact source-only request remains unsent in
`BOOTSTRAP-BLOCKERS.md`. Production foundation, gameplay/audio/save/mobile/device
acceptance remain open. Repository safety/whitespace pass. Goal active;
previous/current turns made execution progress. Older entries are historical.

**Previous recovery — 8 October 2026: second field initializer and rate entry execute in full startup; R3/C8 partial.**

Original ED3F22 performs its retained16-word clear and four field stores, returning
in55cycles. The retained internal ED1A94 clears five words, and ED6708 copies
47words with two PREF calls. Original ED462E saves its arguments and reaches
missing **ED4A54 at258cycles** relative to the previous full stop.

**387 state /295 device comparisons pass; all377 earlier captures remain unchanged.**
The exact ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1022functions,
8996supplemental instructions,1029managed entries;6091continuations with unchanged
8192limit. Module data remains39audited words/45changed bytes; compiled instruction
bytes are unchanged. Runtime/compiler/twelve patches are unchanged.

Supporting units pass82controls:41second-field/internal-clear/copy and41rate-prefix.
They cover complete RAM/CPU/timers/SQ/devices, managed activation, aliases,
unaligned data, live literals and ordered overlaps including saved-register writes.
The saved55cycle field-return and60cycle internal-clear-entry predictions match.

Full checkpoint: `e78b811476f083d97d4090e49402f01340acc32079d394e9d77bd26a0f83aaf1`
(145,032 identities;5,405 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-rate-startup/`.
Unit checkpoints: `8c5af73179bf721f54994d9136ed65fad33d388fc3d35f697ef179de5bd0fc24`
and `ccedecc79dad736cf9a418948bf40c0e081bc502ab939fca3fce705a9f00ecce`.
15,615,180,800bytes available at sealing; capacity guards passed and no native
process or Simulator was live. No deletion or storage compaction performed.

**Next:** execute reviewed19-instruction ED4A54 handler installer and retained
five-call path. Saved prediction:20cycle first return,25cycle next handler entry;
two ordered word stores, exact RAM/CPU/timers/SQ. These predictions are unexecuted
at this checkpoint. Normal boot and complete rate/instance/create return remain open.

G1 remains unresolved: runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39.
The8October source inventory is inherited by verified digest; missing paths and
supported PAL GDI absence were rechecked. Last FFmpeg byte audit10139/10139;
Apple media build unproved. Exact source-only request remains unsent in
`BOOTSTRAP-BLOCKERS.md`. Production foundation, gameplay/audio/save/mobile/device
acceptance remain open. Goal active; execution progress. Older entries are historical.

**Previous recovery — 8 October 2026: slot clear, descriptor copy and first field initializer execute in full startup; R3/C8 partial.**

Original ED3E54 executes its retained4049-word clear (16,196bytes),60-byte
resident descriptor copy and initial slot flag stores. Original ED3EF8 then
executes a16-word clear, writes its initial field flag and returns. Relative to
the previous full stop: first clear returns4589cycles, copy4639, field helper4713;
the retained caller reaches genuinely missing **ED3F22 at4717cycles**.

**378 state /286 device comparisons pass; all368 earlier captures remain unchanged.**
The same ARM64 executable passes both baseline scenes, ten external audits,
release corruption controls and six unchanged images. Closure:1020functions,
8892supplemental instructions,1020managed entries;6091continuations with unchanged
8192limit. Module data remains39audited words/45changed bytes; all compiled
instruction bytes unchanged. No runtime/compiler/twelve-patch change or fake return.

Supporting units pass126controls:93initializer/clear/copy and33field/caller.
They verify exact RAM/CPU/timer/SQ/device state, original managed activation,
clear remainder counts, nonzero buffers, aliases, unaligned data, live literals,
descriptor/frame overlaps and overwritten saved-register behavior. The first-call
13cycle/16stack-store-byte boundary matches the prior prospective prediction.
The field helper returns in60cycles and its caller reaches ED3F22 in64.

Full checkpoint: `6c1b4d347e0b50cd0b31ae025caa06b762bd7ada3f1c6b4be7bf05e03df23061`
(142,009 identities;5,295 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-initialize-startup/` and
`20261008-startup-movie-instance-initialize`, `20261008-startup-movie-instance-fields`.
Unit checkpoints: `ae9a57612de73c4fad900b57296b259bbda448d017b0148a5a5e74be4449f8a5`
and `70c7e9ef462b3bf0fffd4e9f8f326c3898ec0c670217466ed4b08ac009d39a57`.
18,399,150,080bytes available at sealing; all capacity guards passed. No deletion
or compaction. No game/test/compiler or booted Simulator remains live.

**Next:** execute reviewed ED3F22 (16instructions,two blocks,one call) with its
registered ED67F8 fill. Predict55cycles for the original16-word clear/four-field
zeroing/returnED3EA0, then60cycles to **already registered internal blockED1A94**.
That block belongs to the retained record-clear body; its absence from a function-
start inventory does not make it missing. Exact RAM/CPU/timer/SQ predictions and
manifest identity are saved, unexecuted, in `next-startup-preflight.json`.
Test live offsets, ordered stack/destination overlaps, aliases and status/FPU;
connect the original retained block. One classification batch used; execute
before deeper review. Complete instance/create return and normal boot remain unproved.

G1 remains unresolved: runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39;
last FFmpeg byte audit10139/10139, Apple media build unproved. The8October inventory
is inherited by verified digest; its missing paths and supported PAL GDI absence
were rechecked. Exact source-only request remains unsent in `BOOTSTRAP-BLOCKERS.md`.
Production foundation, playback/gameplay/audio/save/mobile/device acceptance remain
open. Repository safety/whitespace pass. Previous/current turns made execution
progress; goal active. Older entries below are historical.

**Previous recovery — 8 October 2026: original movie-instance guard and free-slot search execute in full startup; R3/C8 partial.**

Original ED3DC2 saves its arguments, ED3DFE accepts the non-null descriptor
pointer, and ED3E0E finds the first free slot **EE2210** in the captured one-slot
pool. Relative to the previous full stop: guard entry7cycles/return14, pool
entry19/return36, next genuinely missing initializer **ED3E54 at43cycles**.
Original helpers and caller execute; no synthesized success or guest return.

**369 state /277 device comparisons pass; all363 earlier named captures remain
unchanged.** The same ARM64 executable passes both baseline scenes, ten external
audits, release corruption controls and six unchanged images. Closure:
1018functions,8789supplemental instructions,1005managed entries;6091continuations,
unchanged8192limit. Module data remains39audited words/45changed bytes; compiled
instruction bytes are unchanged. Runtime/compiler/twelve patches unchanged.

Supporting units pass171controls:41instance-prefix,41guard/caller,89pool/caller.
The prefix matches the saved7cycle/12stack-store-byte RAM/CPU/timer/SQ prediction.
Null pointers take the original error call to missingED1AA2 (13cycles).
Pool controls cover signed nonpositive counts, first/later free slots, exhaustion,
aliases, unaligned data and live signed strides. Exhaustion returns zero through
the original caller; captured first-free search takes17cycles, then7more toED3E54.
Captured instance initialization and complete movie-create return remain unproved.

Full checkpoint: `d80c0145271bf9b09d06d3ba909b79ca12ccca85957f5d6f7819cc9c8858f7aa`
(139,052 identities;5,201 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-instance-startup/` and units
`20261008-startup-movie-create-instance`, `create-instance-guard`, `instance-pool`.
Unit checkpoints respectively:
`05eeef1501170944e3a748801d0ee6bdf1c6fa3865c4bde53720be35b7a54944`,
`c7fce3fb5ba2102e36d1a69c2fc6c2565ae61a0dcf595d39a32e18b3bb4a1caf`,
`aa6e2774ad29874ced55dec4f89f97d618b0f1bd6e367d1803437f78ad870ff6`.
All capacity guards passed;19,275,649,024bytes available at sealing. No deletion
or compaction. No game/test/compiler or booted Simulator remains live.

**Next:** execute reviewed ED3E54 (82instructions,13blocks,10calls). Its first
helper **ED67F8 is already registered**, as is the resident60-byte copy10E964.
Predict13cycles/16ordered frame-store bytes toED67F8, SP=FFFE84/PR=ED3E6C,
R4=EE2210/R6=4049words (16,196bytes). Connect the original clear and copy;
do not replace them or misclassify them as missing. Exact first-call RAM/CPU/
timer/SQ predictions are unexecuted in `next-startup-preflight.json`. Include
live literal, descriptor/frame overlap and alias controls. One classification
batch used; execute before deeper callee review.

G1 remains unresolved. Its8October inventory is inherited by verified digest;
missing paths and supported PAL GDI absence rechecked. Runtime0/1,generated0/1474,
headers0/177,contracts0/4,title38/39; last FFmpeg byte verification10139/10139,
Apple media build unproved. Exact source-only request remains unsent in
`BOOTSTRAP-BLOCKERS.md`. Production foundation, boot/playback/gameplay/audio/
save/mobile/device acceptance remain open. Repository safety/whitespace pass.
Previous/current turns made execution progress; goal active. Older entries below
are historical.

**Previous recovery — 8 October 2026: both movie-create descriptor initializers return in full startup; R3/C8 partial.**

Original ED2538 and ED0EC4 execute their nine/seven ordered word stores and
32-byte pointer alignment, then return through the retained caller. Relative
to the previous ED2538 stop, returns occur at26/53cycles; the next genuinely
missing target **ED3DC2** is reached at59cycles. No fabricated returns or
runtime/compiler/twelve-patch changes were needed.

**364 state /272 device comparisons pass; all359 earlier named captures remain
unchanged.** The exact ARM64 executable passes both baseline scenes, ten external
audits, release corruption controls and six unchanged images. Closure:
1015functions,8728supplemental instructions,988managed entries;6091continuations
with unchanged8192limit. The new stores are outside MOVIE; its39audited data words,
45changed bytes and all compiled instruction bytes remain unchanged from the prior run.

Both units pass81controls each (72connected +9activation),162total. They cover
integer wrap, aliases, unaligned data, source/destination overlap and status/FPU.
The first26cycle leaf return matches the prior saved RAM/CPU/timer/SQ prediction;
its retained caller reaches ED0EC4 at31cycles. The second leaf returns in22cycles
and its retained caller reaches ED3DC2 at28. Complete movie-create return, normal
boot and playback remain unproved.

Full checkpoint: `cace994893079abebba4e6c3f9d3d75eefc0f6821f12c81ae8a898f63ded153b`
(136,147 identities;5,085 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-create-init-startup/` and units
`20261008-startup-movie-create-init`, `20261008-startup-movie-create-second-init`.
Unit checkpoints: `302bf1389be5b8d27c5e3ef4164ba4ddcb58ff8922417a15f6a3f796b284e8df`
and `d2478852cc437be31db9b9fcd8372a82917826fc30e19cd58c304f3e1fc3df1a`.
All capacity guards passed;20,631,683,072bytes available at sealing. No content
deleted or compacted. No game/test/compiler or booted Simulator remains live.

**Next:** execute reviewed ED3DC2 (30instructions,9blocks,4calls). Its first
callee ED3DFE is genuinely uncompiled. Predict7cycles,12ordered stack-store bytes,
SP=FFFE94/PR=ED3DCE at that BSR boundary. Exact RAM/CPU/timer/SQ predictions are
unexecuted in `next-startup-preflight.json`. Test aliases, unaligned/overlapping
frames and status/FPU; execute before deeper callee review. One classification
batch used. No instance or complete-create return is claimed.

G1 inventory inherited by verified digest; missing paths and supported PAL GDI
absence rechecked. Runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39
remain unresolved. Last FFmpeg byte audit10139/10139; Apple build unproved.
The exact source-only request remains unsent in `BOOTSTRAP-BLOCKERS.md`.
Production foundation, boot/gameplay/playback/audio/save/mobile/device acceptance
remain open. Repository safety/whitespace pass. Previous/current goal turns made
execution progress; goal active. Earlier entries below are historical.

**Previous recovery — 8 October 2026: movie-create prefix, buffer layout and descriptor copy execute in full startup; R3/C8 partial.**

Original ED03F8 now executes with its retained stream-size, frame-size,
dimension and resident copy helpers. All return through original code.
The buffer check requires 2,949,376 bytes and has 2,949,408 available (32 bytes
headroom). The selected 60-byte descriptor is copied and initializer arguments
are written before the next genuinely missing target **ED2538 at435cycles**
relative to the previous full stop. Complete create return and boot remain unproved.

**360 state /268 device comparisons pass; all348 earlier named captures remain
unchanged.** The same ARM64 executable passes both baseline scenes, ten external
audits, release corruption controls and six unchanged images. Closure:
1013functions,8682supplemental instructions,986managed entries;6091actual
continuations, unchanged8192limit. Module mutations are39source-derived data
words/45changed bytes; compiled instruction bytes are preserved. The runtime,
compiler and twelve patches are unchanged; no runtime code emission was introduced.

Supporting units pass155controls:57create-prefix,41layout/capacity and57descriptor.
The prefix returns atED043C in239cycles; layout returns from real ED0222 atED04DC
in98more cycles; descriptor copying/argument setup reaches ED2538 in98more.
Controls include signed/overflow edges, rejected capacity, both descriptor formats,
no-copy selectors, live literals, aliases, unaligned and overlapping buffers.
The rejected capacity branch reaches missingED5374 without synthesizing a return.
Earlier unit observation stops were explicit; the full run connects the whole
tested path and terminates at the actual missingED2538 call.

Full checkpoint: `0373eaf50021b38d038752ee724cdce4fccfd6c1434729ec2d4c434b69ff7476`
(133,328 identities;5,056 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-create-startup/` and the
`20261008-startup-movie-create-prefix`, `create-layout`, `create-descriptor` units.
Unit checkpoint hashes respectively:
`6d094724656a3b15235d08d8c2a1e2fd9bb6008e5a262afd03512f3da9a621c1`,
`789b1002ffc3e8c35d2e3f51b67d657cde63f3127ec3fca2e5ed4a75e220de13`,
`6120f41d002662b4d2437473ac5f7bab334c641bbf8bc7a3dba416d3b82de64b`.
All capacity guards passed;21,437,984,768bytes available at sealing. No storage
compaction or deletion. No game/test/compiler or booted Simulator remains live.

**Next:** execute reviewed **ED2538**, a25-instruction leaf with no calls.
Predict26cycles toED05B2, nine ordered descriptor stores (36bytes), and32-byte
alignment of two pointer fields. Test source/destination/literal overlap,
unaligned and aliased data, and integer wrap. The saved exact RAM/CPU/timer/SQ
prediction is unexecuted in `next-startup-preflight.json`. Then execute the
retained ED05B2 continuation to measure its next real call. One classification
batch used since native execution; execute before deeper static review.

G1 remains unresolved. The8October inventory is inherited by verified digest;
its missing paths and absent supported PAL GDI were rechecked while sealing.
Runtime0/1, generated0/1474, headers0/177, contracts0/4, title38/39 remain missing;
last FFmpeg byte verification10139/10139, Apple media build unproved. The exact
source-only request remains unsent in `BOOTSTRAP-BLOCKERS.md`. Production
foundation, normal boot, playback, gameplay, audio, saves and mobile/device
acceptance remain open. Repository safety/whitespace pass. Goal active; this
turn made execution progress. Earlier entries below are historical.

**Previous recovery — 8 October 2026: setup binding and validation return in full startup; R3/C8 partial.**

Original ED1AFA and ED1B5E now execute together in full startup. Relative to the
prior ED1AFA stop, the child is entered at15cycles, returns at19, setup binding
returns at25, validation returns at34, and the original slot caller reaches missing
**ED03F8 at55cycles**. It performs the original flag write atEDCF38 and four ordered
stack-argument stores. These are original bodies and retained callers, with no
fabricated returns or changes to the runtime, compiler or twelve patches.

**349 state /257 device comparisons pass; all343 earlier named captures remain
unchanged.** The exact ARM64 binary passes both baseline scenes, ten external audits,
release corruption controls and six unchanged reference images. The new module
write changes data only:27source-derived store words,24changed bytes, instruction
bytes preserved. Closure:1012functions,8460supplemental instructions,962managed
entries;6091actual continuations with unchanged explicit8192limit.

Supporting units pass188controls:45setup-bind prefix,65child/connected return,
33validation return and45slot continuation. The child stores two words in4cycles,
returns through ED1B54 in10; destination/saved-return overlap is covered. Validation
returns in9cycles, with nonzero error-call controls. Slot continuation takes21cycles
on success or14 for its original failure return, covering live flag pointers,
source/stack overlap, aliases, unaligned data and status/FPU. Its runner used an empty
descriptive review path; the checkpoint explicitly binds the actual retained
`ECF0F8-20261007-movie-buffer-setup` review. Native code and results were unchanged.

Full checkpoint: `00ffbd743c34a6935cf0b4598409b1b6255271388edaa8a3b9a2a462925bc928`
(130,489 identities;4,944 freshly hashed). Evidence:
`artifacts/recovery-aot/20261008-startup-movie-setup-bind-startup/` and the
`20261008-startup-movie-setup-bind-prefix`, `setup-bind-child`, `setup-return`,
`slot-continuation` unit directories. All capacity guards passed;22,832,492,544bytes
available at sealing. No new storage sharing or deletion this turn. No game/test/
compiler or booted Simulator remains live.

**Next:** generate reviewed original **ED03F8** (222instructions,24blocks,12calls).
Its first callee **ED01BC is already registered**, as are ED0256 and ED0222.
The saved prediction is28cycles and48ordered stack-store bytes to ED01BC entry,
PR=ED042E/SP=FFFE90. Verify this first-call boundary and connect the existing helper
bodies; do not replace their returns or misclassify them as missing. Predictions
are unexecuted in `next-startup-preflight.json`; one classification batch used,
execute before reviewing deeper callees. Include stack/input/literal overlap and
integer-edge controls. Complete ED03F8 behavior and subsequent boot remain unproved.

The latest8October G1 inventory is inherited by verified digest; all its missing
paths and the absent supported PAL GDI were rechecked while sealing. Runtime0/1,
generated0/1474, headers0/177, contracts0/4, title38/39 remain unresolved; last byte
verification found FFmpeg10139/10139, Apple media build unproved. Exact unsent
source-only request stays in `BOOTSTRAP-BLOCKERS.md`. Production foundation,
normal boot, playback, gameplay, audio, saves and mobile/device acceptance remain
open. Repository safety/whitespace pass. Previous and current turns: progress;
goal active.

**Previous recovery — 8 October 2026: storage hold cleared; full startup advances through the signature guard and descriptor initializer. R3/C8 remains partial.**

After Chris freed space, the prepared native integration completed. **344 state and
252 device comparisons pass; all338 prior named captures are unchanged.** The same
ARM64 executable passes both baseline scenes, ten external audits, release corruption
controls and six unchanged reference images. All disk guards passed. Original ED38A0
returns after10cycles; the descriptor initializer returns after21; the retained setup
caller reaches missing **ED1AFA after30cycles** relative to the previous full stop.
No fabricated guest return or runtime/compiler/patch change was needed.

Full closure is1010functions,8424supplemental instructions and953managed entries;
6091actual continuations, unchanged explicit8192limit. Full checkpoint:
`b1574486c50d32e5184243a4ec5b93c94f9b76ba4710c92d120c87bedc8ae465`
(127,727 identities;5,503 freshly hashed). Evidence remains in the preassigned
`artifacts/recovery-aot/20261007-startup-movie-guard-final-startup/` directory;
execution and verification occurred after the8October resumption.

The next original body **ED1AFA** also passes45 isolated controls:36zero/nonzero,
live literal/stack overlap, alignment, aliases and status/FPU cases, plus9activation
controls. Captured zero argument reaches **ED1B5E in15cycles**, with four ordered
stack stores, matching the prospective whole RAM/CPU/timer/SQ state. Nonzero arguments
stop at original ED1BAA after11cycles. It is not yet integrated into full startup.
Unit checkpoint `a7b846722f541fc89fd7685c2b6111112510c73ac0c0a3bf8192c5df180e3303`
(233 identities), in `artifacts/recovery-aot/20261008-startup-movie-setup-bind-prefix/`.

**Next:** generate and execute reviewed original **ED1B5E**, a three-instruction
leaf with two ordered word stores and an RTS delay slot. Predict4cycles for the
leaf, then6more for retained ED1B54 to returnED03A6/R0zero/SPFFFF14. Test destination/
stack overlap, unaligned and aliased pointers, and status/FPU. Predictions remain
unexecuted in `artifacts/recovery-aot/20261008-movie-setup-bind-child-preflight/`.
One classification batch used since native execution; execute before deeper review.

G1 remains unresolved: the8October source audit still finds runtime0/1,
generated0/1474, headers0/177, contracts0/4, title38/39, FFmpeg10139/10139. Supported
PAL GDI/runtime checkout absent; Apple media integration unproved. Exact source-only
request stays unsent in `BOOTSTRAP-BLOCKERS.md`. Normal boot, video playback, gameplay,
audio, saves, mobile/device acceptance and the production-foundation decision remain
open. No game, test, compiler or Simulator remains live. Repository safety and
whitespace checks pass. This resumed goal turn made progress; goal active.

**Historical execution hold — 7 October 2026:** recovery integration is ready but blocked on
storage. Available 2,771,390,464 bytes; required 2,952,790,016; short
181,399,552 bytes. This hold has recurred across three turns. Exact sharing
is complete for the inspected diagnostics; candidate old binaries belong to retained
checkpoints and were not deleted. Add disk space (1 GiB additional headroom preferred)
to resume the prepared command below. G1 separately remains blocked on matching
source material and the supported PAL dump; the exact unsent request is in
[BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md). No native process or Simulator is live.
See `artifacts/recovery-aot/20261007-movie-guard-final-storage-blocker/audit.json`.

**Previous bounded recovery — 7 October 2026: signature guard and descriptor initializer return verified in units; full integration prepared, capacity hold.**

Four native units pass **148 controls**: signature guard ED38A0 (41), retained
ED18FC call (17), ED1BC4 leaf and ED1900 return (57), retained ED0390 setup
continuation (33). The matching signature returns in10cycles; the next call takes3,
the final leaf/initializer return takes8, and setup reaches missing **ED1AFA** in9.
RAM/CPU/timer/SQ/device comparisons and activation controls pass. The mismatch
signature branch stops at its original missing error handler; no return is fabricated.
The guard stdout had an old copied timing/address label; its correction is recorded
beside the original log. JSON, native assertions and prospective captures agree on
10cycles/ED18FC. Runtime/compiler/twelve patches remain unchanged.

The combined two-body integration now has a passing preflight and compiler syntax
check. It prepares **344 state /252 device comparisons**, retaining338 earlier
named predictions, with prospective1010functions/8424supplemental instructions/
953managed entries. **These combined results have not been executed.** The latest
verified full run remains the339state/247device binding-return artifact, stopping
atED38A0. No normal boot, playback or gameplay claim.

Exact duplicate sharing preserved88additional diagnostic paths, recovering
548,888,576bytes across two measured passes. Original data, assets, saves and source
trees were untouched. Byte hashes/path/owner/mode/access and modified timestamps/
xattrs were preserved; inode/birthtime changes declared. The merged inventory has
32,696paths. At checkpoint, available space was2,771,992,576bytes, **180,797,440bytes
below** the unchanged2,952,790,016byte (2816MiB) capture reserve. No full build was
attempted below that reserve. Further storage reclamation needs an identified safe
scope; do not repeat these sharing passes or delete evidence.

Handoff checkpoint: `ca225213e7682837427f8d152a7950bc3a7105e0fadfda24ac3ae7c8b935e72c`
(1,930 freshly checked identities). Setup-continuation checkpoint:
`409e8c9d315d308bd19c6846b0d6afb4ee0d75131595e457d86b182cd4729931` (222).
Evidence: `artifacts/recovery-aot/20261007-movie-guard-final-handoff/`,
`20261007-movie-guard-final-preflight/`, `20261007-movie-guard-final-observer-syntax/`,
`20261007-movie-guard-final-storage/`, and the four named unit directories.

**Next:** restore at least2816MiB available, preferably additional working margin,
then run the prepared integration exactly once:

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools DCR_GPR_CACHE=0 DCR_EMIT_HOT_TRACE=0 python3 work/dreamcast-recovery/run-startup-movie-guard-final-startup.py
```

After its native process is terminal, run `audit-startup-movie-guard-final-startup.py`,
`audit-loaded-movie-guard-final.py`, and `baseline-loop-regression.py
20261007-startup-movie-guard-final-startup` from `work/dreamcast-recovery/` (baseline
uses the same CLT environment). Expected relative capture cycles are0/10/13/16/21/30,
with final missingED1AFA. Seal the full result only after those audits and same-binary
baselines pass. Do not rerun the one-shot preparer or preflight.

Fresh G1 audit `artifacts/bootstrap-input-audit-20261007-movie-guard-final.json`:
runtime0/1, generated0/1474, headers0/177, contracts0/4, title38/39,
FFmpeg10139/10139. Runtime checkout and supported PAL GDI remain absent; Apple
media build unverified. Exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains
unsent. Production foundation and all boot/gameplay/mobile/device gates stay open.
No owned game/test/compiler or booted Simulator at checkpoint. Repository safety
and whitespace checks pass. Previous status-only turn: no progress; this turn:
progress (unit checkpoint, preserved storage, integration preflight/syntax, source
inventory). Goal remains active; disk hold has not met the repeated-blocker threshold.

**Previous bounded recovery — 7 October 2026: six-callback binding loop and descriptor fields return in full startup, R3/C8 partial.**

Original fifth callback ED0EEE and sixth callback ED16D4 now execute in the full
startup artifact. Relative to the prior ED0EEE stop, the fifth callback returns
at51cycles, sixth at73, binding loop at92, interface wrapper at97, and descriptor
fields at109. The original parent reaches missing **ED38A0 at116cycles**.
All returns come from original generated bodies and retained callers/callees.

**339 state and247 device comparisons pass; all331 earlier named captures are
unchanged.** The same ARM64 executable passes both baseline scenes, ten external
audits, release corruption controls and six unchanged reference images. Closure:
**1008functions**,8409 supplemental instructions,948managed entries;6091actual
continuations. Runtime/compiler/twelve patches unchanged. Supporting units passed
164controls:57fifth callback,57sixth/binding return,17interface return,33descriptor
return/continuation. The captured seven-word clear is idempotent; nonzero fixtures
verify clearing behavior. Descriptor initialization still has further callees;
normal boot/gameplay remain unproved.

Two fifth-callback fixture failures remain preserved. The first changed a PC
literal directly in RAM, bypassing runtime write tracking; repair uses dc_write32.
The second incorrectly required RAM to change although the captured buffer was
already zero and savedPR already matched. The passing repair2 adds nonzero buffers
and verifies the original prospective RAM/CPU/timer/SQ state. Neither required a
runtime/compiler change or a fabricated game return.

Checkpoint: `7604fefbfe4ce71f7fea46f0906ba3dbc489dda325d884dcfe5f9682932005cb`;
124,455 identities, 10,200 freshly hashed.
Storage sharing preserved 32,608 inventoried paths, 4,715 newly shared,
recovering1,245,827,072bytes without removing contents. Paths/owner/mode/access and
modified timestamps/xattrs preserved; inode/birthtime changes declared. Planning
first rejected a historical failed run, then was interrupted before mutations to
exclude21,277 already-recorded native clones. Both planning records remain saved.
All capacity guards passed during the full run. After checkpointing 2,647,658,496bytes
remain; restore2816MiB before another full capture. No game or Simulator is live.

**Next:** execute original **ED38A0** (13instructions,four blocks). The actual live
signature atEDC9AC matches5A5A5A5A. Predict **10cycles**, unchanged RAM/SQ and return
ED18FC with PR/SP preserved. Also test the mismatch branch: prospective12cycles
to explicitly missing tail targetED1AA2, with R4=0/R5=FF000201. Do not synthesize
that return. Cover live pointer/signature/error-code literals, aliases, unaligned
reads and status/FPU. Predictions are unexecuted and saved in
`next-startup-preflight.json`; one classification batch used, execute before
reviewing deeper callees.

Fresh G1 audit is unchanged: runtime0/1,generated0/1474,headers0/177,contracts0/4,
title38/39,FFmpeg10139/10139. Exact supported PAL GDI and Apple media integration
remain absent/unproved. `BOOTSTRAP-BLOCKERS.md` retains the exact unsent source-only
request. Production foundation, normal boot, gameplay, audio, saves and mobile/
physical-device acceptance remain open. Previous/current goal turns: progress.
Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-binding-return-startup/`,
`20261007-movie-binding-return-storage-repair2/`, and the fifth/sixth/interface/
descriptor unit folders. Failed fifth fixtures and storage plans are retained.

**Previous bounded recovery — 7 October 2026: initializer and third/fourth callback returns verified in full startup, R3/C8 partial.**

Original ED8216, ED8246, EDA7C4 and ED5038 now execute together in the native
startup artifact. The counted record loop returns at722cycles, final setup at862,
initializer at868, third callback at927, and fourth callback at949 relative to
the prior ED8216 stop. The retained binding loop reaches missing **ED0EEE at968cycles**.
These are original generated bodies and retained callees; no synthesized returns.

**332 state and240 device comparisons pass; all323 prior named captures are
unchanged.** The same ARM64 executable passes both baseline scenes, ten external
audits, release corruption controls and six unchanged reference images. Closure:
**1006functions**,8398 supplemental instructions,945managed entries. The actual
continuation trace remains6091entries. Runtime/compiler/twelve patches unchanged.
The new callback unit passed32leaf+16connected+9activation controls; the preceding
third-return/binding units passed82controls. Together with the267 initializer
controls,406 unit controls support the integration. Normal boot remains unproved.

The first full audit expected the error text to print the missing callee. It only
prints caller PC/PR. That failed audit is preserved in `audit-log-format-failure1/`.
The corrected audit proves ED0EEE from captured R3, original JSR @R3 atEDA2E2,
live table atEDC614/EDC770 and absence from the compiled inventory. Native code
and execution were unchanged; all state/device comparisons had already passed.

Checkpoint: `76d9c927408e8b6e0e1417e7ac297c3e1003137d4659f821965acec29112f79d`;
121,157 identities, 8,316 freshly hashed.
Exact-file sharing of completed diagnostics preserved 27,893 paths,
including 3,586 newly shared paths. This storage batch recovered
457,723,904bytes; contents/paths/owner/mode/atime/mtime/xattrs preserved, inode/birthtime
changes recorded. A disposable compression probe produced no benefit; its sparse
copy result was not applied to existing evidence. No evidence content was removed.
All capacity guards passed for the full run. After verification 2,494,537,728bytes
remain; restore2816MiB before another full capture. No game/Simulator remains live.

**Next:** execute original **ED0EEE** (nine instructions,two blocks) with retained
ED67F8. Predict7setup+40fill+4return=**51cycles**,28zero bytes atEE6DC4, one temporary
savedPR word atFFFED4, returnEDA2E6/R0zero/SP restored. This prediction is unexecuted;
exact RAM/CPU/timers/SQ are in `next-startup-preflight.json`. Test live literal and
destination/frame overlap, unaligned/aliased addresses and status/FPU. One target
classification batch used; execute before deeper review.

Fresh G1 inventory: runtime0/1,generated0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139. Exact PAL GDI remains absent; Apple media integration unproved.
`BOOTSTRAP-BLOCKERS.md` retains the exact unsent source-only acquisition request.
Production foundation, normal boot, gameplay, audio, saves and mobile/device
acceptance remain open. Previous status-only turn: no progress; this goal turn:
progress. Goal remains active.

Evidence: `artifacts/recovery-aot/20261007-startup-movie-initializer-startup/`,
`20261007-startup-movie-fourth-callback/`, and `20261007-movie-initializer-storage/`.

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

**Previous bounded recovery — 7 October 2026: record serialization and command prefixes.**

Original `10A2DA` now returns from the captured route in **19,029 modeled cycles**.
Its 24 packing patterns and 16 ordered source/stack overlaps pass, including
signed nonzero bytes and reversed output-byte order. Original `10A326` reaches
real `6067AA`: three prospective whole16MiB RAM/69CPU/25TMU comparisons pass at
42/19,071/19,081 cycles. Later transport-query state is observed only.

Original `64A734/664286` then passes 48 argument/alignment/alias/T controls and
the captured **26-cycle** zero-lock/three-argument forwarding path. It rejects
at genuine missing **`6642C0`**, PC `6642A0`, PR `6642B0`, SP `FFFE80`, with lock
`664538`=80 hex and arguments0/FFFEBC/30 hex. The worker, command, transport,
helper and four-record parent returns remain unverified. Other serializer route
and busy-lock behavior are also unproved. Five new original bodies/177 instructions
were emitted separately without unknown/RAW operations or callee substitutions.

The new checkpoint binds **151 identities**, SHA-256
`6e4ce88e665df56ed7957c897e1d96cbaa021176d5a6466209225b9288e68a04`.
All3748 full-startup,155 prior-prefix and151 new identities, plus237 preserved
diagnostic paths, were reverified. The full-startup **235341/38** checkpoint is
unchanged and was not rerun. Both new binaries are ARM64/macOS, minimum13.0,
SDK27.0. A global-selector otool probe exited69 for the existing Xcode license
block; explicit CLT selection verifies platform identity without changing that
license. Both native experiments passed on their first build/run.

Next execute original24-instruction `6642C0` with its held lock and packet intact.
Its source predicts a24-byte frame and21-cycle prefix preparing header words4/0,
forwarding bufferFFFEBC/48 words and calling already registered `663550` with
R5=12/R7=2. That worker path is not executed yet. Preserve the lock80 until its
real caller releases it; no fake response, lock reset or forced completion.
One classification batch used since the last native experiment. Unit400MiB guard
is met; full1536MiB guard is not. Fresh source audit remains runtime0/1,
guest0/1474, headers0/177, contracts0/4, title38/39, FFmpeg10139/10139; PAL verifier
exits2 for the absent descriptor. Exact source-only request stays current/unsent.
G1, normal boot, gameplay, saves, audio and mobile acceptance remain unresolved.
All experiments terminal; no Simulator. Goal turn: **progress**.
Evidence: `artifacts/recovery-aot/20261007-startup-record-helper/` and
`20261007-startup-record-command/`.

**Previous bounded recovery — 7 October 2026: original record-loop prefix.**

Original `10B906` passes 64 signed-byte/counter/alias controls and four isolated
captured record inputs in 13 modeled cycles. Captured record0 returns1; records1–3
return0. The actual original `10B992` gate2 path then passes two prospective
whole16MiB RAM/69CPU/25TMU comparisons at first predicate entry/return in21/34
cycles, preserving its16-byte frame, mode22 store and real callee. Real original
`10A3F2` queries, `10B226/10B1EC` mapping, `10B2E0` and `109AE0` execute next,
then reject explicitly at missing **`10A326`**, PC `10B89E`, PR `10B8A4`,
SP `FFFF88`. That later state is observed only; the four-record parent has not
returned. Five emitted original bodies/761 instructions contain no unknown/RAW
operations; no callee stub or forced progress. Three harness setup/compile/fixture
failures are preserved; none executed guest code. The repaired predicate reuses
the identical emitted body/object.

The full-startup checkpoint remains **235341 / 38 comparisons**, unchanged and
not rerun. New bounded checkpoint binds **155 identities**, SHA-256
`01cfe5622b8ad068e1e91bc2937a2cf1562656e59b9c2569f6551432bbfe2a1d`.
Eight additional exact redundant emitted-image copies preserve all contents and
metadata; measured free-capacity gain350,670,848 bytes. Post-verification checks
all237 preserved diagnostic paths and all3748 full-startup plus155 new identities.
Original source/input/asset/save/executable files were excluded. Unit400MiB guard
is restored; full1536MiB guard remains unmet. Recheck before the next build.

Next reduce original47-instruction `10A326`: source predicts a204-byte frame;
actual record bytes1536/1537/1538/1539 are1/1/0/1, selecting real `10A2DA` after
the matching predicate, then `6067AA`. Review/model those causal dependencies,
execute before broader classification, and keep parent return/transport behavior
unverified until measured. One classification batch used after the native prefix.
Fresh7October source audit exits2: runtime0/1, guest0/1474, headers0/177,
contracts0/4, title38/39, FFmpeg10139/10139. PAL descriptor absent/verifierexit2.
Exact source-only request remains current and unsent. G1, normal boot, gameplay,
audio, saves, mobile and production acceptance remain open. All experiments are
terminal; no Simulator. Goal remains active: **progress**.
Evidence: `artifacts/recovery-aot/20261007-startup-record-loop-prefix/`;
`20261007-startup-record-predicate-repair3/`; `20261007-record-loop-storage/`.

**Previous full-startup checkpoint — 20261006-235341: original counter path.**
All **38 prospective comparisons pass** across the entire 16 MiB RAM, 69 CPU
fields and 25 timer fields. The earlier 36 expectations are unchanged. Original
`010B7A` returns through real `607124 → 64B9E4` in **82 modeled cycles**, preserving
its 80-byte parent frame, 84-byte maximum nested frame and expected MAC/FPU state.
The separate reduction passes **7,936 cases**: 6,144 arithmetic/guard/alias and
alignment cases, 768 non-drawing modes and 1,024 ordered frame/global/literal
overlaps. Drawing, state2 with flag1 and FPSCR.SZ1 remain unproved. The first
harness build failed because helper extraction matched a using-namespace line;
the repair restores test helpers and reuses the identical emitted body/object.
The failed sources/report are preserved; no guest code ran in that failed build.

The same executable passes both supplied scenes, ten external audits, release
corruption controls and six unchanged images. Earlier word-add8/zero-guard9/
reset71/query63/provider24/owner16 and motion contracts remain verified. Measured
closure **859 functions / 83,993 instructions / 2,396 external records** fits
unchanged cap860. Twelve patches and all 51 capacity guards pass; 20 executed
sources were frozen before the next review. Sharing preserves **229 diagnostic
paths** and their logical contents, excluding original source/input/asset/save/
executable files. Checkpoint binds **3,748 identities**, reverifying 3,179 prior
and 99 reduction identities with archived versions of the two changed sources.
SHA-256 `58dbd3c41d15350a334ff59c5d62995f9ac4da97b088aa8cffa3180bca2d7da3`.

Startup exits1 without timeout at missing **`10B992`**, PC `0556EA`, PR `0556EE`,
SP `FFFFC8`, SR `40000101`, R4 `16` hex. This later state is observed, not
predicted. Its 51-instruction body has a 16-byte frame and a four-record loop;
actual gate `758F3C`=2 and toggle `7AA760`=0 precede its frame writes. The actual
gate selects the loop, whose `10B906`, `10B226` and `10B2E0` callees are missing;
`10A3F2` is registered. Next model and execute this causal path, preserving
ordered frame/literal reads, the mode store, loop arguments and real bindings.
No helper stub or forced early return. One new classification batch used;
execute before broad inventory. Current free capacity **479 MiB**;
recheck unchanged storage guards before the next build.

Fresh G1 audit remains runtime0/1, generated0/1474, headers0/177, contracts0/4,
title38/39 and FFmpeg source10139/10139. Supported PAL v1.003 GDI is absent;
Apple media integration and matching Katana closure remain unresolved. Exact
source-only acquisition request is unchanged and unsent. Normal boot, gameplay,
audio, saves, mobile and production acceptance remain open. All runs terminal;
no Simulator launched. Goal remains active: **progress**.
Evidence: `artifacts/recovery-aot/20261006-235341/`.

Updated: **6 October 2026, public-source recovery experiment**. Private-only; publication not approved.

**Previous full-startup checkpoint — 20261006-230001: original zero-flag return.**

All **34 prospective comparisons pass** across the entire16MiB RAM,69CPU fields
and25timer fields. All32 previous predictions are unchanged. Original `04562E`
now returns from its actual zero-byte guard in **nine modeled cycles**: PC/PR
`0561E8`, SP `FFFFA4`, R0zero/T1 and R5 `74D1BC`. One saved-PR store/four-byte
frame; literal read before push, flag read afterward. Its native reduction passed
**5,120 cases**:1,408 zero returns and3,712 explicitly bounded observations at
first real609368 entry for signed nonzero flags. Those observations are not child
or parent-return proof; the actual zero path executes with all real bindings
restored and no test-only observation hook. Original mode16 was already present
in the prior snapshot and is preserved; the old mode0 audit assumption failed,
then was corrected to the mode word in each prospective expected RAM. Both
mode-audit failure and earlier test-packaging/setup failures are preserved.

Earlier owner16/provider24/query63/reset71 and motion contracts remain verified.
The registry audit also corrected a prior inventory claim: nested `659360` and
`60A6E4` are registered real provider bodies. The nonzero driver return remains
unproved. Only missing04562E and a delegating capture observer were added to full
startup. Measured closure **857functions/83663instructions/2383external records**
fits existing cap858; no cap increase. Twelve patches unchanged; all51 capacity
checks pass and20 executed sources are frozen. Sharing preserves153 diagnostic
paths:56baseline captures/58objects/34RAM captures/five redundant emitted-image
copies, including full byte/path/owner/mode/time/xattr checks. Original source,
BOOT/disc, assets and saves excluded; no content removed. Checkpoint binds
**2,607 identities**, reverifying1,988 prior and157 unit identities with declared
archived versions of the two changed sources. SHA-256
`0a02921671ac6e50ee913363d5c826d64308aad8add0dbbe6cfbe70f748f7073`.
The same executable passes both supplied scenes, ten external audits, release
corruption controls and six unchanged images.

Startup exits1 without timeout at genuine missing **`08C25E`**, PC `0561E8`,
PR `0561EE`, SP `FFFFA4`, SR `40000101`, R0zero. This later terminal state is
observed, not predicted or verified by the34 comparisons. Evidence:
`artifacts/recovery-aot/20261006-230001/`.

**Prior preflight for original `08C25E`** (superseded by the reduction result above), seven instructions with no callees or frame.
Read live literals08C34C→758F44 and08C330→756C68, source word before destination,
modulo32 addition, one four-byte store in the RTS delay. Actual words2 and0
predict2; proposed **eight-cycle return** to PC/PR `0561EE`, SP `FFFFA4`, SR/T/
MAC/FPU preserved. This contract was unexecuted at the 230001 checkpoint. Its subsequent reduction verifies ordered source/destination/
literal overlap, aliases, alignments, wrap arithmetic and wholeRAM/CPU/timers.
One classification batch used: execute before more inventory. Expected full858
would exhaust cap858; increase only after actual transitive measurement.
Queue: `artifacts/recovery-aot/20261006-230001/next-startup-preflight.json`.

Fresh G1 inventory remains runtime0/1, generated0/1474, headers0/177, contracts0/4,
title38/39 and FFmpeg source10139/10139. Supported PALv1.003 GDI is still missing;
Apple media integration remains unproved. Source-only acquisition request is
unchanged and unsent. Normal boot, gameplay, audio, saves/VMU, mobile and production
acceptance remain open. All experiments terminal; no Simulator launched.
Goal remains active; checkpoint classification: **progress**.

**Previous full-startup checkpoint — 20261006-223658: original reset returns.**

All **32 prospective startup comparisons pass**, each checking the entire 16 MiB
RAM, 69 CPU fields and 25 timer fields. The 30 earlier predictions are unchanged.
Original `05A2E6` with real `088644` now completes the reverse 18-byte fill and
late global zero in **71 modeled cycles**, returning to PC/PR `05595A` with
SP `FFFFA4` and R0 one. Its separate native reduction passed **35,840 cases**,
including ordered stack/destination/global/literal overlaps, aliases, alignment,
IRQ masks and FPU banks. Zero/negative helper-count termination is unproved.
Earlier owner16/provider24/query63 and motion contracts remain verified.

Startup then exits1 without timeout at missing **`04562E`**, PC `0561E2`,
PR `0561E8`, SP `FFFFA4`. This later terminal state is observed, not predicted.
The same executable passes both supplied scene regressions, ten external audits,
release-corruption controls and six unchanged images. Closure measured before
raising cap856 to858: **856 functions / 83,625 instructions / 2,383 external
records**. The earlier 2,385 metadata estimate failed; its correction reused the
successful native measurement. Twelve patches are unchanged; all51 capacity
checks pass and20 executed sources are frozen. Storage sharing preserves149
completed diagnostic paths; no source, game input, asset or save content removed.
Checkpoint binds **1,988 identities**, reverifying1,403 prior and66 reduction
identities with the two changed source versions preserved. SHA-256
`14c83dad8eb1d4c2f965c6b5e671915a3d06e74f8a69ee85bafb3230fb16f4b0`.
Evidence: `artifacts/recovery-aot/20261006-223658/`.

**Next execute the original `04562E` zero-flag return.** Actual byte at `74D1BF`
is00. Source predicts a **nine-cycle return**, one saved-PR store/four-byte frame,
PC/PR `0561E8`, SP `FFFFA4`, R0zero/T1 and R5 `74D1BC`. This model is unexecuted.
Prove the literal-before-push and byte-after-push order, saved PR, overlaps,
aliases, alignment and full RAM/CPU/timer state. Keep registered `609368` and
`642908` available; their nonzero branch and nested/dynamic calls remain unproved.
Two classification batches used: execute before further classification. Expected
857 functions fit cap858 only if measured closure agrees. Retain32 startup
contracts and exact-binary scene regressions. Queue:
`artifacts/recovery-aot/20261006-223658/next-startup-preflight.json`.

Fresh G1 audit remains runtime0/1, generated0/1474, headers0/177, contracts0/4,
title38/39 and FFmpeg source10139/10139. Exact PALv1.003 GDI remains absent;
Apple media integration is unproved. The source-only acquisition request is
unchanged and unsent. Normal boot, playable gameplay, audio, saves/VMU, mobile
and production acceptance remain open. All experiments terminal; no Simulator
launched. Goal remains active; this checkpoint is **progress**.

**Previous full-startup checkpoint — 20261006-221656: provider and caller return.**

Original `606C9C` is now connected with the already registered `64B9E4` guard.
**All 30 prospective comparisons pass** across whole 16 MiB RAM, 69 CPU fields
and 25 timer fields. The actual guard word1 produces the original minus1 provider
return in24modeled cycles; the original query caller returns1 after its first
query in63cycles. PC/PR `055946`, SP `FFFFA4`. This is an early return, not a
successful eight-query pass. The unchanged16cycle owner-return prediction now
passes in full startup, and the repaired `10EFBC` observer captures correctly.
Both earlier capture failures and the native64cycle model failure are preserved.

Twenty-six prior named phase predictions are unchanged; the unchanged owner
prediction and three provider/caller phases bring the prospective total to30.
The former missing606C9C failure is replaced by its registered entry: identical
wholeRAM,68otherCPU and25timer fields, with PC now `606C9C`. No PC was forced.
The later uncovered terminal state was **not predicted or verified** by these
comparisons. Startup intentionally exits1, without timeout, at genuine missing
**`05A2E6`**: PC `055954`, PR `05595A`, SP `FFFFA4`, SR `40000100`, R0one and
R4 `FFFFFFFF`. Sector count215; audio/IRQ completion is not inferred.

The exact executable passes two supplied scenes, ten external audits, release
corruption controls and six unchanged images. Motion and prior gate/flag/record/
checksum/copy/vector/global/buffer contracts remain verified. Only606C9C was
added; no missing-tail stub or forced success. Measured closure854functions/
83608instructions/2386external records matches the prediction; cap856 retained.
All51capacity checks pass. Twenty executed sources are frozen. Sharing preserves
171completed diagnostic paths, including24derived generated-image copies; every
byte/path/owner/mode/access-modified timestamp/xattr is retained, inode/birthtime
changes declared. Original source/disc/BOOT/assets/saves excluded; no content
removed. Checkpoint binds **1403identities**, reverifies775prior/109reduction
identities, and preserves the two intentionally superseded source versions.
SHA-256 `5ed10d553271515df6dcdbd65e5a12984af35349ef6fc0acf4e044822d54a996`.
Evidence: `artifacts/recovery-aot/20261006-221656/`.

**Next execute original `05A2E6` plus real helper `088644`** (11+6instructions).
The actual source loads destination `78F4D0`, count18, helper088644; pushPR,
JSRdelayR5zero. The helper fills descending bytes, including the final branch
delay store. The caller restores savedPR, reads global `7902C8` after the fill,
and stores zero in its RTS delay. Proposed71cycle return to PC/PR `05595A`,
SP `FFFFA4`, R0one unchanged:4byte frame/20stores/26writtenbytes. This model is
**unexecuted**. Prove ordered stack/destination/late-global/literal overlap,
aliases, alignments, IRQ/T/FPU and wholeRAM/CPU/timers; helper positive counts
including18, without claiming terminating zero/negative counts. Isolated18
would exhaust18, propose20 only after measurement; full856 would exhaust856,
propose858 only after measured transitive closure. Two classification batches
used: execute before broader review. Retain all30startup contracts and exact
binary scenes. Exact queue: `artifacts/recovery-aot/20261006-221656/next-startup-preflight.json`.

Fresh G1 audit still shows runtime0/1, generated0/1474, headers0/177, contracts0/4,
title38/39 and FFmpeg source10139/10139. Exact PALv1.003 GDI remains absent;
Apple media integration unproved. Source-only acquisition request unchanged and
unsent. G1/fullinit/IRQ/audio/normalboot/gameplay/save/VMU/mobile/production stay
open. All processes terminal; no Simulator launched. Goal: **progress**.

**Previous native reduction — original provider and first-query return verified.**

Original `606C9C` and the real registered guard `64B9E4` pass **60,160 cases**:
32,896 returns and 27,264 explicit missing-target rejections. Coverage includes
13 wrapped/high-bit indices, independent P1/P2 stack/guard/tail aliases, four
alignments, all 16 IRQ masks, both T values and FPU banks, 6,144 ordered
stack/table overlaps and 768 live-literal overlaps. Whole 16 MiB RAM, 69 CPU
fields, 25 timer fields and unrelated devices match the source-derived model.
The actual index-zero table word is 1: the provider returns minus1 in **24
modeled cycles**; the zero-status branch rejects missing `64C766` in 26.
The original connected `055798`/`055768` caller returns **1 after its first
query in 63 modeled cycles**, restoring PC/PR `055946` and SP `FFFFA4`.
This proves the early return, not successful completion of all eight queries.

The first run matched provider contracts and caller RAM/CPU but rejected its
64-cycle timer prediction. The model incorrectly counted BRA plus delay as
three cycles; the pinned emitter uses two. Only that expectation changed to63;
the same native suite then passed. Failed report, sources and predictions remain
preserved; original game bodies, inputs, cached runtime and twelve patches did
not change. Closure16/765instructions/3external records was measured at cap16
before raising the isolated cap to18. Checkpoint binds109identities and
reverifies all775 prior identities; SHA-256
`d60b557e09e29ccdce81ebff42190ce70699a1ce12d5142e61e33baec67414fc`.
Evidence: `artifacts/recovery-aot/20261006-startup-query-provider-reduction-repair1/`.

**Next at that checkpoint:** integrate only original `606C9C` into a justified, guarded full-startup
continuation; retain the registered guard and native-tested `10EFBC` observer.
Keep the 27 previous state comparisons and unchanged owner prediction, add the
24-cycle provider and 63-cycle first-query return predictions, and validate the
owner capture in full startup. Expected callback854 fits cap856 unless measured
otherwise. Free capacity through byte-preserving sharing of completed diagnostic
outputs before invoking the existing1536MiB startup guard; do not lower guards.
The latest full startup remains212911,27of28captures. The new provider/caller
return has only reduced native evidence. G1, full initialization, normal boot,
gameplay, audio, save/VMU and mobile remain open. All processes terminal; no
Simulator launched. Goal turn classification: **progress**.

**Previous full-startup checkpoint — 20261006-212911: original first query reaches its provider.**

Original `055798` and its helper `055768` are now connected to startup.
**Twenty-seven prospective comparisons pass** across whole16MiB RAM,69CPU
fields and25timer fields. They include the outer/helper entries and the predicted
18-cycle failure at genuine missing `606C9C`: PC `055768`, PR `05577A`,
SP `FFFF44`, R4zero and R5 `FFFF48`. The four ordered long pushes plus one byte
store preserve17bytes in a96-byte total frame. The helper's signed-byte read
and zero extension retain the original order. **2,048 caller and20,480 helper
cases** pass in isolation, covering ten indices, independent P1/P2 stack/provider
aliases, four alignments, all16IRQ masks, both T values and both FPU banks.
No provider or eight-check-loop return was substituted or claimed.

The owner-return capture remains absent: **27of28 planned startup captures**
are verified. The alias repair still required the outer PR `055942`, while the
original parent's JSR sets inner PR `04E318` before that resumed block. Both
failed capture attempts and their rejected audits remain preserved.
The observer now watches the real returning tail `10EFBC`, after the original
JMP delay slot restores savedPR. A connected native reduction tests that exact
observer using the existing853-function compiled objects and the captured gate
return. It executes original `04E310` from its registered `04E33E` continuation,
then original `10EFBC`, without replaying completed work or altering game bodies.
**The unchanged16-cycle owner-return prediction passes wholeRAM/CPU/timers**, with
PC/PR `055942`, SP `FFFFA4`, byte0 at `758C78` and long `DEAD0CAB` at `5FF840`.
The observer captures exactly once. This verifies the connected return and the
observer separately; full-startup capture validation awaits the next justified
continuation. No startup was rerun for bookkeeping. The first standalone compile
failed only because its copied setup lacked the assertion helper; failed sources/
report are preserved, and the repaired fixture passes with identical game code,
inputs, cached objects and expected results.

The exact startup executable passes two supplied scenes, ten external audits,
release corruption controls and six unchanged images. Prior gate/flag/record/
checksum/copy contracts and motion return remain verified. Raw startup exits1
at the missing provider without timeout. Only the two original query routines
were added. Measured closure:853functions/83589instructions/2387external records,
cap856. Twenty executed sources are frozen; twelve runtime patches and original
inputs remain unchanged. All51capacity checks pass. Checkpoint binds **775
identities**, reverifying967 prior and219 reduction identities; SHA-256
`f761277529d9559ae66e421fa9df029376b793ac884700b236dcab859225f6c9`.
144completed diagnostic paths retain every logical byte/path/owner/mode/access-
modified timestamp/xattr through sharing; inode/birthtime changes are declared.
Original inputs, source, assets and saves were excluded; no content removed.
All experiments are terminal; no Simulator launched.

The source-derived provider contract below was subsequently executed by the
native reduction described above. The frozen full-startup preflight remains
historical; provider/full-caller returns still await integrated startup validation.

Fresh G1 audit remains runtime0/1, generated0/1474, headers0/177, contracts0/4,
title providers38/39 and FFmpeg source10139/10139. Exact PALv1.003 GDI remains
absent; Apple media integration is unverified. The exact source-only acquisition
request remains unchanged and unsent. G1, full initialization, IRQ completion,
audible audio, normal boot, gameplay, saves/VMU and mobile remain open.
R3/C8 partial; goal turn classification: **progress**.

**Previous full-startup checkpoint — 20261006-211159: startup gate returns.**

Original `011238` is now connected to startup. **Twenty-four prospective
comparisons pass** across whole 16 MiB RAM, 69 CPU fields and 25 timer fields,
including its entry/return and all retained record, checksum, copy and earlier
startup contracts. The captured gate returns zero in 48 modeled cycles.
The isolated reduction also covers 57,344 cases, including explicit alternate-
branch rejection at missing `606C9C`; that alternate branch is not the live
startup path. No host initializer or missing-function stub was introduced.

Startup then returns from the owning `04E310` routine through its original
`10EFBC` tail and reaches missing **`055798`**, at PC `055942`, PR `055946`,
SP `FFFFA4`, R0 zero and R4 `DEAD0CAB`. This return is observed in the trace,
**not fully verified by the planned caller capture**. One of 25 planned captures
was absent: the observer watched `04E310`, while startup enters its registered
interior alias `04E318`. Both map to the same original function. The failed
audit and missing capture remain recorded. A four-reference observer repair
now watches/restores `04E318`; native validation awaits the next justified
startup continuation. No game was rerun to repair bookkeeping, and the executed
sources are preserved separately from that pending repair.

The exact executable passes two supplied scenes, ten external audits, release
corruption controls and six unchanged images. Motion return and earlier startup
contracts remain verified. Raw startup exits1 intentionally at the genuine
missing function without timeout. Only `011238` was added; the measured closure
is 851 functions / 83,546 instructions / 2,387 external records, within cap852.
Twenty full-run sources are frozen, all twelve runtime patches and original
inputs are unchanged, and all 51 capacity checks pass. Checkpoint binds
**967 identities**, reverifying 569 prior and 67 reduction identities; SHA-256
`4649951301db31cb0b942cd0ba1cad6fda91307284aed144144ac14270022d75`.

Storage sharing preserves 397 distinct completed diagnostic paths, including
compiled static archives with differing lengths. Their logical bytes, paths,
owner, mode, access/modified timestamps and xattrs remain intact; inode/birthtime
changes are declared. Original inputs, sources, assets and saves were excluded.
A compiled-reduction executor preflight initially used the wrong completion-step
selector; it failed before any target mutation, was preserved and was repaired.
No content was removed. After sharing, 2.2 GiB was available and the full build
passed its original guards. All experiments are terminal; no Simulator launched.

**Next execution:** reduce original `055798` (19 instructions) and its original
helper `055768` (24 instructions) as a connected path. The outer routine saves
R14/R13/PR and begins an eight-check loop. The first helper saves PR, allocates
80 bytes, stores the low index byte, reads it with sign extension, then zero-
extends it in the call delay slot. Actual literal `055810` points to missing
`606C9C`, which must remain an explicit failure. Source predicts 18 modeled
cycles to block PC `055768`, PR `05577A`, SP `FFFF44`, R4 zero and R5 `FFFF48`;
these predictions are unexecuted. Prove stack ordering, aliases, byte alignment,
high-bit indices, IRQ/T/FPU preservation and whole RAM/CPU/timers; never fake the
provider return or claim loop completion. Isolated14 would exhaust cap14;
propose16 only after measurement. Full853 would exceed852; propose856 only if
measured transitive closure requires it. Two classification batches are used:
execute before inspecting more game source. Exact next contract:
`artifacts/recovery-aot/20261006-211159/next-startup-preflight.json`.

Fresh G1 audit still finds runtime0/1, generated0/1474, headers0/177,
contracts0/4, title providers38/39 and FFmpeg source10139/10139. Exact PALv1.003
GDI remains absent; Apple media integration is unverified. The source-only
acquisition request is unchanged and unsent. Original G1, full initialization,
IRQ completion, audible audio, normal boot, gameplay, saves/VMU and mobile
acceptance remain open. R3/C8 partial; goal turn classification: **progress**.

**Previous isolated result — original `011238` startup gate verified.**

The native ARM64 reduction passes **57,344 cases**: 34,816 returns and
22,528 explicit rejections at missing `606C9C`; 10,240 overlap cases change
the branch after the original writes. The sixteen ordered byte copies,
eighty-byte stack frame, live gate/descriptor reads, independent P1/P2 aliases,
four alignments, IRQ masks, both T values and both FPU banks match whole
16 MiB RAM, 69 CPU fields and 25 timer fields. The captured nonzero gate returns
zero to `04E33E` after 48 modeled cycles, restoring SP and R14. The zero-gate
path also takes 48 cycles to its genuine missing provider; `010FCC` stays absent.
No replacement initializer or missing-function stub was introduced.

This result is **isolated, not integrated into full startup**. The last full
checkpoint remains `20261006-204816` below. The reduction checkpoint binds
67 identities and reverifies all 569 identities from that checkpoint; SHA-256
`f3e4b6d083e45d82feb30c8e2b3e8dbbae6a6c0acabeca078939d72cc6485fa0`.
Measured reduction closure is 12 functions / 695 instructions / 3 external
records with cap 14. Original inputs and all twelve runtime patches are unchanged;
all processes are terminal and no Simulator was launched.

**Next:** connect only `011238` to original startup and prospectively verify
the owning `04E310` return through already-registered `10EFBC`. Source review
predicts sixteen additional modeled cycles, a byte clear at `758C78`, and a
long store of `DEAD0CAB` at `5FF840`. The actual saved PR is `055942`, predicting
PC/PR `055942` and SP `FFFFA4`; that caller/tail return remains unexecuted.
Further boundaries are unknown. The full build retains its 1,536 MiB starting
capacity guard; current free space is about 600 MiB. Preserve completed diagnostic
contents through eligible APFS sharing before attempting that build.

Fresh source audit still reports runtime 0/1, generated guest sources 0/1474,
headers 0/177, contracts 0/4, title providers 38/39 and FFmpeg source 10139/10139.
The exact PAL v1.003 descriptor remains absent. Original G1, full initialization,
normal boot, gameplay, audible audio, persistence and mobile acceptance remain
open. Evidence: `artifacts/recovery-aot/20261006-startup-gate-return-reduction/`.

**Previous full-startup checkpoint — 20261006-204816: original record initializer returns.**

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

**Previous full-startup checkpoint — 20261006-203215: checksum and three record copies verified.**

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

**Previous full-startup checkpoint — 20261006-201553: six nine-byte records verified.**

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

**Previous full-startup checkpoint — 20261006-200306: triplet return verified.**

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

**Previous full-startup checkpoint — 20261006-194729: four table records complete.**

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

**Previous full-startup checkpoint — 20261006-193353: six byte records complete.**

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

**Previous full-startup checkpoint — 20261006-192131: second record-copy child returns.**

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

**Previous full-startup checkpoint — 20261006-191301: first record-copy child returns.**

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

**Previous full-startup checkpoint — 20261006-190227: original record initializer reaches its first missing child.**

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

**Previous full-startup checkpoint — 20261006-185155: vector initializer returns; parent return observed.**

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

**Previous full-startup checkpoint — 20261006-184314: original state prefix reaches its first child.**

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

**Previous full-startup checkpoint — 20261006-183420: buffer initializer completes its return.**

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

**Previous full-startup checkpoint — 20261006-182631: original selector returns.**

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

**Previous full-startup checkpoint — 20261006-180910: original buffer prefix reaches its child.**

Original `091A20` runs all seven clear loops (96/512/344/96/512/344/512 bytes),
2,416 byte stores over 2,336 unique bytes, two zero-word stores and a saved PR
stack word in a four-byte frame. **4,096 isolated cases** verify overlapping
ranges, guards, independent P1/P2 buffer/word/stack aliases, IRQ masks, T/FPU
banks, DT/BF-S increments and delays, BSR/zero delay, T1/register effects,
SR/MACL/FPU, whole16MiB RAM/69CPU/25TMU and **9,691 modeled cycles**. The actual
missing child remains an explicit failure; no known helper is omitted.
Isolated closure: one function/62 instructions/two external records.

**Both prospective live phases match**: entry and the first missing-child
failure, without exclusions. The motion-return contract and exact-binary
regressions pass: two supplied scenes, ten external audits, release controls
and six unchanged images. Missing **`0922E0`**, PC `091A80`, PR `091A8C`,
SP `FFFF9C`, SR `40000101`, SPC `667A28`. Mode0/cycles115737351/
sectors215/ARM7instructions2845394/faults0/
AICAframes16623/nonzero0/IRQ36.
The prefix is verified; parent return/final zero, full initialization/IRQ,
audible game audio, normal boot and gameplay remain unproven.

Closure **837functions/82,804instructions/2,393external records**, cap838, is
inventory. Twenty full/four reduction sources frozen;12patches/private inputs/
flash/rootVMU unchanged; all processes terminal; no Simulator launched.
Checkpoint **636 identities**, all **1,270** prior and **76** reduction identities
reverified; SHA256 `4fe86508f70168affded02c8ecb28e5da54b64315bc0f12429567ff83c590eab`.
APFS sharing preserves56 completed diagnostic captures/61 compiled objects,
298,955,726 logical bytes, about285MiB recovered. Bytes/paths/permissions/
timestamps/xattrs retained; inode/birthtime changes declared. All1,270 prior
identities and117 selected output hashes verified before/after. No source,
asset, save or input content removed.

**Next:** execute full original `0922E0` selector. R4 values0/2/3/5/6/7 select
base79D9A0+65..70 and store byte1; invalid values make no write. Verify all
branches/BT-S MOV delay/BRA ADD delays/RTS-NOP, R0/R5/R6/T, unchanged stack/
SR-MACL-FPU/RAM69CPU25TMU/P1-P2 destination, modeled cycles11/13/15/17/19/20.
Actual R4=0 predicts byte79D9E1 from0 to1 and return091A8C in11 cycles,
**unexecuted**. Then execute original parent091A8C continuation, four cycles,
to actually unregistered04AE40 (PC091A8C/PR091A92/SPFFFF9C/R3=04AE40).
Keep saved parentPR04E32A/cleared buffers; parent epilogue/finalzero remain
unexecuted. One classification batch used: execute before broader inventory.
Integrate only0922E0/proposed cap840 after isolated checks; retain motionreturn/
exact-binary regression. No host flag writes or forced initialization.

Fresh G1 audit unchanged: runtime0/1,guest0/1474,headers0/177,contracts0/4,
title38/39,FFmpeg10139/10139. PAL descriptor absent; source-only request
unchanged/unsent. G1/full boot/gameplay/save/VMU/mobile/production remain open.
Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-175916: original status writer returns.**

Original `08BCEA` executes all nine instructions: byte4 at756C60, zero16 at756C64
and zero32 at756C78. **4,096 isolated cases** cover nonzero starting values,
independent P1/P2 destinations/stack aliases, IRQ masks and T/FPU banks. The
three original writes, neighboring bytes, unchanged stack, RTS/store32 delay,
R0–R4 clobbers, preserved SR/MACL/FPU and full return04E324 match whole16MiB RAM,
69CPU and25TMU fields after **10 modeled cycles**. The frozen input already
contains4/zero/zero; the original stores still execute. No host state write.
Isolated closure: one function/nine instructions/zero external records.

**Both prospective live entry/return phases match.** The retained original
motion-return contract and exact-binary regressions pass: two supplied scenes,
ten external audits, release controls and six unchanged images. Startup now
reaches missing **`091A20`**, PC `04E324`, PR `04E32A`, SP `FFFFA0`,
SR `40000100`, SPC `667A28`. Mode0/cycles115727660/sectors215/
ARM7instructions2844300/faults0/AICAframes16621/
nonzero0/IRQ36. Full initialization/IRQ, audible game audio,
normal boot and gameplay remain unproven.

Full closure: **836functions/82,742instructions/2,392external records**, cap838,
is inventory. Twenty full/four reduction sources frozen;12patches/private inputs/
flash/rootVMU unchanged; all processes terminal; no Simulator launched.
Checkpoint **1270 identities**, all **569** prior and **76** reduction identities
reverified; SHA256 `97a61841615e05fc6437599ff7d4d1fd75956562409b9394b3466ad08593afce`.
APFS sharing preserves **56 diagnostic captures and677 compiled objects** from
completed runs:802,566,574 logical bytes, about767MiB recovered. Bytes/paths/
permissions/timestamps/xattrs retained; inode/birthtime changes declared.
All569 prior identities and733 selected output hashes verified before/after;
no source, asset, save or input content removed.

**Next:** execute original `091A20` prefix to actually unregistered `0922E0`.
Seven clear loops96/512/344/96/512/344/512 write2416 bytes,2336 unique bytes;
retain their overlaps. Two zero words758C70/79E574, one savedPR stackword/4-byte
frame, DT/BF-S increments/delays, BSR delay/PR091A8C, missing-lookupPC091A80,
SPFFFF9C/T1/R0-R2-R3-R4-R5-R6-R7/SR-MACL-FPU/RAM69CPU25TMU predict9691modeled
cycles, **unexecuted**. Both0922E0 and later04AE40 are absent from both actual
tables; no existing helper is omitted. Full return04E32A/finalzero79E584 remain
unexecuted. One classification batch used: execute before broader inventory.
Integrate only091A20/cap838 after isolated checks; retain motionreturn/regression.

Fresh G1 audit unchanged: runtime0/1,guest0/1474,headers0/177,contracts0/4,
title38/39,FFmpeg10139/10139. PAL descriptor absent; source-only request
unchanged/unsent. G1/full boot/gameplay/save/VMU/mobile/production remain open.
Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-175126: original flag initializer returns.**

Original `08C218` executes its entry word reset, BRA/store delay and the shared
`08C220` initializer with the real `0513F8` unsigned-word getter. **4,096
isolated cases** cover entry and direct shared calls, getter values 0/6/8000/FFFF,
both conditional branches, P1/P2 flag/getter/stack aliases, IRQ masks and T/FPU
banks. Three byte resets, conditional byte3, final zero word, neighboring byte4,
two saved stack words/8-byte frame, JSR/RTS delays, SR/T/MACL/FPU and restored
PR/R14/SP match whole16MiB RAM,69CPU fields and25TMU fields. No helper/body
omission or host flag writes. The isolated closure is3functions/54instructions;
the entry contains the original shared tail and its separate callable body remains.

**Both prospective live entry/return phases match.** Actual getter value0 returns
to `04E31E` after **29 modeled cycles** (value6 takes34 in isolation). The retained
motion-return contract and exact-binary baseline still pass: two supplied scenes,
ten external audits, release controls and six unchanged reference images.
Startup reaches missing **`08BCEA`**, PC `04E31E`, PR `04E324`, SP `FFFFA0`,
SR `40000100`, SPC `667A28`. Mode0/cycles115727646/sectors215/ARM7instructions2844299/
faults0/AICAframes16621/nonzero0/IRQ36.
Full initialization/IRQ, audible game audio, normal boot and gameplay remain open.

Closure **835functions/82,733instructions/2,396external records**, cap836, is
inventory. Twenty full and four reduction sources frozen;12patches/private
inputs/flash/rootVMU unchanged; all processes terminal; no Simulator launched.
Checkpoint **569 identities**, all **600** prior and **72** reduction identities
reverified; SHA256 `da4f3ca2ee4b9d909ef20ff331b5ee07fc7eb80d6b2dd5835bf8f867eec3a0df`. APFS sharing preserves112 byte-identical completed
172614/173930 diagnostic captures (498,136,892 logical bytes), recovering about
474MiB; content/paths/permissions/timestamps/xattrs preserved, inode/birthtime
changes declared. All600 prior identities reverified before/after; no source,
asset, save or input content removed.

**Next:** execute full original `08BCEA`: nine instructions, byte4 at756C60,
zero16 at756C64 and zero32 at756C78, three original writes, unchanged neighbors/
stack, RTS/store delay, R0–R4 clobbers, SR/MACL/FPU and actual return04E324.
Source predicts10modeled cycles; unexecuted. There are no callees; target is absent
from both actual registration tables. One classification batch used: execute
before broader inventory. Retain motion-return/exact-binary regression; cap838
is a proposed inventory bound only. No host writes or forced initialization.

Fresh G1 audit remains runtime0/1,guest0/1474,headers0/177,contracts0/4,title38/39,
FFmpeg10139/10139; supported PAL descriptor absent. Exact source-only acquisition
request unchanged/unsent. G1/full boot/gameplay/save/VMU/mobile/production remain
open. Goal turn: **progress**; preceding status-only turn was **no progress**.

**Previous full-startup checkpoint — 20261006-173930: original table initializer returns to its caller.**

Original `026BE0` now executes both real `10EF3C` helper calls, clearing
13,312 and 384 overlapping bytes, enabling byte1 and writing 26 record bytes8
at a 512-byte stride. **2,048 isolated cases** verify both traced helper calls,
guards, three stack words/12-byte frame, JSR delays, saved PR/R14/SP, SR/MACL,
FPU state and P1/P2 buffer/status/stack aliases. Whole 16 MiB RAM, 69 CPU fields
and 25 timer fields match after **68,608 modeled cycles**. The original parent
`026E80` epilogue also matches after nine cycles: final FF byte/zero word and
saved PR/R13/R14/SP restore, returning to `04E318`.

**All three prospective live phases match** (child entry/return and parent
return). The original table initializer now returns to its actual caller.
The retained motion-return contract still passes. Startup reaches missing
**`08C218`**, PC `04E318`, PR `04E31E`, SP `FFFFA0`, SR `40000101`, SPC
`667A28`. Mode0/cycles115727613/sectors215/ARM7instructions2844295/faults0/
AICAframes16621/nonzero0/IRQ36. Full initialization/IRQ, audible game audio,
normal boot and gameplay remain unproven.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Closure **834 functions / 82,706 instructions /
2,397 external records**, cap836, is inventory. Twenty full sources frozen.
Initial reduction compilation failed on a mixed-type `auto` declaration; only
that declaration was split after terminal, and a fresh build passed. Failed
source/report and four sources per attempt preserved; no guest/core/input fix.
Checkpoint **600 identities**, all **559** prior and **95** reduction identities
reverified; SHA256 `b539669d906c370131236de4c310d2df1efc27129a1f14c2fb2a913e37634a2c`. Twelve patches/private
inputs/flash/root VMU unchanged; all processes terminal; no Simulator launched.
Prior APFS sharing of112 completed diagnostic captures remains byte-verified;
no source/asset/save/input content removed.

**Next:** original `08C218` resets a word and branches into already registered
`08C220`; that shared initializer calls already registered `0513F8`. Actual
provider/callback bindings are recorded; neither existing body may be replaced
or relabeled missing. Source review27 instructions includes the entry and shared
body. Verify the real helper and R0==6 branch/full return, initial word/three
byte resets, BRA and JSR delay slots, saved R14/PR/8-byte frame, full RAM/CPU/
timers and actual caller. Source16-cycle prefix is unexecuted. One classification
batch used; execute before broader inventory. Retain motion-return regression.

Fresh G1 audit/source-only request unchanged: runtime0/1,guest0/1474,headers0/177,
contracts0/4,title38/39,FFmpeg10139/10139; supported PAL descriptor absent.
G1/full boot/gameplay/save/VMU/mobile/production remain open. Goal: **progress**.

**Previous full-startup checkpoint — 20261006-172614: original table initialization reaches its child.**

Original `026E80` now runs its prefix. **2,048 isolated cases** and two
prospective live phases match whole 16 MiB RAM, 69 CPU fields and 25 timer
fields after 101 modeled emitter cycles. Ten records receive byteFF/float1,
ten byte entries receive FF, two status bytes and one word become zero.
The 36 writes, three saved stack words/12-byte frame, active FR4/inactive FPU
bank, SR/MACL, P1/P2 aliases, IRQ/T/FPU variants and BSR delay/PR match.
Actual source/literals/caller are preserved; no host initialization substitutes
for original instructions. Original-mode FPSCR.PR/SZ=0 is the declared scope.

The retained live motion-return contract still passes. Startup stops at missing
**`026BE0`**, PC `026EC0`, PR `026ED0`, SP `FFFF94`, SR `40000101`, SPC
`667A28`. Mode0/cycles115658992/sectors215/ARM7instructions2836548/faults0/
AICAframes16606/nonzero0/IRQ36. Complete initializer/IRQ, audible game audio,
normal boot and gameplay remain unproven.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Closure **833 functions / 82,669 instructions /
2,398 external records**, cap834, is inventory. Twenty full/four reduction
sources frozen;12 patches/private inputs/flash/root VMU unchanged. Checkpoint
**559 identities**, all **417** prior and **62** reduction identities reverified;
SHA256 `7c3242c085a0fd36782d8ff3b583784c081b6bd5549e08ccc5dab0d7143dcfd2`. All processes terminal; no Simulator launched.

APFS sharing preserves 112 byte-identical captures from completed171357/171851
baselines (498,136,892 logical bytes); files, content, permissions, timestamps
and xattrs preserved, inode/birthtime changes declared. No source/asset/save/input
content removed. Initial planner lacked its parent directory; no files changed
before that failure, corrected prerequisite and reports preserved. Initial next
preflight assumed a cached helper path existed in an older checkpoint; fresh
pinned-tool review verifies the helper against actual BOOT. A guessed rejection
explanation was corrected with evidence; draft preserved. No guest/input change
or execution rerun followed either metadata error.

**Next:** execute full original `026BE0` with its already registered `10EF3C`
byte-clear helper: original clears of 13,312 and 384 overlapping bytes, enable
byte1,26 record bytes8 at512-byte stride, saved stack/PR/R14/JSR delays, RAM/CPU/
timers and source-derived68,608 cycles. Then verify parent `026E80` epilogue,
its final FF byte/zero word and saved PR/R13/R14/SP restoration. Both actual
helper registrations are confirmed; no omitted-helper barrier or clear stub.
Two classification batches used; execute next and retain motion-return regression.

Fresh G1 inventory/source-only request unchanged: runtime0/1,guest0/1474,
headers0/177,contracts0/4,title38/39,FFmpeg10139/10139; supported PAL absent.
G1/full boot/gameplay/save/VMU/mobile/production remain open. Goal: **progress**.

**Previous full-startup checkpoint — 20261006-171851: original byte write and return verified.**

Original `08BD1C` now executes its four instructions and returns normally.
**2,048 isolated cases** cover all256 prior byte values, P1/P2 destination
aliases, IRQ masks and T/FPU banks. The literal read, exactly one byte4,
neighbor preservation, R0 destination/R3=4, SR/MACL, saved registers, unchanged
stack and RTS delay/PR match whole16MiB RAM/69CPU/25TMU after five modeled
emitter cycles. The frozen actual literal/destination byte0 are preserved before
execution; no host write substitutes for original code. Both prospective live
entry/return phases match; actual `8C756C60` changes **0→4**.

The original motion-return contract from171357 still passes:105sectors/
215040encoded/488608decoded/guards/saved registers/PC-PR/result/cache shadow.
Startup now reaches missing **`026E80`**, at PC `0513E6`, PR `04E318`,
SP `FFFFA0`, SR `40000100`, SPC `667A28`. Mode0/cycles115658891/sectors215/
ARM7instructions2836537/faults0/AICAframes16606/nonzero0/IRQ36. Full initializer,
IRQ completion, audible game audio, normal boot and gameplay remain unproven.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged reference images. Closure **832 functions / 82,621
instructions / 2,398 external records**, cap834, is inventory. Twenty full
sources/four reduction sources frozen;12patches/private inputs/flash/root VMU
unchanged. Checkpoint **417 identities**, all **426** prior and **62** reduction
identities reverified; SHA256 `991a3a93bcf0cde5706fadc9ec896eb6256afa92f61a1d2428c9272d7978b077`. All processes
terminal; no Simulator launched. No content removal or storage compaction.

**Next:** reduce original `026E80` prefix to actually unregistered `026BE0`.
Its48-instruction source initializes ten12-byte records (byteFF/float1 at+4),
tenFF bytes, two zero status bytes and one zero word before the child, with
three saved stack words/12-byte frame. Derive and execute exact FPU-bank/branch/
SR-MACL/RAM/CPU/timer effects; do not inject table/status success. Actual literal
values and provider/callback registration are recorded in `next-startup-preflight.json`.
One classification batch used; execute before broader inventory. Keep the live
motion return as a regression requirement.

Fresh G1 audit remains runtime0/1, guest0/1474, headers0/177, contracts0/4,
title38/39, FFmpeg10139/10139. Supported PAL descriptor absent. Source-only
acquisition request unchanged/unsent; G1, full boot/gameplay/save/VMU/mobile/
production adoption remain open. Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-171357: request packing advances startup and the original motion loader returns.**

Original `664258` now calls the existing `663550` provider. Its **8,192 isolated
cases** verify modulo-32 shifts/OR, two request words, six writes in a 24-byte
frame, parameter pointers, zero stack arguments, handle/command forwarding,
BSR delay/PR, SR/MACL, P1/P2 aliases and IRQ/T/FPU variants. Whole 16 MiB RAM,
69 CPU fields and 25 timer fields match after 20 modeled emitter cycles.
Both prospective live phases (parent and real child entry) match. Actual request
words are 2/FF and lock80/retry10/cancel1 remain unchanged at child entry.
The existing provider and parent return captures are observed; their complete
return effects and lock release were not prospectively modeled.

**The retained original motion-return contract now passes in live startup:**
105 sectors / 215,040 encoded bytes / 488,608 decoded bytes, guards, saved
registers, return PC/PR/result and cache shadow match. The prior partial decoded
mismatch is gone. Startup continues to missing **`08BD1C`**, at PC `0513E0`,
PR `0513E6`, SP `FFFF90`, SR `40000100`, SPC `667A28`. Mode0,
cycles115658879/sectors215/ARM7instructions2836535/faults0/AICAframes16606/
nonzero0/IRQ36. No audible game audio, complete initializer/IRQ, boot or gameplay
acceptance follows from those counters.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged reference images. Closure **831 functions / 82,617
instructions / 2,399 external records**, cap832, is inventory. Twenty full
sources and four reduction sources are frozen; twelve patches, original inputs,
flash and root VMU are unchanged. Checkpoint **426 identities**, all **441** prior
and **62** reduction identities reverified; SHA256 `5d10fe6371c21b738ddcefc080354fa61d87662661a053a45cccd75266fe1c23`.
All processes terminal; no Simulator launched. No content removed or compacted.

**Next:** reduce original `08BD1C`, a four-instruction byte writer. The actual
literal points to `8C756C60` (old byte0); the original routine predicts byte4,
R0=destination/R3=4, return to `0513E6`, five modeled cycles. Verify the entire
RAM/CPU/timer state and neighbors before integrating only this routine with
measured inventory cap834. One classification batch used; execute before more
inventory. Do not replace its write with host state injection.

Fresh G1 audit remains runtime0/1, guest0/1474, headers0/177, contracts0/4,
title38/39, FFmpeg10139/10139; PAL descriptor absent, verifier exit2.
Source-only acquisition request unchanged/unsent. Original G1, full boot,
gameplay, VMU persistence, save, mobile and production adoption remain open.
Goal turn: **progress**. The preceding status-only reply was no progress;
this turn revalidated the state and executed the next authorized experiment.

**Previous full-startup checkpoint — 20261006-170200: original lock acquisition reaches its child.**
Original `664226` now executes TAS.B and its acquired-child prefix in live
startup. **16,384 isolated cases** verify lock bytes0/1/80/FF, preservation of
other bits/neighboring bytes, P1/P2 lock and stack aliases, argument extremes,
IRQ masks, T and FPU banks. The 4,096 acquired and12,288 busy prefixes match
whole16MiB RAM/69CPU/25TMU/savedPR/two stackwords/8byteframe/call delays after
10 modeled emittercycles. Busy `663088` is registered in full; its isolated
entry barrier does not prove a busy return or a missing provider.

Both prospective live entry/rejection phases match. The unchanged actual
lock at `8C664538` changes **00→80**, T=1, and reaches missing **`664258`** with
handle1/command2/R6–R7zero/fifthFF/retry10/cancel1 preserved. PC `664240`, PR
`664248`, SP `899730`, SR `60000161`, SPC PRS `04E288`. Lock release, command/
callback/handler/initializer and IRQ completion/live motion return remain unmet;
decoded partial mismatch389633 unchanged. Mode0/cycles111889848/sectors194/
ARM7instructions2411037/faults0/AICAframes15775/nonzero0/IRQ33. No audio or
VMU persistence acceptance.

The first oracle build failed on an assertion variable outside its loop;
only that assertion was corrected in a fresh attempt. The corrected oracle
executed successfully, but its runner then rejected the correct PASS banner
because it expected the previous routine's banner. Both raw failed reports and
source versions remain intact. Separate `execution-validation.json` verifies
execute exit0, actual assertions/result, input/source/binary identities and
saved predictions; the successful execution was not repeated.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **830 functions / 82,594
instructions / 2,399 external records**, cap832, is inventory. Twenty full
sources/four sources per reduction attempt frozen; twelve patches/private
inputs/flash/rootVMU unchanged. Checkpoint **441 identities**, all **411** prior
and **91** reduction identities reverified; SHA256 `4a0cdd3ba25628a74b356e761796c44e0f64b128e7fdaa0745557245abcab12e`.
All processes terminal/zero booted Simulator metadata/none launched. No content
removal/storage compaction; remeasure capacity before the next build.

**Next:** reduce original `664258` request-packing prefix to the call entry of
**already registered `663550`**. Verify modulo32 R6<<24|R7<<16|fifthword, the two
request words, six writes/24byteframe/R14 and R6 parameter pointer/two zero stack
arguments/R4handle/R5=11 in BSR delay/R7=2/SR-MACL/RAM/CPU/timers/frozen caller.
Actual source predicts words2/FF and retains lock80;20cycles unexecuted. An
initial preflight assumption that663550 was missing was rejected by the actual
provider binding before any file/execution change. An isolated child-entry
barrier must not relabel this provider missing; full integration invokes its
real existing body. Compare parent/child entry prospectively, then measure the
actual continuation without assuming a return or next failure. One classification
batch used: execute before more inventory. No unlock/readiness/child stub.

Fresh G1 audit: runtime0/1, guest0/1474, headers0/177, contracts0/4, title38/39,
FFmpeg10139/10139; supported PAL descriptor absent/verifierexit2. **Original G1
unresolved**; exact source-only acquisition request current/unsent. Boot,
gameplay, save, mobile and production adoption remain open. Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-165051: original command wrapper reaches its child.**
Original `64A708` now executes its first-child prefix in live startup. The
isolated **8,192 cases** verify R4–R7/fifth argument extremes, P1/P2 stack,
IRQ masks, T and FPU banks, savedPR/12byte frame/two writes, fifth argument
load/forward, JSR/NOP delay and PR, whole16MiB RAM/69CPU/25TMU after eight
modeled emittercycles. SR/MACL preserve. The unchanged actual caller keeps
handle1/command2/R6–R7zero/fifthFF/retry10/cancel1.

Both prospective live phases (wrapper entry/firstchild rejection) match RAM,
CPU and timers. First missing child **`664226`**, PC `64A708`, PR `64A716`, SP
`899738`, SR `60000161`, SPC PRS `04E288`. Wrapper/command/callback/handler,
initializer and IRQ completion/live motion return remain unmet; decoded
partial mismatch389633 unchanged. Mode0/cycles111889838/sectors194/
ARM7instructions2411036/faults0/AICAframes15775/nonzero0/IRQ33. No audio or
VMU persistence acceptance. Later status helper `64A394` is already registered
but is not reached before this missing child; no helper stub was added.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **829 functions / 82,569
instructions / 2,398 external records**, cap830, is inventory. Twenty full
sources/four reduction sources frozen; twelve patches/private inputs/flash/
rootVMU unchanged. Checkpoint **411 identities**, all **432** prior and **62**
reduction identities reverified; SHA256 `7361bf25402d404d73124eaf604d3716679cac0702bc4ce3067467acb3c3ce96`.
All processes terminal/zero booted Simulator metadata/none launched. No content
removal or storage compaction. Remeasure capacity before the next build.

**Next:** reduce original `664226` lock/first-child prefix. Actual lock pointer
`8C664538` holds byte0: source TAS.B predicts byte80/T1 and acquired child
`664258`, which is unregistered. Preserve the actual lock, neighboring bytes,
PR/two stack words/fifthargument/R4–R7/call delays/wholeRAM/CPU/timers; do not
force a free lock. Busy child `663088` is already registered in the provider,
not a missing dependency. A draft next-action sentence mislabeled it despite
the correct binding table; draft preserved and corrected before checkpoint.
Pinned emitter-derived10cycles remain unexecuted. One classification batch used:
execute before more inventory. No lock-wrapper/command/IRQ completion claim.

Fresh G1 audit: runtime0/1, guest0/1474, headers0/177, contracts0/4, title38/39,
FFmpeg10139/10139; supported PAL descriptor absent/verifierexit2. **Original G1
unresolved**, exact source-only acquisition request current/unsent. Normal boot,
gameplay, save, mobile and production adoption remain open. Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-164338: original callback reaches the command wrapper.**
Original `6647BE` now runs its real readiness child `664B2C` and accessors.
The reduction passes **14,336 readiness cases** (3,584 ready) and **21,504
callback cases**: 14,336 original no-work returns and 7,168 first-command
rejections. Descriptor presence, error/status-bit2, signed record counts,
wrapped selectors, handles0/23, argument extremes, P1/P2 pointer/stack aliases,
IRQ masks, T and FPU banks are covered. Whole16MiB RAM/69CPU/25TMU match.

The unchanged real caller maps channel0 tohandle1; present1/error0/status0xE
produces readiness1, then rejects missing **`64A708`**. Both prospective live
phases match RAM/CPU/timers after **183 modeled emittercycles**, including
nested stack writes and the fifth argument. Command R4=1/R5=2/R6=R7=0,
fifth stack argumentFF; retry10/cancel1 unchanged. PC `66483C`, PR `66484C`,
SP `899744`, SR `60000161`, SPC PRS `04E288`. Actual callback/handler/initializer,
IRQ completion and live motion return remain unmet; decoded partial mismatch
389633 unchanged. Mode0/cycles111889830/sectors194/ARM7instructions2411035/
faults0/AICAframes15775/nonzero0/IRQ33. No audio or VMU persistence acceptance.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **828 functions / 82,547
instructions / 2,397 external records**, cap830, is inventory. Twenty full
sources/four sources per reduction attempt are frozen. Initial metadata hash
validation failed before generation/execution; source files were unchanged.
The failed attempt is preserved, corrected metadata passed in a fresh directory.
Checkpoint **432 identities**, all **419** prior and **72** reduction identities
reverified; SHA256 `5ab9178cd3c5c93f9dc204c5ff40dc06034ad4a50a331472f995c114fbe4e014`.
Twelve patches/private inputs/flash/root VMU unchanged; all processes terminal,
zero booted Simulator metadata and none launched. No content removal/compaction.
Remeasure capacity before the next build.

**Next:** reduce original `64A708` prefix through first unregistered child
`664226` (actual literal64A808 verified). Preserve savedPR, 12byte frame/two
writes, fifth argument load/forward, R4–R7, JSR/NOP delay and PR, wholeRAM/
69CPU/25TMU/frozen caller. Source-derived8cycles are unexecuted. Later status
helper `64A394` is already registered but not reached before firstchild; do not
relabel it missing or substitute success. One classification batch used: execute
before broadening inventory. No wrapper/command/callback/handler/IRQ completion.

Fresh G1 audit: runtime0/1, guest0/1474, headers0/177, contracts0/4, title38/39,
FFmpeg10139/10139; supported PAL descriptor absent/verifierexit2. **Original G1
unresolved**; exact source-only acquisition request current/unsent. Normal boot,
gameplay, save, mobile and production adoption remain open. Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-163159: original channel handler reaches its first callback.**
Original `64B350` prefix now executes in live startup. Both prospective phases
(handler entry and first callback rejection) match whole 16 MiB RAM, all 69 CPU
fields and 25 timer fields. The 44-byte frame, eleven stack writes, argument
reloads, modulo32 multiplication, retry-word read and original JSR delay/PR
match after29 modeled emittercycles. Actual retry10/cancel1 are preserved.
The earlier isolated reduction passes **10,752 cases**.

First missing callback is **`6647BE`**, at diagnostic blockPC `64B37E`, PR
`64B386`, SP `899774`, SR `60000161`, SPC PRS `04E288`. Handler/initializer,
IRQ completion and live motion return remain unmet; partial decoded mismatch
389633 unchanged. Mode0/cycles111889647/sectors194/ARM7instructions2411014/
faults0/AICAframes15775/nonzero0/IRQ33. No audio acceptance.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged reference images. Closure **827 functions /
82,417 instructions / 2,390 external records**, cap828, is inventory. Twenty
full sources and four reduction sources are frozen. Twelve patches, private
inputs, flash and root VMU retain their identities. Checkpoint **419 identities**,
all **410** prior and **62** reduction identities reverified; SHA256
`c05641ce13557f8fd86673dad7a338276fe64e5954b1acc6214066ff1ce0f237`. All processes terminal, zero booted Simulator
metadata; none launched. No storage compaction or content removal this turn.

**Next:** reduce original `6647BE` with its already registered readiness child
`664B2C` and original accessors. Captured channel0 maps to handle1; descriptor
presentbyte1/errorbyte0/statusword0xE predicts readiness1. Then source predicts
unregistered `64A708` at literal664934, with a fifth stack argument and original
record-count loop. Verify that prediction in execution; preserve the real
readiness, retry and cancel state. Later `66453C` is absent but unreachable
before that command barrier; `64B91E` remains registered. One classification
batch used: execute before broadening inventory. Full callback integration will
need a measured closure cap increase from828 after the reduction passes.

Fresh G1 audit: runtime0/1, guest0/1474, headers0/177, contracts0/4, title38/39,
FFmpeg10139/10139. Supported PAL descriptor absent/verifierexit2. **Original G1
unresolved**; source-only acquisition request current/unsent. Normal boot,
gameplay, VMU persistence, save, mobile and production adoption remain open.
Goal turn: **progress**.

**Preceding isolated result — original `64B350` handler prefix passed; full integration pending.**
The original handler passes **10,752 cases** through its first unregistered
callback `6647BE`. Whole 16 MiB RAM, 69 CPU fields and 25 timer fields match,
including its 44-byte stack frame, eleven stack writes, argument reloads,
modulo32 multiplication, retry-word read and original JSR delay/return address
after 29 modeled emitter cycles. The frozen real caller preserves retry10 and
cancel1. Later notifier `64B91E` is already registered in the full program;
execution has not reached it. No callback success or readiness was substituted.

Evidence: `artifacts/recovery-aot/20261006-channel-handler-reduction/`.
Reduction checkpoint binds **62 identities**, reverifies all **410** preceding
full-run identities, SHA256
`548b5d95bb80495ccf2def545cf4473113b8b2887be2efcaad343a22a8fdec24`.
Sources are frozen; all test processes terminal and zero booted Simulator
metadata. The latest full run remains **161749**, described below.

Next: integrate only original `64B350`, compare prospective handler-entry and
first-callback-rejection RAM/CPU/timers, then run the same binary's scene
regressions. **Normal boot, gameplay and original G1 remain unmet.** Missing
Katana inputs and the supported PAL dump are unchanged from the last source
audit; this isolated result does not constitute a fresh source audit.

**Previous full-startup checkpoint — 20261006-161749: original channel wrapper reaches its tail handler.**
Original `64EB72` now executes all twelve instructions. **7,168 isolated cases**
verify modulo32 index/MUL.L arithmetic, record28/32 and descriptor+8 reads,
argument extremes, P1/P2 descriptor/stack aliases, IRQ masks, T and FPU banks.
Whole16MiB RAM/one savedstackword/69CPU/25TMU match after14emittercycles. Original
JMP delay restores R14/SP; PR and SR preserve. Actual channel0 givesR5=FF,
R6=8C6B4334 and rejects unregistered `64B350`; no tail handler was stubbed.

Two prospective live wrapper-entry/rejected-handler phases match wholeRAM/CPU/
timers. Final diagnostic basic-block PC `64EB72` (JMP instruction64EB86),
PR `64EF5E`, SP `8997A0`, SR `60000161`, SPC PRS `04E288`. **Handler/initializer
completion, IRQ completion and live motion return remain unmet**; partial decoded
mismatch389633 unchanged. Mode0/sectors194/ARM7instructions2411011/faults0/
AICAframes15775/nonzero0/IRQ33; no sound-playback acceptance.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **826 functions / 82,327
instructions / 2,390 external records**, cap828, is inventory. Twenty full
sources/four reduction sources frozen; twelve patches/owner inputs/flash/root
VMU unchanged. Checkpoint **410 identities**, all **438** prior and **62** reduction
identities reverified; SHA256 `cf6fbab6c0073be268539c4841acaa78fb9b8f7b6746fea8d2a3f8fc2045bd11`.
All processes terminal; zero booted Simulator metadata, none launched. No storage
compaction/content removal this turn. Free **6.24 GiB**; remeasure before building.
SunPad/SARecomp/PaperPad pins and disabled push URLs reverified.

Next reduce original `64B350` handler prefix through first actually missing
callback `6647BE`, loaded from literal64B418 (actual target verified). Preserve
44byte frame/eight pushes/three local stack words, R5/R6 argument reload,
MACL/index/retry-word read/cancel-address load/signed literals/JSR delay and PR,
wholeRAM/69CPU/25TMU/frozen caller. Actual retryword10/cancelword1; do not force
retry counts or readiness. Source29cycles remain unexecuted. Later notifier
`64B91E` is already registered and unreachable before this first missing callback;
it must not be relabeled missing. One classification batch used: execute next.

Fresh G1 audit: runtime0/1, guest0/1474, headers0/177, contracts0/4, title38/39,
FFmpeg10139/10139; supported PAL descriptor absent/verifierexit2. **Original G1
unresolved**; exact source-only acquisition request current/unsent. Full boot,
gameplay, VMU persistence, save, mobile and production adoption remain open.
Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-160823: original channel initializer prefix executes.**
Original `64EF30` now clears channel status and follows its real zero-flag branch
to unregistered `64EB72`. The isolated prefix passes **2,048 cases** (512 zero /
1,536 present flags): four channels, signed-byte extremes, P1/P2 stack aliases,
IRQ masks, T and FPU banks. Whole 16 MiB RAM, five saved stack words, all 69 CPU
and 25 timer fields match. Both first-child paths reject honestly:64EB72/64E1F2.
The initial independent24-cycle timer model was rejected; pinned emitter source
charges MUL.L2, so corrected25cycles pass. Rejected sources/logs remain intact;
no game/runtime/input change was made to accommodate the test.

Two prospective live phases match: channel-init entry and first missing-child
rejection, including whole RAM/CPU/timers. Final PC `64EF5A`, PR `64EF5E`,
SP `8997A0`, SR `60000161`, SPC PRS `04E288`. **Initializer completion, IRQ
completion and live motion return remain unmet**; partial decoded mismatch389633
is unchanged. Mode0/sectors194/ARM7instructions2411009/faults0/AICAframes15775/
nonzero0/IRQ33. No sound-playback acceptance.

Exact-binary regression passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **825 functions / 82,315
instructions / 2,390 external records**, cap828, is inventory. Twenty full source
versions and four sources per reduction attempt frozen; twelve patches/owner
inputs/flash/root VMU unchanged. Checkpoint **438 identities**, all **859** prior
and **92** reduction identities reverified; SHA256 `3b44afc3af1544a2d738458e4f0ab324645298c678259b4509f40213cce9b9db`.
All processes terminal; zero booted Simulator metadata, none launched. No storage
compaction or content removal this turn. Free **7.05 GiB**; remeasure before building.

Next reduce original `64EB72`, a twelve-instruction channel wrapper: source reads
record offsets28/32 then descriptor+8, restores R14/SP in the original JMP delay
and tail-jumps to unregistered `64B350` without changing PR. Actual channel0:
descriptor8C8A5230, derivedR5=FF/R6=8C6B4334. Source-derived14cycles are unexecuted.
Preserve one saved stack word/MACL/index/wholeRAM/69CPU/25TMU; test pointer/stack
aliases and the actual frozen caller before integration. One classification
batch used; execute before broadening inventory. No child stub or ready flags.

Fresh source audit remains runtime0/1, guest0/1474, headers0/177, contracts0/4,
title38/39, FFmpeg10139/10139; supported PAL descriptor absent. **Original G1
unresolved**; exact source-only acquisition request remains current and unsent.
Full boot, gameplay, VMU persistence, save, mobile and production adoption remain
open. Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-155608: original VMU decoder returns.**
Original `64A8FC` now executes with its registered `64A882` lookup child. The
isolated reduction passes **3,584 cases** (2,048 success / 1,536 null), including
function masks, packed-byte extremes, P1/P2 aliases, IRQ masks, T and FPU banks.
It verifies five halfwords, four saved stack words, whole 16 MiB RAM, all 69 CPU
and 25 timer fields. The actual frozen caller returns0 / fields1,512,4,1,0 after
281 modeled emitter cycles. These are VMU metadata fields, not save acceptance.

Two prospective live VMU entry/return phases match whole RAM/CPU/timers. Startup
advances to unregistered `64EF30` at PC `64C07A`, PR `64B748`, SP `8997B4`,
SR `60000161`; SPC remains PRS `04E288`. **IRQ completion and live motion return
remain unmet**; partial decoded mismatch389633 is unchanged. Channel R4=0 has
three initializer flags0/0/0; source says the first child will be `64EB72`, also
unregistered. `64E1F2`, `64EBA2` and `64E6E8` are absent too. One classification
batch used. Next reduce this original channel-initializer prefix and its actual
caller, preserving status clear, stack, MACL/index arithmetic and both flag paths;
never force channel-ready flags or substitute a child success.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. **824 functions / 82,249 instructions /
2,388 external records**, cap828, is inventory. Twenty full-source versions and
four reduction sources frozen; twelve patches/private inputs/flash/root VMU
unchanged. Checkpoint **859 identities**, all **1,017** prior and **62** reduction
identities reverified; SHA256 `186bdb52b300444469eaf832a3fac6fafced37fe6021036eec1e329d1861997d`.

Fresh APFS sharing preserves63 prior compiled outputs,56 prior baseline captures,
11 prior startup captures,62 now-terminal compiled outputs and three generated
resident-image C++ copies. Full-pass available-byte changes are separately
recorded from first-pass measurements. The last category extends sharing to
byte-identical generated outputs; original disc/BOOT/runtime sources are not
selected. Contents, paths, permissions, timestamps and xattrs retain; inode and
birthtime changes declared. No input/source/asset/save/log content removed.
All processes terminal; zero booted Simulator metadata; none launched. Free
**9.18 GiB** at final verification (checkpoint had0.12GiB; later change
confirmed twice, origin undetermined): remeasure before building. Mode0/sectors194/ARM7instructions2411006/
faults0/AICAframes15775/nonzero0/IRQ33; no audio-playback acceptance.

Fresh source audit and absent supported PAL descriptor still leave original
**G1 unresolved**. Exact source-only acquisition request remains current and
unsent. Full boot, gameplay, VMU persistence, save, mobile and production adoption
remain open. Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-153926: original IRQ halfword accessor returns.**
Original `664556` is now integrated. Three prospective phases match whole
16 MiB RAM/stack, all69 CPU and25 timer fields: motion entry, IRQ helper entry
and return. The helper leaves RAM unchanged and returns255 after five modeled
emitter cycles. The original scheduler calls it ten times, then rejects missing
`64A8FC` at PC `664760`, PR `66476A`, SP `899744`, SR `60000161`. Interrupt SPC
still points to PRS `04E288`. **Interrupt completion and live motion return
remain unmet.** First decoded mismatch remains389633; no complete decode claim.

The new dependency decodes a VMU function word through already registered
`64A882`. Captured device bytes identify Visual Memory, function mask0E and
selected function02. Source-derived expected five halfwords are1/512/4/1/0;
they have not been executed or verified yet. No controller-property or VMU
persistence acceptance follows. The next reduction must preserve both original
null and success branches, stack writes and actual frozen IRQ caller.

Exact-binary regression passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **823 functions / 82,199
instructions / 2,389 external records**, cap824, is inventory. Twenty source
versions frozen; twelve patches/private inputs/flash/root VMU unchanged.
Checkpoint **1,017 identities**, all **613** prior identities reverified;
SHA256 `63b384a200b8f1120a8366e920feaa1c1e69854203ab9ccd6e4eb26f650a672f`. Two classification batches consumed: execution
is next, before more inventory. An unverified draft's controller wording is
preserved and corrected to the actual VMU descriptor.

Exact APFS sharing preserves172 completed provider objects/archives,56 completed
diagnostic captures and143 completed reduced-test objects/archives. Full-pass
available changes142802944/234352640/264527872 bytes; first-pass measurements
are separately recorded. Contents, paths, permissions, timestamps and xattrs
retain; inode/birthtime changes declared. No sources/inputs/assets/saves/logs
or predictions removed, no live build selected. All processes terminal, zero
booted Simulator metadata; no Simulator launched. Free **0.20 GiB**: remeasure
before another build. Mode0/sectors194/ARM7instructions2410934/faults0/AICAframes
15774/nonzero0/IRQ33. No audio-playback acceptance.

Fresh G1 inventory and absent supported PAL descriptor are unchanged; exact
source-only request remains current. Next reduce original `64A8FC` with real
registered `64A882`, including VMU-word success/null cases, halfword packing,
wholeRAM/stack/69CPU/25TMU and the frozen IRQ caller. Original G1, full boot,
gameplay, save, mobile and production adoption remain unresolved.
Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-152405: language prefix and live motion read.**
Original `051354` now executes. Its language/resource prefix matches two
prospective phases: whole 16 MiB RAM/stack, 69 CPU fields and 25 timer fields,
with 66 additional modeled emitter cycles. Original `09F5A0`/`09F664` reads all
**105 sectors / 215,040 encoded sector bytes**, verified against the owner-disc
reference. During PRS decoding at `04E288`, a real interrupt enters the original
scheduler and rejects missing `664556`. Final PC `6646B8`, PR `6646C4`, SP
`899744`, SR `60000161`, SPC `04E288`. **The live motion-load return is unmet.**
First decoded mismatch is byte389633; this is a partial buffer observation,
not a completed decode or boot. The transferred-state motion pass remains
separate. No resource-callback or initialization completion is claimed.

Original `664556` is a four-instruction halfword accessor. Its reduction passes
**2,048 cases**, including P1/P2 aliases, offset/wrap arithmetic, signed MOV.W
into R1 and unsigned EXTU.W in the original return delay slot. Whole RAM stays
unchanged; all 69 CPU and 25 timer fields match, five modeled emitter cycles.
The actual frozen IRQ caller resumes once and returns **255**. The helper is
not yet integrated into full startup; interrupt completion remains unproven.

The exact full-run binary passes two supplied scenes, ten external audits,
release controls and six unchanged images. Closure **822 functions / 82,195
instructions / 2,389 external records**, cap824, is inventory. Twelve patches,
private originals, flash and root VMU unchanged. Twenty startup/four reduction
source versions frozen. Checkpoint **613 identities**, all **204** prior identities
reverified; SHA256 `63599ef336866b8765f396213254b266325bc7dada01774015ff2073fc2e9258`. Full motion-return checker and unmet
explanation preserved; separate partial audit leaves the return contract open.

APFS sharing preserves112 completed diagnostic captures in144501/150748,
498136892 logical bytes; full-pass available-space change481906688 bytes,
first-pass16252928 separately recorded. Hashes, paths, permissions, timestamps
and xattrs retain; inode/birthtime changes declared. No input/source/asset/save
content or live output removed. All processes terminal, zero booted Simulator
metadata; no Simulator launched. Free **0.16 GiB**: remeasure before building.
Mode0,194 sectors,2,410,916 ARM7 instructions/zero faults,15,774 AICA frames/
zero nonzero frames,33 IRQs. No audio-playback acceptance.

G1 source inventory/PAL descriptor/source-only request remain unchanged and
unresolved. Next capacity check, then add only verified original `664556`
(823/cap824). Predict its whole-RAM/CPU/timer entry and return from the captured
IRQ caller, then run one changed full startup to check interrupt return and
motion-decode resumption. Normal boot, gameplay, saves, mobile and production
adoption remain unverified. Goal turn: **progress**.

**Latest recovery measurement — original motion loader, 20261006-motion-loader-reduction-repair3.**
Original registered `09F5A0`/`09F664` reads **105 sectors / 215,040 sector bytes**
and decodes **488,608 bytes** matching independent owner-CDI extraction and PRS
decoding. The direct call uses the independently predicted and validated
language/resource-prefix RAM and CPU state. Original buffer bounds, R8–R15,
PR, return PC/result and cache shadow pass. Existing public BIOS HLE is declared
and initialized exactly as in the full provider, before restoring transferred
RAM/CPU. No game data or successful helper return is injected.

This is **one direct transferred-state test**. Live audio, clock and interrupt
opaque state were not transferred. Whole RAM and all CPU/timer changes after
loading were captured but not independently predicted. The full startup still
stops at missing `051354` in checkpoint150748; complete initialization and normal
boot remain unverified. The supplied-scene motion result is distinct evidence.

Three fixture corrections are preserved: a mixed-width C++ declaration, the
separate timer input file, and missing fixture BIOS-service registration. None
changes runtime/game code or input/reference bytes. Cached ARM64 library and
callback objects are hashed; current full-binary baseline evidence reverified,
not rerun because its core and binary are unchanged. The isolated test has no
scene-regression claim. Checkpoint **204 identities**, all **449** prior identities
reverified; SHA256 `2364cd216fc0b8b01e710e880d2fcd9542b359e3a3d72b8681079b749f2d57c2`. All processes terminal, zero booted
Simulator metadata; no Simulator launched. Free **0.30 GiB**: capacity must
be checked before another full build. No deletion or storage sharing this turn.

Fresh G1 inventory is unchanged; supported PAL verifier exits2 for the absent
descriptor. The exact source-only acquisition request remains current. Next add
original `051354` to one bounded full-startup run with prospective language-prefix
entry and original motion-file checks, preserving its live device state. Keep
its resource callbacks and next missing target explicit. Original G1, full boot,
gameplay, saves, mobile and production adoption remain unresolved.
Goal turn: **progress**.

**Previous full-startup checkpoint — 20261006-150748: original initialization-call prefix.**
The full startup now executes original `04E310`, saves PR and passes R4=0 through
its original call delay slot. It reaches the predicted missing `051354` at
PC `04E310`, PR `04E318`, SP `FFFFA0`, SR `40000000`.

The isolated caller passes **512 cases**. A separate language/resource prefix
passes **9,216 cases**, including original registered `09AC9C`, language-index
adjustments and byte/halfword writes. Both verify whole 16 MiB RAM, stack,
69 CPU fields and 25 timer fields, and resume the actual frozen caller once.
The language test deliberately excludes `09F5A0`, which **is registered in the
full provider**. Its rejection is a test boundary, not a missing production
loader. Motion loading, resource free branch and initialization completion
remain unverified. Independent emitter costs: five caller cycles and 71 for
the combined isolated language/resource prefix; hardware timing is unproven.

One changed full startup matches both prospective entry/failure predictions.
Exact-binary regression passes two supplied scenes, ten external audits,
release controls and six unchanged images. A trace assertion initially used
the decoded JSR address instead of the trace's function-entry PC; rejected
checker preserved, auxiliary label corrected. No runtime, game, independent
state prediction or reduction oracle change; no startup rerun.

Closure **821 functions / 82,116 instructions / 2,390 external records**, cap
824, is inventory. Twelve patches/private inputs/flash/root VMU unchanged.
Five source versions per reduction and twenty full-startup source versions
frozen. Checkpoint binds **449 identities**, with all **996** prior identities
reverified. SHA256: `f8c2d1638221b5fb9a49beabcdeb60ae8687f15235db078f220c6d2798c31faa`.
All processes terminal, zero booted Simulators in fresh device metadata;
mobile tooling remains license-blocked. Free disk **0.49 GiB**: check capacity
before another build. No deletion or storage sharing in this iteration.

Fresh original G1 audit unchanged: runtime 0/1, generated 0/1,474, headers
0/177, contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. PAL verifier exits 2
for the absent supported descriptor. Exact source-only request remains current.
**Original G1, normal boot, gameplay, saves, mobile and production adoption
remain unresolved.** Next reduce the registered motion-file loader using an
independent extraction/decode expectation for owner-CDI `s_mot.PRS`, before
adding `051354` to full startup. Goal turn: **progress**.

**Previous recovery checkpoint — 20261006-144501: original mode-0 prologue and dispatch.**
Original `0558C0` reads actual mode **0** at `7649F0`; signed jump-table offset
68 selects `05593C`, whose first call rejects missing `04E310`. PC `05593C`,
PR `055942`, SP `FFFFA4`, SR `40000000`, R0 `44`. Original mode/table remain
unchanged. Only original 32-byte saved-register/PR stack writes are predicted.

The one-function/909-instruction reduction passes **1,280 cases**: 128 mode-0
and 1,152 default/invalid paths. Modes 6/7/14/15 take the original table default;
26/27/7FFFFFFF/80000000/FFFFFFFF take the unsigned bounds path. All IRQ masks,
T values, physical FPU banks, two canaries and eight TSTR states. Whole 16 MiB
RAM/stack/guards, 69 CPU and 25 timer/batch fields match independent expectations;
actual frozen caller executes once. Mode 0 explicitly rejects `04E310`; default/
invalid paths reject missing cleanup `04562E`. They do **not** complete a return:
cleanup `08C25E` is also absent. Source-derived emitter cycles 31/32/27; no
hardware timing claim, no mode/readiness injection, no runtime/oracle repair.

One changed startup matches two predictions saved before launch: entry and
first missing-helper boundary. Entire RAM/CPU/timers match with no excluded
ranges; IRQ count/redirect unchanged. Earlier captures omitted/prior evidence
reverified, audio/device counters unpredicted. Exact-binary regression passes
two supplied scenes, ten external audits, release controls and six unchanged
images. Mode 0, 89 sectors, 2,045,955 ARM7 instructions/zero faults, 15,062 AICA
frames/zero nonzero frames, IRQ count 32. Seven commands/peripheral operation/
status 1 remain pending; no playback or normal boot claim.

Closure **820 functions / 82,079 instructions / 2,383 external records**, bounded
cap **824**, is inventory. Five reduction/twenty startup source versions frozen;
twelve patches/private originals/flash/root VMU unchanged. Checkpoint
`144501/mode-prefix-checkpoint.json`: **996 identities**, all **415** prior
identities reverified. SHA256: `fffb4d451e5f727ce55197c93da7a3a0f2f11f89aa4985c9771d60ba3feeb0c4`.
Draft handoff incorrectly labeled `09AC9C` missing before its registration audit;
actual callback provider includes it. Rejected draft/checker preserved and final
handoff validated against both tables; no execution/oracle change or rerun.

APFS sharing preserves **190 completed compiled objects/archives** and **168
completed diagnostic captures**, selected only from 141129/142605/143441;
references also completed. All hashes/paths/permissions/timestamps/xattrs retain,
inode/birthtime changes declared. No asset/save/input/source/log/prediction path
or content removed, no live build/baseline selected. Available-space full-pass
changes: 102,162,432 and 732,766,208 bytes; first-pass changes separately recorded.
Free **about 1.2 GiB** after regression. All processes terminal/no Simulator
booted/launched; safety/diff pass. Remeasure before another build.

Fresh G1 inventory unchanged: runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. Supported PAL descriptor
absent; exact source-only request current, **original G1 unresolved**.
Next execute original `04E310` prefix through actual missing `051354`, preserving
saved PR and original R4=0 call-delay slot, full RAM/stack/CPU/timers and frozen
caller. `051354`'s child `09AC9C` is already registered; its later reduction must
include that original behavior. Actual halfword `74D448=0`. Two classification
batches consumed: execute before further inventory. Initialization/default return,
complete main-loop/normal boot/gameplay/save/mobile/production remain unverified.
Goal turn: **progress**.

**Previous recovery checkpoint — 20261006-143441: original signed guard and conditional caller.**
Original `010B58` returns **0** for the actual flag `164AEC=1` and signed
count `164AE8=0`. Original `0556D8` tests that result and calls `0558C0`,
which rejects its missing registration. PC `0556DC`, PR `0556E0`, SP `FFFFC8`,
SR `40000001`. Entire RAM remains unchanged; no readiness or mode injection.

The two-function/113-instruction reduction passes **4,096 cases**: 2,048 signed
guards and 2,048 conditional caller paths. Flags 0/1/80000000/FFFFFFFF, counts
0/1/2/3/4/7FFFFFFF/80000000/FFFFFFFF, all IRQ masks, both T values/physical FPU
banks and eight TSTR states. Whole 16 MiB RAM/stack/guards, 69 CPU and 25 timer/
batch fields match independent expectations. Both missing caller dependencies
`0558C0`/`010B7A` reject explicitly. Actual frozen guard and continuation each
execute once. Source-derived emitter costs 8/13/15/16 plus five caller cycles;
actual path 13+5. No oracle/runtime/game repair, no hardware timing claim.

One changed startup matches three predictions saved before launch: entry,
return and missing-helper boundary, including all RAM/CPU/timer fields with
no excluded ranges. IRQ count/redirect unchanged across guard. Earlier captures
omitted; prior evidence reverified. Audio/device counters were not predicted.
Exact-binary regression passes two supplied scenes, ten external audits,
release controls and six unchanged images. Mode 0, 89 sectors, 2,045,951 ARM7
instructions/zero faults, 15,062 AICA frames/zero nonzero frames, IRQ count 32.
Seven sound commands/peripheral operation/status 1 remain pending; no playback.

Closure **819 functions / 81,170 instructions / 2,246 external records**, cap 820,
is inventory. Five reduction/twenty startup source versions frozen; twelve
patches/private originals/flash/root VMU unchanged. Checkpoint
`143441/main-guard-checkpoint.json` binds **415 identities**, all **438** prior
identities reverified. SHA256: `275ef5e2cfc4a89b9c7728564478535d38f78caec5f47edcfda030fefa3ef471`.
Free disk **about 1.0 GiB**, remeasure before next build; no deletion/sharing.
All game/build processes terminal, no Simulator booted/launched. Safety/diff pass.

Fresh G1 inventory unchanged: runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. Supported PAL descriptor
absent; exact source-only request remains current, **original G1 unresolved**.
Next reduce original `0558C0` prologue/table dispatch. Actual mode 0 at `7649F0`,
signed table offset 68 at `055908`, selects `05593C`; first helper `04E310`
is absent from both providers. Verify full stack/RAM/CPU/timers and frozen
caller, with explicit original-helper rejection and source-defined invalid-mode
return. One classification batch consumed; one remains before execution.
Current closure cap must account for the next added function. Complete main-loop,
normal boot/gameplay/save/mobile/production adoption remain unverified.
Goal turn: **progress**.

**Previous recovery checkpoint — 20261006-142605: original task scan and complete timed-work return.**
Original `65FEDE` scans thirty-two 100-byte records at `8A8010`, restoring
PR/R13/R14/SP. Original `648422` then finishes its eight 116-byte inactive
records, timestamp differences/stores and full caller restoration, returning
`0556D4` with SP `FFFFC8`. Total elapsed timer is **3** at `67789C`.
Startup stops at missing `010B58`: PC `0556D4`, PR `0556D8`, SR `40000001`.
This is one original work-routine return, not a complete main-loop iteration.

Four-function/117-instruction reduction passes **4,608 cases**: 1,024 inactive
scans, 2,048 active `65FED0` rejections, 1,024 caller returns and 512 active
`660754` rejections. Whole 16 MiB RAM/stack/guards, all 69 CPU and 25 timer/batch
fields match independent expectations; actual frozen scan and suffix each resume
once. Eight TSTR states, all IRQ masks, both T/FPU banks and canary patterns are
covered. Independent emitter costs are 236 scan and 121 suffix cycles, not
hardware timing accuracy. The first caller oracle reversed saved-register stack
order; rejected sources/logs are preserved, only expected indexing corrected.
No original game/runtime/cycle-model change or missing-helper stub.

One changed startup matches three predictions saved before launch: scan entry,
scan return and complete timed-work return. Entire RAM and CPU/timer fields
match with no excluded ranges. Later failure and audio/device counters were not
predicted; IRQ count/redirect remain unchanged across the scan. Exact-binary
regression passes two supplied scenes, ten external audits, release controls
and six unchanged images. Mode 0, 89 sectors, 2,045,949 ARM7 instructions,
zero faults, 15,062 AICA frames/zero nonzero frames, IRQ count 32. Seven commands
and peripheral operation/status 1 remain pending; no playback claim.

Closure **818 functions / 81,153 instructions / 2,246 external records**, cap 820,
is inventory. Five reduction/twenty startup source versions frozen; twelve
patches, protected inputs, flash and root VMU unchanged. Checkpoint
`142605/record32-checkpoint.json` binds **438 identities**, all **3,973** prior
identities reverified. SHA256: `6886a890f0cea225929dc87ab05f7e2cd8ef2903e5f892085d693ec3af3f2944`.
Packaging initially required timer leaves in the callback table; corrected to
actual provider/callback union, with rejected checker preserved. No execution
rerun. Free disk **about 1.6 GiB**; no deletion/sharing this turn. All game/build
processes terminal, no Simulator booted/launched. Safety and diff checks pass.

Fresh G1 inventory unchanged: runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. Supported PAL descriptor
absent; exact source-only request remains current, **original G1 unresolved**.
Next reduce original `010B58`: read-only signed guard returns -1 iff flag
`164AEC` is nonzero and signed count `164AE8` is 2 or 3, otherwise 0. Actual
flag 1/count 0; target absent from both providers. Test signed boundaries/flags,
whole RAM/CPU/timers and frozen caller, then original `0556D8` continuation.
One static batch consumed; one remains before execution. Normal boot/gameplay/
save/mobile/production adoption remain unverified. Goal turn: **progress**.

**Previous recovery checkpoint — 20261006-141129: original sixteen-record task scan returns.**
Original `10E4CC` scans sixteen 40-byte records using existing `65EAA8`/
`65EACE` nesting and IRQ-mask helpers. The captured records are inactive;
nesting count and saved IRQ mask at `67FF20`/`67FF24` return to 0, PR/R13/R14/SP
restore normally. `648422` then stores elapsed timer **1** at `6778A8` and rejects
missing `65FEDE`: PC `648474`, PR `64847C`, SP `FFFFA8`, SR `40000001`.

The three-function/62-instruction reduction passes **6,144 cases**: 3,072 inactive
and 3,072 active-record rejections, with nesting counts 0/1/3, all sixteen active
positions, IRQ masks, T values, physical FPU banks and two canary patterns.
The actual frozen call resumes once. Whole 16 MiB RAM/stack/guards, 69 CPU fields
and 25 timer/batch fields match independent expectations. Active records reject
missing `10E46E`, never skip it. The current emitter model gives 178 cycles for
zero nesting and 148 for nested scans; no hardware timing/interrupt-delivery claim.

One changed original startup matches three prospective complete RAM/CPU/timer
phases: entry, return and failure after 23 caller cycles. No excluded ranges,
IRQ-count change or async redirect across the scan. Earlier phases are omitted
and their previous evidence reverified; audio/device counters unpredicted.
The exact binary passes two supplied-scene regressions, ten external audits,
release controls and six unchanged image hashes. Mode 0, 89 sectors, 2,045,909
ARM7 instructions, zero faults, 15,061 AICA frames/zero nonzero frames; seven
commands and peripheral operation/status 1 remain pending. No playback claim.
Closure **817 functions / 81,133 instructions / 2,246 external records**, cap 820,
is inventory. Twelve patches/private originals/flash/root VMU unchanged.
Five reduction and twenty startup source versions frozen. Checkpoint
`141129/record16-checkpoint.json` binds **3,973 identities**, previous **547**
and supplemental 88 sharing identities reverified. SHA256: `581dc651976134d7c1d334d759693c4e49c1b0e97bd9ee02d1d9ae675c0b8711`.

Disk reached 337 MiB during the build. Exact APFS sharing of **3,195 compiled
object/archive outputs from completed runs** recovered about **2.4 GiB**.
Terminal reports, selected hashes and clone helpers are bound by the checkpoint.
Bytes/paths/permissions/timestamps/xattrs retained; inode/birthtime changes declared.
No source/asset/save/input/capture/log/prediction content removed; live build
outputs were not selected. Free disk **about 2.3 GiB** after regression.
All game/build processes terminal, no booted Simulator in device metadata,
none launched. Repository safety and diff check pass. No repair or rerun needed.

Fresh G1 inventory unchanged: runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. Supported PAL descriptor
missing; **original G1 unresolved**, exact source-only request remains current.
Next reduce original `65FEDE` thirty-two 100-byte records: actual flags all 0;
both it and active helper `65FED0` are absent in both provider tables. Test active
positions with explicit rejection, full RAM/stack/CPU/timer preservation.
One static batch consumed; one remains before execution. Normal boot/gameplay/
save/mobile/production adoption remain unverified. Goal turn: **progress**.

**Previous recovery checkpoint — 20261006-140051: original critical task scan and return.**
Original `65F25A` tail-jumps to `6615A2`, which scans eight 156-byte records
under the IRQ mask saved/raised by `6607A0` and restored by `6607B8`.
The captured records are inactive. PR, R11–R14 and SP restore normally;
`648422` then records elapsed timer value **1** at `6778A4` and rejects missing
`10E4CC`: PC `648460`, PR `648468`, SP `FFFFA8`, SR `40000001`.

The prefix passes **2,048 cases** with explicit missing-helper rejection.
The complete four-function/60-instruction scan passes **3,584 cases**:
2,048 inactive scans, 1,024 active-record rejections, 128 mask-save and 384
mask-restore cases. Each reduction resumes the actual frozen call once.
Active records reject missing `661490`, with no stub. Whole 16 MiB RAM,
stack/guards, all 69 CPU fields and 25 timer/batch fields match independent
expectations. The emitter model predicts 129 wrapper/125 direct cycles;
hardware timing and interrupt delivery remain unverified. No oracle/runtime repair.

One changed original startup matches three prospective full RAM/CPU/timer
phases: critical-task entry/return and failure after the 23-cycle caller
continuation. No excluded ranges or IRQ-count change/async redirect across
scan. Earlier captures are omitted; prior evidence reverified. Audio/device
counters were not predicted. Exact-binary regression passes two scenes, ten
external audits, release controls and six unchanged image hashes.
Mode 0, 89 sectors, 2,045,886 ARM7 instructions, zero faults, 15,061 AICA frames
with zero nonzero frames; seven commands and peripheral operation/status 1 pending.
Closure **816 functions / 81,109 instructions / 2,246 external records**, cap 820,
is inventory. Twelve patches/private originals/flash/root VMU remain unchanged.
Five sources frozen per reduction and twenty for startup. Checkpoint
`140051/task-critical-checkpoint.json`: **547 identities**, previous **424**
reverified. SHA256: `b546e4ccc5cb6c6691bd2147ab14db5c936fddffc6e9a9cea7146c886ba9f70d`.

APFS sharing preserves 56 completed baseline captures from each of `135001`
and `140051`, retaining bytes/paths/metadata except declared inode/birthtime.
Selected hashes reverified; about 0.5 GiB recovered. Latest supplemental sharing
audit binds 88 identities, SHA256 `d17dfc25595647d59d412c18ded4bb923035ef78b79eac29a7cb2025d02886de`. Free disk about **1.0 GiB**;
measure before building. All game/build processes terminal, no booted Simulator
in device metadata, none launched. Repository safety and diff check pass.

Fresh G1 inventory unchanged: runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. PAL descriptor missing;
**original G1 and the exact source-only request remain unresolved/current**.
Next reduce original `10E4CC` sixteen 40-byte records using already-registered
`65EAA8`/`65EACE`. Actual flags are all 0, nesting count `67FF20` and saved mask
`67FF24` are 0. Test active-record rejection at missing `10E46E`, original count/
mask save and restoration. One static batch consumed; one remains before execution.
No normal boot/gameplay/save/mobile/production acceptance. Goal turn: **progress**.

**Previous recovery checkpoint — 20261006-135001: original timed startup work and task scan.**
Original `648422` now reads TMU0 through `60634E`, calls the eight-record
`65F914` loop, reads the timer again and uses original `606358` to store the
elapsed value at `6778A0`. All eight records in the captured state are inactive;
the loop returns normally. Startup next rejects missing `65F25A` at PC `64844C`,
PR `648454`, SP `FFFFA8`. Full startup and a complete main-loop iteration remain open.

The four-function/120-instruction reduction passes **1,920 cases**: 1,024 timed
work paths, 384 inactive-record scans and 512 active-record rejection cases.
Each active position explicitly rejects missing `65F8C8`; it is never skipped.
The actual captured call resumes once. Whole 16 MiB RAM/stack/guards, all 69 CPU
fields and 25 timer/batch fields match independent expectations. A prospective
arithmetic check corrected the loop total from 75 to 71 cycles before any
build/execution; rejected setup sources remain preserved. The current emitter
model gives 117 cycles through the next missing call, not a hardware timing claim.

One changed original startup matches those full RAM/CPU/timer predictions at
work entry, task-scan entry/return and failure, with no excluded ranges. Original
observers call original code; no async redirect or IRQ-count change across the
scan. Earlier phase captures are omitted and their prior evidence is reverified;
device/audio counters were not predicted. The exact binary's baseline passes
two supplied scenes, ten external audits, release controls and six image hashes.
Mode 0, 89 sectors, zero ARM7 faults and zero nonzero AICA frames; seven sound
commands and peripheral operation/status 1 remain pending. No playback claim.

Closure **812 functions / 81,049 instructions / 2,246 external records**, cap 816,
is compilation inventory. Twelve patches, private originals, flash/root VMU and
reference pins are unchanged. Twenty full-run sources and five reduction sources
are frozen. Checkpoint `135001/startup-work-checkpoint.json` binds **424 identities**
and reverifies the previous **1,142**. SHA256:
`f3040c25a82c1e01b10e4f7b1f93ae512ccd2b75cb9fac931a1eedcb823577ac`.
Repository safety and `git diff --check` pass. Free disk about **1.5 GiB**;
all game/build processes terminal, no booted Simulator in device metadata,
none launched. No commit, push or publication.

Fresh source audit remains runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39 and FFmpeg 10,139/10,139. Supported PAL descriptor
is absent; **original G1 remains unresolved**, source-only request unchanged.
Next execute the `65F25A` tail jump into `6615A2`: it saves 24 bytes and calls
missing `6607A0` with R4=SP before its eight 156-byte records and `6607B8` restore.
All these new helpers are absent in both provider tables. Two static batches
consumed; execute the reduced prefix before classifying more title code.
Normal boot/gameplay/save/mobile/production adoption remain unverified.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-133000: original timer initializer returns normally.**
Original `06DD60` configures TMU1, calls `64507E` → `645002`, replaces the
captured object's callback `608286` with `06DE78`, initializes both arrays and
restores PR/R14/SP. It returns to main-loop `0556CE`; startup next rejects
uncompiled `648422`. Full boot and a complete loop iteration remain unverified.

A prefix reduction passes 1,024 cases plus eight hypothetical frozen timer
states. A read-only probe then captures the actual 25 timer/batch fields while
matching the previously predicted whole 16 MiB RAM and 69 CPU fields. The probe
reuses 31 verified compiled objects; only the observer host unit is rebuilt.
The full initializer reduction passes **1,344 cases plus the actual frozen call**.
Its independently derived issue-cost model predicts 195 cycles, including 28
before and 167 after TMU1 starts. This is runtime-model agreement, not hardware
timing or interrupt-delivery proof. Rejected expectations for block ticks, R1
and weighted cycle costs remain preserved with their exact source versions.

One changed original startup prospectively matches whole 16 MiB RAM, all
**69 CPU fields and 25 timer/batch fields at entry and return**, without exclusions.
The observers always call original code. Earlier phase captures are omitted in
this narrow run; their previous evidence is reverified, not claimed as new tests.
The later failure state and audio counters were not predicted. Observed mode 0,
89 sectors, zero ARM7 faults, 15,061 AICA frames with zero nonzero frames;
seven sound commands and peripheral operation/status 1 remain pending.
Closure **810 functions / 80,937 instructions / 2,230 external records**, cap 812,
is compilation inventory, not game coverage. Twelve runtime patches are unchanged.

Both the probe and changed binary pass the supplied-scene regression: two scenes,
ten external audits, release controls and six unchanged images. The changed
binary's first regression failed solely while copying evidence with ENOSPC;
partial output remains in `baseline-regression`. Its successful identical-binary
retry is in `baseline-regression-repair1`. No game/runtime repair was needed.
The `132926` wrapper bootstrap rejection launched no game; `133000` is the
canonical provider run with narrow timer captures.

Checkpoint `133000/timer-init-checkpoint.json`: **1,142 identities**, previous
789 reverified. SHA256:
`bfed00f458771f03d2d701ec9f8c4b11c71c246003696123bad0c0a97bf40d68`.
APFS page sharing preserves capture paths, bytes, permissions, timestamps and
extended attributes; inode/birthtime changes are declared. Sharing 152 older
captures recovered about 2.3 GiB, following 169 newer captures and exact probe
sharing. Every selected hash is reverified. Free disk is about **2.2 GiB** after
regression; measure again before building. All game/build processes are terminal,
no booted Simulator in device metadata, none launched. Private originals,
flash and root VMU identities remain unchanged; repository-safety check passes.

Fresh source audit remains runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, providers 38/39 and FFmpeg 10,139/10,139. PAL descriptor absent;
**original G1 remains unresolved**, exact source-only request unchanged.
Next reduce `648422` using actually registered `60634E`/`606358`; `65F914` is
its first uncompiled helper. One static classification batch remains before the
next execution experiment. No normal-boot, gameplay, save, mobile or production
adoption acceptance follows. Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-125850: original startup dispatcher returns to the main loop.**
Original `60549E` selects `604490` from live object `6701F4` at +0x74.
The callback stores R4/R5 at `698420` +36/+40 under temporary IRQ mask 15,
restores the mask, and returns to `0556B6`. Original `055820` tail now returns;
the main-loop prefix reaches missing `06DD60`: PC `0556CA`, PR `0556CE`,
SP `FFFFC8`, SR `40000001`. A complete loop iteration remains unverified.

The two-function/37-instruction reduction (cap 4, one external record) passes
**512 cases**: 128 each for null dispatch, selected callback, direct stores and
unknown-target rejection. Whole 16 MiB RAM/stack/guards and all **69 captured
CPU fields**, including the now-captured numeric DBR, match. The frozen missing
`60549E` call resumes once. First rejected oracle expected JMP address `6054AC`
where this runtime reports basic-block entry `6054A8`; exact five sources/log/
report preserved, expectation corrected, no runtime/game change.

One changed startup prospectively matches entire RAM/all 69 fields at dispatch
entry/exit, main-loop prerequisite and failure, without exclusions. Three original
calls retained; no async redirect/IRQ-count change across dispatch. Earlier file-
pool/critical/nested/module/sound phases and exact-binary baseline pass: two
supplied scenes, ten external audits, release controls and six unchanged images.
Mode 0, 89 sectors, 2,045,833 ARM7 instructions, zero faults, 15,061 AICA frames,
zero nonzero frames, three G2 runs/67,264 bytes, IRQ count 32; counters unpredicted.
Critical count/IRQ mask 0, seven commands and peripheral operation/status 1 pending.
Closure **809 functions / 80,882 instructions / 2,229 external records**, cap 812;
not executed coverage. Twelve patches/private originals/flash/root VMU unchanged.
Five successful/five rejected reduction and nineteen full-run sources frozen.
Checkpoint `125850/startup-dispatch-checkpoint.json`: **789 identities**, all
previous 808 reverified. SHA256:
`1375db951880bbbb7dbb4bc16fa3b818487750470b6eb1113fe7d7da9e421bbf`.

APFS sharing preserves 68 identical completed startup captures and 56 baseline
observations, all selected hashes reverified; no paths/content removed. About
0.6 GiB recovered; free **777 MiB**, capacity must be checked before another build.
All processes terminal; no booted Simulator in local device metadata, none launched.
Fresh original source audit unchanged: runtime 0/1, generated 0/1,474, headers
0/177, contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. Supported PAL
verifier exits 2 for absent descriptor. **G1 unresolved**, source-only request current.

Next execute `06DD60` timer-init prefix: TSTR bit 1 clear/set, channel 1 reload/
count `00BEBC20`/control 0, RAM `790680`, saved R14/PR; then existing `64507E`
wrapper toward registered `645002`. Both helpers are actually registered in the
provider; `06DD60` is absent in both providers. Two static batches consumed;
next must execute a reduction. Test synthetic initial TMU states and all 69 fields;
actual preexisting TMU values were not captured, so numerical full-run device
predictions need new evidence. No stubs/forced mode/input/readiness. Normal boot,
title/playback/gameplay/save/mobile/production adoption/G1 remain unverified.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-124152: original file-record reset and stack restoration.**
Original `09F6D2` zeroes sixteen words at 40-byte intervals through the
640-byte array `7A4D0C` and separate global `7A4F8C`. Other record bytes are
preserved. Original `055862` zeroes `7869F4` and restores PR/R13/R14/SP before
calling missing `60549E`: PC `055862`, PR `0556B6`, SP `FFFFC8`, SR `40000001`,
R13/R14 0. The outer `055820` tail has not completed its final dependency.

The two-function/29-instruction reduction (cap 4, one external record) passes
**256 cases**: 128 record and 128 caller-tail cases, two canary patterns,
all IRQ masks, SR flags, both T values and both physical FPU banks. Entire
16 MiB RAM/guards/stack and **68 captured CPU fields** match independent
expectations. The frozen missing call is resumed once. DBR preservation is
checked in synthetic tests; its prior numeric value was not captured.
One original startup run prospectively matches entire 16 MiB RAM/68 fields at
entry, exit, tail prerequisite and failure, without exclusions. This includes
SSR, SGR and eight banked registers. New DBR observations show `8C000010`
unchanged across the reset; its initial numeric value was not predicted.
Read-only observers always call original code; no async redirect/IRQ-count
change across reset. Earlier critical/nested/module/sound phase predictions
and exact-binary regression pass (two supplied scenes, ten external audits,
release controls and six unchanged images). No runtime/oracle repair or rerun.

Observed mode 0, 89 sectors, 2,045,826 ARM7 instructions, zero faults,
15,061 AICA frames/zero nonzero frames, three G2 runs/67,264 bytes, IRQ count 32.
Counters unpredicted. Critical count/IRQ mask 0 preserved; seven commands and
peripheral operation/status 1 remain pending, batch/software busy 0.
Closure **807 functions / 80,845 instructions / 2,229 external records**, cap 808;
not execution coverage. Twelve patches, originals, flash and root VMU unchanged;
five reduction/nineteen full-run sources frozen. Checkpoint
`124152/file-pool-startup-checkpoint.json` binds **808 immutable/protected
identities**; all previous 973 reverify. SHA256:
`be3c11e27603fda49b4450fa580500b28b3f0189036ad99acf7e5432f148e781`.

Disk fell to 306 MiB after regression. APFS sharing preserves 119 completed
startup RAM/AICA/flash captures and 72 completed baseline observations from
`123020`/`124152`; all selected paths/content/metadata and hashes retained
(inode/birthtime changed). Saves/expected data/assets/sources/inputs/logs/
predictions excluded; about 0.9 GiB recovered, free disk now **1.2 GiB**.
All processes terminal/no Simulator launched. Fresh source audit unchanged:
runtime 0/1, generated 0/1,474, headers 0/177, contracts 0/4, providers 38/39,
FFmpeg 10,139/10,139; PAL descriptor absent. **Original G1 unresolved**;
exact source-only request remains in `BOOTSTRAP-BLOCKERS.md`.

Next reduce original `60549E` null/non-null dispatch and selected `604490`.
Actual object `6701F4` has callback `604490` at +0x74; global `677154` points
at `698420`. The callback stores R4/R5 at +36/+40 under temporary IRQ mask 15,
then restores the mask. Verify RAM/stack and all 69 now-captured CPU fields,
including DBR, then follow original `0556B6` toward the next dependency
`06DD60` (loop flag `1A2C94` is 0). All three targets are absent in actual
registrations of both providers. One new static batch (`604490`); existing
main/dispatcher reviews reused. No forced mode/input/readiness. Pending commands,
normal boot/title/gameplay/save/audio/mobile/production/G1 remain unverified.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-114927: original two-clear and signed-guard initialization.**
Original `10F6AC` zeroes 0x280 bytes at `7B038C` and 0x400 bytes at `7B060C`.
Original `6620DC` takes the nonpositive guard path: counter `67FFA4` increments
0 to 1 and 0xC00 bytes at `8AA838` are cleared. Positive counts skip that clear.
The outer initializer remains incomplete; startup rejects missing `661E00`
from `647D8E`, PR `647D94`, SP `FFFFB8`, SR `400000F1`, SPC `64542C`.
Critical count 1, saved IRQ mask 0 and IRQ mask 15 are retained.

The four-function/76-instruction reduction (cap 8, six external records) passes
**896 cases**: 128 two-clear and 768 signed-guard cases. Two canary patterns,
positive/nonpositive/wrapping counts, all 16 IRQ masks, both T values and both
physical FPU banks are checked. Entire 16 MiB RAM, stack/guards and all 58 fields
match independent expectations; the frozen `647D82` caller passes and explicitly
rejects `661E00`. Expected full RAM/state were saved before frozen execution.
No runtime, guest or oracle repair was needed for this reduction or startup.

One changed startup prospectively matches **entire 16 MiB RAM/all 58 fields**
at the guarded prerequisite and final failure, without exclusions. All six
relevant calls remain in the trace. The capture is read-only. Earlier module
entry/return still matches its original prediction using archived sources;
its former later critical/failure scope is explicitly excluded from that phase
auditor. Earlier sound callback RAM/AICA/58-field expectations and other phase
checks pass within recorded scopes. Exact binary regression passes two supplied
scenes, ten external audits, release controls and six unchanged images.

Device/audio counters were not predicted. Observed mode 0, 89 sectors,
2,041,011 ARM7 instructions, zero faults, 15,052 AICA frames and zero nonzero
frames; three G2 runs/67,264 bytes. Seven sound commands and peripheral
operation/status 1 remain pending, batch count/software busy 0. Neither IRQ
restoration/delivery nor request completion/playback is established.
Callback closure: **799 functions / 80,746 instructions / 2,235 external
records**, cap 804; this is not execution coverage. Twelve patches, owner
inputs, flash and root VMU are unchanged. Five reduction/nineteen full-run
sources are frozen. Checkpoint `114927/guarded-init-startup-checkpoint.json`
binds **527 immutable/protected identities**; all previous 547 reverify.
SHA256: `59b43f584e2ac130ff88a34b884fa6a9f7187266b57b946239ce41603c8ee38b`.

One checkpoint packaging lookup searched the full-run snapshot for the prior
isolated table tests. Its source/rejection remain under `checkpoint-repair-1`;
repair1 uses the actual isolated reduction archive. No build/game rerun or
runtime/oracle change followed. All experiments terminal; no Simulator launched.
Free disk: **2.3 GiB**; no deletion or block-sharing this turn.

Fresh original G1 audit is unchanged: runtime 0/1, generated 0/1,474, headers
0/177, contracts 0/4, providers 38/39, FFmpeg 10,139/10,139. Supported PAL
verifier exits 2 for the absent descriptor. **G1 remains unresolved** and the
exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current.

Next execute `661E00`'s signed guard at `67FF6C`/conditional 0x480-byte clear
at `8AA3B8`, then `65FA6C`'s 0xC80-byte clear at `8A8010` and return value 1.
Verify captured `647D8E` caller and follow `65F328`. Two static batches consumed;
next step must execute a reduction. Preserve critical count/IRQ mask and pending
requests. No outer initialization completion, normal boot/title/gameplay/input/
save/audio/hardware/mobile/production adoption or G1 acceptance follows.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-114203: original table/module return and critical entry.**
Original `66E8DC` clears 64 bytes at `8EE064`, then writes eight records with
indices 15 through 8. The module executes original `6607CE` RTS/NOP and
`66B8B0`'s two zero-global stores, returning to `647D7C`. Original `65EAA8`
increments critical count `67FF20` from 0 to 1, saves IRQ mask 0 at `67FF24`
and raises the CPU IRQ mask to 15. The **outer initializer is incomplete**:
startup explicitly stops before missing `10F6AC` from `647D82`.

The eight-function/131-instruction reduction passes **576 cases**: 128 table,
64 module-return and 384 critical-entry cases. Two table patterns, signed/
wrapping counts, all 16 IRQ masks, both T values and both physical FPU banks
are covered. Entire 16 MiB RAM, stack/guards and all 58 fields match independent
expectations; the frozen caller and saved outer continuation pass.
The first attempt replayed the captured `66B8F0` tail's already-executed
POP_PR, popping twice and skipping the setter. Its exact five sources/report/
log are retained. The fixture now resumes missing target `66E8DC` once;
expectations, guest code and runtime were not changed to fix that failure.

One changed startup prospectively matches **entire 16 MiB RAM/all 58 fields**
at module entry, module return, critical prerequisite and final failure,
without exclusions. The scoped observer always calls the original initializer.
Table/RTS/setter/critical calls remain in the traces. Latest boundary: PC
`647D82`, PR `647D88`, SP `FFFFB8`, SR `400000F1`, SPC `64542C`.
Device/audio counters were not predicted. Observed mode 0, 89 sectors,
2,038,331 ARM7 instructions, zero faults, 15,047 AICA frames, zero nonzero
frames; three G2 runs/67,264 bytes. Seven sound commands and peripheral
operation/status 1 remain pending; batch count/software busy are 0.
IRQ-mask entry is not proof of interrupt delivery, restoration or device behavior.

The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Earlier sound callback main RAM/AICA/58-field
predictions and prior phase audits pass in their recorded scopes. Callback
closure: **797 functions / 80,720 instructions / 2,237 external records**, cap
800; compiled closure is not execution coverage. Twelve patches, originals,
flash and root VMU remain unchanged. Successful reduction/full-run sources
are frozen, and the failed reduction is checkpoint-bound. Checkpoint
`114203/table-init-startup-checkpoint.json` binds **547 immutable/protected
identities**; all previous 509 reverify. SHA256:
`3cf00013603abc6b7b8d5093b1a3ecb0d177df0d0cce029f019022b22e23718a`.
All experiments are terminal; no Simulator launched. Free disk: **3.2 GiB**.
No block-sharing or deletion was performed this turn.

Fresh original-source audit still verifies runtime 0/1, generated 0/1,474,
headers 0/177, contracts 0/4, providers 38/39, FFmpeg 10,139/10,139.
Supported PAL descriptor remains absent; verifier exits 2. Original **G1 remains
unresolved**; exact source-only request in `BOOTSTRAP-BLOCKERS.md` remains current.

Next execute `10F6AC`'s two zero fills (0x280/0x400 bytes at `7B038C`/`7B060C`)
and `6620DC`'s signed guard/conditional 0xC00-byte clear at `8AA838`, checking
counter `67FFA4`, saved `647D82` caller and next outer dependency `661E00`.
Two static batches consumed; next step must execute a reduction. Preserve
critical count 1/IRQ mask 15, pending peripheral and seven commands. No outer
initializer completion, responses, playback, normal boot/title/gameplay/input/
save/hardware/mobile/production adoption or G1 acceptance follows.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-112911: original module buffers and table initialization.**
Original `6607D2` executes two zero fills (0x4E0 bytes at `8A8C90`, 0x820 bytes
at `8A9170`) and 512 stores of `80808080` at `8A9990`. Child `66B8E4` zeroes
0x220 bytes at `8DA98C` and tails missing `66E8DC`. Startup explicitly stops
there; the module initializer and the outer one-time initializer are incomplete.

The six-function/61-instruction reduction passes **704 cases**: 256 initializer/
shim cases, 128 child-clear cases, 64 original RTS/NOP cases and 256 two-global
setter cases. Two initial buffer patterns, all 16 interrupt masks, both T values
and both physical FPU banks are checked; entire 16 MiB RAM, stack/guards and all
58 CPU/FPU fields match independent expectations. The captured `65F208` caller
passes. Original `6607CE` and `66B8B0` are verified separately in isolation;
they remain unexecuted on the startup path because `66E8DC` is still missing.
Independent expected RAM/state files were saved before frozen execution.

One changed startup prospectively matches **entire 16 MiB RAM/all 58 fields**
at initializer entry, tail prerequisite and final failure, without exclusions.
At dispatch entry only PC advances from the previous `65F208` rejection to
target `6607D2`, as original dispatch specifies. The observer always calls the
original registered initializer and changes no guest state. Five bootstrap-entry
and five initializer calls remain in the traces. Latest boundary: PC `66B8F0`,
PR `660800`, SP `FFFFB4`, SR `40000001`, SPC `64542C`. Device/audio counters were
not predicted. Observed mode 0, 89 sectors, 2,038,282 ARM7 instructions, zero
faults, 15,047 AICA frames and zero nonzero frames. Three G2 runs still total
67,264 bytes. Seven sound ring commands and peripheral operation/status 1 remain
pending; batch count and software busy are 0. No responses or readiness injected.

Earlier sound callback entry/return still match original prospective main RAM,
AICA and 58-field expectations using archived sources. Exact binary scene
regression, ten external audits, release controls and six unchanged images pass;
other earlier phase audits pass within recorded scopes. Callback closure:
**795 compiled functions / 80,682 instructions / 2,239 external records**, cap 800;
compiled closure is not execution coverage. Twelve patches, owner inputs, flash
and root VMU are unchanged. Five reduction/nineteen full-run sources are frozen.
Checkpoint `112911/module-init-startup-checkpoint.json` binds **509 immutable/
protected identities**, and all previous 499 identities reverify. SHA256:
`ab562a771341d2d4d0fe77df30682d5a404acffdf190259011a9f9b99136dde8`.

Available disk fell to 1.1 GiB after the run. The existing APFS clone method
preserved all 728 selected completed baseline captures byte-for-byte, including
permissions/owner/xattrs/access/modified timestamps; inode/birth times change.
Saves, expected-decoded assets, originals, source, logs and predictions were
excluded. Paths/contents were not removed. Hashes reverify; observed available
space gain 3,237,175,296 bytes (~3.0 GiB). Free disk is now **4.1 GiB**.
The clone plan/reports/helper are checkpoint-bound. All experiments are terminal;
no Simulator was launched.

Fresh source audit remains runtime 0/1, generated 0/1,474, headers 0/177,
contracts 0/4, title/providers 38/39 and FFmpeg 10,139/10,139. The PAL descriptor
remains absent. Original **G1 is unresolved**; source-only acquisition request
in `BOOTSTRAP-BLOCKERS.md` remains current. No new source lead was assumed.

Next: execute `66E8DC`'s 64-byte clear and eight 8-byte records with indices
15 through 8; verify the captured `66B8F0` caller through `6607CE`/`66B8B0` and
module return to `647D7C`. Then verify `65EAA8`'s nested critical-section count,
saved interrupt mask and outer caller before following `10F6AC`. Two static
batches are consumed; next step must execute a reduction. No initializer
completion, firmware consumption/playback, normal boot/title/gameplay/input/save,
hardware/mobile, production adoption or G1 acceptance follows from this turn.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-111827: original bootstrap flag and initialization guard.**
Original `647D5C` now clears flag `677894` from 1 to 0 for the captured caller
`055848`. Its continuation `055856` calls the signed one-time guard `647D62`:
count `677898` is 0, so original code increments it to 1, calls `65EAA0` to
zero `67FF20`, and enters tail shim `65F208`. Startup explicitly stops before
missing `6607D2`. The guard count does **not** prove initializer completion.

The five-function/83-instruction isolated reduction passes **640 cases**:
256 flag stores and 384 signed guard cases, across all 16 interrupt masks,
both T values and both FPU banks. Positive counts skip initialization; zero,
negative and wrapping counts take the original increment/reset path. Entire
16 MiB RAM, stack/guards and all 58 CPU/FPU fields match independent expectations.
The actual frozen caller passes, and its positive-count alternative explicitly
rejects the later `09F6D2` dependency. No initializer or response is stubbed.

In changed startup, the prospective **entire 16 MiB RAM and all 58 fields**
match both `first-service-init-prerequisite` and final failure, with no exclusions.
All four original store/guard/reset/shim calls remain in the trace. The capture
is read-only; no runtime/core repair was made. Latest boundary: PC `65F208`,
PR `647D7C`, SP `FFFFB8`, SR `40000000`, SPC `64542C`; the saved parent PR is
`05585C`. Device/audio counters were not predicted. Observed mode 0, 89 sectors,
2,035,801 ARM7 instructions, zero faults, 15,042 AICA frames and zero nonzero
frames. Three G2 runs total 67,264 bytes. Seven sound ring commands and
peripheral operation/status 1 remain pending; batch count/software busy are 0.

Earlier sound callback entry/return still match their original prospective
16 MiB RAM, 2 MiB AICA and 58-field expectations using archived source versions.
The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images. Earlier worker, dispatch, registration,
language, frame-wait, driver/handles, initialization and material checks pass
within their recorded scopes. Callback closure: **791 compiled functions /
80,636 instructions / 2,239 external records**, cap 796; this is not execution
coverage of every function. Twelve patches, original inputs, flash and VMU are
unchanged. Checkpoint `111827/bootstrap-service-startup-checkpoint.json` binds
**499 immutable/protected identities**; all previous 480 identities reverified.
SHA256: `26ae25dfd86b8cebe76c7722828a02580605968d98b8da322f5fac8c35aacd5f`.
All experiments are terminal; no Simulator was launched. Free disk: **1.9 GiB**.

Fresh original-source audit still verifies runtime 0/1, generated entries 0/1,474,
headers 0/177, contracts 0/4, title/providers 38/39 and FFmpeg 10,139/10,139.
The supported PAL descriptor is absent; its verifier exits 2. Original **G1
remains unresolved** and the exact source-only request in `BOOTSTRAP-BLOCKERS.md`
remains current. No renewed source search was attempted without a concrete lead.

Next: execute a reduction of `6607D2`: original zero fills of 0x4E0 and 0x820
bytes, 512 stores of `80808080`, then `66B8E4`'s 0x220-byte clear and its
`66E8DC` tail dependency. Verify full RAM/stack/58 fields and saved `65F208`
caller before another startup. Two static classification batches are consumed;
the next step must execute, not expand inventories. Later `6607CE` is original
RTS/NOP and `66B8B0` writes two globals; neither is executed yet. Keep disk use
bounded and preserve existing evidence. No module initialization completion,
firmware consumption, playback, normal boot, title, gameplay, input/save,
hardware/mobile, production adoption or G1 acceptance follows.
Goal turn classification: **progress**.

**Previous recovery checkpoint — 20261006-103934: SDK command 10 dispatch reaches sound callback.**
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

**Previous recovery checkpoint — 20261006-102235: progress callback and first SDK transfer prerequisite.**
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

**Previous recovery checkpoint — 20261006-073430: operation1 setup reaches original notification.**
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

**Previous recovery checkpoint — 20261006-072216: counter queries reach operation1 handler.**
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

**Previous recovery checkpoint — 20261006-071002: first sound batch reaches modeled DMA.**
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

**Previous recovery checkpoint — 20261006-064555: original request queued; first frame wait returns.**
Original606B38 executes its getter and both setters, queuing operation1/status1
for modeled peripheral record0. All four callback calls are retained in the
read-only first-request trace. The prospective1,024-byte record region and32-byte
guards and planned current-mask word match at first request and final failure.
Request execution/completion,
latest whole RAM/control/FPU state and device/audio counters are not predicted.

The four-function/64-instruction reduction passes8,192 cases:3,584 busy/free
callback,2,560 operation setter and2,048 argument setter cases, all eight slots,
all16 interrupt masks and both FPU banks. Entire RAM, stack, ABI and neighbors
match. No completion or readiness override was added.

Observed dispatcher continuations053542→055842 prove the first original frame
wait returns; later interruptSPC0534F4 proves the second wait is entered. This
source-bound proof is retrospective and uses modeled interrupts. Before queued
worker operation1 runs, startup stops at sound service65E0B8:
PC647CF2,PR647CF8,SP89693C,SR60000061,SPC0534F4.

The old registration audit rejects the final record layout, now legitimately
changed. Its source/failure log are retained. A retrospective delta verifies only
the status/op bytes changed0→1; the unchanged original registration prediction
passes first-worker-entry, selected retrospectively. No runtime repair/rerun.
Prior worker phase predictions and language, sound and material audits pass.
The exact binary passes two supplied scenes, ten external audits, release controls
and six unchanged images. Original inputs, flash and root VMUs unchanged.
Closure765 functions/79,553 instructions/2,210 external records, cap768; twelve
patches unchanged. Mode0,sectors89,1,657,281 reported ARM instructions, zero
faults/nonzero audio frames. No hardware/input/save, presented title, normal boot,
gameplay, audio playback, production adoption or G1 acceptance.

Next reduce65E0B8 sound service and66AF04/66A8E8/66A7A8 dependencies; verify
driver-read ABI, descriptor iteration/wrap, errors and limit of32 successful calls.
Keep the pending request/unexecuted64BFE0 visible. Katana/PAL blockers unchanged.
Checkpoint064555/peripheral-request-startup-checkpoint.json binds411 identities
and the prior checkpoint; previous441 identities reverified. SHA256:
`ef28a4e85e8748ceaf0bce21c74bcc19dfd7d3a858cd9a73964c0b863a799f54`.
All experiments terminal; no Simulator launched. Free space7.9GiB; goal active.

**Previous recovery checkpoint — 20261006-063555: original peripheral mask reaches its callback.**
Original64BF74/664AEC check eight mapped records. The modeled record0 produces
mask1; the caller stores it, checks original readiness and reaches uncompiled
606B38. Boundary:PC606A76,PR606A7A,SP89691C,SR60000060,SPC645DB8.
The callback has not executed. No full-run connection/ready flag was forced.

The eight-function/252-instruction reduction passes16,128 cases:3,840 per-record
and12,288 aggregate, all256 bitmasks, both identifier orders/FPU banks and
byte-sign/flag branches. Original MACL effects and memory guards are checked.
The first frozen-prefix oracle used8937E8 forR14; the original606C60 literal
is8937E4. The failed source/log remains preserved; only the oracle was corrected.

The full run matches the prospective entire16MiB RAM image and all58 GPR/control/
physical FPU fields, with no excluded bytes. Device/audio counters were not
predicted. Prior worker entry/yield/scheduler-return predictions still pass.
The exact binary passes two supplied scenes, ten external audits, release
controls and six unchanged images; callback/ready, language, sound and material
audits pass. Original inputs, flash and root VMUs unchanged. Closure762 functions/
79,497 instructions/2,210 external records, cap768; twelve patches unchanged.
Mode0,sectors89,1,280,806 reported ARM instructions, zero faults/nonzero audio.
This is modeled peripheral state, not actual hardware/input. No callback
completion, title, normal boot, gameplay, save, audio playback, production
adoption or G1 acceptance.

Next reduce606B38 with64B9E4/64B9F4/64BA04: busy-record early return and original
operation1/argument/status writes, then one changed startup. Katana/PAL blockers
and source-only request unchanged. Checkpoint063555/peripheral-status-startup-checkpoint.json
binds441 identities and the previous checkpoint; previous510 identities reverified.
SHA256:`01eaa5c3eb64396504a4e9287b68553e3983760316e19f0dcda501cffdbd44a2`.
All experiments terminal; no Simulator launched. Free space8.5GiB; goal active.

**Previous recovery checkpoint — 20261006-062416: original idle worker yields and scheduler returns.**
Original worker64B64E finds operation0, captures its context through66CB5E,
sets its due sequence to1 and restores the saved scheduler. The resumed66CBD2
block finishes its eight-slot scan and returns to606A28. It stops at the next
uncompiled routine64BF74:PC606A28,PR606A34,SP89691C,SR60000061,SPC645DB8.
No synthetic callback, status/queue completion or connection flag was injected.

The11-function/547-instruction worker reduction passes2,592 cases:1,080 signed
delay/negative/wrap and1,512 idle/out-of-range operation cases, both FPU banks,
exact context/stack/RAM/flash/sound. The actual frozen worker yield and scheduler
return also pass. Uncompiled operation1 explicitly rejects at64BFE0.
An initial C++ declaration error and two incorrect test expectations are retained:
32 slots instead of the actual8, and instructionPC744 instead of the generated
diagnostic's source block73E. A focused rejection control confirms the latter;
no runtime repair was used to change either result.

Read-only startup captures at first worker entry, yield and scheduler return
match three prospective16MiB RAM images exactly, with no excluded bytes.
All GPR/control fields and both physical FPU banks match; device clock/ARM7/AICA
counters were not predicted. Later startup is outside that prospective proof.
The exact binary passes two supplied scenes, ten external audits, release controls
and six unchanged images. Callback/ready, language, sound and material audits pass.
Original inputs, declared flash and root VMUs unchanged. Closure760 functions/
79,436 instructions/2,210 external records, cap768; twelve core patches unchanged.
Mode0,sectors89,1,280,711 reported ARM instructions, zero faults and zero nonzero
audio frames. No operation completion, title, normal boot, gameplay, hardware
parity, real input/save, audio playback, production adoption or G1 acceptance.

Next reduce64BF74→664AEC and original status/address helpers64A596/64A5A8/
6630E0; verify the eight-record bitmask and ABI before one changed startup.
Katana/PAL blockers and the exact source-only request remain unchanged.
Checkpoint062416/peripheral-worker-startup-checkpoint.json binds510 identities
and the previous checkpoint; previous381 identities reverified. SHA256:
`3f5cc3dcce7e29b97c81710eccdd51c1bae04321db7ea55da5a3b542c675e1e6`.
All experiments terminal; no Simulator launched. Free space9.2GiB; goal active.

**Previous recovery checkpoint — 20261006-061041: original scheduler restores a worker context.**
The scheduler reduction passes1,680 cases:576 context restores,816 idle/deferred/
cancel/release cases and288 eligible transfers. Original control/FPU semantics,
both physical FPU banks and declared RAM effects are checked. No synthetic
callback invocation or status/queue completion was added.

The changed startup performs the original context transfer and stops before
uncompiled worker64B64E:PC/PR64B64E,SP8997B4,SR60000060,SPC645DB8.
The prospective whole16MiB RAM comparison passes outside80 explicitly unknown
caller-control/FPU capture bytes. Restored GPR/control and all16 active FR words
match the existing descriptor. The64-byte inactive FPU bank was not predicted
in the full run; isolated reductions cover both banks. Worker execution remains
unverified.

The exact binary passes two supplied-scene baselines, ten external audits,
release controls and six unchanged images. Callback/ready, language, sound
ring/metadata/handles/driver and material/companion audits pass. Original inputs,
declared flash and root VMUs are unchanged. Closure756 functions/79,144
instructions/2,190 external records, cap760; twelve core patches unchanged.
Mode0,sectors89,1,280,676 reported ARM instructions, zero faults and zero nonzero
audio frames. No title, normal boot, gameplay, hardware parity, real input/save,
audio playback, production adoption or G1 acceptance follows.

Next reduce the actual selected64B64E worker operation and its yield/context
return dependencies, then run one changed startup. G1 still needs matching
Katana sources and the supported PAL dump; BOOTSTRAP-BLOCKERS.md remains the
exact source-only request. Checkpoint061041/handler-scheduler-startup-checkpoint.json
binds381 identities, the previous checkpoint and storage supplement; previous708
identities reverified. SHA256:
`ced6c0ee71e493ea489bd3835289bfb7260c1be5b95318fde56ddbd37da2eecf`.
All experiments terminal; no Simulator launched. Free space10GiB; goal active.

**Previous recovery checkpoint — 20261006-054159: registration returns into a frame wait.**
053823 executes original010F22 and its five helpers. The full1,024-byte callback
record area, outer guards, cleared interrupt slot and registration-ready/return
flags match the prediction saved before execution. The title caller returns
through056420 to055820 and stops at its original readiness query607124.

The two-function/21-instruction status reduction passes1,792 raw and1,792
converted cases across all eight records, zero/nonzero/sign-bit values and all16
interrupt masks. It changes no status word.054159 executes that original query
and progresses into its first0534F0 frame wait and the public runtime's modeled
interrupt/polling flow. It stops at local original64B648 from6069F8:
PC606A22,PR606A28,SP89691C,SR60000060; exceptionSPC645DB8.

The planned readiness call-trace comparison is unverified:104,152 further calls
pushed the query outside the retained final32 history. Its rejected oracle and
original prediction remain preserved. A separate retrospective saved-frame
proof verifies055842 atFFFFB8,053542 atFFFF98 and exceptionSPC645DB8 against
original caller/wait source. No runtime/game repair or rerun was used to turn
that rejected trace comparison into a pass. Callback/ready-store comparisons
remain prospective on both full captures.

Both changed exact binaries pass two supplied-scene baselines, ten external
audits, release controls and six unchanged images. Language,39-command sound
ring, sound metadata/handles/driver and material/companion checks pass. Original
inputs, declared flash and root VMUs are unchanged. Latest750 functions/
78,940 instructions/2,189 external records, cap752; twelve core patches unchanged.
Mode0,sectors89,1,280,656 reported ARM instructions since reset, zero faults and
zero nonzero audio frames. The modeled interrupt is bounded runtime evidence;
there is no hardware parity, presented title, normal boot, real input/save,
audio playback, gameplay, production adoption or G1 acceptance.

Next reduce original64B648/66459A/66CBA0 scheduling with the captured handler
manager/context and original callback64B64E. Preserve original control/FPU and
context-transfer behavior; do not force status, handler or queue completion.
G1 still needs matching Katana sources and the supported PAL dump; the exact
source-only request remains in BOOTSTRAP-BLOCKERS.md.
Checkpoint `054159/registration-frame-wait-startup-checkpoint.json` binds708
verified identities plus the previous053059 checkpoint; SHA256:
`216efff87e7ee0f55d0ef7711061fc0cf7adb2f780217e1c8f85d7a84703c06d`.
All experiments are terminal. No Simulator launched by this work; live simctl
was previously license-blocked. Free space11GiB; protected material preserved.
Goal active.

Baseline storage compaction preserves591 completed capture files byte-for-byte
while sharing identical APFS data blocks. Permissions, owner, extended attributes
and access/modified timestamps are retained; inode/birth times change. Logs,
predictions, sources and original/private inputs were excluded. Free space is
now11GiB, about7.8GiB reclaimed. All708 checkpoint identities and591 capture
hashes were reverified; no evidence path or contents were removed.
Supplement: `054159/baseline-clone-checkpoint-supplement.json`.


**Previous recovery checkpoint — 20261006-053059: peripheral initialization returns.**
Original startup completes `64BB56` and installs its temporary callback in slot20.
The full16MiB RAM image matches the prediction saved before execution outside
84 bytes of reviewed stack scratch. This includes eight peripheral records,
the selected record's four buffer markers, cleared metadata, original type/count
fields and callback slot. The79,008-byte handler pool is unchanged.
It stops before the caller callback `010F22`, atPC606908/PR60690E/SPFFFF8C,
SR400000F0. Full registration has not yet returned to the title caller.

The original initialization reduction passes2,210 initialization,256 mode and
48 slot cases: every low-byte selection mask, all four original record types,
caller/null buffers, invalid types, high mask bits and slot bounds. A second
reduction passes1,120 combined-callback,1,120 slot-disable and60 pointer-setter
cases, covering all16 interrupt masks and valid/invalid indices. It executes
original010F22 in isolation: clears its interrupt slot and installs010E9E/010F0E
in eight128-byte records. Stored callbacks are never invoked by these tests.
Both reductions check exact RAM outside reviewed stack scratch, flash/sound
and preserved ABI. No guest/runtime patch or forced completion was added.

The changed exact binary passes two supplied-scene baselines, ten external
audits, release controls and six unchanged images. Language,39-command sound
ring, sound metadata/handles/driver and material/companion checks pass. Original
inputs, declared flash and root VMU are unchanged. Latest742 functions/
78,843 instructions/2,190 external records, cap744; twelve core patches unchanged.
Mode0,sectors89,1,021,314 reported ARM instructions since reset, zero faults and
zero nonzero audio frames. No actual input/save, interrupt delivery, normal
boot, title, audio playback, gameplay, hardware, production adoption or G1 acceptance.

Next compile original010F22/606930/606B1A/606B20/64B990/64B9AC and follow normal
startup return. Its scoped callback/ready-store prediction is already saved;
the first subsequent continuation or interrupt remains an execution question.
The callback's slot clear is original behavior, not a host bypass.
G1 still needs the matching Katana sources and supported PAL dump; the exact
source-only request remains in BOOTSTRAP-BLOCKERS.md.
Checkpoint `053059/peripheral-callback-startup-checkpoint.json` binds397 verified
identities plus the previous052330 checkpoint; SHA256:
`f1f898a7f3fb6fc076458cef9138129d8a4986cef9748fc3905c057303eaded9`.
All experiments are terminal. No Simulator launched; full Xcode remains
license-blocked. Free space4.1GiB; protected material preserved. Goal active.

**Previous recovery checkpoint — 20261006-052330: first original handler descriptor.**
Original startup now executes the registration prologue, clears its buffers,
allocates the handler manager and constructs its first descriptor. The second
run's entire 79,008-byte handler pool, context, outer guards and captured call
state match the prediction saved before execution. The descriptor references
callback 64B64E and payload stack 8997B4; that callback has not executed.
Startup stops at original 64BB56: PC6068E8, PR6068F2, SPFFFF8C,
SR400000F1, arguments 15/1/6B4314. That routine is locally available.

The isolated original pool/context tests pass 1,632 cases; descriptor/enqueue
tests pass 792 cases. Two rejected enqueue oracles are preserved: the serial
field copies pool+204, and the 256-byte context copy includes 24 saved caller
bytes plus 80 caller-stack bytes beyond the 152-byte capture. The first full
run, 051044, rejected a zero-GBR prediction; original037200 establishes8FFE00.
That correction is retrospective, and68 live FPU bytes were not predicted in
that first run. The second complete-pool prediction has no excluded bytes.
These corrections changed test expectations, not guest/runtime behavior.

Both exact binaries pass two supplied-scene baselines, ten external audits,
release controls and six unchanged images. Language, the39-command sound ring,
sound metadata/handles/driver and material/companion checks pass. Original inputs,
declared flash and root VMUs are unchanged. Latest735 functions/78525 instructions/
2184 external records, cap736; twelve core patches unchanged. Mode0,sectors89,
1,019,211 reported ARM instructions since reset, zero faults and zero nonzero
audio frames. No full registration, actual input/save, normal boot, title,
audio playback, gameplay, hardware, production adoption or G1 acceptance.

Next reduce original64BB56 and its64BB28/64BC8C dependencies on the actual
arguments, then run one changed startup. Inspect64AC2E and010F22 before claiming
registration success. G1 still requires matching Katana sources and supported
PAL dump; source-only request remains in BOOTSTRAP-BLOCKERS.md.
Checkpoint `052330/handler-registration-startup-checkpoint.json` binds770
verified identities plus the previous050028 checkpoint; SHA256:
`b726ba531ad096db6e80e964b5d59fa11cda15c13202e819d4ad5242d00ebd32`.
All experiments are terminal. No Simulator launched; full Xcode remains
license-blocked. Free space4.7GiB; protected material preserved. Goal active.

**Previous recovery checkpoint — 20261006-050028: original peripheral queries and registration preparation.**
The six-function/186-instruction query reduction passes1114 geometry,
1114 compatibility and528 route cases. It covers captured/absent devices,
every descriptor-byte value, packed function order, route-table/status branches,
exact RAM outside reviewed stack scratch, flash/sound guards and preserved ABI.
No BIOS replies or live input are synthesized by these tests.

045821 completes the original query loop. Its entire6400-byte work pool and
outer guards match the pre-run prediction: unit1 compatible, units7/13/19 absent,
and route1 for all four. Original route lookup returns1 even for absent slots;
this is not device-presence evidence. Three header words are cleared on return.
050028 follows original010B00: exactly608 bytes at6B6134 become zero, guards and
remaining flags stay unchanged, and6068A8 receives15/1/6B4314/010F22 as predicted.
It stops before that callee, atPC010B10/PR010B22/SPFFFFA8.

Both exact binaries pass two supplied-scene baselines, ten external audits,
release controls and six unchanged images. Language mapping,39 queued commands,
sound metadata/handles/driver, material/companion globals and original flash/root
VMU remain verified. Latest727 functions/78082 instructions/2167 external records;
cap728, twelve core patches unchanged. Mode0, sectors89,949883 reported ARM
instructions since latest reset, zero faults and zero nonzero audio frames.
No registration completion, filesystem/save, actual input, normal boot, title,
audio playback, gameplay, hardware, production adoption or G1 acceptance.

Next reduce6068A8's registration prologue and first64B50C initialization against
the captured arguments. Verify buffer and interrupt-mask effects; inspect
64BB56/64AC2E and callback010F22 before claiming successful registration.
Private checkpoint binds355 verified identities plus045050's prior checkpoint:
`050028/peripheral-registration-startup-checkpoint.json`, SHA256 `77fbc2a6ccfd2d44850799fd0209909f0a88c603532f7521edc5ce7290b834b1`.
All new oracles are terminal. No Simulator launched; simctl remains license-blocked.
Free space6.3GiB; protected material preserved. Goal remains active.

**Previous recovery checkpoint — 20261006-045050: original language mapping and peripheral working records.**
044744 executes the pointer store and language-query chain. All four predicted
words match: pointer8942AC=786A04, mapping758C80/758C7C=1/1 and temporary
cache67714C=0. The isolated query passes712 native and360 test-only alternate-ABI
cases. Hardware READ ABI parity remains unresolved; the alternate callback is
never installed in full startup.

045050 executes original10B91E/109AE0/10A428/10A3F2. An independent audit compares
the entire6400-byte pool and both outer guards: four1536-byte cleared payloads,
indices0–3, original unit words1/7/13/19 and cleared trailing fields match the
prediction saved before execution. It stops at the first status query6067F4,
PC10A3F2/PR10A404/SPFFFF78. Captured unit1 advertises functions0E; units7/13/19
are absent. This verifies working records, not functioning input or VMU storage.

Both exact binaries pass two supplied-scene baselines, ten external audits,
release controls and six unchanged images. Existing39-command ring, sound
metadata/handles/driver, companion globals, flash and root VMU remain verified.
The older material audit rejects its8.0 assertion because original056242 now
calls64449A(16); a separate source-derived check verifies16.0 and remaining
material fields/header/globals. This factor derivation is retrospective.

Latest723 functions/77908 instructions/2169 external records; cap728,
twelve core patches unchanged. Mode0, sectors89,949341 reported ARM instructions
since latest reset, zero faults and zero nonzero audio frames. No normal boot,
title, audible playback, gameplay, G1 or production-adoption acceptance.
Next reduce6067F4/6066D8/64A9CA using the actual VMU/absent-device records and
original bit-field/lookup behavior, then run one changed startup.

Private checkpoint binds345 verified identities plus042919's prior checkpoint:
`045050/language-peripheral-startup-checkpoint.json`, SHA256 `41936ed8b2254ad79d8466b348755639de0a0b7d6815a65e66c141b0410d7329`.
All new oracles are terminal. No Simulator launched; simctl is license-blocked.
Free space7.3GiB; no protected material deleted. Goal remains active.

**Previous recovery checkpoint — 20261006-042919: original sound programs and default material setup.**
Original094D0C initializes programs for32 channels through647958/6478DC.
042001 independently verifies the entire7168-byte ring: the seven prior records
plus32 exact program commands, pending39 and producer39. The first descriptor
is00790003, correcting040512's unexecuted channel4 prediction: the actual stack
argument wasB0, while scratchR0=4 was not the caller's channel. The old checkpoint
remains intact; the corrected prediction was saved before the changed run.

The isolated sound-program reduction passes1229 descriptor and300 queue cases,
including all command bytes, variadic truncation, missing arguments, queue full,
wrap and preserved ABI. The original material compiler658BA0 and dimension
encoder6588A0 pass232 packet and90 dimension cases under current startup flags.
No host queue completion, game-mode change or no-op routine was added.

042659 executes original6443EC default material setup and verifies its fields,
four header words and copied globals.042919 executes the following042100 leaf
and verifies six more copied globals, including the original16.0 factor. The
three packet words predicted independently before allocation match exactly;
the fourth is calculated from the original allocated texture descriptor.
This verifies descriptors, not texture contents in VRAM or graphics output.

All three changed exact binaries pass both supplied-scene regressions, ten
external audits, release controls and six unchanged images. Latest714 functions/
77701 instructions/2169 external records; cap720, twelve core patches unchanged.
Flash, original input files and root VMU remain unchanged. Mode0, sectors89;
932335 reported ARM instructions since latest reset, zero faults and zero nonzero
audio frames. No normal boot, title, audible playback or gameplay acceptance.

Next execute reviewed original610F32, a three-instruction routine storing its
input pointer at8C8942AC, then follow the actual continuation. Current failure
PC8C0564DE/PR8C0564E6/SP8CFFFFAC, inputR4=8C786A04. The source is local.
G1 Katana/PAL, R3/R4 and production adoption remain unresolved. Synthetic flash,
fixed RTC, interpreted sound CPU and unproved IP.BIN handoff remain limitations.

Private checkpoint:042919/program-material-startup-checkpoint.json binds1021
verified identities plus the previous674-identity checkpoint. SHA256:
ed3169a55457a41eb746ae8fc1512b5e0751a80b69282e471e0b35e8e0923b23.
A preparation error used a gap-fixture map;041850's unchanged generation was
terminated before oracle build/execution and is explicitly recorded as aborted.
All three changed runs, regressions and reductions are terminal. No Simulator
was launched; simctl enumeration still returns69 because the Xcode license is
not accepted. Free space8.6GiB; no protected material was deleted.

**Previous recovery checkpoint — 20261006-040512: original stereo settings and seven queued sound commands.**
The original settings query passes126 isolated cases for erased/empty partitions,
stereo/mono records, invalid mode, bad CRC, missing arguments and busy/bounds
conditions. A separate, declared synthetic stereo flash fixture is accepted by the
original checksum and query routines. It preserves the factory fixture and is
not owner-console settings or retail BIOS evidence.

040033 follows original stereo selection and queues command8A.040512 follows
original defaults and queues six more commands:84,82 and four83 channel settings.
An independent audit compares the entire7168-byte ring, pending count7 and
producer index7. Flash and root VMU remain unchanged. The first stereo comparison
was recorded after execution began and is retrospective; the seven-command
prediction was saved before execution. Neither run injects queue completion.

Both exact binaries pass the existing supplied-scene baselines, ten external
audits, release controls and six unchanged images. Latest708 functions/
76704 instructions/2181 external records; cap712, twelve core patches unchanged.
Mode0, sectors89, zero ARM faults and zero nonzero audio frames. No playable
startup, title, audible playback, game coverage or production acceptance.

Next reduce original variadic sound-program descriptor routine647958, then
review following6478DC. Current missing target8C647958 is called by094D34 at
PC8C094D48/PR8C094D6C/SP8CFFFF84. The then-unexecuted prediction00790403
is superseded by042001: the captured stack requests channel0 (00790003). The needed routine is
locally available. G1 Katana/PAL and R3/R4 remain unresolved. Synthetic flash,
fixed RTC, interpreted sound CPU and unproved IP.BIN handoff remain limitations.

Private checkpoint:040512/stereo-settings-startup-checkpoint.json binds674
verified identities and the previous1337-identity checkpoint. SHA256:
9de8fe56a4f83414e689713e761c39b1f035225760ea8f4a813b52b48e96e517.
Both full runs and the reduction are terminal; no Simulator was launched.

**Previous recovery checkpoint — 20261006-034703: original settings-partition formatting.**
Original startup reaches the mono/stereo settings query after its verified sound
metadata and eight channel handles.033953 reads the exact erased64-byte header.
034703 follows the original invalid-header/format path: BIOS erase/program
services produce exactly18 header bytes, with an independently verified128KiB
flash image and16KiB cache. The declared input and root VMU remain unchanged.
Formatting does not create the missing configuration block or prove audio settings.

Patch12 models in-memory bit clearing and partition erase.31 read-only and32
write/erase controls pass; the old runtime rejects a valid programming command.
Both new exact binaries pass existing supplied-scene baselines, ten external
audits, release controls and six unchanged images. Latest699 functions/
76296 instructions/2186 external records; cap712, twelve recorded core patches.
The initial external audit erroneously included a NUL in the16-byte cookie;
its rejected source is preserved. Original bytes and the public contract correct
the oracle, with no guest/runtime change.

Next reduce original logical-block/settings readers607F14/607F90/60305E with
absent, invalid and valid configuration cases. Current failure is607F14 at
PC64FB4A/PR64FB54/SP8CFFBF30, before that query. Mode0,sectors89; reported ARM
instructions755,990 since latest reset, zero faults, zero nonzero audio frames.
No normal boot, title, audible playback, gameplay or disk flash persistence.
G1 Katana/PAL, R3/R4 and production adoption remain unresolved; synthetic flash,
fixed RTC and unproved IP.BIN handoff persist. No Simulator was launched.


Private checkpoint: `034703/sound-settings-startup-checkpoint.json` binds1337
verified identities across four exact-binary baselines, both reductions, original
inputs, source snapshots, audits and the unchanged root VMU. SHA256:
`2f16b516a531832a15f0455316ae955eb28e1bcfdc49a713c225bea0005643b6`.
All oracles are terminal; no Simulator was launched. Repository safety and
whitespace checks pass. Goal remains active; G1 and full startup are unresolved.

**Previous recovery checkpoint — 20261006-033505: sound metadata and channel handles.**
The isolated original byte/word readers pass234 cases covering byte lanes,
aligned words, upper bounds and interrupt-mask preservation. Full startup033003
and033505 read back the four raw version bytes and six firmware buffer offsets,
matching the prediction saved before execution. Two driver uploads follow the
two original call sites; their count is retained rather than suppressed.

033505 allocates all eight kind1 sound-channel handles through original647800
and65E770. Captured pointers and allocation flags match the original record
layout; kind2/3 pools remain unallocated. It next stops at uncompiled original
8C60300C from094C88. Mode remains0: no normal boot, title, audible playback or
gameplay acceptance. The sound CPU interpreter remains an experimental lane.

Both latest exact binaries pass the existing two supplied-scene regressions,
ten external audits, release controls and six unchanged reference images.
033505 contains683 functions/75718 instructions/2182 external records;
cap688 and eleven core patches. Source snapshots and independent metadata/
handle audits are preserved alongside each run. Next: inspect and verify the
actual60300C startup target. Original G1 Katana/PAL source closure and R3/R4
remain unresolved; no Simulator was launched.

**Previous recovery checkpoint — 20261006-031607: original sound firmware starts.**
Original sound enqueue/dequeue and DMA routines upload all33,376 payload bytes of
MASARU.DRV to sound RAM.031248 independently verifies the entire2MiB destination,
including the preserved SEGA fill outside the payload. The33,408-byte source file
still matches the declared owner disc. One G2 DMA completes; total reads89 sectors.

The original completion interrupt110808→66CC80 returns, marks the command complete
and empties the active queue. Original65DFBC releases sound reset.031607 reports
745,969 interpreted ARM instructions, zero faults and PC1574. The firmware writes
SEGA at sound offsetF8; an independent audit binds that marker to its original
LDR/LDR/STR sequence at17C–184 and exact literals. This establishes bounded firmware
initialization, not full ARM7 accuracy, timing, sound commands or playback.
Native SH4 AOT and interpreted sound CPU execution remain distinct.

Three changed full runs are terminal:031023 reaches66ACD8 after exact enqueue;
031248 reaches110808 after exact upload;031607 reaches65E834 after completion,
release and the original startup delay. All three exact binaries pass both
supplied-scene regressions, ten external audits, model-release controls and six
unchanged reference images.31 BIOS checks pass per run. Latest677 functions/
75368 instructions/2168 external records; cap680, eleven core patches unchanged.
No host command-completion, callback, game-mode or queue injection was added.
110808 was absent from the historical symbol map: its focused map is bound to the
actually executed target; the historical map remains unchanged.

**Next narrowed step:** verify original sound-byte/word readers66AEC8/66AE90 and
66CE4C against the captured firmware state before compiling65E834's metadata
readback. The source is available locally. Current failure PC647C2C, PR647C36,
SP8CFFFF84; mode remains0. No title, normal boot, audible playback or gameplay.
G1 Katana/PAL, R3/R4 and production adoption remain unresolved. Synthetic flash,
fixed RTC and IP.BIN handoff limitations persist. No Simulator was launched.

Private evidence: `031607/sound-firmware-startup-checkpoint.json` (768 verified
identities), firmware/driver audits and all three exact-binary baselines.
Checkpoint SHA256 `52a5fcd946ecdee502ff77186cdd0d1b73e4e681d2e0ec06230cf8ebcac8d6a2`.
Root VMU and original inputs remain unchanged.

**Previous recovery checkpoint — 20261006-030606: sound CPU boot fault corrected.**
Original startup passes the G2 status reader, asserts sound reset and fills all
2MiB of sound RAM with the original SEGA word from8C67FF04. MASARU.DRV remains
byte-exact in main RAM (33,408 bytes); total reads remain89 sectors. Startup now
stops at uncompiled original8C66ADC0, a locally available sound-command queue
routine. PC8C65E3A2, PR8C65E3AA, SP8CFFFF58; mode remains0. No title or normal
boot, uploaded sound firmware, playback or gameplay is established.

The seven-instruction G2 reduction passes512 full RAM/context controls before
patch10 and512 explicit read-only empty-FIFO cases after it. The public candidate
completes bus writes synchronously; latency/contention accuracy is unproved.
Enabling the ARM7 interpreter exposed an earlier causal fault in025557: blank
boot RAM let execution fall beyond2MiB. Patch11 installs the pinned public HLE
boot's ARM self-loop word. The isolated check executes52,768 instructions atPC0,
verifies exact sound RAM and reset behavior, and reports no fault. In030606 the
original startup also has zero ARM7 faults and holds the sound CPU in reset at
the queue boundary. No synthetic sound-queue completion is enabled. ARM7 remains
interpreted, separate from native SH4 AOT; production suitability is unresolved.

Both supplied-scene regressions, ten external audits, model-release controls and
six unchanged reference images pass against the exact latest binary. All31 BIOS
service checks pass.666 functions/74755 instructions/2170 external records;
cap672 and eleven core patches.030439's diagnostic-string compile error is
preserved with its source snapshot;030606 repairs it.025557's sound fault remains
recorded despite that run's later missing-target message. Independent captures
verify the initial idle word and the original SEGA fill in both full runs; an
initial all-FF audit expectation was rejected and corrected from original bytes.

**Next:** compile reviewed original66ADC0, verify its queue writes against the
captured inputs, then follow the actual driver transfer/release path. The needed
routine is available locally. G1 Katana/PAL and R3/R4 remain unresolved; this
experiment does not select a production foundation. Synthetic flash, fixed RTC
and unproved IP.BIN handoff remain explicit limits. No Simulator was launched.
Private evidence: `030606/sound-boot-checkpoint.json`, sound driver/state audits,
exact-binary baseline and `20261006-g2-reduction/`. Root VMU remains unchanged.

**Previous recovery checkpoint — 20261006-024628: original startup loads and binds its sound driver.**
Original094C40→095B00 loads all33,408 bytes of `MASARU.DRV` from the declared
owner disc into0CD00000. Independent extraction matches every byte (SHA256
`5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9`).
The original sound allocator initializes64 command records,64 queue records,
256 work records and21 channel descriptors.024409 independently verifies the
initial templates and two allocated commands.024628 verifies the resulting
32-byte driver descriptor at8C8D7C28. Total disc reads89 sectors; mode remains0.

Three changed builds are terminal:024144 reaches647CAC after loading the driver;
024409 completes allocator setup and reaches647364;024628 binds the driver and
reaches1115C8 through the original G2 transfer path. The failure is atPC110B0C,
PR110B10, SP8CFFFF28, SR400000F1, before the first sound reset-register write.
All three exact binaries pass both supplied-scene regressions, ten external
audits, model-release controls and six images unchanged from000204. BIOS service
checks remain31. Latest665 functions/74748 instructions/2170 external records;
cap672, nine core patches unchanged. No Simulator was launched.

**Next narrowed experiment:** independently verify original1115C8's seven
instructions and the0xA05F688C G2 status-register contract before resuming startup.
The current public runtime has no dedicated handling for this address; it falls
through to the generic MMIO map/default zero. Do not interpret that as proved
hardware readiness. Bind a small register/ABI test and decide the required runtime
behavior from source. The harness also leaves ARM7 execution disabled: explicitly
address that before accepting sound firmware startup or a later handshake.

No title screen, normal boot, sound playback or gameplay is established. G1
Katana/PAL, R3/R4, mobile acceptance and production adoption remain open. Earlier
synthetic flash and fixed RTC limitations still apply. Root VMU is unchanged;
all game processes are terminal, and free space is16GiB. Evidence is private under
`024144/`, `024409/` and `024628/`, including sound audits, baseline regressions,
source snapshots and `024628/sound-startup-checkpoint.json`.

**Previous recovery checkpoint — 20261006-023049/continuation-1: original startup reads 72 disc sectors.**
Original entry010000 now completes the measured system-information, cache/vector,
heap/video/controller and RTC registration prefix, resumes both interrupt returns,
and reaches original0534C0(2,2). RAM independently verifies cadence2/2/2/0,
callback053570 at handle12 and RTC callback10D6A0 at handle9. No host mode or
callback writes were added. The next missing AOT target is **8C094C40**, called
at05649C; mode remains0. There is no title screen or playable game yet.

Patch9 implements explicit-image BIOS SYSINFO and read-only flash services.
The declared128KiB input is a synthetic factory-only US experiment with identity
SPADLAB1, erased settings and no claim to owner-console configuration. Unsupported
services reject.31 isolated contract tests pass; omitting the image rejects the
first SYSINFO call. Flash READ returns the documented byte count; Flycast's zero
return differs, and hardware parity remains unresolved. RTC retains the public
candidate's fixed default value; advancing clock accuracy is not established.

The exact binary passes both supplied-scene baselines, ten external audits,
model-release controls and six images unchanged from000204. Earlier015039's
continuous two-cycle evidence was not rerun.632 functions/73034 instructions/
2138 external records, cap640, nine patches. No Simulator or game process remains.
The original023049 run stopped prematurely after RTE because the harness only
followed its async flag; continuation-1 also follows the saved PC until the actual
entry return. The parent result is not evidence that original main returned.

**Next:** inspect and compile094C40 and its measured dependencies, then retry
original startup and its nearest baseline. G1 Katana/PAL, R3/R4, normal boot,
gameplay, mobile acceptance and production adoption remain open. This is a local
implementation step, not a renewed private-source acquisition requirement.

Evidence: private023049/continuation-1 `original-flash-startup-audit.json`,
`baseline-regression/regression-report.json`, `missing-flash-control/` and
`system-flash-startup-checkpoint.json`.

**Previous recovery checkpoint — 20261006-015039: two supplied running cycles return.**
Original0506BA and original endframe0534F0 now execute twice on the same scene
and uninterrupted guest clock: four updates, two new render completions, and
cadence reloads performed by original code. Independent RAM/log audit verifies
timer6→4→2, removal of two deferred tasks, eleven retained root links, exact RAM
continuity between cycles, finite changing player positions and monotonic cycles.
Two corruption controls reject altered timer state and a backwards clock.

Rendered captures visibly show terrain, clipped Sonic, rings000 and lives00;
the HUD timer advances00:00:04→00:00:08. Camera/placement/contact remain wrong or
unproved. This is a supplied scene, not normal title/new-game startup or gameplay.
The original mode global7649F0 is still0;053570's mode4/5/9 stage callback branch
has not executed. Do not infer that branch from these images.

The exact binary passes both flag-off baseline cases, ten external state/geometry
audits, model-release controls,168 controller controls per case, and six images
unchanged from000204. Five first-tick audits and HUD payload/corruption controls
also pass. The optional run stops at its explicit two-cycle inspection guard;
its process exit1 is intentional and is not a complete harness/gameplay pass.

**Next:** trace original0558C0's mode transitions and04E60E startup, including the
normal stage-specific callback at cadence zero. Establish the actual producer
path before running mode4; do not host-write7649F0 or a callback pointer. The
current two-cycle result is sufficient to leave the missing-cadence-callee chase.
Normal startup, camera/contact, full cleanup, actual input/audio, G1 Katana/PAL,
R3/R4 and production adoption remain open.

Five changed builds are terminal. Latest586 functions/71476 instructions/2086
external records; cap592 and eight core patches. Per-run VMU files remain isolated
and the original root VMU hash is unchanged. Latest free space22GiB. No Simulator
was launched; Xcode license still prevents device enumeration.

Private015039/two-cycle-checkpoint.json: SHA256
`9f6ed1adc80b08a0aedbdbd1808c3ab0f4788eb02e064636b20d077e12179117`,
1518 verified identities binding all five runs and the relevant regression,
source, input, audit and image evidence.

**Previous recovery checkpoint — 20261006-013526: cadence callback returns.**
Original053570 now executes on the first vblank tick, decrements counter758F40
from2 to1, returns to649878 and resumes the original wait at05355A. The pending
Maple interrupt also returns. Execution next rejects uncompiled original096540
at053566. This is progress through the running wrapper, not its completion or
proof of consecutive rendered frames.

Compiled five original routines:053570,6069F8,09EA60,6054CC,6054E4. Callback
closure580 functions/70230 instructions/2029 external records; cap592. Eight core
patches unchanged. Observation wrappers do not alter guest state. The exact
binary's flag-off regression passes both supplied startups, ten external audits,
model-release controls,168 decoder controls per startup and six images matching
000204. Five loop first-tick audits and HUD payload/corruption controls also pass.

Each run now uses its own ignored VMU image under its output directory. The
previously created root131072-byte VMU is preserved and ignored; its SHA256 is
unchanged. This isolates test state and does not establish game save acceptance.

**Next:** inspect and compile original096540 and only its measured dependencies;
then continue the original wait/wrapper and repeat the nearest baseline check.
G1 Katana/PAL, normal startup, consecutive frames, camera/contact, gameplay,
physical input, R3/R4 and production adoption remain unresolved.

Private013526/cadence-return-checkpoint.json: SHA256
`40cfba67f8b10d24490e6e05be09dc009c378eef50703a28ded16f0a7b7e6707`,
341 verified identities. Changed build and cached baseline are terminal.

**Previous recovery checkpoint — 20261006-012816: baseline restored; cadence callback reached.**
The repaired graphics/Maple interrupt dispatch now passes both supplied startup
cases. All five independent state/geometry audits pass per case, model-release
controls pass, and all six renders match000204 exactly. The original camera/
placement limitations remain; these are fixture renders, not normal gameplay.

Compiled original608DDA and the controller routines newly reached by active
polling. Optional frame callback6771B4 is zero in all six frame calls. Before
live decoding, original605930 passes168 isolated cases per startup: trigger
thresholds/hysteresis, button polarity, supported/absent axes, exact RAM writes,
stack/return and preserved registers. The live neutral records in both captures
show capabilities000F06FE, zero buttons and six zero analog outputs; physical
input and touch routing are not established by this observation.

An opt-in loop probe uses the exact passing baseline binary. On its first device
clock tick, original64985E walks event0900's chain and reaches053570 through
649872. The former65-tick disconnected-dispatch stall is resolved. It now stops
explicitly because053570 itself is uncompiled. Counter2 has not decremented;
no running-wrapper return or completed consecutive frame is claimed. The loop's
five first-tick audits and the HUD payload/corruption audit pass separately.

**Next:** compile original053570 and inspect its required6069F8/09EA60 calls and
producer state. Measure the actual decrement and interrupt return, then continue
the running wrapper. Current575 functions approach the576 cap; increase only for
the measured required source set. Do not force the counter or callbacks.

Five changed native builds and one cached loop probe are terminal. Latest575
functions/69977 instructions/2016 external records; eight core patches unchanged.
Safety/diff checks pass; no Simulator/game process; latest disk11GiB. G1 Katana/
PAL, normal startup/bank1, camera/contact, full teardown, R3/R4, gameplay and
production adoption remain unresolved.

Private012816/input-cadence-checkpoint.json: SHA256
`1c3234c2e02e7fa45125b042a7eadab2f73c9a6d21bb6ffc210532d1d3c529d2`,
431 identities verified. Prior failed builds and the callback-reset proof remain
preserved; no inputs or previous evidence were deleted.

**Previous recovery checkpoint — 20261006-011538: callback-reset cause repaired; baseline reopened.**
The timing probe in010621 records65 vertical-blank ticks and215955541 guest
cycles while the cadence counter stays2. This is not an insufficient run budget.
The13 captured RAM/VRAM files match005752 exactly; observation did not alter it.
SPG scanline fields are cached last-read values, not continuous measurements.

011146 proves the cause: the harness repeats work-area initializer654DE4 after
video/peripheral startup. Its644EA0 resets the graphics callback at8A720C from
original event-chain dispatcher64985E to empty original default6453DC, while
leaving event0900's linked registrations intact. Appending cadence callback053570
cannot repair that disconnected dispatch. No runtime clock patch is indicated.

The camera/startup harness now retains the initialized work area.011335 restored
controller interrupts and returned from the first frame, then failed an old
assertion requiring the last interrupt to be0320.011538 checks each interrupt's
enabled pending source instead: five IRQ6 entries plus one Maple IRQ4 entry
(event0360, pending/enabled bit1000), with priority checked. The first frame
returns after17 continuations with restored stack/PR/preserved registers.

**Current boundary:** original608DF0 reaches uncompiled608DDA at608E3E during the
next, terrain-only frame. The flag-off baseline fails at exactly the same target.
**Current baseline acceptance is reopened.** Earlier image/state checks remain
historical bounded observations; their disabled event chain did not prove timing.
The repaired run has not reached the cadence wait or completed the second frame.

**Next:** compile the locally available11-instruction original608DDA wrapper;
observe callback global6771B4 and follow its original producer if nonzero. Restore
the full baseline, then retry the cadence continuation. Do not reinstall the
empty default, force a callback/counter, or raise the continuation limit.

Four changed builds and the latest baseline are terminal. Callback closure remains
565 functions/69634 instructions/2009 external records; eight core patches and
cap576 unchanged. Safety and diff checks pass. No Simulator/game is running;
latest disk11GiB. G1 Katana/PAL, R3/R4, normal startup/bank1, camera/contact,
full teardown, gameplay and production adoption remain unresolved.

Private011538/callback-repair-checkpoint.json: SHA256
`ee9d8729f92a109dece43487c2781d447a7dce5e6affacefa15df5abf0745988`,
962 identities verified. Diagnostic cause checkpoint011146:
`19874b7e667a92b33379fed520816c265e8f0bfb24cc15896db6e81008064502`.

**Previous recovery checkpoint —20261006-005752: running-game route corrected; cadence wait exposed.**
Source review found that the earlier050474→0508DE experiment combined a running
update with the pause redraw/menu path. It was not normal frame sequencing.
Original running wrapper0506BA drives updates from its cadence count, then the
shared HUD. The current opt-in experiment uses that wrapper after the retained
supplied first-frame prefix; normal boot and full startup order remain open.

Original0534C0 receives(2,2), exactly as0503C0 supplies. Independent snapshots
verify cadence2/2/2/0 and callback053570 appended to the original0900 event chain
at handle11. The wrapper completes one update, then waits at05355A for counter
758F40 to fall from2 to1. After64 interrupt entries and64 saved-PC resumes, the
existing128-continuation guard stops it. Counter2 remains unchanged; no normal
wrapper return, second rendered frame or gameplay acceptance is established.

**Next:** capture device-clock cycles, PVR scanline/vblank/ticks and Holly pending/
acknowledgment/event counters at entry to053556 and at budget exhaustion. Trace
why the registered053570 callback has not decremented the counter. Do not reset
that counter or blindly raise the guard. The cause is not established yet.

Completed earlier this turn: original60A748 passes40 isolated bit-mask/ABI/exact
RAM controls and its live header matches the prior prediction. In005138 the
shared first HUD sprite produces the exact header and four vertices: corners
(16,32)–(80,96), reciprocal depth0.5 and UV0–1. Header/vertex corruption controls
fail as intended. This proves packet construction, not HUD rasterization.

The latest exact binary passes both flag-off baselines, all five independent
audits per case, model-release controls,40-header controls per case and six
images unchanged from000204. HUD load still passes56 links/54 textures/162016
bytes with two corruption controls.565 callback functions/69634instructions/
2009external records are static scope; cap576 and eight core patches unchanged.
All three changed runs terminal. No Simulator/game app running; disk13GiB.
G1 Katana/PAL, normal startup/bank1, camera/contact, full teardown, R3/R4 and
production adoption remain unresolved.

Private `20261006-005752/cadence-checkpoint.json`: SHA256
0aac9f9028982f17fd2414b89d8ee67158f0d590de0835ef230f723159620a26,
371 verified identities. Earlier claims of two isolated outer-update returns
remain bounded observations; they do not establish normal frame timing/order.

**Previous recovery checkpoint —20261006-004452: original HUD load and selection verified.**
The next display path required CON_REGULAR.PRS, already present in the owner's
local dump. Original09AD7E's startup table identifies the archive/list pair.
The opt-in experiment now calls original09B3F0 with those exact arguments before
outer display. This supplies one missing producer; normal common-startup order
and all remaining common resources are not established.

Independent decoding and capture comparison verify56 links to54 unique textures,
162016 transferred bytes, preserved earlier registrations and unchanged VRAM
outside new payloads. Entries12/13/16 intentionally share ID3B7 and identical
payloads. A bad link and corrupted payload are rejected by negative controls.
Original60A7C8 then selects HUD sprite11, ID3A3, with the matching header fields.

**Next:** reduce original60A748 to an isolated header bit-mask check using the
captured inputs and recorded expected values, then compile it into the same
outer-display continuation. Current stop:PC642C54/PR642C5A/SP8CE3EFA4,
R4=02100028. No HUD rasterization or complete second frame is proved.

The exact expanded binary passes both flag-off baseline startups, five independent
audits each, model-release corruption controls and all six images unchanged from
000204. Two original outer updates still return. Callback scope554functions/
68733instructions/2005external records; bound576 and eight core patches unchanged.
All three changed attempts are terminal. No Simulator/game app running; disk13GiB.
G1 Katana/PAL, normal startup/bank1, camera/contact, full teardown, R3/R4,
continuous gameplay and production adoption remain unresolved.

Private `20261006-004452/hud-checkpoint.json`: SHA256
7de672a9eb96dab8d9d8355a42b18715d323b0490def8c763e20bb917a54e58d,
361 verified identities. Original000204 remains the complete baseline wrapper pass.

**Previous recovery checkpoint —20261006-002919: two original outer updates return.**
The original random-number routine10EF54 passes32 isolated draws covering four
seeds, unsigned overflow, exact RAM writes and preserved MACL/stack/registers.
The live seed comes from original graphics initialization, not host poison.

The experiment exposed a supplied-startup ordering error: repeated partial
updates registered Sonic two/three times, causing artificial self-collision.
Complete the prefix with original034480 (which ends in02F9BA), then use original
050474 for both updates. All nine registration counters now clear after each
pass without host resets. Both outer updates return. The second removes the two
deferred root tasks and advances the RNG once without spawning an effect.
Root links are independently checked; heap/free-list integrity remains unproved.

Outer display0508DE now stops at uncompiled08C15A after its task display returns.
The37-instruction routine is available locally. **Next:** inspect its source and
live branch inputs, compile the original entry and continue the same experiment.
No complete second rendered frame, normal startup or gameplay is claimed.

The exact binary passes both cached baseline cases with the experiment disabled,
all five independent audits per case, model-angle restoration/negative controls,
and all six render images unchanged from000204. Sonic remains clipped. Callback
closure547functions/68385instructions/1999external records is static scope;
bound576, unknown/RAW rejection and eight core patches remain unchanged.

Private `20261006-002919/outer-loop-checkpoint.json`: SHA256
29d16c202e836185618e8771051d14a63176e257d97d8fd6f34d7c29d740b307,
339 verified identities. All runs terminal; no Simulator/game app running.
Latest free disk11GiB. Original000204 remains the full wrapper pass. G1 Katana/PAL,
normal startup/bank1, real coroutine switching, camera/contact, full teardown,
R3/R4 and foundation adoption remain unresolved.

**Previous recovery checkpoint —20261006-001309: original outer update reaches a second Sonic update.**
Added an opt-in `--startup-loop` experiment. Original09B620 creates the missing
36-byte coroutine root at7A4C8C; no pointer is fabricated. Outer050474 now passes
controller processing and the empty coroutine scheduler, then advances Sonic
from y=-9.08 to approximately-9.2392 with vertical velocity-0.1592.
It stops explicitly in the effects task at uncompiled original10EF54. The source
exists locally: a16-instruction random-number routine, not a missing input pack.
No complete second update, outer display, or consecutive-frame pass is claimed.

The same expanded binary passes both baseline startup cases with the experiment
disabled. All five independent audits pass per case; model-angle restoration,
stale-yaw negative controls and all six render images match000204 exactly.
Sonic remains clipped. Latest callback compilation538functions/67056instructions/
1982external records is static scope. Bound576 replaces512 because the measured
outer-loop closure exceeded512; unknown/RAW rejection and eight core patches stay.

**Next:** reduce10EF54 to a focused original-routine arithmetic check: seed at
5FF840 advances modulo2^32 by seed*41C64E6D+3039, returning(next>>16)&7FFF and
preserving MACL/stack. Source-derived controls are recorded but not executed.
Then compile that original entry and resume the same `--startup-loop` experiment.
Keep first-tick/second-tick collision captures separate and fail on the next
unregistered target. Normal startup/bank1, real coroutine switching, correct
camera/contact, full teardown, G1/PAL and foundation adoption remain unresolved.

Private `20261006-001309/outer-loop-checkpoint.json`: SHA256
88ba2eb37b7b0316576c9136f08fe12d39ddb10de0839b838e2604d5c04f3eac,
336 verified identities. Three changed continuation runs and a separate cached
baseline regression are terminal. No Simulator/game app launched; Xcode license
still blocks Simulator enumeration. Original000204 remains the full wrapper pass.

**Previous recovery checkpoint —20261006-000204: repeat images now match exactly.**
The supplied startup omitted the installed auxiliary destructor0B44B0. Original
initializer0B450E saves nine model angles; the destructor restores those values.
Calling it before releasing the task work fixes the resident yaw carryover without
host-written angle resets. Both runs now reproduce the first235343 character,
terrain and combined renders byte-for-byte. Sonic remains clipped at the top.

Both supplied startups pass update/display/frame/release and all five independent
state/terrain/collision/texture audits. The new external release audit compares
all nine restored angles with their saved values and original BOOT bytes, and
rejects stale-yaw negative controls in both cases. Full task-tree teardown,
consecutive updates, correct camera/framing and playable-game acceptance remain
open.511 callback functions/63810 instruction occurrences/1923 external records
are static scope; cap512, unknown rejection and eight core patches unchanged.

**Next:** extend the live case to a second consecutive update/display/frame,
retaining the first tick's evidence separately. Original caller050474 invokes
09A544 then06DE22,034480,09FB84,06DE22 and029062; inspect those intervening
contracts before treating repeated bare scheduler calls as the full game loop.
Keep the first explicit dependency failure; no forced camera/model state. The
normal bank1/startup producer and production-foundation decision remain open.
G1 Katana/PAL and R3/R4 remain unresolved.

Private evidence: `20261006-000204/model-release-contract.json`, SHA256
8efe2a54e7a61b34c62a544d1d8e58fce4f47f5cb8a9ab1e5b0fbae71564773f;
295 bound identities. Run terminal exit0. No Simulator/game app launched;
Simulator enumeration still reports the Xcode license prerequisite. Latest disk
measurement12GiB free; protected inputs/evidence retained.

**Previous recovery checkpoint —235343: two update/display/frame/release repetitions pass.**
Original09A544 update and09A564 display now return in both supplied startups.
Original608DF0 completes the combined render before resource release, leaving
zero queued packets. Original destructors free all108 character/effect/board/
terrain texture records; both4 MiB VRAM banks reset. This is a passing runtime
harness, not normal startup, full task-tree teardown or playable-game acceptance.

Both archived cases independently pass12 terrain selections,929 triangles, ten
collision misses,50 exact texture transfers and the bounded update-state audit.
The original terrain release leaves50 exact index/registration-pointer pairs in
the loaded module; these are checked, not broadly excluded from the byte audit.
Original029000 resets static collision pools for each supplied startup. Each
case retains separate RAM/query/terrain captures under `startup-1`/`startup-2`.

The combined640x480 render shows textured terrain plus Sonic clipped at the top.
Terrain-only images are identical. Combined renders differ in475 pixels in
x457–487/y0–37. Resident model1D1B20 yaw starts at0 in case1, but61440 in case2;
both end the update at61440. This carryover is observed, not yet established as
the complete cause. First character captures differ in960 pixels. Camera/framing,
normal placement, positive contact and continuous gameplay remain unverified.

**Next:** trace the resident model yaw's original initialization/cleanup and its
ordering relative to pose generation; explain the localized repeat difference
without forcing model bytes or hiding it with a new baseline. Then continue the
original frame loop/camera and contact path. Normal bank1/startup producer and
production-foundation decision remain open. G1 Katana/PAL and R3/R4 are unresolved.

235343 supersedes215723 as the latest passing expanded runtime harness; visual
repeatability remains open. Callback compilation510functions/63763instructions/
1923external records is static scope, not general execution coverage. Eight core
patches unchanged; cap512/unknown-instruction rejection retained.

Private evidence: `20261005-235343/update-display-contract.json`, SHA256
11bf63b117ae00f424b5221c2fc3bdcec3fa9158937559fde1c2977bccd2194b;
311 verified identities. All five new runs terminal; no Simulator/SonicPad app
launched. [Render](artifacts/recovery-aot/20261005-235343/scene-3-render.png).
See [evidence](RECOVERY-PRS.md).

**Previous complete bounded checkpoint —215723:**

Original group3 now returns through the stage's nine child callbacks and two
character auxiliary initializers. Seven proximity triggers are independently
checked against live player collision coordinates and original radii: all are
outside activation range. Child links/callbacks stay intact; auxiliary model
pointers and installed callbacks match the original instructions.

The repeat test caught a cleanup omission in215545: group3 changes model pointer
1D1568, so the next startup rendered a different packet count.215723 invokes the
installed original0B3F8C and07CD96 destructors and verifies both model links return
to their pre-group3 values. Both repetitions now retain the previous character,
camera, motion, lighting, texture, frame/interrupt and resource-reset checks.

Both render captures match213949 byte-for-byte; the second present buffer matches
the first render. This is still partial Sonic clipped against black. The first
stage child's camera-facing test selects its nonpositive branch; its positive
render branch is untested. The next effect child has zero effect count, and the
seven proximity children remain inactive. No complete scene or gameplay claim.

**At215723, the next experiment was:** execute original09A544 before releasing the live resources. Its source
traverses update group7, then groups0–6. Keep missing-target failures explicit and
inspect the first changed dependency. Original09A564 display traversal, SET loads,
normal startup, full task-tree teardown and full scene remain open. Katana/PAL G1,
R3/R4 and production adoption remain unresolved.

Evidence: `20261005-215723/report.json` and `group3-startup-contract.json`, SHA256
7608af0cf8438e5f349b9db324bc0c812ab2c0e7a186ac5593f72a7a00048443.
244 input/source/generated/patch/evidence/binary identities verified; exact source
snapshots saved privately. Eight core patches unchanged. No Simulator or SonicPad
app launched. See [recovery loop](RECOVERY-LOOP.md) and [evidence](RECOVERY-PRS.md).

**Previous recovery result —175004:** original outer graphics setup056242 now
executes and returns. Two cases verify the default camera matrix,640x480 screen,
projection, near/far3/3000 and an exact60-byte background packet in RAM. The
original caller now produces matrix and texture-manager setup in its own order.
All prior57-texture load/release/reset, model/task,73 first-pose matrices,
collision, file and stage checks still pass.

Character display0AFD9A executes its visibility check and culls Sonic under the
default camera. **No Sonic drawing or displayed frame is proved.** The next
strict boundary is094240, PC0AB150/PR0AB16E/SP8CFFEFBC. Its source is locally
available and audited; continue that original character path and then the
original gameplay camera update. Full group1 return, whole scheduler, normal
boot, arbitrary motion and final deformed mesh correctness remain open.

The enlarged callback output crosses the generator's sharding threshold. The
harness now compiles and prefixes all18 generated program files, applies the
same diagnostic-O0 setting to each, and hashes every part plus its header.
The first execution gap was original background helper665040 and its two
callees; compiling those existing routines resolved it. The old matrix-only
expectation was corrected from the original default camera transform.

Evidence: `20261005-175004/report.json` and `graphics-startup-contract.json`.
56 current input/source/generated identities plus binary verified; callback
closure272 routines/31916 analyzed instructions (static counts). Same six core
patches and CLT lane. G1/R3/R4 and production adoption remain unresolved.

**Previous recovery result —173842:** original character execution now reaches
the display entry0AFD9A after model/motion and collision updates. Two repetitions
verify19 keyed animation channels across two blend frames and73 hierarchical
matrices (1168 finite elements). Host double-precision parent*T*Rz*Ry*Rx*S
expectations come from source key0 and node data captured before guest mutation;
maximum matrix-element error is2.23368e-7 in both runs. The collision center
matches the supplied yaw-only spawn plus the original height offset. All prior
57-texture release/reset, task, model-record, stage and file checks still pass.

The matrix traversal initially wrote through zero because its stack producer
was absent from the supplied startup sequence. Original63CD24 now runs with the
exact05624E caller arguments (8C7F0B60,128,0); base, capacity, depth, initial basis
and eventual stack restoration are checked. Outer graphics056242 is still not
executed. This repairs a demonstrated startup prerequisite without a fabricated
buffer or forced successful return.

Next **0AFD9A**, atPC0AB136/PR0AB13C/SP8CFFEFBC/R4=8CE40010, begins the character
display path. Its first visibility dependency is038FA0; use original camera/state
producers and require actual drawing evidence, not merely a culled return.
Full group1 return, a complete scheduler tick, normal boot and a Sonic frame are
open. First-pose matrices are verified; arbitrary motion and final deformed-mesh
values are not. G1/R3/R4 and production adoption remain unresolved.

Evidence: `20261005-173842/report.json` and `motion-pose-contract.json`.
38 distinct current source/input/generated hashes and binary verified. Callback
closure241 functions/27334 analyzed instructions (static counts only). Same six
core patches, CLT and diagnostic-O0. Safety/syntax checks pass; no Simulator,
game or native probe remains running. The next action is original display
execution with the verified pose and texture inputs.

**Previous recovery result —172737:** original character initialization advances
through auxiliary task creation, model setup and the first animation blend.
Two repetitions verify five occupied task nodes (the character plus two group2
and two group3 tasks), queue links, callbacks, player indices and the three
model pointers copied into auxiliary work. An independent source snapshot checks
73 model nodes,34 records,12 disabled rows and3264 exact vertex/normal bytes,
including the original FF result for optional targets absent from the tree.
All prior57-texture load/release/reset,12 stage and two joined-update cases pass.

The next explicit boundary is **8C05A040**, called fromPC8C07C41E with
PR8C07C426/SP8CFFEEFC. Animation blending returned, but its numerical results
are not yet independently verified. Continue this original model/motion
transformation with its source-derived matrix/motion oracle, then resume the
character continuation toward a completed group1 invocation. A full scheduler
tick, normal boot and Sonic frame remain open.

Evidence: `20261005-172737/report.json` and `startup-model-contract.json`.
37 distinct current source/input/generated hashes plus the binary verified;
callback closure225 routines/25756 analyzed instructions, not dynamic coverage.
The queue test initially omitted the common-work producer's group2 task; exact
records and original06DFF0 resolved that oracle error. No guest state was changed
to satisfy either new check. Same six core patches, CLT lane and diagnostic-O0.
No Simulator/game remains running. G1/R3/R4 and production adoption stay open.

**Previous recovery result —171115:** both original texture batches pass twice:
28 Sonic textures and29 effect textures fromSONIC.PRS/SON_EFF.PRS. The oracle
verifies127,776 copied VRAM bytes, unchanged bytes outside those ranges, all57
rendering headers/records/permanent list links, allocation ownership and both
banks' free/used lists. Original resource release frees all57 records/extents and
returns the resource counter to0. Both bank resets,12 stage cases, two stage
updates and prior file checks pass. Evidence: `20261005-171115/texture-batch-contract.json`
and `texture-batch-reference.json`.

At171115, C8 remained partial. Header651CA6/651C50 allowed the complete57-texture path to
return. The then-next dependency was Sonic auxiliary-task initializer0B440E at
PC0A9682/PR0A96A0/SP8CFFEFBC. It creates a group2 task with callback0B450E and
136-byte work; next verify the original task/work/copy behavior, then continue
character initialization toward a completed scheduler tick. No Sonic frame or
normal boot has been established.

The repeat initially exposed a fixture cleanup gap: allocator reset left the
resource counter at1 and the next load was skipped. Original09AC9C release now
handles that state through original manager/VRAM freeing routines; no counter
is forced to zero. Scratch reuse is checked explicitly: the effect file overwrites
part of SONIC's compressed/decoded buffers. Expected VRAM comes from independent
file references, never from observed output descriptors.

Original video608442 initializes VRAM banks; outer graphics056242 remains open.
Heap, selection and manager call order are supplied. Callback unit:214
routines/22576 analyzed instructions, not dynamic coverage. Same six core patches,
diagnostic-O0 and CLT lane. G1/R3/R4 and production adoption remain open; original
Katana/PAL acquisition is separate. Only these57 textures are covered.

**Previous game-execution result:** original SBOARD initialization now reaches
state0→2→3 on native ARM64. Twelve cases cover acts0/1 and three/four/sixteen free
tasks. With enough space, original code creates nine child tasks, sets their
parent/sibling links, callbacks, work records and position, and leaves three free
nodes. Exhausted-pool branches also complete through the original flag setters.
Heap consumption is304/336/896 bytes. Fresh original147-sector loads isolate
resource counters; exact module mutations, guards and return state are checked.

**Previous155509 startup boundary:** original lookup8C07EC7E(0) returns null from
player-work table8C7906E8 in all twelve cases. The supplied player record is not
that work table. State3 must wait for its original producer and scheduler path;
a return through zero-filled memory is not useful execution proof. No additional
player pointer was fabricated. Evidence: `20261005-155509/report.json` and
`initialization-contract.json`; prior eight callback cases pass in155425.
The broader108-entry audited SBOARD map plus proven resident helpers produces
124 callback-unit routines/4902 analyzed instructions; these are not executed
coverage counts. G1/R3/R4 remain open. C8 in RECOVERY-LOOP governs the next work.

**Latest output result:** the synthetic AppKit presentation defect is corrected.
A reduced test independently reproduces calibrated-color conversion and filtered
edges. Explicit sRGB tagging plus nearest-neighbor drawing yields zero channel
mismatches in that reduction. The real window then passes both full1280×960
capture comparisons after conversion to sRGB: maximum channel error1/255,
within the declared2/255 quantization tolerance, with exact square bounds.
CUA verified red→blue through its native toggle. Closing the window exits cleanly.
Evidence: `20261005-154000/presentation-report.json`, `presentation-contract.json`
and `reduction.log`. Earlier152955 failures remain preserved. This is an AppKit
software-scanout bridge, not Metal or a Sonic frame.

**Apple audio output:** verified synthetic PCM passes two AudioQueue phases.
Each submits13230 frames in three buffers, receives all completion callbacks,
observes an advancing sample clock, then stops/resets; the second phase restarts
successfully. Queue volume0.25 limits the brief synthetic signal. Evidence:
`20261005-154214/audio-output-report.json`. No recording or independent listening
verified audible quality; actual Sonic audio and route/lifecycle coverage remain
open. Source-independent output feasibility now has bounded macOS evidence.

**Audio capture passes its bounded scope:** synthetic sample RAM and AICA
register writes produce441 exact stereo frames at44.1kHz for looping, one-shot
and master-muted cases. Both native checks and an independent WAV reader match
every sample. A single synthetic ARM7 branch runs through the explicitly
identified interpreter (225792 steps/case); no Sonic firmware or SH-4 game code.
Evidence: `20261005-153448/audio-report.json`. The bounded Apple output result above does not establish sound quality,
voices/music, real firmware or full lifecycle behavior.

**Previous recovery result:** original SBOARD state2 now runs through task creation
`0x0C905480` and into its initializer. Eight cases across two real loads cover
both acts with three/four initial free tasks. With a fourth node, original code
links a group3 task, allocates two-byte work storage, and records stage38/act.
It stops explicitly at `0x0C904AC0`. With no free node, registration returns
without initialization and the original caller reaches `0x8C052700`. Stage state
remains2 in both cases. Camera, child and auxiliary queues remain intact.

Original initialization writes the task pointer at module global `0x0C97C030`.
The test verifies all other module bytes are unchanged and that a failed load
preserves the entire live image. Code activation happens once per actual load;
it no longer reactivates a modified module between fixture cases. No generated
instruction or source-core patch changed. Startup/scheduling remain supplied.
Evidence: `20261005-152213/report.json` and `continuation-contract.json`.

**Output experiment:** a game-data-free native ARM64 probe sends synthetic
store-queue packets through TA decoding, software rasterization and scanout.
All640×480 output pixels match an independent oracle: an8100-pixel red square
occludes rear blue; moving blue forward produces8100 blue pixels. Both outputs
have four triangles and one virtual page flip. Evidence:
`20261005-152538/render-report.json`; no guest-code/image symbols are linked.
This is an offscreen software result, not an Apple window, Metal backend,
transparency/texture test, Sonic frame or audio result.

**Previous recovery result:** the video/camera prerequisite now connects to the
real SBOARD loader and stage callback. Four cases (acts 0/1 across two loads)
pass on native ARM64. Original code reads both camera files from the disc,
creates the camera task, registers the stage, reaches state 2, and restores the
stage projection to 457.059 after a 320 control. Each case preserves one shared
heap and the created task queues between these operations.

The next missing compiled target is **`0x0C905480`**, attempted at original
JSR `0x8C90318C`. Stage state remains 2; the failure is explicit. Its nine
instructions call existing resident task registration `0x8C0A1620` with metadata
`0x0C90A5B8`; task availability and that descriptor require review next.
The stage/player records, initial heap/free task pool, selection and host calls
are still fixtures. No scheduler, camera/child callback, frame or audio proof.

Evidence: `docs/artifacts/recovery-aot/20261005-151600/report.json` and
`joined-camera-contract.json`. Resident closure: 357 routines / 24,712 analyzed
instructions / 161 external records; these are not executed-coverage counts.
All file/provider and failed-load controls pass. Inputs and six core patches
are unchanged. First run151418 failed a harness assertion: managed-module
rejection precedes trace recording. Checking exact caller/target/PR fixes it.

**Foundation checkpoint:** continue bounded public-source recovery; do not adopt
it as the production core yet. The connected AOT path supports further work,
but original startup, scheduling and automatic module lifetime remain unproved.
Source inspection confirms Windows-only window/D3D11 and waveOut output; Apple
presentation/audio and complete graphics semantics still require implementation
and tests. See the bounded work estimates in RECOVERY-LOOP. G1/R3/R4 stay open.

**Previous recovery result:** original video setup `0x8C608442` returns on native
ARM64 with 640×480 globals using the public runtime's BIOS/hardware model.
Original camera setup `0x8C0196FE` then selects both stage-38 SBOARD acts from
their actual camera records, creates the camera task, and sets the active
callback. Original `0x8C0194FC` produces projection457.059 for angle12743 and320
for the 90-degree control, for both acts. No display width or active camera
pointer was substituted by the harness. Stage selection, a task pool, heap and
host-loaded camera bytes remain declared fixtures; the camera task is not run.

Evidence: `docs/artifacts/recovery-aot/20261005-150530/video-report.json` and
`camera-prerequisite.json`. The generated closure contains245 routines/20217
analyzed instructions with112 external records; the measured path does not prove
every branch. Hardware bit-exact FSCA behavior, a displayed frame and audio are
unverified. The first video experiment150041 timed out during a bounded8MiB
clear with verbose per-call logging. Changing only that logging mode allowed
the unchanged original initializer to pass in150208.

**Earlier recovery result:** original SBOARD loading and registration now connect
to its state-zero callback. A slice of thirteen original routines (459 analyzed
instructions) execute natively with declared player, heap, task-pool and queue
state. Both selected-character branches (0/2), with available/exhausted task
pools, pass across two real file loads: eight cases. Available tasks reach state
2 through original allocation/initialization; an empty pool stays at state 1.
Checks cover player writes, task callbacks/queues, zeroed work blocks, auxiliary
arguments, heap state, guards, return and preserved registers. No scheduler or
child-task execution is claimed.

The next state rejects at unregistered `0x8C0194FC`, called from SBOARD
`0x8C903186`. The state remains 2. This routine reads camera state and delegates
to math code; its globals must be initialized through an evidenced path.
Evidence: `docs/artifacts/recovery-aot/20261005-145336/report.json` and
`callback-contract.json`. Original loading/registration/failed-load checks still
pass. The first attempt's boundary assertion confused caller PC with attempted
target; it was corrected using the recorded dispatch target, without game edits.

**Earlier recovery result:** the original 48-instruction camera initialization
routine now executes as native ARM64. All 4,893 records from 101 owner-held
CAM files pass expected state writes, surrounding-memory guards, input
preservation, return and register checks. An absent overlay callback rejects.
Globals and state are supplied by the harness: this proves the isolated routine,
not original startup, selected camera callbacks, rendering or gameplay.
Evidence: `docs/artifacts/recovery-aot/20261005-144437/camera-report.json`.

AL_MAIN now has a callable entry traced from original AL_GARDEN00 code at
`0x8CB80032` to `0x8C910D52`. Re-rooting preserves every prior function,
instruction and external-call CSV row (including multiplicity); only ordering
changes. Evidence and refreshed 248-image linkage: `20261005-142912/`.
ADV01OBJ's root is still unresolved. A shared camera callback table alone does
not identify the overlay occupying that address, and none of the 101 CAM files
selects the four overlapping table entries. Do not seed those addresses blindly.

**Next executable step:** return to original SBOARD initialization904AC0 and
its called setup/resource routines using the connected loader/task harness.
Retain the exhausted-pool continuation052700. Replace fixture startup through
original pool/player/scheduler paths before making natural-boot or gameplay
claims. Avoid more synthetic output variants unless a real game dependency
requires them. G1/R3/R4 and production adoption remain open.

**Previous recovery result:** ADVERTISE, SUMMARY and ALERTWND are executable
menu modules. Original state machine `0x8C0558C0` loads them at `0x0C900000`
and invokes their header functions. Their bounded static audits and independent
PRS comparisons pass with no UNKNOWN/RAW instructions: 480 function entries and
28,227 unique instruction addresses across the three images. Menus have not run.

BRO_USA/BRO_JAP are verified containers of four bounded HTML documents and zero
alignment padding; the resident browser reads that offset/length layout for
BRO_USA. The LCD-named files are 64 KiB Chao Adventure transfer images: original
AL_MAIN passes their buffer with `SONICADV__VM` to the memory-card provider.
Their full VMU format, transfer behavior and guest CPU execution remain unverified.
They are not classified as inert textures or native SH-4 game modules.

Nine PRS files remain under format/consumer investigation: five character motion
files, two board-motion files and SHTPATH1/2. Exact original load destinations
are now recorded, and all nine match the independent decoder. Their names and
prefixes do not establish data-only status. BIN and other auxiliary roles remain
open. AL_MAIN's former root hypothesis is superseded above; ADV01OBJ stays open.

Current separate-image inventory: **248 modules**, 16,773 function entries,
1,327,984 unique-address sum and 121,875 external records. These include 58,099
known resident entries, 55,066 unresolved records, 8,540 records at 266 resident
candidates, 65 local candidates and 105 other targets. No coverage percentage
or execution claim follows. Resident map remains 6,250 entries / 527,279 unique
addresses; original Tikal callback evidence and 207 event audits remain valid.

Evidence: `docs/artifacts/recovery-aot/20261005-141622/` (ADVERTISE/SUMMARY),
`141730/` (ALERTWND), `142208/` (payload roles and combined linkage), `142348/`
(nine pending PRS load destinations/reference decoding). Original bytes unchanged.

**Remaining static work:** trace motion/path consumers and ADV01OBJ's root;
classify BIN/auxiliary roles beyond the 101 camera files. Review event helper
`0x8C04BC2E` and candidate
provenance before expanding resident closure. Menu/event/task execution and
multiple active module lifetimes still need proof before graphics/audio and
playability. SBOARD's prior execution remains scoped to supplied task/heap state
and explicit host activation. R3/R4 and original Katana/PAL G1 remain open.
No game app or Simulator launched. See [RECOVERY-LOOP.md](RECOVERY-LOOP.md).

The current source audit still finds 1,474 missing generated-source/metadata
records, 177 headers, four SDK contracts, the Katana source archive, and one of
39 title-provider files. FFmpeg source is complete. The owner V1.004 CDI is still
not the supported PAL GDI. The exact source-only request remains in
[BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md). No game app or Simulator launched.

## Earlier recovery measurements

**Active recovery: R0–R2 passed; R3 partial.** Chris authorized an isolated
alternative-source investigation; the old instruction to park all core research
below is superseded within [RECOVERY-LOOP.md](RECOVERY-LOOP.md).
Pinned public DreamcastRecompiled source initially built on native Apple ARM64
without patches; the recorded GBR patch is now applied. Eight focused upstream
tests and the initial three generated execution oracles passed,
including changed memory inputs, float/double output checks and missing-target
failures. This establishes a small working public AOT route, not Sonic gameplay
or compatibility with Katana's ABI.

The candidate read the local V1.004/JUE CDI, preserving its SHA-256, and extracted
its boot image. Game-entry static discovery: 4,929 blocks / 34,814 instructions,
36 unknown words, 269 unresolved indirect transfers and 12 out-of-image edges.
Some unknowns appear to be data misclassified as code. R3 next: classify those
sites, compare raw-recompiler closure, and inventory dynamically loaded and
auxiliary code. No external source input is required for that next experiment.
Original G1 remains blocked; this CDI does not satisfy the required PAL GDI.

**R3 follow-up, 10:40 CDT:** the reproduced GBR-relative store gap is repaired
with a recorded patch against the source pin. Native execution passes all 256
displacements with two bases/values, memory guards and preserved registers,
both with and without the optional GPR cache. Eight prior tests and three prior
execution oracles still pass. Patched Sonic static analysis converges after 14
passes at 6,103 functions / 513,457 unique instruction addresses, with 1,063
external records at 818 sites. Both previously excluded Sonic store sites are
now represented. No Sonic code was generated or executed. Next: classify
remaining targets and decoded auxiliary program identities, beginning with Mini Cart.
Full disc directory inventory has 2,827 entries; Mini Cart is present but its
hash differs from the pinned SARecomp requirement. R3 and original G1 stay open.
Evidence and reproduction commands are in [RECOVERY-LOOP.md](RECOVERY-LOOP.md).

**R3 auxiliary follow-up, 10:56 CDT:** bounded Python PRS decoding and a pinned
public Rust reference produce identical Mini Cart bytes. Decoded size matches
SARecomp's reference, but SHA-256 differs. Local header/metadata supports the
`0x8C900000` placement hypothesis and four callback roots. Module discovery has
211 entries; its 28 previously undiscovered resident targets expand the main
scan to 6,160 entries / 520,807 unique addresses. All 1,044 module external call
records now match analyzed resident entries, while the main image still has
1,064 external records. No loading/execution proof or G1 change. Next: bounded
header-based classification of remaining PRS candidates, keeping overlays
separate. See [RECOVERY-PRS.md](RECOVERY-PRS.md) for identities and commands.

**R3 scan follow-up, 11:09 CDT:** all 843 PRS streams decoded, all 535 raw BIN
files read, original CDI hash preserved. Three registration-header and 23
prologue candidates match the independent decoder. Other prefixes remain
unclassified, not proven data-only. AL_RACE (594 entries) and SBOARD (108 entries)
have separate static coverage/link reports. Next is AL_MAIN, using 28 overlay
targets from AL_RACE as explicit hypotheses. Original G1 and R3 remain open;
no game/Simulator launched or Sonic C++ emitted.

Reproduce: `python3 scripts/run-recovery-aot.py`;
static disc probe: `python3 scripts/run-recovery-aot.py --disc-probe 'ref/Sonic Adventure.cdi'`.
Evidence: `docs/artifacts/recovery-aot/20261005-080322/` and
`docs/artifacts/recovery-aot/20261005-080517/`. No Simulator/game launched.

## Earlier baseline audit (retained for original G0/G1)

Root at audit start: `65fefee`. Unknown work preserved; no push or release.

**Blocked audit, 02:36 CDT:** at `18794e9`, the third resumed goal turn confirms
the same external prerequisites. `private-inputs/`, `.local/linux-sdk/`,
`.local/baseline/r354/` and `.local/working-product/` remain absent; the source
audit returns 2 with unchanged missing-file counts. Xcode still requires EA2002
while EA1990 is accepted. The requested source path/repository has not been
provided. Bounded independent work is finished; park the goal as **blocked**
until matching source/dump access or an external prerequisite changes.
Revalidated existing binary/source identities and ran 142 input checks plus
16 verifier tests: all pass. No game build or Simulator launch was attempted.

Continuation at `864ae6e`: added the read-only source/disc verifiers, an explicitly
incomplete dependency lock and the required private provenance document.
16 synthetic verifier regressions pass. The source audit verifies the complete
materialized FFmpeg tree but remains blocked by the missing private inputs.

Lowest unmet goal: **G0, partially reopened** — native macOS CLT builds now work;
full Xcode/mobile tooling still requires the newer license. G1 remains unresolved.

**Progress this turn:**

- Found and verified the independently installed Apple Command Line Tools lane.
  A native ARM64 Objective-C++ Foundation probe compiles, links and runs.
  Mach-O target: MACOS, minimum 13.0, SDK 27.0. The earlier claim that all
  Apple compiles are blocked is superseded.
- All 142 single-stick Dreamcast checks pass with Apple clang 21.0.0
  (46 adapter, 50 shell, 46 touch). The preserved Homebrew LLVM lane also passes.
  The runner defaults to Apple CLT, honors compiler overrides and fails on a
  compiler error before any stale binary can run.
- Recovered **38/39 exact r354 title/provider source files** privately from
  pinned/public Git history. Every staged size/hash matches the manifest;
  the exact r354 CMake file remains missing.
- Acquired and SHA-256 verified the pinned **FFmpeg implementation source**.
  The retired SDK's headers/Windows binaries were incorrectly described as
  codec source in the earlier inventory.
- Completed a scoped local-source scan to exit 0: 1,235,012 filenames across
  project, document, download, desktop, local-tool and mounted-volume roots;
  zero traversal errors and zero relevant candidates. Earlier unfinished
  root-wide searches did not establish an exhaustive machine-wide result.
- Corrected the acquisition request to **source/metadata only**. The removed
  12-part recovery archive contains game assets and is not the requested input.

Source closure: **blocked**. Still missing the actual Katana runtime/compiler,
generated guest code and SDK contracts, compatible providers/build configuration,
audio/movie interfaces, auxiliary Mini Cart source/generator closure, and the
exact owner's PAL v1.003 GDI/three tracks. The present V1.004 CDI is incompatible.
The recovered title snapshot and FFmpeg source do not close these gaps.

References verified: SARecomp `d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2`,
SunPad `c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b`,
PaperPad `a6f6999a570156f6794ff3fa79f810cfad71c4fc`; all push-disabled.

Highest original-baseline behavior: native macOS Foundation probe and input logic tests.
No Katana core, SonicPad game app, graphics/media playback or gameplay build exists.
No Simulator/game instance was launched. Stored device plists all say shutdown;
live simctl inspection remains license-blocked. Physical acceptance not performed.

Known-good commands:

```sh
scripts/run-host-tests.sh
SONICPAD_HOST_TOOLCHAIN=llvm scripts/run-host-tests.sh
python3 scripts/fetch-media-source.py
```

Diagnostic commands intentionally exit 2 for open prerequisites:

```sh
scripts/check-prerequisites.sh       # macOS probe passes; full Xcode unavailable
python3 scripts/recover-title-source.py  # 38/39 verified; r354 CMake missing
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

Evidence: `docs/artifacts/2026-10-05-astra/`,
`docs/artifacts/prerequisites.9Hgol1/`,
`docs/artifacts/host-tests/{apple,llvm}/` and private source-provenance JSON.
See [JOURNAL.md](JOURNAL.md) for hashes and [TEST-MATRIX.md](TEST-MATRIX.md)
for acceptance scope. Rows 1, 20 and 29 remain partial; gameplay/ABI rows
cannot advance on shell-only evidence.

`dependencies.lock.json` records verified pins/toolchain and unresolved graph
entries, not a complete build closure. [RIGHTS-STATUS.md](RIGHTS-STATUS.md)
records private-only authorization and inspected notices. Actual current commands
and verifier scope are in [BUILDING.md](BUILDING.md).

Next external input for the original Katana route: an accessible authenticated Katana/runtime/generated-source
package or generator repository/path, plus the owner's supported PAL dump.
The exact source-only request is in [BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md).
For full Xcode/iOS tools Chris must accept its newer license once:
`sudo xcodebuild -license accept`. macOS work does not require that step.
