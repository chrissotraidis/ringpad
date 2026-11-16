# RingPad bootstrap blockers

9 October video-copy preparation: three bodies compile and the observer passes
syntax checks; native execution awaits the disk guard. Refreshed audit
`artifacts/bootstrap-input-audit-20261009-video-preparation.json` still finds
1,657 missing source paths and no supported PAL GDI. All 10,139 FFmpeg files
match pinned identities; Apple media build remains unproven. Scoped upstream
metadata refresh finds no new source archive. G1 remains unresolved; source-only
request unchanged and unsent. No decoded movie or gameplay.

9 October first video payload: stream advance passes a frozen 119-cycle check;
a post-run 2,645-cycle CPU/RAM/TMU audit reaches the original video callback with
a real 2,018-byte payload. The next video-copy prediction is sealed but not
natively verified. Free space fell below the full-run guard. G1 source/PAL gaps
and source-only request remain unchanged and unsent; no decoded movie or gameplay.
Evidence: `artifacts/recovery-aot/20261009-live-movie-stream-advance-final-verification.json`.

9 October padding consumption: three original bodies pass a frozen 118-cycle
CPU/RAM/TMU check; a post-run 30-cycle audit confirms the caller accounts for
2,048 bytes and reaches missing stream-advance wrapper `8CED1DD0`. G1 source/PAL
gaps and source-only request remain unchanged and unsent. No decoded movie,
normal boot or gameplay. Evidence:
`artifacts/recovery-aot/20261009-live-movie-packet-consumer-final-verification.json`.

9 October system/packet headers: two original bodies pass separate frozen
257-cycle and 129-cycle CPU/RAM/TMU checks. Startup reaches missing payload
consumer `8CED1EF8`; its query and selected padding callback are identified.
G1 source/PAL gaps and source-only request remain unchanged and unsent. No
decoded movie, normal boot or gameplay. Evidence:
`artifacts/recovery-aot/20261009-live-movie-worker-system-final-verification.json`
and `artifacts/recovery-aot/20261009-live-movie-worker-packet-final-verification.json`.

9 October first movie pack: three original functions pass the frozen 231-cycle
CPU/RAM/TMU prediction and consume the real 12-byte pack header. A post-run
94-cycle CPU/RAM/TMU audit reaches missing system-header parser `8CED6C88`.
G1 source/PAL gaps and source-only request remain unchanged and unsent. No
decoded movie, normal boot or gameplay. Evidence:
`artifacts/recovery-aot/20261009-live-movie-worker-pack-final-verification.json`.

9 October main-loop tail: nine original MOVIE bodies pass an exact 851-cycle
CPU/RAM/TMU check. The worker receives 65,536 bytes matching the original disc
payload and reaches missing stream parser `8CED690A`; the first pack-header
path is reviewed but unexecuted. G1 source/PAL gaps and source-only request
remain unchanged and unsent. No decoded movie, normal boot or gameplay.
Evidence: `artifacts/recovery-aot/20261009-live-movie-main-tail-final-verification-repair1.json`.

9 October object status: original getter passes an exact eight-cycle return;
next missing loop-tail callback is `8CED0914`. Live-verified capture sharing now
uses the immediate parent checkpoint, avoiding duplicate writes without deleting
evidence. G1 source/PAL gaps and source-only request remain unchanged. Evidence:
`artifacts/recovery-aot/20261009-live-movie-main-status-final-verification.json`.
No decoded movie, normal boot or gameplay acceptance.

9 October main-state query: three original MOVIE bodies pass an exact321-cycle
CPU/RAM/TMU check; a further eight-cycle caller audit reaches missing status
getter `8CECF4F0`. G1 source/PAL gaps remain unresolved; source-only request
unchanged and unsent. Evidence:
`artifacts/recovery-aot/20261009-live-movie-main-state-final-verification-repair1.json`.
No decoded movie, normal boot or gameplay acceptance.

9 October thread-resume follow-up: original context restore passes209cycles;
main MOVIE callback chain passes142cycles and clears its frame counter. Next
missing routine is `8CEB0430`, first virtual dependency `8CECFEC8`. G1 source/PAL
gaps remain unresolved; source-only request unchanged and unsent. Evidence:
`artifacts/recovery-aot/20261009-live-movie-main-resume-final-verification.json`.
No decoded movie, normal boot or gameplay acceptance.

9 October frame-wake follow-up: two original resident bodies pass the exact
40-cycle CPU/RAM/TMU check. IRQ return reaches missing resume trampoline
`8C5FFB60`; the subsequent context restore remains unverified. The movie payload
remains intact. G1 source/PAL gaps and source-only request are unchanged. Evidence:
`artifacts/recovery-aot/20261009-live-movie-frame-wake-final-verification.json`.

9 October movie-data completion: first frame service and completed-read path
verify; all 462,848 transferred bytes are now available in the resource buffer.
Next missing original path is frame-two context wake `8C5FFCFC`. G1 source/PAL
gaps remain unresolved; source-only request unchanged and unsent. No decoded
movie or normal boot. Evidence:
`artifacts/recovery-aot/20261009-live-movie-read-completion-final-verification.json`.

