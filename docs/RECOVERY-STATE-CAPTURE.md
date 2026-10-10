# Recovery runtime-state capture

**Live integration completed — 9 October:** the retained integer batch now reaches
65EB00 in the original runtime,2,820 cycles afterED8536. CPU/RAM/TMU/SQ matches the
frozen rate-pool endpoint. Checkpoint
`20261009-original-startup-integer-integration/original-startup-integer-integration-checkpoint.json`
SHA `053f450f7ddd70c0819a11a5eb34f54f3a3ec82b3b0f8933d6598bd1a0ea1b27`.
This removes the need to reconstruct hardware for this path. Next is the separate
bounded FPU comparison; consult currentSTATUS/goal-loop. The runner's missing
entry-SQ capture is a postcheck limitation, explicitly covered by the separate
final-endpoint verifier; preserve old artifacts and fix the label in a fresh successor.

**Capture completed — 9 October:** a fresh observer at the original ED8536
boundary records the hardware inventory without changing the six prior captures.
Checkpoint SHA `4b0b17b5f9a6ce176edd2407e188ab6acc4c3d498217bf9217647b4078a54c73`
in `20261009-original-startup-hardware-capture/`; separate
`20261009-original-startup-hardware-final-verification.json` passes1,632 identities.
Read its explicit limits: raw diagnostic structs are ABI-specific, dynamic renderer
caches are not fully serialized, and this boundary precedes isolatedEB0254.

Correction: older `original-startup-*-state.txt` files already contain Holly masks
and pending flags plus partial AICA/G2/scheduler state. The original audit searched
filenames and missed these fields. Original IRQ masks are nonzero and1,226 IRQs
have occurred. The partial replay's zero masks are not original startup state.

**Next experiment:** preserve the live original startup runtime and extend its
exact MOVIE registration closure. Reuse
`20261008-startup-movie-instance-buffer-acquire/movie-instance-buffer-acquire.o`
and its `movie-instance-buffer-acquire/movie-instance-buffer-acquire-manifest.hpp`.
Checkpoint `movie-instance-buffer-acquire-checkpoint.json` SHA
`60d859894feda51e362379dd261a762b75704cb8f146dbf4e86e49fbb251ecc2` records
55 original instructions and96 connected/9 activation controls, nextED7E46.
Verify the retained manifest's address set, current link membership and subsequent
actually reached dependencies before building a fresh observer. Do not rerun an
old output or replace the separate default FPU implementation by accident. Batch
retained integer bodies up to the source-evidenced FPU boundary. Save new hardware
state and compare expected prefix captures; do not resume an old bookkeepingPC.

Fresh capture sources are `tests/recovery/board-join-hardware-capture-20261009.cpp`
and `tests/recovery/startup-hardware-capture-20261009.hpp`; runner is
`work/dreamcast-recovery/run-startup-hardware-capture-20261009.py`. Its final
assertion incorrectly searched generic exception text for the target; fix that
in a fresh successor using statePC/PR/r2, as the separate verifier does. Native
execution itself matches the existing boundary. Original sources remain unchanged.

The following investigation plan is retained as history; capture is now done and
full portable restore is neither implemented nor needed for the preferred live path.

9 October 2026. This is the next isolated recovery experiment, not a change of
production foundation or a clearance of G1.

The current partial replay reaches a complete repeat with empty buffers. Original
ED39D2 and ED3A2E query slot6/7 readiness, receive0 and leave instance state2.
Native CPU/RAM/TMU matches the frozen prediction through13,152 modeled cycles.
The second entry is stopped by a test-only observer before game instructions.
Generating more unused branches or rerunning this loop cannot prove media delivery.

The latest fixture restores CPU,16MiB main RAM, TMU, store queues, flash bytes and
AICA RAM. Its nine Holly mask values are0. It does not restore original hardware
registers, drive state or scheduler phase. This is a fixture limitation, not evidence
that the original game's boot configured zero masks.

Required next work:

1. Reuse the existing original startup prefix and its verified disc map. Begin with
   `work/dreamcast-recovery/run-startup-movie-instance-video-complete-startup.py`
   and its immutable `20261008-startup-movie-instance-video-complete-startup`
   report/checkpoint. Do not rerun that script in its existing output directory.
   Its observer compile source is `tests/recovery/board-join.cpp`; retain the
   existing startup authority in `tests/recovery/startup-check.hpp`. Preserve dirty
   files and old sealed source copies. First identify a named reachable observation
   boundary; do not claim the default closure already reaches EB0254.
2. Add a fresh, bounded capture of hardware state at that boundary, recording exact
   source/object/disc identities and CPU/RAM context together. Required families:
   Holly masks/pending/ack state; MMIO and PVR/SPG state; GD-ROM mounted disc and
   in-flight drive/stream state; AICA registers/ARM state; device-clock phase and
   scheduler pending cycles. Verify the actual members accessed by the next path.
   Neither retail payload injection nor guessed register values are acceptable.
3. Prefer keeping the original live runtime across the next transition. If a
   serialized restore is needed, compare every required captured family before
   executing. The current root snapshot alone cannot provide those values. The
   older full-startup directories contain no named hardware/MMIO/graphics captures.
4. Run a bounded continuation with original interrupt handling and exact callback
   registrations. Stop on a real missing target, verified asynchronous redirect,
   captured media-production event or explicitly labeled diagnostic budget. Run
   only one native instance. Do not claim presentation from PVR timing counters.

The isolated ARM64 FPU candidate remains unpromoted. The default startup closure
still has its separate missing-code boundary; do not transplant candidate results
into production claims. Preserve the2.75GiB full-capture and400MiB focused guards.

Evidence under `docs/artifacts/recovery-aot/`:

- `20261009-startup-movie-state-two-repeat-repair1/movie-state-two-repeat-checkpoint.json`
  SHA `174fff8753a49cd0c416ee39a408e6d3b4239147fefacd2052a6804d586110e0`.
- `20261009-movie-state-two-repeat-final-verification.json`:1,631 sealed identities,
  full native prediction match,174 reviewed instructions across five bodies;
  only two bodies/86 instructions newly compiled for this captured path.
- `20261009-movie-empty-loop-hardware-audit.json`
  SHA `65eba8b59547a852917b7708c3cb963a940898d89ca10ce364372cc662b3f01f`.

The exact upstream source-only acquisition request remains in
[BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md). All1,657 missing paths and the
supported PAL GDI are still absent. None of this independent recovery clears G1.
