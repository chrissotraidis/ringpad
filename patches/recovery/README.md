# Experimental DreamcastRecompiled patches

Private feasibility work, not adoption of a production core or Katana ABI.
Upstream: https://github.com/Duw0ng/DreamcastRecompiled
Base: `ba7df34a50b5a7c676a590bb8b50bd784328c9b9`.
Upstream license: MIT, copyright 2026 Duw0ng; retain its notice with source.

`0001-gbr-long-store.patch` adds SH-4 `MOV.L R0,@(disp,GBR)` (`C2dd`):
unsigned 8-bit displacement, scaled by four, 32-bit store, unchanged registers.
The encoding/operation is in the [Renesas instruction table](https://www.renesas.com/en/document/mas/sh-4a-extended-functions-software-manual),
printed page 42. The patch adds decode, IR formatting/lowering, native emission
and an explicit memory-order barrier for store-queue fusion. It reuses the
runtime's existing 32-bit memory helper; it does not introduce a no-op fallback.

SHA-256: `f92171b1d715a12d2cd01956e11e1be26cd3e61808e59c5388d78e6a4c4e9ff6`.
The recovery runner verifies the ordered patch sequence in an isolated Git
index. It accepts only a clean pinned checkout or an exact sequence prefix,
then applies missing patches. Other changes are rejected and preserved. The
real Git index is not modified. Reports bind the base pin and every patch hash.

Run `python3 scripts/run-recovery-aot.py` for twelve focused upstream tests and
native execution checks, including every C2 displacement with changed GBR/R0,
neighboring memory guards and register preservation. The same oracle also runs
with the optional GPR cache actually enabled. The default lane disables that
cache and hot-trace generation for reproducibility.

The store-queue fusion barrier was inspected in source; the native tests exercise
ordinary guest RAM, not complete MMIO/cache/exception behavior. No full-game or
physical-device claim follows from this patch.

`0002-cfg-call-analysis.patch` replaces branch-insensitive backward call-target
inference with constant merging over discovered CFG predecessors. Conflicting
or unknown inputs stay unknown; loops converge by dropping disagreeing values.
Targets are sampled before delay-slot writes. Callee return edges invalidate
register values because no preservation contract is established. Unsupported
or malformed flow is withheld rather than guessed. Ordinary JSR, relative
BSRF, external JMP tail calls and initial-image pointer-cell estimates remain
supported; generated indirect dispatch stays dynamic.

SHA-256: `cb4cfac13f45234f2f09f7b8c5706e12331413f74e58336e0d59570ca87e1a95`.
The patch changes one upstream assertion: a join choosing between two different
callbacks no longer has a single resolved target. Both candidate callbacks must
still be present. This is the repaired contract, not removal of the test.

Ten source-free analysis controls cover conflicting/equal/unknown joins,
unchanged/changed loop pointers, delay-slot sampling, straight-line propagation,
callee clobbers and implicit GPR updates. The original analyzer fails the new
counterexample; the patched analyzer and native callback oracle pass. Evidence:
`docs/artifacts/recovery-aot/20261005-113352/` and
`work/dreamcast-recovery/cfg-negative-control/report.json`.

Limits: CFG discovery is itself heuristic; candidate jump tables may be incomplete.
File-backed literals/pointer cells describe initial bytes and may change at runtime.
`resolved` is therefore a control-flow-consistent input-image estimate, not a
universal runtime proof. No ABI preservation or immutable-memory assumption was
introduced to recover lost coverage. Further discovery remains required.

`0003-cfg-call-candidates.patch` retains initial-image values seen during CFG
propagation in a separate `cfg_call_candidates` collection. A later merge or an
unknown branch can still leave the call unresolved. Raw closure validates these
candidates with its existing executable-entry checks before registering code;
it does not force calls to take those targets. The emitter/runtime are unchanged.
This is not an exhaustive set of possible targets or evidence of execution.

SHA-256: `df039a60f3ed343c78b6820d1ec136e1bbc624faf20f1f65b19f242e23c024bc`.
Two additional controls verify that an unknown jump on a separate path and a
callee-clobbered loop preserve valid candidates while keeping resolution unknown.
The impossible-path counterexample must not acquire a false candidate.
Twelve analysis controls, twelve selected upstream tests and six native execution
variants pass in `docs/artifacts/recovery-aot/20261005-114809/`.

The patch recovers ten of eleven lost resident entries automatically. A reviewed
12-byte callback table supplies the last one and three additional entries as
explicit initial-image seeds; their hashes/layout are recorded privately in
`work/dreamcast-recovery/lost-entry-review/callback-table.json`. This table is
specific to the owner-held V1.004 experimental input and does not satisfy PAL G1.

`0004-bulk-literal-invalidation.patch` marks every affected main-RAM literal
page dirty after `dc_load_bytes` and `dc_zero_bytes`. Their fast paths previously
bypassed scalar-write invalidation, so compiled PC-relative literal reads could
return initial-image constants after memory changed. Existing zero-size guards
precede page arithmetic. Other memory-region paths are unchanged.

SHA-256: `1ba1628f225ce36bcd0178d9a32f8e9b20f0324e37ec0565cef6fdfc6f6c85ad`.
Synthetic baseline `20261005-130304/` reproduced both faults. Patched diagnostic
`20261005-130453/` checks bulk writes, clears, physical/P2 aliases, writes spanning
two pages, empty operations and scalar-write control. Run
`python3 scripts/run-recovery-overlay.py`; its separate stale-code flags remain
true because this patch does not implement compiled-module activation.

All twelve upstream tests, twelve analysis controls and six execution variants
pass (`20261005-130553/`), as does the original provider/combined loader regression
(`20261005-130932/`). Initial image loading also dirties pages conservatively,
causing literal reads to consult RAM; its performance cost is unmeasured.

`0005-explicit-code-modules.patch` adds explicit main-RAM AOT module slots.
`dc_begin_code_module` retires entries before loading, clears dispatch caches and
relocation aliases, and rejects overlapping slot declarations/non-quiescent
calls. `dc_activate_code_module` compares every loaded byte to a trusted compiled
manifest, validates all entries before registration, then activates the slot.
Managed targets reject inactive/undeclared entries before signature relocation.
Physical/P1/P2 and RAM mirror requests use the same managed slot.

SHA-256: `7ed99c5c1f14d89c580e134a4424fcff18c03e2b9eb681d4c482ee2187ff1c85`.
All direct and inline-fast dispatch paths are conservatively disabled while slots
exist, so generated static calls and indirect hints cannot bypass activation.
Ordinary checked dispatch still caches functions; retirement clears its cache.
Dispatch-depth tracking rejects activation/retirement during checked execution.
Performance is unmeasured. Multiple simultaneous active slots and real scheduler
integration remain untested. This is an opt-in experimental API, not automatic
invalidation of arbitrary guest code writes. The caller MUST retire the slot
before loading and activate at a quiescent host boundary; manifest/function
pointers must come from the trusted build. Runtime self-modifying code is not
thereby supported. Managed code copied outside its slot remains unverified.

`run-recovery-modules.py` proves independently compiled A -> B -> A, byte mismatch,
invalid/duplicate manifests without partial registrations, aliases, cached calls,
resident calls with an observed direct-hint positive control, unloaded/unknown
entries, cache/relocation retirement and non-quiescent rejection. Generated
function/registrar names are separated by per-translation-unit compiler macros;
no guest instructions are rewritten. First generator attempt exposed an incorrect
synthetic ELF section index (131513), fixed in the fixture. Initial proof 131617;
final expanded proof `20261005-132103/` also hashes all generated build inputs.

Full CPU regression `20261005-131706/` passes. The joined real provider/SBOARD
registration run `20261005-131931/` verifies two actual file loads and registration
in all four declared heap cases; missing loads leave retired code unavailable.
This is not full startup, task scheduling, gameplay or a production core decision.


`0006-explicit-data-ranges.patch` adds repeatable, half-open static data ranges to
raw discovery. Only explicit metadata suppresses heuristic candidates. Ranges
must be nonempty and within the raw image. Explicit roots/seeds and decoded or
resolved/candidate executable edges into data reject before target filtering;
final emitted-program instructions are checked too. Unresolved dynamic transfers
remain unresolved; this does not prove that data can never become executable.

SHA-256: `403b19c0de4f611b1fb3a7f03476f3246e020bde82eec7435d94efa270f3629b`.
No ranges means unchanged default behavior. `run-recovery-discovery.py` reproduces
four data false positives, preserves four valid veneers, excludes annotated data
without changing bytes, and rejects invalid ranges, explicit seeds, direct calls,
indirect calls through an alias and branches into data (`20261005-135649/`).
The first implementation's BSR negative control failed; checking executable
edges before heuristic filtering repairs the cause rather than dropping the test.

`audit-recovery-motion-data.py` binds annotations to the exact owner STG01 hash,
validated motion/model layout and observed consumer instructions. It requires
zero entry/instruction losses outside declared data and zero unknown/RAW counts.
The third motion has no proven consumer; that limit is explicit in its manifest.
Actual audit `20261005-135713/` passes, and its unannotated control reproduces the
original raw map byte-for-byte. This is a bounded static annotation facility,
not a general discovery correction or a gameplay claim. R3/G1 remain open.


`0007-gbr-long-load.patch` adds `MOV.L @(disp,GBR),R0` (`C6dd`):
unsigned disp8 multiplied by four, 32-bit load into R0, unchanged T.
Reference: [Renesas SH-4A software manual](https://www.renesas.com/en/document/mas/sh-4a-software-manual).
The decoder, IR, formatting and native emitter use the existing guest read
helper. The load is a register overwrite and a memory barrier for SQ fusion.
Unknown/RAW rejection remains enabled. No fallback or native override is added.

SHA-256: `47f603705ac6889411e797f0571c50eb9f7381931a84eccf2fb86e2066a960f1`.
`20261005-181501/report.json` passes twelve focused upstream tests and eight
native execution variants. The two new variants exercise all256 displacements,
two source bases with distinct 32-bit patterns, separate output words, memory
guards, input preservation, R0/result and R1/cursor, other GPRs, GBR, SR/T, PR and
return PC. One runs with GBR caching explicitly verified active. The other uses
the default uncached lane. Both also reject an unregistered target.
These are ordinary RAM tests; full exception/MMIO and Sonic rendering semantics
remain separate. The gap was observed in original mesh functions03742C/0375F0/
037970; the preceding failed generation181005 is retained.


`0008-gbr-load-analysis-clobber.patch` teaches CFG target inference and raw
callback heuristics that the GBR load overwrites R0. Without this, a prior
literal function address could survive the load in static analysis. The new
control reproduces that false resolution against the seven-patch baseline;
the eight-patch run leaves the target unresolved. Runtime dispatch remains
dynamic. Three raw local-path overwrite checks receive the same classification.

SHA-256: `95eb8404b02f986c17a5370cce5125fa8a9376cce78fe0921d3ccbc6134c0fd8`.
`20261005-181921/report.json` passes all13 call-discovery controls, twelve focused
upstream tests and eight native execution variants. The raw heuristic edits
have source review and regression coverage; the new counterexample specifically
checks CFG resolution. This does not establish complete indirect-call discovery.

`0009-explicit-system-flash.patch` replaces placeholder SYSINFO/flash responses
with an explicit128KiB read-only data input. `dc_set_system_flash_image` validates
size and invalidates SYSINFO initialization. INIT copies8 identity bytes and5
factory-property bytes to the24-byte record at8C000068, clearing its padding;
ID requires INIT and returns the record address. FLASHROM_INFO returns one of
five partition extents; READ copies the requested bytes and returns the byte
count. Overflow/out-of-range reads return-1 without writing; destinations outside
the supported main-RAM/alias range and unsupported commands throw explicitly.
Icons, flash writes/erase, direct flash-MMIO and persistence are not implemented.

The byte-count return follows [KallistiOS's documented syscall contract](https://github.com/KallistiOS/KallistiOS/blob/d458073c00025f065cc68bebb8ef15ea83264e2b/kernel/arch/dreamcast/include/dc/syscalls.h)
and the [public redream implementation](https://github.com/inolen/redream/blob/master/src/guest/bios/syscalls.c).
KallistiOS's wrapper forwards the result unchanged. Flycast REIOS returns0,
and Sonic6078C4 tests for0 before consuming an optional two-byte override.
This disagreement is recorded rather than hidden: hardware parity remains open;
no game-specific forced return is installed. System-info layout is documented
in the pinned public references under the ignored BIOS-service review directory.

`tests/recovery/system-flash-check.hpp` exercises isolated service endpoints with
nonzero data, image replacement, exact RAM/context comparisons, RAM aliases,
partition layouts, source/destination bounds and unsupported-command rejection.
The provider oracle executes these checks before the game experiment; the live
runtime receives none of that synthetic test state. A separate `--system-flash`
input is required for original startup to pass SYSINFO. No BIOS/game download or
production foundation change is implied. `us-factory-v1.bin` is explicitly a
synthetic factory-only experiment with erased user partitions, not a full device
configuration. Earlier baseline snapshots remain immutable.

SHA-256: `df46a6d1bd55b55371031eee845c8425309199aba305b9632a0e4772a9130d35`.

## 0010 — Explicit read-only FIFO status

The0x005F688C SB_FFST register now returns the empty status explicitly and rejects
writes, including byte/halfword read-modify-write paths. The current runtime
completes CPU bus writes synchronously, so it has no posted FIFO contents between
guest operations. This model does not establish bus latency/contention accuracy.
KallistiOS's pinned dc/fifo.h and g2bus.c establish bit0 AICA, bit4 G2 and bit5
SH4 nonempty masks; Flycast's pinned sb.cpp registers read-only status and returns0.
References and hashes are in the private g2-status review directory. No reference
implementation code was copied into this independently written patch.

The original seven-instruction1115C8 reduction passes512 full RAM/context cases
with controlled busy/empty values before the patch and512 empty-FIFO/read-only
cases after it, including ignored writes and completed sound memory/register
writes. Busy injection is an isolated generic-MMIO test, never live game state.

SHA256: `a7631fe208515757139300b309dc4d0a6cc411381d196d74e03c52b5f204cbd2`.

## 0011 — Post-BIOS sound CPU idle state

The experimental HLE handoff initializes the first sound-RAM word to the ARM
self-branch instruction0xEAFFFFFE. Pinned Flycast REIOS59ed35a7 sets the same
initial word before entering the title. This replaces the omitted public boot
state; it does not install a retail BIOS, supply a sound driver, complete IP.BIN
execution or force a title callback/handshake. The original game still owns
sound reset, RAM clearing, driver upload and subsequent release.

The isolated check compares all2MiB of sound RAM, executes52,768 ARM instructions
atPC0 without a fault or RAM change, and verifies reset stops execution. The
original G2 reader's512 exact RAM/context/read-only checks also pass in this
generated runtime. Full startup and sound playback acceptance remain separate.
Source: [pinned public REIOS boot](https://github.com/flyinghead/flycast/blob/59ed35a7ea7c1940d4c8ac221a662d0e6d6dc9ea/core/reios/reios.cpp#L655).

SHA256: `d1c9b34bfda00ef59f2e0663773a77f3796c6eb22740b774c3e4753b0b79e36f`.

## 0012 — In-memory flash programming and erase

Raw BIOS command2 clears bits using guest RAM and returns the programmed byte
count; command3 erases only an exact declared partition start and returns0.
Out-of-range programming and invalid erase offsets return-1 without mutation.
Unsupported RAM sources throw before mutation. The explicit128KiB image is
copied into runtime memory; these services never write the input file.

The contract follows the inspected, pinned
[KallistiOS flash API](https://github.com/KallistiOS/KallistiOS/blob/d458073c00025f065cc68bebb8ef15ea83264e2b/kernel/arch/dreamcast/include/dc/flashrom.h)
and [Flycast raw services](https://github.com/flyinghead/flycast/blob/59ed35a7ea7c1940d4c8ac221a662d0e6d6dc9ea/core/reios/reios.cpp#L290).
No reference implementation code was copied. The prior READ return discrepancy
remains recorded under0009; this patch does not change that ABI choice.

31 read-only controls and32 programming/erase controls compare complete RAM,
flash and context, including aliases, guards, boundaries, repeated programming,
all partition extents and readback. The same test rejects the old runtime's
unsupported valid write. Original034703 subsequently formats only its18-byte
user-partition header and populates the exact16KiB cache. This is not a valid
configuration block, settings-query success, hardware timing, direct flash-MMIO,
disk persistence, normal boot or gameplay proof. Earlier0009 snapshots retain
their read-only scope. Current captures record both before/after flash images.

SHA256: `1729b657cfe4418690e087b0d88474a6e6197c509390998e92f6b16ea25d9b34`.

## 0013 — ARM64 floating-point candidates (not promoted)

Three preserved alternatives apply on top of the unchanged twelve-patch source:
`0013-arm64-fpu-mode.patch` is the initial mode fix;
`0013-arm64-fpu-mode-owned.patch` adds host-state ownership;
`0013-arm64-fpu-domain.patch` additionally synchronizes checked guest entry and
in-guest FPSCR writes. They are alternatives, not a sequential patch stack.
None is added to the default runner.

The latest candidate preserves FPCR/FPSR across checked dispatch and unwinding,
uses nearest/flush-off for host renderer work, synchronizes guest arithmetic and
emits Clang ARM64 FENV pragmas. The actual emitter's runtime matches the staged
source byte for byte. Eight original65EB00 prefix controls,64 host-state controls,
eight renderer cases and24 original cached-transform entry cases pass. Earlier
host leaks and18/24 cached-entry failures remain preserved.

All257 generated guest units in the saved scene harness compile with strict flags.
After one registration-macro command repair, both scenes return and match v3's
six images/two collision RAM captures. Nine candidate follow-up audits pass.
Legacy geometry still expects nearest translation; a separate source-bound RZ
consistency audit passes without replacing it or its reference images. These are
not general vector/exception/denormal or normal-boot proofs. Global promotion
remains gated; isolated scalar-prefix experiments may use the explicit candidate.
See STATUS.md for exact checkpoints and remaining scope.