9 October owner-service follow-up: four original resident bodies pass a 545-cycle
CPU/RAM/TMU check; the movie payload remains intact. Continuation reaches the
MOVIE callback `8CED5518` and the module guard; its generated body is missing.
G1 source/PAL gaps remain unresolved; source-only request unchanged and unsent.
Evidence: `artifacts/recovery-aot/20261009-live-movie-owner-final-verification.json`.

9 October actual movie payload: original startup transfers 226 sectors / 462,848
bytes into RAM, independently byte-identical to the owner CDI. Three resident
functions advance task/record service to missing owner-service `8C660754`.
This does not establish decoding or normal boot. G1 source/PAL gaps unchanged;
source-only acquisition request remains unsent. Evidence:
`artifacts/recovery-aot/20261009-live-movie-record-final-verification.json`.

9 October scheduler return verified: two original functions complete the empty
4 KiB request; exact CPU/RAM/TMU prediction passes for 498 cycles. Continuation
reaches missing `8C661490`. G1 remains unresolved: 1,657 source paths and PAL GDI
absent; source-only request unchanged and unsent. No normal boot or gameplay.
Evidence: `artifacts/recovery-aot/20261009-live-movie-scheduler-final-verification.json`.

9 October scheduler discovery: first state-2 repeat is exact for 5,962 cycles.
Continuation past polling reaches real missing scheduler target `8C65F8C8` after
a graphics interrupt; the longer discovery interval is not fully modeled.
All 1,657 required source paths and supported PAL GDI remain absent. G1 unresolved,
source-only request unchanged/unsent; no decoded movie or gameplay. Evidence:
`artifacts/recovery-aot/20261009-live-movie-state-two-final-verification.json`.

9 October service-pass follow-up: 3,709 more live cycles and 55 original bodies
verify the empty packet/parser and slot paths to missing state-2 handler `8CED39D2`.
No movie decoding or gameplay. All 1,657 required source paths and supported PAL
GDI remain absent; G1 unresolved and source-only request unchanged/unsent.
Evidence: `artifacts/recovery-aot/20261009-live-movie-service-pass-final-verification.json`.

9 October worker-iteration follow-up: 1,700 more live cycles verify metadata,
empty buffer handling and completion of the first original movie worker iteration.
The 65,536-byte request returns zero available bytes; no movie is decoded. All
1,657 required source paths and the supported PAL GDI remain absent. G1 unresolved;
source-only request unchanged and unsent. Evidence:
`artifacts/recovery-aot/20261009-live-movie-worker-iteration-final-verification.json`.

9 October worker follow-up: original startup advances through 567 verified worker
cycles to metadata query `8CED4508`, including the instance state 1→2 transition.
All 1,657 required source paths and the exact supported PAL GDI remain absent.
G1 unresolved; source-only request unchanged and unsent. No decoded movie or
gameplay. Evidence:
`artifacts/recovery-aot/20261009-live-movie-worker-batch-final-verification.json`.

9 October callback/context follow-up: 255 more live cycles are verified through
original callback and worker-context restoration. This does not replace the
missing Katana/title-source closure: all1,657 source paths and supported PAL GDI
remain absent. G1 unresolved, source request unsent. No decoded movie or gameplay.
Evidence: `artifacts/recovery-aot/20261009-live-frame-callback-final-verification.json`
and `artifacts/recovery-aot/20261009-live-movie-context-final-verification.json`.

9 October interrupt audit: the scoped live frame prefix now matches exact CPU,
RAM, VRAM, TMU, SQ and graphics-packet expectations including two interrupts.
This recovery evidence does not replace the missing source closure: all 1,657
source paths and supported PAL GDI remain absent. G1 unresolved, request unsent,
no decoded movie or gameplay. Evidence:
`artifacts/recovery-aot/20261009-live-frame-irq-final-verification.json`.

9 October stream-ready follow-up: all 1,657 source paths and supported PAL GDI
remain absent. The original movie resource method returns with exact CPU/RAM/TMU
agreement after 182 cycles. Subsequent graphics packets match, but two interrupts
leave frame timing and RAM differences under audit. G1 and playability remain
unresolved; source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-stream-ready-saved-audit.json`.

9 October live file/stream follow-up: all1,657 missing source paths and supported
PAL GDI remain absent. Resource preparation matches159 live cycles; directory
lookup, descriptor/stream creation and position setup match16,860 more. Movie
contents remain unread/undecoded. G1 unresolved, source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-file-open-final-verification.json` and
`artifacts/recovery-aot/20261009-live-resource-open-final-verification.json`.

9 October live outer/entry follow-up: all1,657 missing source paths and supported
PAL GDI remain absent. Indexed queries, outer return and original movie-file
selection match346 live cycles; G1/playability unresolved, request unsent.
No movie file opened or playback established. Evidence:
`artifacts/recovery-aot/20261009-live-outer-entry-final-verification.json`.

9 October live output-setup follow-up: all1,657 missing source paths and supported
PAL GDI remain absent. Output setup, scalar exact-input FPU and slot4 allocation
match1,035 live cycles; G1/playability unresolved, request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-output-setup-final-verification.json`.

9 October live creation/stream follow-up: all1,657 missing source paths and
supported PAL GDI remain absent. Callback binding, creation return and stream
sizing match202 live cycles; G1/playability unresolved, request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-create-stream-final-verification.json`.

9 October live instance-return follow-up: all1,657 missing source paths and
supported PAL GDI remain absent. Instance return/binding prefix match102 live
cycles; G1/playability unresolved, source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-instance-return-final-verification.json`.

9 October live mode-return follow-up: all1,657 missing source paths and supported
PAL GDI remain absent. Mode setup/dispatcher match172 live cycles; G1/playability
remain unresolved and source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-mode-return-final-verification.json`.

9 October live setup-return follow-up: all1,657 missing source paths and the
supported PAL GDI remain absent. Setup return and MOVIE state update match97 live
cycles; G1/playability remain unresolved, source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-setup-return-final-verification.json`.

9 October live reset-completion follow-up: all1,657 missing source paths and the
supported PAL GDI remain absent. Two live reset batches match392 predicted cycles;
G1/playability remain unresolved. Source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-reset-completion-final-verification.json`.

9 October live movie-handler follow-up: all1,657 missing source paths and supported
PAL GDI remain absent. Handler setup/reset entry passes the frozen185-cycle model;
G1 and playability remain unresolved. Source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-movie-handler-saved-verification.json`.

9 October live resident-return follow-up: all1,657 missing source paths and supported
PAL GDI remain absent. The isolated resident constructor now returns to MOVIE with
live IRQ-mask restoration verified; G1/playability remain unresolved. Request unsent.
Evidence: `artifacts/recovery-aot/20261009-live-resident-return-final-verification.json`.

9 October live stereo follow-up: all1,657 missing source paths and supported PAL
GDI remain absent. Stereo setup and pan command writes pass the live recovery model;
G1/playability remain unresolved. Source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-stereo-final-verification.json`.

9 October live service-return follow-up: all 1,657 missing source paths and supported
PAL GDI remain absent. The isolated recovery now verifies activation queue writes,
service return and record initialization live; G1/playability remain unresolved.
Source request unsent. Evidence: `artifacts/recovery-aot/20261009-live-service-return-final-verification.json`.

9 October live channel-pair follow-up: all 1,657 missing source paths and supported
PAL GDI remain absent. Original channel construction and two command queue writes
now pass the live recovery model. G1/playability remain unresolved; source request
unsent. Evidence: `artifacts/recovery-aot/20261009-live-channel-pair-final-verification.json`.

9 October second-buffer audit: all 1,657 missing source paths and supported PAL GDI
remain absent. Saved live buffer/allocator state now passes the source model; G1
and playability remain unresolved, source request unsent. Evidence:
`artifacts/recovery-aot/20261009-live-buffer-pair-final-verification.json`.

9 October live-buffer follow-up: all 1,657 missing source paths and supported
PAL GDI remain absent. Original buffer return and two audio-memory writes now pass
in the isolated live startup; G1/playability remain unresolved. Source request unsent.
Evidence: `artifacts/recovery-aot/20261009-live-buffer-return-final-verification.json`.

9 October live-constructor follow-up: all 1,657 missing source paths and supported
PAL GDI remain absent. Seven retained bodies now reach buffer creation in the
isolated live startup; G1 and gameplay are not cleared. Source-only request unsent.
Evidence: `artifacts/recovery-aot/20261009-live-constructor-prefix-final-verification.json`.

9 October live-rate/channel follow-up: all 1,657 missing source paths and supported
PAL GDI remain absent. Original arithmetic and channel initialization now return
in live startup with frozen CPU/RAM/TMU predictions, but do not clear G1 or gameplay.
Exact source-only request remains unsent. Evidence:
`artifacts/recovery-aot/20261009-live-rate-channel-final-verification.json`.

9 October audio-domain follow-up: all 1,657 missing source paths and supported
PAL GDI remain absent. Bounded audio FPU correction passes controls and live startup;
this does not clear G1 or establish gameplay. Source-only request remains unsent.
Evidence: `artifacts/recovery-aot/20261009-original-startup-audio-domain-final-verification.json`.

9 October live-FPU follow-up: all 1,657 missing source paths and supported PAL
GDI remain absent. The isolated scalar correction passes the live-startup test,
but upstream register/RAM/MMIO and host gain differences require further review.
G1 and the exact unsent source-only request remain unchanged. Evidence:
`artifacts/recovery-aot/20261009-original-startup-live-fpu-final-verification.json`.

9 October live-integration follow-up: all1657 missing source paths and supported
PAL GDI remain absent. A batch of21 retained original routines now runs with live
startup hardware and matches the captured CPU/RAM/TMU/SQ endpoint at missing65EB00.
This does not clear source access, FPU correctness, normal boot or gameplay.
`artifacts/recovery-aot/20261009-original-startup-integer-final-verification.json`
records the check. Exact source-only acquisition request remains unsent.

9 October original-hardware follow-up: all1657 source paths and supported PAL
GDI remain absent. Original startup hardware capture matches six prior artifacts;
clocks/interrupts function at its earlier missingED8536 boundary. This corrects
the prior filename-only audit: older state.txt files held partial hardware fields.
No source-access, movie playback or normal-boot gate is cleared. Recheck:
`artifacts/recovery-aot/20261009-original-startup-hardware-final-verification.json`.
Exact source-only acquisition request remains unsent.

9 October empty-repeat follow-up: all1657 missing source paths and supported PAL
GDI remain absent; G1 unresolved. Native partial replay matches full CPU/RAM/TMU
through13,152 modeled cycles but loops with empty media/readiness flags. Hardware
state needed for meaningful scheduling is not restored by this fixture; see
[RECOVERY-STATE-CAPTURE.md](RECOVERY-STATE-CAPTURE.md). Recheck:
`artifacts/recovery-aot/20261009-movie-state-two-repeat-final-verification.json`.
The upstream source-only acquisition request remains unsent and unchanged.

9 October common-service follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-slot-seven-service-final-verification.json`.
Original resource reservation/return and all populated common-service callbacks
now pass full CPU/RAM/TMU through7,190 modeled cycles. Payload remains empty;
no media decode/presentation/normal boot. G1 unresolved; source-only request unsent.

9 October slot3-buffer follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-slot-three-buffer-final-verification.json`.
Original slot2 now completes and slot3 reaches resource transfer; full state verified
through6,059 modeled cycles. Buffers remain empty. No media decode/presentation/
normal boot claim. G1 unresolved; exact source-only acquisition request remains unsent.

9 October channel-service follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-channel-service-final-verification.json`.
Original slot1 processing completes and reaches slot2 with full state verified
through4,750 modeled cycles. Media counters remain0 and parser metadata unavailable;
no delivery/decode/presentation/normal boot claim. G1 unresolved; exact source-only
acquisition request below remains unsent.

9 October parser follow-up: all1657 missing source paths and supported PAL GDI
remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-parser-empty-final-verification.json`.
Original service/ring/parser checks match full state through3,879 modeled cycles;
captured parser input length0, success return0 and no bytes consumed. This does not
establish media delivery/decode, presentation or normal boot. G1 unresolved; the
exact source-only acquisition request below remains unsent.

9 October worker-iteration follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-worker-repeat-final-verification.json`.
Original empty-queue handling and first worker iteration match full state through
3,481 modeled cycles; second service pass reaches an instance handler. No media
read/decode/presentation/normal boot. G1 unresolved; source-only request unsent.

9 October empty-resource follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-worker-resource-final-verification.json`.
Metadata/buffer/resource checks match full state through2,488 modeled cycles.
Captured input queue returns0 bytes; this does not prove missing disc data or movie
playback. G1 unresolved; exact source-only acquisition request remains unsent.

9 October worker-metadata follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-worker-process-final-verification.json`.
Original aggregate/time/instance-processing prefixes match complete state through
1,781 modeled cycles. No media decode/presented frame/normal boot. G1 unresolved;
exact source-only acquisition request remains unsent.

9 October worker-service follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-service-state-final-verification.json`.
Original worker service advances the captured instance state1→2; complete state
matches through1,338 modeled cycles. No media decoding/presentation or normal boot.
G1 unresolved; exact source-only acquisition request unsent.

9 October context/worker follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-worker-entry-final-verification.json`.
Original context save/RTE/worker prefix passes full-state checks through1,226
modeled cycles. This does not establish normal boot/media playback or production
source closure. G1 unresolved; exact source-only acquisition request unsent.

9 October frame-state/callback follow-up: all1657 missing source paths and the
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-frame-callback-final-verification.json`.
Captured native frame/callback prefix matches full state through985 modeled cycles;
next original context service is unresolved. No normal boot/media playback.
G1 remains unresolved; exact source-only acquisition request unsent.

9 October movie-frame discovery follow-up: all1657 missing source paths and the
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-loop-graphics-final-verification.json`.
Original frame bookkeeping reaches a movie callback and its limited graphics
packet/DMA contract passes. Whole-frame state verification, normal boot and media
playback remain open. G1 unresolved; exact source-only acquisition request unsent.

9 October resource-method completion follow-up: all1657 missing source paths and
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-resource-method-complete-final-verification.json`.
The complete captured resource method returns to movie entry; no media read,
decoding, playback or normal boot is established. G1 remains unresolved; exact
source-only acquisition request unsent.

9 October file-open completion follow-up: all1657 missing source paths and
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-file-open-complete-final-verification.json`.
The original movie file opens against captured directory state and returns a
stream handle; no media decoding/playback or normal boot is established. G1 remains
unresolved; exact source-only acquisition request unsent.

9 October resource-open follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-resource-open-final-verification.json`.
Original movie preparation reaches resident open65FB12; open success and playback
remain unproven. G1 unresolved; exact source-only acquisition request unsent.

9 October entry-dispatch follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-entry-connected-final-verification.json`.
Connected movie entry selects SLH448_1.SFD; its resource method has not executed.
Normal boot, playback and G1 remain unresolved. Exact source-only request unsent.

9 October outer-return follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-resource-outer-final-verification.json`.
The connected resource/outer setup returns to movie-entry caller; normal boot,
playback and G1 remain unresolved. Exact source-only acquisition request unsent.

9 October output-completion follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-output-complete-final-verification.json`.
The complete captured output-setup function returns; normal boot/playback/G1 are
still unresolved. Exact source-only acquisition request remains unsent.

9 October output-mode follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-output-mode-eight-final-verification.json`.
Original stream-size and first output-setting execution do not establish normal
boot/playback or clear G1. Exact source-only acquisition request remains unsent.

9 October creation-tail follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-create-tail-final-verification.json`.
The connected creation tail binds a callback and returns the real handle; normal
boot/playback/G1 remain unresolved. Exact source-only request remains unsent.

9 October instance-return follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-instance-return-final-verification.json`.
The captured instance returns its real handle to the creation caller; normal boot,
playback and G1 remain unresolved. Exact source-only request remains unsent.

9 October setup-branch-return follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-setup-return-final-verification.json`.
The internal MOVIE setup branch returns to its dispatcher; normal boot, playback
and G1 remain unresolved. Exact source-only acquisition request remains unsent.

9 October channel-enable/movie-state follow-up: all1657 missing source paths and
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-status-connected-final-verification.json`.
Setup return and movie-state update do not establish playback or clear G1.
Exact source-only acquisition request remains unsent.

9 October outer-reset-return follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-reset-complete-final-verification.json`.
The captured reset completes and movie setup resumes; playback and G1 remain
unresolved. Exact source-only acquisition request remains unsent.

9 October record-reset follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-reset-connected-final-verification.json`.
The idle record reset returns within connected movie setup; playback and G1
remain unresolved. Exact source-only acquisition request remains unsent.

9 October allocation/reset-entry follow-up: all1657 missing source paths and
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-handler-connected-final-verification.json`.
The connected recovery sequence does not establish playback or clear G1.
Exact source-only acquisition request remains unsent.

9 October MOVIE-wrapper follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-movie-wrapper-final-verification.json`.
The recovered wrapper/parent continuation does not prove playback or clear G1.
Exact source-only acquisition request remains unsent.

9 October resident-return follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-resident-complete-final-verification.json`.
The original resident return to MOVIE does not establish playback or clear G1.
Exact source-only acquisition request remains unsent.

9 October stereo-setup follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-stereo-complete-final-verification.json`.
Original count/pan command submission and stereo-setup return do not clear G1.
Exact source-only acquisition request remains unsent.

9 October service-return follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-record-complete-final-verification.json`.
Original constructor-return evidence does not clear G1. Exact source-only
acquisition request remains unsent.

9 October channel-pair-return follow-up: all1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-service-pair-final-verification.json`.
Original command submission/pair return does not clear G1. Exact source-only
acquisition request remains unsent.

9 October channel-setup follow-up: all1657 missing source paths and supported PAL
GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-service-channels-final-verification.json`.
Recovered constructor prefixes do not clear G1; exact source-only request remains
unsent.

9 October buffer/allocator follow-up: all 1657 missing source paths and supported
PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261009-service-buffers-final-verification.json`.
The isolated buffer-memory results do not clear G1. Exact source-only acquisition
request below remains unsent.

8 October constructor-prefix follow-up: all1657 missing source paths and the
supported PAL GDI remain absent. Recheck:
`artifacts/recovery-aot/20261008-service-prefix-final-verification.json`.
The isolated resource-allocation path does not clear G1. Exact source-only
acquisition request below remains unsent.

8 October channel-constructor follow-up: all1657 previously missing paths are still
absent, and the supported PAL GDI remains absent. Recheck evidence:
`artifacts/recovery-aot/20261008-channel-complete-final-verification.json`.
The isolated whole-constructor result does not clear G1 or supply the Katana source
closure. Exact source-only acquisition request below remains unsent.

8 October FPU follow-up: every previously missing input path was rechecked in
`artifacts/bootstrap-missing-path-recheck-20261008-fpu-domain.json`; none appeared.
The isolated public-runtime experiments below do not provide the missing Katana
source closure or supported PAL input. Source-only request remains unsent.
Earlier experiment counts below are historical.

8October recheck: `artifacts/bootstrap-input-audit-20261008-movie-guard-final.json`.
Original source closure and PAL-input gaps below remain unchanged. Full recovery
startup now passes344state/252device comparisons; that does not clear G1. Exact
source-only acquisition request remains unsent.

Latest local recheck: `artifacts/bootstrap-input-audit-20261007-movie-guard-final.json`.
All original source and PAL-input gaps below remain unchanged. The148 new recovery
unit controls and prepared integration do not clear G1. The exact source-only
acquisition request remains unsent; no new package was acquired.

Updated: **5 October 2026, Astra audit**. Private-only. This replaces the earlier
recommendation to acquire the complete retired recovery archive.

Declared local source paths and byte identities revalidated **7 October 2026**,
after the six-callback binding loop and descriptor fields returned in full startup:
`artifacts/bootstrap-input-audit-20261007-movie-binding-return.json`. Missing-source
identities and the source-only acquisition request below are unchanged. Recovery
startup experiments do not supply the missing Katana closure or supported PAL dump.

## G0: macOS toolchain reopened

The independently installed Apple Command Line Tools work when selected for
the command:

```sh
DEVELOPER_DIR=/Library/Developer/CommandLineTools /usr/bin/clang++ --version
scripts/check-prerequisites.sh
scripts/run-host-tests.sh
```

Apple clang 21.0.0 compiled, linked and ran an ARM64 Objective-C++ Foundation
probe. Mach-O metadata: **MACOS**, minimum 13.0, SDK 27.0, Apple linker 27037.1.
All 142 Dreamcast checks also pass with this Apple toolchain. Global developer
selection and license settings were not changed.

Full Xcode 27.0 remains license-blocked: it requires EA2002; the machine records
EA1990 accepted for Xcode 26.6. Simulator/iOS tooling remains unverified.
Chris can clear this with `sudo xcodebuild -license accept`. This blocks the
full Xcode/mobile lane, **not all macOS compilation**. The prerequisite script
exits 2 while full Xcode is unavailable, even when its macOS probe passes.

## G1: exact source inventory

Public pin: `d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2`.
Retained-source identities below come from its public baseline manifests.
Their hashes establish source identity; Apple ABI compatibility is unverified.

| Required material | Actual state | Exact identity or missing interface |
|---|---|---|
| Katana runtime/compiler source | **missing** | `katana-source-178448be.zip`, 6,185,732 bytes, SHA-256 `85e5bd27952552f0957559cca82bde5b4024cd71e50e94650cd922f42430cd66`, or a compatible complete source checkout with declared provenance. |
| Runtime translation units | **missing** | Linux build reads `src/runtime/{block_abi,block_table,block_dispatch,block_guards,cache_control,code_invalidation,crash_report,exception,fpu,fpu_simd,guest_program_range,memory,native_bringup_dispatch,native_port,runtime}.cpp` plus all transitive sources/includes. |
| Runtime/SDK headers | **missing** | r354 `sdk/include/`: 177 files (175 .hpp + 2 .in), 1,516,512 bytes. Verify actual files against the public per-file hashes. |
| Generated SDK contracts | **missing** | `sdk/generated/include/`: 3 .hpp + identity materials JSON, 65,232 bytes; `katana/{abi_contract,build_contract,component_identity}.hpp` and matching build/ABI provenance. |
| Generated guest source/metadata | **missing** | r354 `product/generated/`: 1,476 files, 4,356,016,177 bytes; 1,460 .cpp, 2 .hpp, 10 .json plus manifest/build metadata. Includes `generated/code/unit-*.cpp`, `native-port-*-shard-*.cpp`, dispatch, `generated/include/`, and `.katana-generated-artifacts`. Obtain source/metadata or reproduce it with the matching generator. |
| Title/provider snapshot | **present-verified, partial** | 38/39 r354 files privately staged under `.local/source-only/r354/title-source/`, each size/hash verified. Exact r354 `CMakeLists.txt` (15,384 bytes) remains missing. Current public CMake exists at a different identity. |
| Mini Cart auxiliary source/generator | **missing dependency closure** | Public `generate_minicart_aot.cpp` requires `katana/io/{raw_binary_loader,input_provenance}.hpp`, `analysis/control_flow_analysis.hpp`, `ir/{lower,optimize}.hpp`, `codegen/{cpp_emitter,native_aot_profile}.hpp`, texture interfaces and their implementations. Supply complete generator/build configuration or compatible retained auxiliary source. |
| Katana audio/movie/codec integration | **missing** | `src/runtime/native_port_{audio,audio_execution_domain,audio_command_queue,telemetry,movie,ffmpeg_codec,codec,sound_bank}.cpp` and transitive source/headers. See LinuxAudio/LinuxMovie and audio preparation helpers. |
| FFmpeg implementation source | **present-verified archive, extracted source** | Public revision `5a03dfa0f607ee6156a59bdad3987cd3b858ee5d`, archive SHA-256 `5de5245447bafc6734be2e6151becfc389550863d6bcef31b1146a21a69ac49a`, privately acquired under `.local/media-source/` via `scripts/fetch-media-source.py`. Apple libraries and decoder tests remain not-run. |
| Exact supported disc | **missing** | Owner-supplied PAL v1.003 GDI and three tracks below. |
| Native-content assets | **absent; derive from owner's disc** | The 2,072-file `native-content/` set contains game assets. It is not source material to request from upstream. Extraction/install identity remains unverified. |

Five of the six title files that differed from r354 were recovered from public
Git history at `40cb8ca05dd849390cbcde31f4b00da0a4137511`:
`native_port_manifest.cpp`, `native_title_adapter.cpp`,
`sonic_private_scenario_launcher.cpp/.hpp`, and `refresh_native_provider.cpp`.
Together with 33 matching current files, this yields 38/39.
`scripts/recover-title-source.py` reproduces the snapshot, records provenance,
refuses to overwrite unknown work, and exits 2 for the CMake gap.
It does not prove compatibility with the missing guest/runtime source.

**Media correction:** r354 `sdk/ffmpeg/` has 144 headers, five Windows .lib
files and five DLLs. These 154 files are not FFmpeg implementation source.
The actual public source pin above comes from `tools/build-lean-ffmpeg.py`.
The Katana codec interfaces still need source.

## Supported game input

Target: Sonic Adventure **PAL v1.003 (1999), MK-5100050**. Keep the GDI and all
track filenames exactly as referenced by its descriptor; preserve the original.

| Track | LBA | Type / sector bytes | Bytes | SHA-256 |
|---|---:|---|---:|---|
| 1 | 0 | 4 / 2352 | 26,721,072 | `cb45a8de12af5b13abb559fc8acc714744a4e22965fe67158cdbd0037cace093` |
| 2 | 11511 | 0 / 2352 | 13,994,400 | `57eff24181c0581e969a40ec4972336da0891dcd093f814d7dd577c64e0142a6` |
| 3 | 45000 | 4 / 2352 | 1,185,760,800 | `189f89cb695ff81b40878463f326ead759437794d733329ebf3943196ad59b45` |

Canonical descriptor SHA-256:
`d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e`.
Installer acceptance depends on its parsed layout and track checks.

Present input: `ref/Sonic Adventure.cdi`, 757,339,066 bytes, SHA-256
`105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533`.
Earlier IP.BIN inspection identified MK-51000 V1.004/JUE. Converting its
container cannot reproduce the supported PAL track bytes. Do not bypass validation.

## Retired archive: historical identity, not the requested input

The manifest records `r354-development.zip.part001…012`, tag
`r354-baseline`, total 3,115,490,620 bytes, archive SHA-256
`07f28aa2052827c34f94fe20ad1c53b129405ec019f85486675186d9e7508ee9`.
Parts 001–011 are 268,435,456 bytes each; part012 is 162,700,604 bytes.
The baseline pack hash `3c831c4588e31ef428e785352c0660ac4d766e0dec485406029903a0a00c9fd2`
is a separate identity; neither identifies a newly assembled source-only package.

The recovery archive mixes source, Windows binaries and installed game assets.
Upstream DEVELOPMENT.md explains its removal and prohibits re-upload.
`restore-baseline.py` restores an existing owner-held backup; it does not
provide a source download or establish an Apple bootstrap.
Do not request or hunt the complete removed archive. A maintainer can instead
identify the source/metadata subset above from their local material.

## Completed checks and scope

Astra's scan completed to exit 0 across `~/GitHub`, `~/Documents`,
`~/Downloads`, `~/Desktop`, `~/.local`, and `/Volumes`:
**1,235,012 filenames**, no traversal errors. It excluded `.git`,
`node_modules` and `__pycache__`. No matching GDI, runtime header, Katana
source archive, archive part or named Katana library was found.
Evidence: `docs/artifacts/2026-10-05-astra/local-source-scan.json`.

Earlier root-wide searches returned unfinished sessions and suppressed errors;
their claim of an exhaustive machine-wide search is withdrawn. This completed
scan is bounded to the listed roots and filename criteria.

Available public Git history supplies the title files above but no runtime/SDK
archive. Earlier authenticated GitHub checks found header consumers without the
supplying repository. Those searches do not rule out private access. The player
installer is not a source closure.

### Expanded online source search (2026-10-05)

The reference checkout was shallow. A metadata-only unshallow fetch retained
the pinned HEAD and exposed all 120 public commits. Every commit's file tree
was checked for the source archive, `include/katana`, `src/runtime`,
`native_port.hpp`, `cpp_emitter`, `raw_binary_loader`, `block_abi.cpp` and
`block_dispatch.cpp`; none of those supplying paths was present. This is a
completed tree/path audit, not a claim about unavailable private repositories.

Other Dreamcast projects advertise
`https://github.com/sonicfreak1337/KatanaRecomp`. Its authenticated GitHub API,
raw source and pinned Codeload URLs return 404. jsDelivr's repository metadata
and candidate source URLs also return 404. The historical `SARecompZZZZZ`
repository name redirects to the existing public SARecomp repository.

GitHub repository/code searches, the available fork, public gists, Actions
artifacts, release metadata and packaging sources supplied no runtime archive.
The fork's tree is the same pinned commit. GitLab and Codeberg project searches
found no KatanaRecomp repository. Sourcegraph searches including archived
repositories and forks completed with zero matches. Software Heritage had no
matching KatanaRecomp origin; Wayback queries for the repository prefix, raw
source and Codeload source returned no captures. Internet Archive metadata
searches found no matching archive. General web searches supplied references
and player-download pages, not verified runtime source.

Limitations: grep.app returned HTTP 429; one Internet Archive query returned a
backend error. These are inconclusive checks, not negative results. No verified
source download was found, no retail-containing recovery bundle was fetched,
and no maintainer request was sent. G1 remains unresolved. A filename and hash
establish the expected identity, not current public availability.

## Source-only acquisition request (Chris sends manually)

> I'm building RingPad privately against SARecomp
> d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2. Could you provide authenticated
> access to matching Katana runtime/compiler source, generated SDK contracts,
> retained guest C++/metadata and auxiliary Mini Cart source or a reproducible
> generator route?
>
> The known runtime archive is katana-source-178448be.zip, 6,185,732 bytes,
> SHA-256 85e5bd27952552f0957559cca82bde5b4024cd71e50e94650cd922f42430cd66.
> I recovered 38/39 r354 title-source files by hash from public Git history;
> the exact r354 CMakeLists.txt remains missing. Please include compatible
> build configuration, provider identities, audio/movie/codec integration
> and the generator configuration/patches. A complete package matching the
> current public revision is also acceptable with provenance and compatibility
> information.
>
> I acquired the pinned public FFmpeg source. I will supply my own verified
> PAL v1.003 disc and derive game assets locally. Please exclude installed
> assets, discs, saves, Windows binaries and the complete removed recovery
> archive. This is a request for private source access; no publication is planned.

Owner action: place the supported GDI and tracks in
`private-inputs/pal-v1003/`, and authenticated source material in
`private-inputs/` or provide its accessible repository/path.
No request has been sent.

G0: macOS CLT lane verified; full Xcode/mobile tools still blocked.
G1: **unresolved** pending actual runtime, guest source, matching headers/providers,
media integration, auxiliary code and exact disc. G2/gameplay remain not-run.

## Executable recheck (retained-source input scope)

```sh
scripts/check-source-closure.sh --report docs/artifacts/bootstrap-input-audit.json
python3 scripts/verify-game.py private-inputs/pal-v1003/sonic-adventure.gdi
```

The current audit verifies 38/39 title/provider files, 0/1,474 portable
guest-source/metadata entries, 0/177 SDK headers, 0/4 SDK contract entries and
0/1 Katana source archive. The two excluded Windows build metadata files are
`build.ninja` and `compile_commands.json`; source/build configuration and
generation metadata remain required. All 10,139 FFmpeg archive/materialized-file
entries pass, including actual implementation bodies rather than cache stamps.
Current source audit exit: 2. The provided CDI and absent GDI also return 2.

The source audit checks the dependency lock and actual reference revisions.
Even complete input hashes would not alone establish G1: the JSON deliberately
records `g1_complete: false` and lists the remaining build/ABI, auxiliary,
provenance and disc/extraction reviews. See [BUILDING.md](BUILDING.md).

## Recovery update — 6 October, checkpoint155608

The isolated original64A8FC VMU decoder now passes3584cases and its real IRQ
entry/return in fullstartup. Fields1/512/4/1/0 identify metadata decoding only.
Nextmissing64EF30 has channel0/flags0,0,0 and first unregisteredchild64EB72.
No IRQ completion, fullboot, gameplay, VMUpersistence, save or Apple-mobile
acceptance. Source audit exit2 still reports runtime0/1, guest0/1474,
headers0/177, contracts0/4, title38/39, FFmpeg10139/10139. Supported PAL verifier
exit2: descriptor absent. Original source-only request above remains current
and unsent; the experimental owner-CDI route does not clear G1.

## Recovery update — 6 October, checkpoint160823

Original64EF30 channel-init prefix passes2048cases and prospective live entry/
first-child-rejection phases,25emittercycles. Fullstartup still rejects64EB72;
initializer/IRQ/motion completion and VMU persistence remain unverified. No G1
change: runtime0/1, generated0/1474, headers0/177, SDKcontracts0/4, title38/39,
FFmpeg10139/10139. Source audit exits2; supported PAL descriptor absent.
The exact source-only request above remains current and unsent. Next isolated
wrapper64EB72 reads the existing descriptor and tail-jumps64B350, also absent;
its arithmetic/timing/fullRAM/CPU/TMU still need execution verification.

## Recovery update — 6 October, checkpoint161749

Original64EB72 wrapper passes7168cases and prospective live entry/rejected-tail
phases,14emittercycles. Its handler64B350 remains uncompiled; handler/initializer/
IRQ/motion completion and VMU persistence unverified. First handler callback
6647BE is absent; later64B91E notifier is actually registered. No G1 change:
runtime0/1, generated0/1474, headers0/177, SDKcontracts0/4, title38/39,
FFmpeg10139/10139. Source audit exits2; supported PAL descriptor absent,
verifierexit2. Exact source-only request above stays current and unsent.
SunPad/SARecomp/PaperPad pins and disabled push URLs reverified, not source closure.
