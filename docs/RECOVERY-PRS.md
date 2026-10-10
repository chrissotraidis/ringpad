# PRS, loader and stage-entry recovery evidence

Private R3 research, 2026-10-05. Original Katana/PAL G1 remains unresolved.

Latest: the original provider now initializes, opens a real filename, reads and
closes it, and the original combined loader decompresses Mini Cart byte-for-byte
as native ARM64. The final section records current evidence; earlier sections
retain the chronological investigation and superseded next steps.

## Input and decoder identities

The owner-held V1.004 disc's `SONICADV/MINICART.PRS` decodes successfully with
the bounded Python reader and separately compiled public Rust decoder. Outputs
are byte-identical. Eight synthetic Python tests cover literals, both copy
encodings, overlapping copies, extended length, control reload, truncation,
invalid references and the output-size bound. No game data is in those tests.

| Identity | Observed value |
|---|---|
| Encoded bytes | 331,320; exactly consumed, no trailing bytes |
| Encoded SHA-256 | `8f8d410eafb2315b79d52588ce7a5d249018277009f036091caf4651d7adc17b` |
| Decoded bytes | 1,434,052 |
| Decoded SHA-256 | `002ab2a1f083d1a41902ced39c2e47e929fae68b33fa7094b5c748a8d22b09b6` |
| Expected decoded SHA-256 in pinned SARecomp | `2d3ec72d9f62a0ec626155f822a77bac7209999aa89f825b3082a69cf85db2a1` |

Matching size is not matching content. Neither encoded nor decoded identity
passes the existing SARecomp contract. Source inputs and extracted bytes were
not modified. Derived images live only under ignored `work/` and artifacts.

Reference codec: [Sewer56/prs-rs](https://github.com/Sewer56/prs-rs/tree/db0deb17bf433a69f4478e1d7b4a7c1bb85854f2),
commit `db0deb17bf433a69f4478e1d7b4a7c1bb85854f2`, GPLv3. Only these public
source files and its license were fetched; no packaged binaries or game data:

| Repository path | SHA-256 |
|---|---|
| `src/prs-rs/src/impls/decomp/common.rs` | `dd2484c4b2a507d73468a6f4e8e45f127d2bb5d3a8cfe34ec6bdec90f8be80ee` |
| `src/prs-rs/src/impls/decomp/decompress.rs` | `727a17391fa3d896fbcea22e4ec67a6ca0835f81ccc13fad40f1349d10c4acec` |
| `LICENSE` | `b8050c0bda7f43b06713ad96d56c545fcfc5cb9ec1ef0b69fb6b4ba93bf7dae9` |

The untouched files are staged as `common.rs`, `decompress.rs` and `LICENSE` in
`work/dreamcast-recovery/prs-reference/`. Its `provenance.json` records exact raw
source URLs and rustc 1.96.1. This utility is a separate offline reference, not a
linked SonicPad component. The wrapper calls its unsafe decoder only after the
bounded Python reader has validated the same input and established output size.

## Placement and entry evidence

Treat `0x8C900000` as a placement hypothesis. The decoded header loads the
physical metadata pointer `0x0C916C80` into R5 and tail-jumps to resident
`0x8C0A1690`. Under that placement, metadata lies at decoded offset `0x16C80`.
The local resident code reads callback fields at offsets 8, 12, 16 and 20;
those fields point to `0x8C90A120`, `0x8C90A14E`, `0x8C90A1F8` and `0x8C90A2C8`
after normalizing the RAM alias. These relationships support the hypothesis;
the actual game loader has not been executed.

Reference-derived result-path seeds at offsets `0x7A` and `0xA81E` are explicitly
separate from those four local metadata callbacks. Their instruction starts are
present, but the reference's PAL content hash is not reused. Katana's synthetic
`0x82980000` code address is not a Dreamcast RAM placement for this candidate.

Module analysis converges in four passes: 211 function entries, 19,715 unique
instruction addresses, no unknown/RAW operations in the discovered set, and
1,044 external call records at 867 sites. These target 147 distinct resident
addresses. Before enrichment, 994 records/119 targets matched known entries;
50 records/28 targets lay inside the resident image but outside discovered code.

Seeding those 28 addresses expands resident analysis from 6,103 to 6,160 entries
and from 513,457 to 520,807 unique instruction addresses. All seed entries decode
without unknown/RAW operations. All 1,044 module call records now match analyzed
resident entries. The main image still has 1,064 external records of its own.
Seed membership and static linkage do not prove execution, semantic correctness,
relocation correctness or completeness of all dynamically loaded programs.

## Reproduction

Use a fresh ignored output directory for each decoder/comparison invocation:

```sh
python3 tests/recovery/test_prs.py
python3 scripts/decode-recovery-prs.py \
  docs/artifacts/recovery-aot/20261005-103149/MINICART.PRS \
  work/dreamcast-recovery/minicart-decoded-new

PRS_REFERENCE_SOURCE="$PWD/work/dreamcast-recovery/prs-reference" \
DEVELOPER_DIR=/Library/Developer/CommandLineTools \
  rustc --edition=2021 -O tests/recovery/prs-reference.rs \
  -o work/dreamcast-recovery/prs-reference/prs-reference
work/dreamcast-recovery/prs-reference/prs-reference \
  docs/artifacts/recovery-aot/20261005-103149/MINICART.PRS \
  work/dreamcast-recovery/minicart-decoded-new/rust-decoded.bin 1434052
cmp work/dreamcast-recovery/minicart-decoded-new/decoded.bin \
  work/dreamcast-recovery/minicart-decoded-new/rust-decoded.bin

python3 scripts/run-recovery-aot.py \
  --coverage work/dreamcast-recovery/minicart-decoded/decoded.bin \
  --base 0x8C900000 \
  --seed-file work/dreamcast-recovery/minicart-decoded/entry-seeds.txt
python3 scripts/run-recovery-aot.py \
  --coverage docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  --seed-file work/dreamcast-recovery/minicart-links/resident-seeds.txt
python3 scripts/compare-recovery-links.py \
  docs/artifacts/recovery-aot/20261005-105205 \
  docs/artifacts/recovery-aot/20261005-105333 \
  work/dreamcast-recovery/minicart-link-comparison-new \
  --resident-base 0x8C010000 --resident-size 6751168
```

`entry-seeds.txt` contains the four metadata callbacks followed by
`0x8C90007A` and `0x8C90A81E`, one address per line. The link-comparison command
emits entry hypotheses for a subsequent resident scan, with its CSV inputs hashed.
The coverage runner records source pin, exact patch, seed hashes, placement,
commands, limits and return codes. It emits no Sonic C++ or game executable.

Evidence: decoded identity/layout in `work/dreamcast-recovery/minicart-decoded/`;
codec source/provenance in `work/dreamcast-recovery/prs-reference/`; source-bound
resident coverage `docs/artifacts/recovery-aot/20261005-105205/`; module coverage
`20261005-105333/`; final linkage in `work/dreamcast-recovery/minicart-links-seeded/`.

## Complete PRS/BIN filename inventory, limited structural classification

The bounded scanner read all 843 PRS and 535 BIN files from the verified local
disc inventory. All PRS streams decoded and consumed their complete inputs;
all raw BIN reads completed. Original CDI hashes before/after match. No file
hit the 16 MiB input/output, 15-second per-file or 10-minute scan bounds.

| PRS prefix observation | Count | Interpretation |
|---|---:|---|
| `PVMH` | 582 | Identified prefix, not an audit of every embedded payload |
| Mini Cart-style registration header | 3 | Local metadata yields placement/callback hypotheses |
| SH-4 stack prologue | 23 | Candidate code, including stage and adventure-field files |
| Other | 235 | Unclassified; not declared data-only |

All 535 BIN files fall outside those narrow header patterns. This includes the
known main boot program, whose independent read hash still matches the earlier
extractor. Thus “other” is explicitly not an exclusion of executable content.
Other disc extensions (MLT/DRV/AFS and media formats) have directory identities
but have not received equivalent executable-payload analysis.

The three registration files are `AL_RACE.PRS`, `MINICART.PRS` and `SBOARD.PRS`.
Each has exactly one 64-KiB-aligned placement hypothesis under the observed
metadata constraints. The scanner records all hypotheses rather than choosing
one silently; it does not discover arbitrary unaligned placements. All 26
registration/prologue candidates decode byte-identically with the separate Rust
reference. Four source-free classifier tests pass, including changed placement,
invalid callback alignment, out-of-bounds header literal and unknown prefixes.

| Module | Placement hypothesis | Entries | Unique instruction addresses | External records |
|---|---|---:|---:|---:|
| AL_RACE | `0x8CB80000` | 594 | 38,480 | 1,125 |
| SBOARD | `0x8C900000` | 108 | 3,960 | 320 |

AL_RACE converged after six passes and SBOARD after four, with no unknown/RAW
operations in their discovered sets and no budget exhaustion. Their callback
seeds came from their own metadata. Against the 6,160-entry resident scan:

- AL_RACE: 990 records match existing resident entries; 20 records identify
  15 further in-image targets; 31 are unresolved dynamic records; 84 have targets
  outside resident discovery. The latter include 28 addresses in the presumed
  AL_MAIN overlay range, plus values 3 and 6 requiring dataflow review.
- SBOARD: 302 records match resident entries; 16 records identify seven additional
  in-image targets; two are unresolved dynamic records.

Do not combine Mini Cart, SBOARD and presumed AL_MAIN at `0x8C900000` into one
resident image. These are distinct overlay identities; load lifetime remains
unverified. Stable static analysis is not runtime linkage or gameplay proof.

```sh
python3 tests/recovery/test_prs_headers.py
python3 scripts/scan-recovery-prs.py \
  docs/artifacts/recovery-aot/20261005-103149 \
  docs/artifacts/recovery-aot/20261005-105205 \
  work/dreamcast-recovery/prs-candidates-new --retain-non-pvm
python3 scripts/scan-recovery-prs.py \
  docs/artifacts/recovery-aot/20261005-103149 \
  docs/artifacts/recovery-aot/20261005-105205 \
  work/dreamcast-recovery/bin-inventory-new --kind bin
```

Private scan reports: `work/dreamcast-recovery/prs-candidates/report.json`
(all PRS identities, prefixes and placement hypotheses), `reference-checks.json`
(26 independent decoder matches), and `work/dreamcast-recovery/bin-inventory/report.json`.
Non-PVMH PRS candidates are retained separately (261 images, 71,070,317 decoded
bytes); this is retention for analysis, not selection of 261 code modules.
Static evidence: `docs/artifacts/recovery-aot/20261005-110624/` (AL_RACE),
`20261005-110731/` (SBOARD); links in `work/dreamcast-recovery/al-race-links/`
and `sboard-links/`.

### AL_MAIN and call-target reliability

AL_MAIN decoded identity: 2,226,336 bytes, SHA-256
`8ef90203bb56bd559b1a5626212193ae6b7516f044e14cd2140e214cc5be10ad`.
Under the explicit `0x8C900000` placement hypothesis, seeded with the 28 AL_RACE
references, discovery converges in five passes: 324 entries, 41,429 unique
instruction addresses and 1,479 external records. No unknown/RAW operations in
the discovered set or budget exhaustion. All 28 seeded addresses are represented
in analyzed code; they account for 82 AL_RACE records. Because the addresses were
supplied as seeds, this is byte/discovery consistency, not independent proof of
correct entry points, placement, loader behavior or full closure.

Against the resident scan, AL_MAIN has 1,389 records matching 122 entries,
69 records pointing at 23 new in-image addresses, and 21 unresolved dynamic
records. Preserve those new addresses as hypotheses until call analysis is
repaired. Evidence: `docs/artifacts/recovery-aot/20261005-111328/`;
`work/dreamcast-recovery/al-race-to-main-links/` and `al-main-resident-links/`.

AL_RACE sites `0x8CB8DE86` (JSR R9) and `0x8CB8DEDE` (JSR R12) were inferred
as targets 6 and 3. Local control flow separates the loop-counter assignments
from the calling paths. Entry literals contain physical addresses `0x0CB91E9C`
and `0x0CB9227C`, respectively. Their canonical addresses already belong to
discovered AL_RACE code. This is a branch/literal review, not module execution;
private details: `work/dreamcast-recovery/al-race-call-review.json`.

The pinned analyzer's backward linear fallback ignores branches. The new
source-free `tests/recovery/indirect-analysis.cpp` reproduces a false resolved
target 6 from a path that cannot reach the call. This demonstrates that the
earlier `resolved` CSV column is an analyzer assertion, **not a proven edge**.
Matching an inferred address with an existing function is likewise insufficient
to prove linkage. Do not treat old static counts as semantic acceptance.

The IR preserves dynamic JSR semantics; emitted direct hints compare against
the actual register target and otherwise dispatch dynamically. A native ARM64
oracle passes the call path (42), non-call path (0), and changed callback pointer
(84). Both callbacks are explicitly seeded, isolating runtime dispatch from the
known discovery defect. The diagnostic still reports `static_false_target: true`;
its successful execution does not mean static analysis is repaired.
All eight focused tests and six native variants pass in
`docs/artifacts/recovery-aot/20261005-112056/`.

### Control-flow repair, 11:36 CDT

The counterexample above is now a passing regression. Recorded patch
`0002-cfg-call-analysis.patch` merges constants over discovered CFG edges,
retaining a value only when known predecessors agree. It handles cycles by
discarding disagreeing values, samples indirect targets before their delay slots,
and invalidates registers after calls without a preservation summary. Tail calls
and file-backed pointer estimates remain supported. Initial bytes and discovered
CFG edges are still hypotheses about runtime state; this is not an immutable
memory, complete branch-table or full-game proof.

Ten analysis controls pass (joins, loops, delay slot, straight line, callee
clobbers and implicit GPR updates). The same regression rejects the original
analyzer. Twelve selected upstream tests and all six native execution variants
pass. The upstream two-callback join assertion was corrected to require an
unresolved single target while retaining both candidate checks. The initial
over-conservative repair failed a legitimate tail-call test; that regression was
fixed before the final results below. No runtime dispatch override was added.

| Final static audit | Before repair | After repair |
|---|---:|---:|
| AL_RACE entries / unique instruction addresses | 594 / 38,480 | 594 / 38,480 |
| AL_RACE external records | 1,125 | 1,625 |
| AL_RACE unresolved records | 31 | 700 |
| Seeded resident entries | 6,160 | 6,149 |
| Resident unique instruction addresses | 520,807 | 519,136 |
| Resident external records | 1,064 | 7,499 |
| Resident unresolved records | 973 | 7,456 |

AL_RACE `0x8CB8DE86` now estimates `0x0CB91E9C`, agreeing with the entry literal;
`0x8CB8DEDE` is unresolved because a preceding callee could change R12.
These are read from the new `all-calls.csv`, including internal calls, rather
than inferred from absence in the external-call list. Resident closure still
converges in 14 passes without cap exhaustion. No new entries were added;
11 entries and 1,671 instruction addresses disappeared. Do not declare them
unneeded: their exact identities are in the comparison report.

Final evidence under `docs/artifacts/recovery-aot/`:
`20261005-113352/` (tests/native oracles), `20261005-113429/` (AL_RACE),
`20261005-113525/` (resident). Private supplemental evidence:
`work/dreamcast-recovery/cfg-negative-control/report.json` (original failure),
`cfg-upstream-tests/` (first broad-test failure and repair), and
`cfg-final-resident-comparison.json` (hashed old/new sets and missing entries).
Earlier intermediate patch reports are retained and are not the final result.

The missing-entry investigation and overlay re-audits below supersede that next
step. No full-game or all-executable-coverage claim yet; G1 stays open.

### Retained candidates and restored entries

Re-running the original analyzer with the new reporting wrapper reproduced its
6,160 entries / 520,807 unique instruction addresses and exported the old call
edges. Every missing entry was reviewed against actual bytes, literal references
and incoming calls. `0x8C09B82A` has a literal-loaded tail-jump predecessor;
an unknown jump on a different path caused the conservative analysis to withhold
all estimates. The `0x8C0A5BC0` cluster is associated with a 24-byte record table
(including a `SEQ EDITOR` label), not proven required gameplay. Other entries
are literal-loaded continuations, table callbacks or their direct descendants.
None was discarded merely because it looked optional.

Patch `0003-cfg-call-candidates.patch` retains values encountered during CFG
propagation as **discovery candidates**, separately from a uniquely resolved
call. Raw closure applies its existing executable-entry checks. This recovers
ten missing entries without changing the runtime dispatcher or assuming callee
register preservation. It does not enumerate every possible target.

The remaining `0x8C0ED168` resides in a locally reviewed callback table at
`0x8C569EEC`: six records, stride 12, pointer field offset 8. The consumer computes
`index * 12 + base` and calls the pointer at offset 8. Its index remains unknown
across a callee. Four nonzero initial-image callbacks are therefore recorded as
explicit seed hypotheses, not one resolved call:
`0x8C0ED168`, `0x8C0ED17E`, `0x8C0ED194`, `0x8C0ED1CA`.
The manifest binds the full boot SHA and table SHA
`de450b43e8922786890c029826fc1024a4f7db2123df66376b6ac1bafe9ad266`.
The reviewed six-record bound is not a general table-discovery claim.

Result: **6,163 entries / 520,848 unique instruction addresses**, restoring every
old entry and instruction address and adding three callbacks / 41 addresses.
Closure converges in 14 passes without cap exhaustion. Its 7,570 external records
include 7,527 unresolved targets and 43 targets without symbols. Restored code
discovery does not mean those unresolved calls are proven or executable end to end.

Twelve analysis controls, twelve selected upstream tests and six native execution
variants pass. The impossible-path counterexample must not acquire a false
candidate. AL_RACE's original bad values 3/6 remain absent from resolved calls
and candidate lists; the R9 site retains its entry literal while R12 remains
unknown across a callee.

Final overlay re-audits retain all prior entries and instruction addresses:

| Overlay | Entries | Unique instruction addresses | External records |
|---|---:|---:|---:|
| Mini Cart | 211 | 19,715 | 1,055 |
| SBOARD | 108 | 3,960 | 320 |
| AL_MAIN | 324 | 41,429 | 1,490 |
| AL_RACE | 594 | 38,480 | 1,625 |

Candidate comparisons keep each overlay separate. Mini Cart candidates all
match its own or resident entries. The other modules supply 40 distinct new
resident-address hypotheses; AL_RACE also references the separately placed
AL_MAIN image. Initial address consistency is not relocation/load-lifetime proof.

Evidence under `docs/artifacts/recovery-aot/`: `20261005-114809/` (tests),
`20261005-114858/` (ten entries recovered automatically), `20261005-115130/`
(reviewed callback-table seeds), `20261005-115316/` (AL_RACE),
`20261005-115532/` (Mini Cart), `20261005-115534/` (SBOARD),
`20261005-115536/` (AL_MAIN). Private incoming edges, function/CFG details,
table/seed manifests, hashes, comparisons and regression evidence are in
`work/dreamcast-recovery/lost-entry-review/`.

The first patch-package attempt incorrectly represented sparse-checkout omissions
as deletions in a temporary index. The exact-diff guard rejected it before any
application or build. Restricting extraction to the three changed source files
produced the verified patch. Original source/worktree files were preserved.

### Overlay-derived resident enrichment

The 40 new resident-address hypotheses are bound to their originating candidate
CSV/JSON hashes in `lost-entry-review/overlay-resident-seeds.json`. Supplying
them with the prior Mini Cart and reviewed table seeds yields **6,242 entries /
526,980 unique instruction addresses**, 14 passes, no cap exhaustion and zero
unknown/RAW instructions in that discovered set. All 40 seeds are represented.
There are 7,650 external records: 7,607 unresolved and 43 without callable symbols.
Candidate registration has not converted those unresolved calls into proven edges.

Every enumerated candidate for Mini Cart, SBOARD, AL_MAIN and AL_RACE now matches
an analyzed entry in the appropriate module/resident image. AL_RACE additionally
uses the separate AL_MAIN placement hypothesis. Overlapping Mini Cart/SBOARD/
AL_MAIN images were not merged. This is consistency for the enumerated candidates,
not exhaustive code discovery, loading, ABI or gameplay proof.

Evidence: `docs/artifacts/recovery-aot/20261005-115820/` and
`work/dreamcast-recovery/lost-entry-review/overlay-enriched-result.json`.

```sh
python3 scripts/run-recovery-aot.py \
  --coverage docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  --seed-file work/dreamcast-recovery/minicart-links/resident-seeds.txt \
  --seed-file work/dreamcast-recovery/lost-entry-review/callback-table-seeds.txt \
  --seed-file work/dreamcast-recovery/lost-entry-review/overlay-resident-seeds.txt
```

Previous next step (addressed by the trace below): trace the resident loading/decompression path from the verified
filename strings, beginning with `MINICART.PRS` at `0x8C15CFB0`. The anchors
are recorded in `lost-entry-review/loader-name-anchors.json`. Confirm placement,
entry invocation and callback registration against code before treating the
overlay hypotheses as loader behavior; then expand the remaining prologue and
unclassified-format inventory. G1 and R3 remain open.


## Native execution of the original PRS routine

The resident loader at `0x8C09F664` preserves the destination, reads the named
file through `0x8C09E8A0` into `0x0CD00000`, and on a nonnegative read result
calls `0x8C04E260` with that input and saved destination. It subsequently calls
`0xAC5FFE02`; that helper's semantics remain unverified. The loader returns the
file-read result, not the decoder's byte count. This is a static trace, not a
successful file-I/O or complete-loader execution.

The isolated decoder occupies **152 bytes / 74 analyzed instructions** at
`0x8C04E260..0x8C04E2F8`. Routine SHA-256:
`712feef2faa16af74be8dcee29e925ff5cdda767bbbcb4419fd9f5b88869c877`.
The runner requires boot SHA-256
`68478ff33fe6c4d48a423b363b3e14669106040d01284d889aa4db69ab2e0b29`.
It emits private C++, compiles it to a Mach-O ARM64 executable, and executes the
actual routine using the pinned public runtime plus the three recorded patches.
No interpreter fallback or missing-function no-op was added.

All nine cases pass: five synthetic streams and Mini Cart, AL_MAIN, AL_RACE and
SBOARD. The real outputs are respectively 1,434,052, 2,226,336, 343,956 and
508,012 bytes. Python validates each stream before execution; real outputs also
match the hash-bound, previously recorded independent Rust results. Checks
cover every output byte, exact input consumption, return length/pointers,
stack/return PC/PR, preserved R8–R14, input bytes and surrounding output guards.
An unregistered target throws explicitly. Linked-import screening finds no
executable-memory or process-launch imports in this test binary.

This is the first native execution proof for an isolated piece of this owner's
Sonic image. It establishes neither app boot nor loading, graphics, sound,
mutable-code behavior or gameplay. `r3_complete` and `r4_complete` remain false.
Evidence: `docs/artifacts/recovery-aot/20261005-121144/`.
Implementation: `scripts/run-recovery-prs-aot.py` and
`tests/recovery/prs-execution.cpp`; reproduction is in [BUILDING.md](BUILDING.md).

## Actual stage entries replace root-offset assumptions

The selector loads the common destination `0x0C900000`; the AL_RACE and
AL_GARDEN00 secondary loads use `0x0CB80000`. A separate stage table at
`0x8C1A1B68`, consumed by `0x8C051266`, supplies the task entry. Task creation
passes it through `0x8C09A1CE` / `0x8C09A28C`, storing it at task offset 16.
The dispatcher loads that field at `0x8C09A44C` and calls it at `0x8C09A456`.
This is a source-derived static chain; the scheduler has not been executed.

| Stage index | Selected module | Task entry, normalized RAM alias |
|---|---|---|
| 35 | Mini Cart | `0x8C900120` |
| 38 | SBOARD | `0x8C900040` |
| 39 | AL_GARDEN00, loaded after AL_MAIN | `0x8CB80000` |
| 42 | AL_RACE, loaded after AL_MAIN | `0x8CB80040` |

The first, second and fourth entries were absent from the earlier inventories.
The load address is therefore not sufficient as a function-entry assumption.
Mini Cart's actual header references metadata `0x0C917498` and callbacks
`0x8C90C8C0`, `0x8C90C8FC`, `0x8C90C982`; SBOARD's references metadata
`0x0C900188` and callbacks `0x8C903140`, `0x8C903148`. Both tail-jump to
registration helper `0x8C0A1690`, which installs metadata callback fields into
existing task fields and conditionally invokes the initialization callback.
AL_RACE's actual entry is an ordinary state dispatcher, not that header form.

| Re-audit | Entries | Unique instruction addresses | External records | Artifact timestamp |
|---|---:|---:|---:|---|
| Mini Cart | 212 | 19,719 | 1,056 | `20261005-121751` |
| SBOARD | 108 | 3,964 | 314 | `20261005-121754` |
| AL_RACE | 598 | 38,595 | 1,632 | `20261005-121755` |
| AL_GARDEN00 | 24 | 1,089 | 85 | `20261005-122229` |
| AL_MAIN, including garden-derived targets | 328 | 41,622 | 1,501 | `20261005-122345` |

SBOARD replaces one old entry with the actual stage entry. The removed entry,
`0x8C903146`, is the store in the RTS delay slot of callback `0x8C903140`.
Every old instruction address remains represented, including that store;
`0x8C903148` remains a separate callback. This comparison establishes no separate
caller into the delay slot. Candidate/entry counts can change without lost code.

AL_GARDEN00 is owner-disc candidate 0033: 30,165 encoded bytes, 85,824 decoded,
SHA-256 `a88d6966e7d9a5731bef27538f31f14b0d35f1bef86d9823e6d4005d578fc9df`.
Its original decoded output already matched the independent Rust reference.
The stage-selected placement/root now gives a bounded audit with zero unknown/
RAW operations in its discovered code, 13 unresolved external records, and five
new targets: three resident entries and two AL_MAIN entries. The latter seeds
expand AL_MAIN as shown above. These are static candidates, not proven calls.
There are now 21 other prologue-prefix PRS candidates without a module audit;
nonmatching prefixes, other formats and auxiliary execution also remain open.

Private trace, seed hashes, instruction comparisons and linkage evidence are in
`work/dreamcast-recovery/loader-review/`. In particular, `loader-chain.json`
records the loader and task route; `stage-entry-reruns.json` contains exact
commands, and `stage-entry-linkage.json` records the initial missing targets.


The three new resident seeds converge at **6,248 entries / 527,202 unique
instruction addresses** in `20261005-122414/`, retaining every prior resident
entry/address. AL_MAIN likewise retains its prior entries/addresses. All
currently enumerated static targets/candidates from the five audited modules
match entries in their identified module/resident images. This does not resolve
unknown dynamic targets or establish exhaustive coverage. Final comparison:
`loader-review/stage-enriched-result.json`.

Previous next experiment (completed below): inspect allocator `0x8C09A720` and failure cleanup
`0x8C09A426`, called by registration `0x8C0A1690`. The actual SBOARD metadata
requests one byte, initializes it through `0x0C903140`, and installs
`0x0C903148` as task execution callback. Establish the allocator's valid initial
state, then execute that exact registration path with a declared task/heap
fixture. Check the allocated byte, task fields, stack/register preservation and
explicit failure behavior. Do not replace these callees with success stubs or
alter metadata to skip allocation. A bounded fixture is not a full scheduler.
The complete file-read/post-decode-helper path and remaining module inventory
remain separate next gates. No further Katana search without a new source lead.


## Native SBOARD registration, including actual allocation

The five original routines now execute together as native ARM64: stage header
`0x8C900040`, registration `0x8C0A1690`, allocator `0x8C09A720`, initialization
callback `0x8C903140`, and failure scheduler `0x8C09A426`. The private generator
extracts **296 unchanged code/literal/metadata bytes**, analyzing 124 instructions.
It supplies the actual initialization callback as a seed for the unresolved
metadata-driven call. No native function replacement or guest-byte patch is used.

The allocator reads free-list head `0x8C7A32BC`, rounds the one-byte request to
32 bytes including its 16-byte header, and removes or splits a free block. The
fixture declares a valid empty-linked block plus end sentinel, and an existing
task; it does not claim to execute heap/task startup. Four cases pass:

- A 256-byte block splits, the allocated byte is initialized to zero, and the
  actual execution callback `0x0C903148` is installed.
- An exact 32-byte block is consumed and clears the free-list head.
- A 16-byte block fails without changing the heap.
- An empty free list fails without changing the heap.

On allocation failure, the original code returns null and replaces task+16 with
`0x8C09A2FC`, scheduling cleanup. It does not immediately free the task. Tests
compare every byte in the heap/task arenas plus surrounding guards, original
metadata, free-list head, returned task/null, stack/return PC/PR and R8–R14.
Executing the installed gameplay or deferred-cleanup callback fails explicitly
because those continuations are outside this slice. This is not scheduler boot.
The linked-import check passes; original input hashes remain unchanged.

Evidence: `docs/artifacts/recovery-aot/20261005-123149/` and
`loader-review/registration-contract.json`. Reproduce:

```sh
python3 scripts/run-recovery-registration.py \
  docs/artifacts/recovery-aot/20261005-080717/BOOT.BIN \
  work/dreamcast-recovery/prs-candidates/candidate-0698/decoded.bin
```

### The next loader boundary is an initialized file provider

The file-open wrapper `0x8C605416` dispatches through offset 60 of the table
selected by global `0x8C677160`, initially zero in the image. Initializer
`0x8C605358` installs table `0x8C6701F4` and calls its initialization callback
`0x8C603D78`; its open callback is `0x8C604214`. These routines are present in the
owner image. This is an initialization/contract investigation, not evidence of
missing downloadable source. Do not force the global/table into place and call
that successful initialization.

The file wrapper then calls size helper `0x8C604B6C`, arithmetic helpers with
2048, sector reader `0x8C604C5E`, and close helper `0x8C604B34`. It returns -1 for
null open, but returns zero after a non-null open without checking read/close
results. Success must therefore be judged by actual bytes and provider status,
not just this wrapper's return code. The arithmetic helper semantics are not yet
independently verified.

The post-decode routine at `0xAC5FFE02` reads `0xFF00001C`, ORs `0x800`, and
writes it back. The public candidate's internal-MMIO path provides sparse shadow
backing and explicitly leaves device-specific side effects open. A successful
register write would not prove cache/code-lifetime correctness for loaded AOT
modules. Preserve this boundary in the runtime decision.

**Next:** trace provider initialization and open/read completion down to actual
public-runtime disc services; identify state, callbacks and host I/O required
for one real file read before attempting the complete loader. Keep correct
file bytes, status/failure handling and post-decode semantics as separate checks.
Evidence: `loader-review/file-provider-contract.json` and per-routine disassembly.
R3/R4 and original G1 remain open; no game or Simulator launch.


## Native original BIOS wrappers read the owner disc

Four original wrappers at `0x8C10F558`, `0x8C10F56C`, `0x8C10F580` and
`0x8C10F594` compile to native ARM64: 80 original bytes, 24 instructions. They
invoke the candidate's existing **GD-ROM HLE service** through the BIOS vector
at `0x8C0000BC`. This explicitly uses a host implementation of the BIOS service;
it is not an original BIOS execution or the game's filename-provider path.
No downloaded BIOS, interpreter fallback or added success stub was used.

Both read commands 16 and 17 return the exact **162 sectors / 331,776 bytes**
containing Mini Cart, including final-sector padding. The first 331,320 bytes
match the earlier extracted PRS hash. All sector bytes are independently read
from the original CDI using the recorded track geometry before comparison.
The test checks busy/completed/acknowledged status, runtime sector/byte counts,
output guards and preserved wrapper registers/return state. Unmapped FAD and
missing-disc failures are checked against explicit errors. Original CDI, map
and boot hashes remain unchanged; native identity/import screening passes.

Final evidence: `docs/artifacts/recovery-aot/20261005-124122/`; initial pass
`20261005-124002/`. Script: `scripts/run-recovery-disc-aot.py`; oracle:
`tests/recovery/disc-execution.cpp`. This proves that the necessary low-level
read service works on this owner disc. File opening, directory lookup, provider
callbacks and the complete loader still need original-code execution.

### Provider setup is now bound to actual caller arguments

`0x8C603D72` forwards to the provider initializer. Its observed caller at
`0x8C010212` supplies R4=8, R5=`0x8C698420`, R6=2100 and R7=`0x8C6997A0`.
The fifth stack argument is zero: each of the three observed outer call sites
sets R5=0, which `0x8C010180` saves in that stack slot. Evidence and original
instruction sections are in `loader-review/provider-argument-trace.json`.

The lower service-table accessor `0x8C649068` returns `0x8C670F80`. Its init
callback `0x8C648D28` reaches the verified BIOS init wrapper; the read helper
`0x8C648D36` selects command 16/17 and reaches the request wrapper. Provider
startup additionally registers callbacks through `0x8C64968C` and waits through
`0x8C604A30`. Their initialization/state requirements remain to be exercised.

**Next executable step:** generate a bounded original-provider initialization
closure rooted at `0x8C603D72`, using these exact arguments and the identified
service-table/callback candidates. Load the original image and public BIOS
services; do not force the provider global or directory state. Capture the
first missing target/state transition or successful initialization, then verify
one real filename lookup/read against the already verified sector bytes.
Keep initializer, filename lookup, transfer completion and full loader acceptance
separate. The post-decode MMIO side-effect boundary and original G1 remain open.


## Original provider and combined loader execute successfully

The first native provider experiment (`20261005-124705/`) generated 110 routines
but failed after 14 calls and zero sectors. Global work-area pointer
`0x8C67FD44` was null; callback registration reached a write to `0x238` in the
block starting `0x8C64501E` (store instruction `0x8C64502E`). This was a missing
startup prerequisite, not evidence of missing source or a reason to add a stub.

Original startup calls `0x8C608442` -> `0x8C65636C` -> `0x8C654DE4`. Executing
the last routine installs and initializes work area `0x8C8A6FD4`. The fixture
then calls the original provider with the traced arguments above. Its global
starts at zero; no provider/global/directory shortcut is applied. Initialization
returns zero after 21 actual directory sectors. The original caller next selects
`SONICADV`: it supplies string `0x8C112E0C` to `0x8C604B1C`, which dispatches to
`0x8C603FE2`. That directory selection also executes successfully.

**Final evidence: `docs/artifacts/recovery-aot/20261005-125600/`.** The generated
closure contains 128 original routines / 5,051 analyzed instructions, with 54
static external/unresolved records. That is the emitted inventory, not a claim
that every routine or unresolved path executed. The original image is embedded
unchanged; public BIOS services supply GD-ROM I/O. IP.BIN is checked against the
mapped owner disc. The fixture then verifies:

- `0x8C09E8A0` opens `MINICART.PRS` by name, reads 162 sectors / 331,776 bytes
  exactly, and closes the handle (the original handle's flag at offset 76 is zero).
  Output guards, sector/byte counters, stack/return PC/PR and R8–R14 pass.
- A missing filename returns -1 with no sector read and unchanged output.
- `0x8C09F664` performs the complete original file-read/decompression path,
  producing **1,434,052 exact bytes** at `0x0C900000`, matching the independently
  validated PRS output. It reads 162 sectors and preserves guards/return state.
- A missing module passed to that combined loader returns -1, reads no sectors,
  and leaves the previous decoded module intact.

The post-decode routine executes at its P2 alias and its `0xFF00001C` shadow
register acquires bit `0x800` as expected. **Hardware/cache side effects remain
unproven.** No host JIT, game-byte patch, forced provider state or newly added
success stub was used. Native identity/import screening and before/after input
hashes pass. The run is bounded to 20 seconds; the report binds test/generator
sources, exact patches, seed list and all private inputs.

Earlier evidence is retained: `124937/` initial provider success; `125151/`
filename success; `125328/` close-state/ABI checks; `125449/` combined loading.
The final report supersedes these narrower results. Exact reproduction is in
[BUILDING.md](BUILDING.md), via `scripts/run-recovery-provider.py`.

**Next:** inspect the public runtime's existing overlay/relocation and dispatch
support, then join a real loaded module to its matching precompiled entry. Use
SBOARD's actual header/registration as the smallest verified candidate. Prove
that loading, replacing and reloading overlapping images cannot dispatch stale
code or use stale initial-image literals. Reject mismatched/unavailable compiled
modules explicitly. Keep this separate from the remaining executable inventory,
full scheduler/gameplay, graphics/audio, device testing and original Katana/PAL G1.


### Overlay diagnostic and bulk literal invalidation

The next-step inspection above is complete. Synthetic generated code demonstrates
that `targets` selects the old native function after replacement bytes are loaded,
even after clearing `dispatch_cache`. Relocation-template lookup only helps when
no target exists. Thus cache flushing alone cannot implement overlay activation.
Generated direct calls and overlapping native symbol names also need review.

A separate reproducible bug affected data: `dc_load_bytes` and `dc_zero_bytes`
fast main-RAM paths did not dirty literal pages. Scalar writes already did.
Patch 0004 marks all touched pages; zero-size guards remain ahead of arithmetic.
Conservative initial-image dirtying is intentional; performance is unmeasured.

Evidence under `docs/artifacts/recovery-aot/`:
- `20261005-130304/`: both stale-code flags and both stale-literal flags true.
- `20261005-130453/`: stale-literal flags false; scalar, cross-page, alias and
  empty-operation controls pass. Stale-code flags remain true.
- `20261005-130553/`: twelve upstream tests, twelve analysis controls and six
  native execution variants pass under the four-patch sequence.
- `20261005-130932/`: original provider/combined-loader regression passes exact
  sectors/decoded bytes, state/guards/handle and missing-file/module controls.

Reproduce with `python3 scripts/run-recovery-overlay.py`. Its successful exit
means the diagnostic completed and literal regressions passed; it does not mean
loaded-module support. Next is the synthetic image-bound activation contract in
RECOVERY-LOOP, followed by the real SBOARD load-to-registration join. No guest
bytes changed; R3/R4 and original Katana/PAL G1 remain incomplete.


### Explicit module activation and original SBOARD loader join

Patch 0005 supplies a managed slot boundary: retire before writing, verify all
bytes against the build manifest, validate entries, activate. Inactive/undeclared
entries cannot use old registrations/cache or signature relocation. Direct/hinted
and inline-fast dispatch are disabled for runtimes with slots. Normal checked
cache use remains; retirement clears it and relocation aliases. Checked dispatch
depth rejects non-quiescent activation/retirement. No runtime compiler or guest
instruction replacement is used. Build macros distinguish generated function and
registrar names across translation units sharing one runtime.

Synthetic test initial generator failure `131513` was a wrong ELF section index
for the resident fixture; corrected fixture passes `131617`. Expanded final test
`20261005-132103/` verifies A -> B -> A, exact mismatched-image rejection, invalid/
duplicate manifests without partial installation, aliases including RAM mirrors,
cache hits, resident direct-hint positive control/fallback, retired/unknown entry
rejection, preserved state and cache/relocation retirement. Generated build inputs
are hashed. Full CPU regression `20261005-131706/` passes 12 selected upstream
cases, 12 analysis controls and 6 native execution variants.

The optional provider `--sboard` lane links the original five-routine registration
slice beside the original 128-routine provider closure, with distinct symbols.
It does NOT preload the embedded board image. Original filename at `0x8C15D030`
contains `sboard.PRS`; the already initialized provider finds the disc file.
The original combined loader reads 147 sectors from the mapped owner CDI and
decodes 508012 bytes matching the pinned independent SBOARD reference. Only then
does the host activate the two board entries. Three original resident registration
helpers are registered separately against the unchanged full boot image.

Evidence `20261005-131931/`: two load/activate cycles, four full task/heap cases
per cycle, guards, metadata, closed handle and preserved state all pass. Correct
bytes before activation still reject entry. Retire followed by missing-file load
returns -1 without sectors or output mutation; old code remains unavailable.
Mini Cart's prior read/decompression and missing-file/module cases also pass.
Shared `registration-check.hpp` keeps the joined and isolated fixtures identical.

Limits: host manually brackets loading with the API. This does not establish
full game-loader interception, task creation, scheduler execution, self-modifying
code, code copied outside its managed slot, multiple simultaneous slots or
performance. Manifest bytes and pointers are trusted build products; the runtime
compares actual bytes against that manifest. Task/heap state is still a fixture.
SBOARD execution callback is deliberately unregistered and rejects; cleanup
continuation is also unimplemented in this slice. No full R3/R4/G1 claim follows.

Next static lifecycle inspection is recorded at
`work/dreamcast-recovery/loader-review/sboard-execution/function.txt` and
`next-contract.json`: callback 8C903148 reads the init byte from task+36;
state zero calls 8C9031AE and then resident 8C0A2860. Trace prerequisites and real
stage-task dispatch rather than setting a later state to bypass initialization.
Continue the remaining module inventory in the ordered recovery loop.


### Full stage-selector binding pass, 38 module identities

The original selector 8C09EAB0 uses a 43-case signed-word branch table at
8C09EB48; stage-task creation independently uses the entry table at8C1A1B68.
Validated exact load-site instruction patterns bind filenames and destination
literals. All primary code loads use R12=0C900000; reviewed secondary loads
use their own immediate destinations. Empty stages11/13/14/30/31 have no task
entry. Repeated ADV00 and SHOOTING stages remain separate stage bindings to the
same file. Conditional texture/other loads are not declared executable by this
pass. It is a targeted stage-code manifest, not every file load in the program.

All 26 original prefix candidates bind. Twelve additional other-prefix files:
STG01/02/03, B_CHAOS0/2/4/6, B_ROBO, B_E101, B_E101_R, ADV01OBJ and SHOOTING.
Notable corrected placement assumptions: ADV0100/ADV0130 load at8C920000 after
ADV01OBJ at8C900000; their stage entries are8C9200C0/8C922440. TIKAL_PROG is a
tail load at8CB00000 in stages19/34. Its callable root is not established by
this table. AL_MAIN and ADV01OBJ likewise lack a task-table root in their own
image; load-base audits are explicitly hypotheses. All 33 newly audited inputs
match the separate Rust decoder byte-for-byte; the five prior identities are
retained with their prior audited maps rather than regenerated.

Initial batch evidence: `20261005-133022/`. Twelve coverage reports failed because
our helper required a symbol at load base, even when the raw compiler correctly
used the interior stage entry. Added explicit `--entry` to coverage.cpp; an
independent synthetic input with non-code prefix and root atbase+4 yields exactly
three known instructions. Repaired only the failed reports using unchanged raw
maps: `20261005-133233/`. No fake base entry was added. The default-base and focused
function interfaces remain usable. Contract-only recheck is recorded in
`work/dreamcast-recovery/loader-review/stage-inventory-contract.log`.

| Module | Load base | Stage task entry / root hypothesis | Entries | Unique instructions |
|---|---|---|---:|---:|
| STG00.PRS | `0x8c900000` | 0x8c900000 | 22 | 2648 |
| STG01.PRS | `0x8c900000` | 0x8c900000 | 299 | 22254 |
| STG02.PRS | `0x8c900000` | 0x8c900000 | 212 | 19389 |
| STG03.PRS | `0x8c900000` | 0x8c907a66 | 206 | 23252 |
| STG04.PRS | `0x8c900000` | 0x8c90b420 | 70 | 7684 |
| STG05.PRS | `0x8c900000` | 0x8c911e0a | 21 | 582 |
| STG06.PRS | `0x8c900000` | 0x8c9004e0 | 440 | 43060 |
| STG07.PRS | `0x8c900000` | 0x8c90b4cc | 17 | 593 |
| STG08.PRS | `0x8c900000` | 0x8c90dfe0 | 120 | 8082 |
| STG09.PRS | `0x8c900000` | 0x8c900afc | 599 | 46148 |
| STG10.PRS | `0x8c900000` | 0x8c900000 | 403 | 34858 |
| STG12.PRS | `0x8c900000` | 0x8c900000 | 549 | 38059 |
| B_CHAOS0.PRS | `0x8c900000` | 0x8c900000 | 83 | 7805 |
| B_CHAOS2.PRS | `0x8c900000` | 0x8c900080 | 90 | 10498 |
| B_CHAOS4.PRS | `0x8c900000` | 0x8c900000 | 122 | 11557 |
| B_CHAOS6.PRS | `0x8c900000` | 0x8c900000 | 124 | 10994 |
| B_CHAOS7.PRS | `0x8c900000` | 0x8c900000 | 225 | 13865 |
| TIKAL_PROG.PRS | `0x8cb00000` | base hypothesis | 39 | 2038 |
| B_EGM1.PRS | `0x8c900000` | 0x8c900000 | 137 | 7148 |
| B_EGM2.PRS | `0x8c900000` | 0x8c900000 | 177 | 12549 |
| B_EGM3.PRS | `0x8c900000` | 0x8c900000 | 285 | 17185 |
| B_ROBO.PRS | `0x8c900000` | 0x8c900000 | 190 | 13174 |
| B_E101.PRS | `0x8c900000` | 0x8c900000 | 49 | 3999 |
| B_E101_R.PRS | `0x8c900000` | 0x8c900060 | 171 | 11389 |
| ADV00.PRS | `0x8c900000` | 0x8c900000 | 564 | 29878 |
| ADV01OBJ.PRS | `0x8c900000` | base hypothesis | 60 | 1946 |
| ADV0100.PRS | `0x8c920000` | 0x8c9200c0 | 258 | 17676 |
| ADV0130.PRS | `0x8c920000` | 0x8c922440 | 170 | 14855 |
| ADV02.PRS | `0x8c900000` | 0x8c900000 | 428 | 30310 |
| ADV03.PRS | `0x8c900000` | 0x8c900000 | 146 | 7386 |
| MINICART.PRS | `0x8c900000` | 0x8c900120 | 212 | 19719 |
| SHOOTING.PRS | `0x8c900000` | 0x8c900ae0, 0x8c900e20 | 207 | 11719 |
| SBOARD.PRS | `0x8c900000` | 0x8c900040 | 108 | 3964 |
| AL_MAIN.PRS | `0x8c900000` | base hypothesis | 328 | 41622 |
| AL_GARDEN00.PRS | `0x8cb80000` | 0x8cb80000 | 24 | 1089 |
| AL_GARDEN01.PRS | `0x8cb80000` | 0x8cb80000 | 3 | 232 |
| AL_GARDEN02.PRS | `0x8cb80000` | 0x8cb80000 | 17 | 504 |
| AL_RACE.PRS | `0x8cb80000` | 0x8cb80040 | 598 | 38595 |

Counts are per separate image, never merged by overlapping virtual address.
Total discovered entries7773, unique-address sum588305, external records37916.
Linkage classification against resident map122414:28032 known resident entries,
8309 unresolved records,1209 records/157 unique new resident candidates,
276 local candidates and90 outside-module/resident targets. These records are
not equivalent to missing implementations. They can include data/code discovery
errors. `133233/module-linkage.json` retains per-candidate module/caller/site;
no new resident seeds were automatically accepted.

Only STG01's new raw closure reports UNKNOWN/RAW:222 instruction occurrences in
19 functions, with170 unique undecoded addresses in the separate audit. Diagnostic
`133447` enables the existing DCR_CLUSTER_DEBUG flag; the function map stays
identical. Three illustrative entries (8CA33A80,8CA371A0,8CA38280) are marked
clean/strong by the cluster heuristic, while their bytes also form float arrays.
The first decodes MOV.L to R8 followed by JMP through R0; its literal is float-like.
This supports a false-promotion investigation but does not alone prove exclusive
data use. `133233/stg01-unresolved.json` records raw-byte hashes and next checks.
No fake SH-4 implementation, data exclusion, or source patch was applied to hide
these words. Remaining decoder/discovery correctness and real execution stay open.

The 223 other-prefix PRS files not bound here, all remaining BIN/auxiliary roles,
582 PVMH payload exclusions, root/callback completeness, overlay lifetime,
startup/scheduler, graphics/audio and gameplay remain unproved. Original G1 and
R3/R4 remain incomplete. Next: classify STG01's promoted records from actual uses
and a source-free counterexample before changing heuristics or importing new seeds.


## STG01 shape-data classification (supersedes its prior raw counts)

Input SHA-256: `2139e5c46f62eb8d11abeb64f646eea49c30a60c7470644672fb62768bbe05d4`.
The 170 unique unknown addresses belong to vertex/normal arrays described by
three motion headers: 0CA35CD0, 0CA40A40, 0CA4B7B0. Model 0CA2E400, referenced by
object 0CA2E428, supplies 180 vectors per array. The bounded parser checks 30
frames, flags 0x30, two channels, frame keys, finite floats and unit normals.
There are 54 unique 2160-byte spans. Layout comes from public
[SA Tools Animation.cs](https://github.com/X-Hax/sa_tools/blob/5d52a39a04e056b8a7e690ce87386750b4b07058/Libraries/SAModel/Animation.cs)
and [motion flags](https://github.com/X-Hax/sa_tools/blob/5d52a39a04e056b8a7e690ce87386750b4b07058/Libraries/SAModel/Enums.cs),
read as a format reference only. No external implementation was linked.

Original site 8C901A18 calls resident 8C040C8E with two alternative motion objects
and the model object; focused resident analysis forwards these arguments to
8C0400BA, 8C040060 and 8C040100. Header 0CA35CD0 has the same validated layout but
no proven consumer. Static format evidence does not establish runtime behavior.

The source-free fixture demonstrates why the raw heuristic misclassifies these
float patterns. Patch 0006 supplies explicit data ranges rather than broadening
unknown-opcode suppression. Actual re-audit (`20261005-135713/`) has 255 functions,
21,258 unique instructions and 1,671 external records. It removes 44 entries and
996 instruction addresses, all inside validated ranges; zero unknown/RAW remains.
Original bytes and unannotated map are unchanged. Controls (`135649/`) reject
contradictory direct/indirect/branch edges. Earlier 38-module aggregate and linkage
counts remain historical until recomputed with this audit. R3 is incomplete.


## Original event-loader inventory and Tikal callback root

Original resident loader8C04B502 formats `ev%04X.prs` using8C114BB8, clears the
0CB80000 slot, calls combined loader8C09F664, stores header[0] at8C74D320 and
JSRs it at8C04B56C withR4=0. Its negative-ID/preloaded path and runtime behavior
remain untested. Header[1] is passed to8C04B266; that secondary format still
requires review. The original bytes, instruction path and focused reports are
retained under loader-review/tikal-root/event-load/.

All207 matching event files from the owner scan have in-image, aligned entry
pointers.188 use0CB80020;19 use other offsets. Script
`run-recovery-event-inventory.py` verifies exact BOOT/disc/decoded/encoded identities,
pinned generator plus six patches, reference decoder hash, entry bounds and
bounded closure. The first guard used10 bytes for an11-byte NUL-terminated format
string; the fixture guard was corrected before audit. Full207-file audit140837
passes: every independent decoded output matches; no time/closure bounds or
UNKNOWN/RAW instructions in discovered code. Aggregate8564 functions,712448
unique instruction-address sum,82899 external records. These are separate modules
and static discovery counts, not executed story coverage.

EV0022 audit140618 independently established its root before the batch. Its code
loads0CB00000 from8CB80418 intoR5 at8CB8027A and invokes8C04BC2E at8CB80290. The
resident helper passes the preserved callback to8C09A1CE/8C09A28C, which installs
it at task+16. Exact opcodes/input/detail hashes are in
`work/dreamcast-recovery/loader-review/tikal-root/callback-root.json`.
This replaces TIKAL_PROG's base-root hypothesis with static task-callback evidence.
The actor-absent path, allocation state and actual scheduled invocation remain
unexecuted. Fourteen event files contain this root pointer; only EV0022's complete
argument/store chain was reviewed. Do not infer the other13 consumers from bytes.

The resident shape seed8C040C8E adds itself and8C040060,77 unique instruction
addresses, and loses none of the previous entries/addresses (140143). Updated
resident6250 entries/527279 unique addresses,7651 external records. The207 events
plus38 prior images give245 separate modules. Current linkage140837/linkage:
16293 functions,1299757 per-image unique-address sum,120541 external records;
57293 known resident,54666 unresolved,8414 records at238 candidate resident
addresses,63 local candidates,105 other. All module-level tallies reconcile.
The reusable refresh script reproduces the original baseline aggregate and
candidate records exactly before applying corrections. Historical counts above
remain scoped to their original evidence; current reports supersede them.

Sixteen remaining non-PVM/unbound PRS files: ADVERTISE, ALERTWND, A_MOT, BRO_JAP,
BRO_USA, B_MOT, E_MOT, K_MOT, M_SBMOT, SAMP_LCD, SAMP_LCD_J, SHTPATH1, SHTPATH2,
SUMMARY, S_MOT and S_SBMOT. Their names are not sufficient classification. The
582 PVMH prefixes,535 BIN roles and other auxiliary formats still require their
appropriate consumer/format evidence. ADV01OBJ and AL_MAIN roots remain open.
R3 and original G1 remain unresolved; no game or Simulator launched.


## Three menu modules and payload-role evidence

Original menu state machine8C0558C0 initializesR11=0C900000, loads ADVERTISE,
SUMMARY and ALERTWND via8C09F664, then JSRs header cells. The same base is reused
by other overlays, so this does not prove lifetime integration. Observed header
calls seed ADVERTISE offsets0/4/8; SUMMARY0/4/12/16/24/28; ALERTWND0/4/8.
ALERTWND's other path calls absolute cells0C90000C/14/10; their three targets are
already discovered. All3/9/6 header pointers respectively are present in the
maps, though each discovered pointer is not a runtime invocation proof.

| Image | Bounded entries | Unique instruction addresses | External records |
|---|---:|---:|---:|
| ADVERTISE |359|21827|873|
| SUMMARY |89|4659|267|
| ALERTWND |32|1741|194|

Evidence141622/141730 (20261005) verifies exact owner inputs, six pinned patches,
independent decoding, explicit roots, zero UNKNOWN/RAW and no closure exhaustion.
The first guard failed because its MOV.L mask retained the source-register bits;
the fixed mask validates that register separately. No guest bytes or analyzer
behavior changed. Original consumer report and executable audit scripts live at
`work/dreamcast-recovery/loader-review/menu-modules/`. These do not execute menus.

Payload-role audit142208 checks BRO_USA/BRO_JAP: exactly four offset/length pairs,
nonoverlapping in-file spans beginning/ending as HTML, zero alignment gaps and
last span ending exactly at EOF. Original resident8C10C3E0 loads BRO_USA and
consumes all four offset/length pairs. BRO_JAP's format matches, but no resident
filename binding was found. HTML/network/browser behavior is not tested.

SAMP_LCD/SAMP_LCD_J each decode to65536 bytes with CHAO_ADVENTURE at0x200.
Stage loading places them at0CB70000; AL_MAIN8C9022C6 copies Chao state at+0x3000
and passes baseR6,128R7 and SONICADV__VM filename to resident8C6072FE at8C90232E.
The original call chain is retained, not executed. This is an observed Chao
transfer-image role; full VMU format, CPU semantics, persistence and transfer
success remain unproved. Do not discard these bytes as textures, recompile them
as SH-4 merely because they contain code-like words, or bypass required Chao
save flows. Independent decoding matches all four files. The initial signature
length assertion was corrected to use the actual signature length.

Nine remaining candidates now have exact filename/destination bindings and
independent-decoder equality (142348): S_MOT/B_MOT/A_MOT/K_MOT/E_MOT at0CC00000,
S_SBMOT at0CB08000, M_SBMOT at0CADC000, SHTPATH1/2 at0CB80000. Their full formats
and consumer paths remain unresolved. No native entry was invented from a name
or floating-point prefix. All535 BIN roles and other auxiliary formats remain open.

Current248-module combined inventory/linkage is142208/linkage. It contains16773
entries,1327984 per-image unique-address sum and121875 external records:58099
known resident,55066 unresolved,8540 records at266 resident candidates,65 local
candidates,105 other. Original resident map140143 and Tikal callback proof are
unchanged. Original G1, R3/R4, menu/gameplay and physical acceptance remain open.

## AL_MAIN caller root and camera records

AL_GARDEN00 loads literal `0x0C910D52` at `0x8CB80030` and calls it at
`0x8CB80032`. Audit `20261005-142912/library-root.json` re-roots AL_MAIN at
that entry, retaining the two prior seed files and their identities. All 328
function rows, 41,622 unique instruction rows and 1,501 external-call rows are
identical as multisets to the prior audit; file ordering changes. The original
base is rediscovered through internal calls rather than supplied as the root.
This resolves the callable-root hypothesis, not startup or Chao execution.
The combined inventory is now `142912/linkage/`; all 248-module totals remain
unchanged. `complete-row-comparison.json` retains the full-row loss check.

All 101 `CAM*.BIN` files have a 64-byte header followed by exactly N 64-byte
records, where the first little-endian header word is N and the remaining header
bytes are zero. All 4,893 records have mode byte 0 below 76 and adjustment byte 2
below 27. Original camera initialization `0x8C0196FE` selects a buffer and adds
64 to reach records; `0x8C019F44` indexes records by 64. The resident descriptor
at `0x8C1986DC` supplies 76 mode entries and 27 adjustment entries. Other fields
and camera behavior remain incompletely interpreted; this is not a full format
or exclusive non-code proof for every remaining BIN file.

Original `0x8C01969E` selects the first record's mode, indexes the 16-byte mode
table and writes the chosen pointer into camera state at +12. Its exact 48
instructions and original literals/tables were compiled to native ARM64 and
executed separately for every record using declared globals and state. All 4,893
cases pass expected state writes, input/guard preservation, return, stack and
unmodified-register checks. No external calls or RAW operations are emitted in
this one-routine slice; an absent overlay target rejects explicitly. No callback,
camera rendering, startup or scheduler execution follows from this result.
Evidence: `20261005-144437/camera-report.json`, with exact input/script/binary
hashes, six-patch validation and commands. Private reproduction scripts are under
`work/dreamcast-recovery/loader-review/library-roots/`.

ADV01OBJ remains unresolved. Camera table modes 46/47/55/57 point into its
potential address range, but no camera record selects those modes. Furthermore,
the +0x160 target decodes as data in this image. Shared overlay address space
makes an address match insufficient evidence of module ownership. Stage 29/32
load ADV01OBJ alongside ADV0100/ADV0130 and use EC00–05/EC30–35 camera prefixes;
MR camera prefixes are not evidence for these stages. The lone immediate 57
in the audited ADV0130 path is a timer comparison, not a camera-mode assignment.
No speculative target was added. There are 434 BIN files outside this camera
check, including the already identified resident image; they are not all asserted
to be unknown code. Next execution work is C2 in RECOVERY-LOOP, not a blind
expansion of static candidates.

## Loaded SBOARD state-zero callback

The provider runner's optional `--sboard-callback` mode extends the actual
load/byte-verification/activation/registration path. The callback generator
selects thirteen original routines,459 analyzed instructions, including three
already used by registration. Unselected continuations remain unregistered;
there are no native overrides or replacement guest instructions. The generated
program still lists thirteen external records: selected-path success does not
close every branch.

Original callback8C903148 calls8C9031AE, which changes player-slot0's flags by
OR0x1000 and its state byte to44. It reads character global8C74D448. Character0
selects child callback8C0B4B06; character2 selects8C0C13EE. Original task creation
allocates and zeroes two64-byte work blocks. After initialization,8C903148 sets
state1 and calls8C0A2860 with77/8. That routine creates and registers an auxiliary
task through8C0A1620/8C0A1690; its init8C0A2B2C stores both arguments. A successful
return advances SBOARD state to2. An empty task pool leaves state1 without
allocating those blocks. No interpretation of the auxiliary task as audible
output is established.

Evidence `20261005-145336/report.json` and `callback-contract.json`: both character
values with two/no free task nodes, repeated across two original147-sector
SBOARD loads, pass all eight cases. Checked outputs include full player region
and guards, task callbacks/queue heads, zeroed work blocks, auxiliary77/8 state,
heap head, task/heap guards, return and preserved GPRs. Trace counts are17 calls
with available tasks and10 with no tasks. The earlier provider/Mini Cart/file
failure/SBOARD registration and failed-load checks pass in the same execution.
All owner input hashes remain unchanged and the binary is native Mach-O ARM64.

Declared fixtures still supply the player pointer, selected character, free task
nodes, empty task queues, heap and absent auxiliary task. Original registration
initializes SBOARD state0; the harness never sets it to1/2 to bypass work. It calls
the stored task callback directly, so task scheduler and child callback execution
are unverified. The first run145246 passed the initial success path but failed
an oracle assertion that expected the callee address in ctx.pc. The runtime keeps
the caller block there; the repaired assertion verifies both the explicit error
target and last dispatch-history target. No runtime/game correction was needed.

The following state stops at8C0194FC, called by8C903186 from block8C90317E, and
leaves state2. It reads camera state through8C113C58 and updates parameter8C6BADA8;
modes28/29/30 with a nonzero secondary mode involve8C0579C0. Its tail calls
8C644240, which calls8C63E734 and uses float global8C893868. Those prerequisites
must be initialized and their semantics checked before extending execution.
The earlier isolated camera leaf does not establish this complete path. See C3
in RECOVERY-LOOP. Original startup, module lifetimes, graphics/audio, R3/R4 and
Katana/PAL G1 remain open; no Simulator or game app launched.


## Original video and camera prerequisite execution

The width global8C893868 is written by original video setup8C608442 at offsets
relative to8C893864. The1/0/1 argument branch is visible at8C05644A and forwarded
through8C010180 to8C608442. The public-source native experiment preserves those
instructions and follows their calls, including work-area initialization and
video-memory clearing. A96-function/8278-instruction closure was generated;90
external records remain for other paths.

Initial run150041 timed out after20 seconds. The trace identifies a bounded loop
at8C6696CC:2097152 calls to four-byte copy8C65627E clear8MiB fromA5000000. Verbose
call/return printing produced56MiB and obscured progress. With only in-memory
trace history, the unchanged guest/runtime passed150208: original initialization
returns with640.0/480.0 at8C893868/86C. No width fixture or loop bypass was added.
This uses the public BIOS and MMIO models, not an Apple displayed frame.

Camera state pointer8C113C58 is already8C6BAC8C in the original boot image; it
need not be overwritten. Stage38's camera-slot table8C1986E8 has0CB50000 and
0CB58000. Original stage loader case8C09F38E loads SBOARD and uses SBoard00/01
prefixes at8C09F3C4/3CA for the camera filename builder. CAMSBOARD00S.BIN and
CAMSBOARD01S.BIN are byte-identical320-byte files, four records each, SHA-256
`df73ce45a74fbe42a43337485023dcda07cee4a3a905e93a193038982255f636`.
The first mode is0. Camera files are loaded by the host in this experiment;
the original camera filename-loading path has not been executed here.

Native run150530 adds original8C0196FE and8C0194FC to the closure:245 routines,
20217 analyzed instructions,112 external records. Declared stage38/act0 or1,
free task node and512-byte heap allow real camera task creation and initialization.
The original code selects the correct record pointer, installs its callback,
and stores task callback8C01993A. The task is not scheduled. Calls to8C0194FC
flow through8C644240 and the FSCA-based tangent routine8C63E734. For both acts,
angle12743 produces457.059 and control16384 produces320. The independent numeric
oracle is640/(2*tan((angle>>1)*2*pi/65536)), with finite result and0.002 tolerance;
parameter, width, camera input, return and stack are checked. This does not prove
bit-exact Dreamcast FSCA rounding or exception behavior.

Evidence:150530/video-report.json and camera-prerequisite.json; reproducible
private runner loader-review/viewport-native/run-camera.py. The next experiment
must join these prerequisites to the original SBOARD load/state2 callback and
preserve task/heap state. Static next call0C905480 remains unexecuted. Original
startup, scheduler, renderer/audio and full-game acceptance remain open.

## Connected camera/provider/SBOARD execution — 151600

Extended the provider harness with `--sboard-camera`. Resident closure357
routines/24712 analyzed instructions/161 external records includes original
video608442, camera manager0196FE, projection0194FC and filename builder09E98E.
Registration and callback units retain their5/13-routine scope. These counts are
analysis, not executed coverage. All six core patches remain unchanged.

Original video sets640x480. After original provider initialization and SBOARD
loading, the original camera filename builder reads acts0/1 from FAD58375/58376
to0CB50000/0CB58000. Each320-byte result matches the owner camera reference.
Per-case fixture supplies stage38/act/character, stage/player records, one4096-byte
heap and three free tasks. Camera manager, stage registration and both callback
phases then run without resetting those queues/heap. Original code consumes
304 heap bytes including headers and links camera/group0, child/group2 and
auxiliary/group0 tasks. Both character branches retain their expected callback;
auxiliary work receives77/8. Projection control320 changes to457.059 naturally.

Four cases across two147-sector SBOARD loads pass, including guards, normal-call
return/register checks, exact module bytes and failed-load retirement. Stage2
rejects at JSR90318C with PC90318A, R2=0C905480 and PR903190; state remains2.
First151418 failed the test's trace-target assertion because managed-module
validation throws before trace recording. Correcting that oracle to these
actual call registers passes without a runtime/game edit. The failed run is kept.

Static next function905480 contains nine instructions and calls8C0A1620 with
R4=3, R5=0C90A5B8 and R6=0. Its metadata/callback and allocation requirements
are the next bounded investigation. Evidence:151600/report.json,
joined-camera-contract.json; focused next-function text is under
work/dreamcast-recovery/loader-review/sboard-next-905480/.

No original task-pool/player startup, scheduler, child/camera-task execution,
frame, audio, hardware-exact FSCA or gameplay is claimed. Source review confirms
Windows-only D3D11/window and waveOut output. Continue bounded experiments;
hold production adoption. RECOVERY-LOOP records source-bound implementation
measurements rather than a speculative complete-port schedule. G1/R3/R4 stay open.

## State2 task-allocation continuation — 152213

Actual descriptor0C90A5B8 is {2,0,0C9054F6,0C90554A,0,0}. Original905480 passes
3/descriptor/0 to8C0A1620. The connected camera mode adds905480/9054F6; analysis
also includes tail9054CA, for16 callback functions/532 instructions/22 external
records. Available fourth node creates group3 task, workheap+320, writes38/act,
and stores the task pointer at0C97C030 before failing at0C904AC0. Three initial
nodes are exhausted by camera/child/auxiliary;905480 returns without a new task
and original stage code next rejects at8C052700. Eight cases across two actual
SBOARD loads pass. State remains2, heap use336/304, existing queues/guards intact.

Activation occurs once per actual load before any module-data writes. All image
bytes except the verified task-pointer cell remain identical; failed load checks
the whole live snapshot, and real reload restores/validates original bytes.
This avoids pretending that mutable module data is immutable code. No production
module-lifetime integration or scheduler is claimed. Exact evidence/metadata and
first boundaries are in152213/report.json and continuation-contract.json.

A separate synthetic output experiment152538 verifies every pixel of two
software scanouts after store-queue/TA rasterization, including nearer/farther
opaque depth controls. No guest code/image is linked or loaded. It is independent
of Sonic execution, Apple presentation, transparency/textures and audio; R4
remains open. Reproduction and limits are in BUILDING and RECOVERY-LOOP.

## Original stage initialization and startup boundary — 155509

The camera mode now compiles all108 entries of the existing audited SBOARD map
(SHA f0774e00496328c0f8ae4a26042b576e56bb7cf79cac20c92875fa508be22f26),
plus the selected resident allocation/registration helpers.124 routines contain
4902 analyzed instructions and279 external records. Native overrides remain
cleared; no runtime code generation or placeholder success was introduced.

Actual descriptors select four904B08 initializers, three903A56 initializers,
one9056B4 initializer and resident0A280C. Their original execution links nine
children to the continuation, copies work/parameters, decrements seven in-image
resource counters, initializes two module pointers and sets position(0,10000,0).
Resident052700 and04E300 set original completion flags; stage reaches3 naturally.
Three/four/sixteen-node cases use304/336/896 heap bytes and leave0/0/3 free nodes.
Both acts run twice:12 passing cases. Each case reloads147 original sectors and
verifies all508012 stage bytes before activation; module-data mutations no longer
carry between fixtures. Failed loads preserve the entire live snapshot and leave
code retired. Original video/provider/camera/Mini Cart controls still pass.

155229 first passed initialization.155321 tried the next stage3 phase and disproved
an expected missing-target rejection:07EC7E was already in the provider closure.
That return is excluded from acceptance because player-work startup was missing.
Final155509 executes the actual lookup separately and verifies a null result and
zero table slot8C7906E8 in all12 cases, without dereferencing it downstream.
Next C8 traces the original writer and its startup dependencies, replacing supplied
state before executing stage3. Do not manufacture a work pointer. Source text for
the selected leaves/descriptors is private under loader-review/stage-resources.

155425 independently retains eight original callback cases plus allocation,
provider, Mini Cart and failed-load controls. Six core patches and owner inputs
are unchanged; only the bounded harness/generator mode changed. Reports bind
sources, generated units, binary identity and inputs.155509/initialization-contract.json
records exact scope. R3/R4/G1, scheduler/normal boot, real game output and supported
PAL acquisition remain open. No Simulator or SonicPad game app was launched.

## Original startup producers joined to stage updates —160408

Literal cross-references locate table publication at8C06E062 inside common
player initializer8C06DFF0; pool initializer8C09A0EA allocates196608 bytes and
constructs4096 linked48-byte tasks plus eight empty queues. These original
routines, table reset06DFA0 and their bounded dependencies are now compiled in
the camera-mode unit:132 routines/5387 analyzed instructions/288 external records.
No core patch or original game byte changed.

Native tests check every pool link/field, original player tables/132 parameter
bytes/self-pointers/velocity/scale, and allocation survival across original file
loads. From that state, original camera/stage registration and callbacks reach
state3 and execute one update. Its original player lookup returns8CE700C0;
a320 projection control changes to457.059 with angle31C7. Two repeats and the
previous12 initialization cases pass, including provider/Mini Cart and failed-load
controls. This is a connected common-initializer slice, not normal player boot.

Two harness failures are retained.160039 put the supplied heap at the loader's
0CD00000 compressed scratch; the bad link3652D6FF exactly matches encoded SBOARD
bytes at0x70. Moving to8CE40000 and comparing all allocated bytes across loads
fixes the collision.160147 reused a nonzero auxiliary handle from a prior isolated
case, so original registration correctly returned0 and left state1. Running its
original cleanup callback0A2B70 before the next case fixes isolation. Neither
failure justified changing the runtime or forcing stage state.

155933 passed the independent producer checks;160316 passed the join; final160408
also enforces producer-case counts in the runner and binds current hashes.
Private startup-producers-contract.json records exact commands via report.json,
source/input identities, static cross-references, remaining fixtures and failures.
No Simulator/game app was launched; no no-op provider or fabricated player-work
pointer was introduced. Heap setup, character selection, task flags/group and
host call order remain supplied, and full task teardown remains unproved.

Next C8: original selector8C051120 resets player tables, chooses Sonic callback
8C0A93DC and allocates flags7/group1, publishes primary/secondary work and calls
placement8C05165A. Companion/selection calls083760/081200 remain part of that path.
Replace the current direct flags3/group2 allocation and common-helper invocation
with this original path, then dispatch through scheduler09A42C. Do not enter
0A9620 directly: it is an interior case requiring the0A93DC stack frame. R3/R4/G1
remain open; no normal boot, full renderer/game audio or gameplay acceptance.

## Original selector and scheduler prefix —161825

Original051120 replaces direct task allocation: flags7/group1, Sonic callback
0A93DC, extra work and primary/secondary tables. Placement05165A reads the original
stage38/act0 record and yields(1645,-9,219), yawC000. Companion checks take the
original no-companion branch; the previously missing04B6DA byte getter is compiled.

Two isolated scheduler cases call09A42C(group1), which invokes0A93DC, its early
0AB28C/0B1730 path and06DFF0. The resulting player tables,132 parameter bytes,
self pointers, zero velocity, scales and heap guards agree with the common-helper
control. The next original call8C02F650 is absent from the generated slice and
rejects explicitly. Caller block0A9632/PR0A965C/SP8CFFEFB8, task argument, original
record8C1C9BB8 and mode/count arguments are checked; state remains0. The rejected
invocation is abandoned, never resumed or patched to success. Existing12 stage
cases and2 common-helper/stage joins pass. No complete scheduler tick or normal
boot is established.

The routine's bytes are present. Static inspection shows28-byte descriptor
allocation and52-byte records copied through608240, linked at main+56; the
observed mode0 avoids the02F5A0 tail branch. That gives the next executable
oracle. Future modes and later Sonic init dependencies remain open.

The150-function/17710-instruction unit exceeded the180-second optimizer budget
in160937. Diagnostic-O0 for that unit allowed161337 to expose this boundary and
161825 to verify it; other units retain their existing build settings. No runtime
performance claim. Ninja compiler processes can have their own process groups;
timeout cleanup now terminates snapshotted descendants too. A synthetic parent
and separate-session child both disappear after timeout, recorded in161738.
All current inputs, source/generated files and binary hashes were verified. See
161825/scheduler-prefix-contract.json for scope and next test. G1/R3/R4 stay open.

## Original Sonic descriptor and texture-file loading —162925

The next13 selected resident entries carry the original scheduler/Sonic path
past02F650. The28-byte descriptor has mode0, flags32, count1 and task ownership;
main+56 links it. Its52-byte record matches original8C1C9BB8 except the four bytes
at20, which original0A9670 replaces from player work+268. Sonic reaches state1.
The earlier common-work and heap checks still hold.

09AC4E selects SONIC through table1C62BC/entry1C61F4.09B3F0 and60AFA4 prepare
texture-list storage;09B45C formats SONIC.PRS via10EF80/10F7E4 and original
string/copy helpers. The existing provider reads18 sectors to0CEF7000;04E260
decompresses108000 bytes at0CD00000. Every36861 encoded and108000 decoded byte
matches an independent ISO read/Python decoder with the previously recorded
hashes. Guards, actual filename, closed handle and still-zero resource counter
are checked. Both module-load rounds pass;12 stage cases and2 stage joins remain
green. No guest bytes, core patches or task success state were fabricated.

Next60BE74 rejects atPC09B4F6/PR09B4FE/SP8CFFB678, R4=0CD00000 andR5=8CFFEFA0.
It checks PVMH and leads into texture parsing/allocation/upload. Read its original
helper/global initialization and verify record/allocation/VRAM behavior before
claiming textures or a Sonic frame. SON_EFF has not executed. No complete
scheduler tick, normal boot or G1/R3/R4 pass. Callback closure163 routines/19168
instructions remains a static count; diagnostic-O0 remains explicit. Earlier
162120/162238/162342/162458/162557/162659 discovery failures are preserved.

## Original PVM metadata and texture-manager producer —164032

60BE74 and its original reading/seeking/metadata helpers traverse28 SONIC.PRS
rows. All28-byte names, ordinals and global IDs match the34-byte source records
under flags0109.60BE08 parses the first PVRT chunk: payload0CD003F0, format0201
before the original shift, width/height64, next-chunk position0CD02EA0. The
independent reference describes28 chunks, but runtime claims only the first
PVRT descriptor, not28 uploads.

The next prerequisite was texture table6771C4/capacity6771C8, both initially0.
652624 dereferences that table, so executing it in the old fixture would produce
invalid state. Literal/call tracing identified652320's writer, wrapper60C96A,
and original graphics caller056242. The exact caller supplies8C7649F4 and2048.
The test calls the original wrapper with those arguments and verifies every
68-byte record's initialized ID/sub-ID/use count and reset cursors. The outer
graphics initializer and VRAM state remain open, explicitly supplied call order.
No manager entries were fabricated.

After this initialization, original652624 returns the first descriptor at764A08
and the original copy populates its PVRT fields. The path rejects at652B3C,
PC60C366/PR60C36E/SP8CFFB600, R4=7A3488 andR5=764A08. The manager record remains
unregistered before surface allocation. Two repeats, the12 earlier stage cases,
two stage joins and exact texture-file checks pass. Next trace652BCC/65742C and
652C48/65767A/657120 before verifying allocation and actual VRAM transfer.
Earlier163255/163356/163512/163624/163844 failures remain preserved. No new core
patch, texture upload, complete tick, normal boot, game frame or G1/R3/R4 pass.

## 2026-10-05 — R3/C8 first texture allocation and VRAM reset

165342 passes two first-texture allocation/reset cases and all prior12 stage,
two stage-update, metadata and exact file-byte checks. Original652B3C→652BCC→
65742C/657C48 converts the64×64 mipmapped16-bit descriptor;66826A reserves10944
bytes for its10928-byte payload and publishes the correct owner/list/accounting.
Original666E80 restores both4MiB bank lists after the rejected transfer.

Correction:164032's statement that VRAM initialization was unexecuted was too
broad. Existing video608442→65636C→6550C4→666E80 already establishes the banks;
BOOT's zero data was not evidence of zero live state. Outer graphics056242 is
still unexecuted. Heap, selection and direct invocation order remain supplied.

164341/164456/164613/164733 exposed conversion/size/free-query/allocator targets;
165142 reached65767A and failed the old boundary oracle.165342 uses independent
PVM size and original allocator semantics to check the resulting memory state,
then deliberately rejects transfer65767A atPC652C58/PR652C66/SP8CFFB5E8 with
R4=8C764A0C,R5=0CD003F0,R6=R7=0. Callback195 functions/21407 instructions are
static counts, not dynamic coverage. Same public pin, six patches and CLT lane;
no Simulator/game launch, core replacement, input change or publication.

Command: the unchanged `--sboard-camera` provider command in BUILDING.md.
Evidence: `docs/artifacts/recovery-aot/20261005-165342/report.json` and
`surface-allocation-contract.json` (SHA256 4bc246455c3b9d955dbb02a609772a6e9ca81c39cb81b18f133cfe61a6a83ee3).
Binary SHA256 c88b0fa549b4dff6ed0c341209791a618a1ba56625d40d04cf401cf3fb05d52b.

Next:65767A branches658260/6581E0/657220 and657120 completion; verify actual
VRAM bytes and bounds, not MMIO writes alone. Upload, other textures/SON_EFF,
completed scheduler tick, normal boot, G1/R3/R4 and production adoption stay open.

## 2026-10-05 — R3/C8 first texture transfer and registration

170149 passes two original first-texture CPU transfer/completion/registration
cases alongside prior12 stage cases, two stage updates,28 PVM metadata records,
file-byte/guard checks, allocation accounting and original bank reset.

65767A→6581E0→657F20 selects table index6 (10924+4 bytes);657220 branches to
65627E because destinationA4000004 is unaligned. Every10928 destination byte
matches the independently decoded PVM payload at0CD003F0. All outside bytes of
8MiB runtime VRAM match the pre-call snapshot; write epoch advances. No VRAM
fixture override, DMA completion or rendered-texture claim. Original657120 sees
synchronous mode1 and returns0; wrapper652C48/652B3C returns1. Original65235E
publishes ID1000, flags2, use-count1, first-list record pointer and manager
cursor/count1, leaving2047 slots free. All27 remaining metadata IDs stay intact.

Discovery165656 reached missing657120;165821 verified exact bytes twice there.
165918 reached missing65235E after completion;170029 reached missing651CA6 after
registration.170149 updates the oracle to that boundary, PC60C384/PR60C392/
SP8CFFB600/R4=8C7649F4. Next651CA6 and651C50 encode rendering headers; then
continue the remaining27 textures and exact per-extent checks. SON_EFF/full batch,
DMA, normal boot, G1/R3/R4 and production adoption remain open.

Command: unchanged `--sboard-camera` provider command in BUILDING.md.
Evidence: `docs/artifacts/recovery-aot/20261005-170149/report.json` and
`texture-transfer-contract.json`, SHA256 5c84aecf2c3b761eefe26a2a7f87747ab4a95dd65b2129037753b1a1138e9a99.
Binary SHA256 94b4f3ea69b43ad48e28ecfdb79ad91ad88b784a121cb84c20844bd5281b1ea7. Callback closure {'functions': 202, 'instructions': 21818, 'external_records': 1367}; static counts only.
Same public pin/six patches/CLT lane, no Simulator/game launch or publication.

## 2026-10-05 — R3/C8 complete Sonic/effect texture batches and original release

171115 passes two complete57-texture load/verify/release/reset cases (28SONIC,
29SON_EFF), plus12 stage cases and two joined stage updates.127776 exact VRAM
bytes, all outside bytes,57 headers/records/permanent-list links, bank selection,
rounded extents/ownership/used/free lists and1991 unused manager slots agree with
independently decoded input. Banks allocate64192 and63936 bytes. This proves
these files' CPU upload/registration, not rendering or all game textures.

170401 advanced through headers into the second file and rejected0B440E.
170728 passed the full first batch oracle but second run skipped loading because
allocator reset left resource counter1. Added original09AC9C release, following
60C5FC/651B00/657AA8/66759A to original lookup/free/coalescing routines.170844
and170955 discovered its missing651B00 and668680 dependencies.171115 verifies
counter0,57 free records, no used VRAM extents, full bank availability and retained
VRAM bytes before explicit original666E80 fixture reset. Both repetitions pass.

Runner now hash-checks independent SON_EFF input (5676 encoded/23264 decoded)
plus SONIC. Scratch reuse is checked rather than incorrectly expecting both
files' old buffers to coexist. All21 read sectors, closed handle, final filename,
resource counter and outer guards are checked. Header/VRAM expectations derive
from reference bytes, not runtime descriptors. Native callback closure:{'functions': 214, 'instructions': 22576, 'external_records': 1361}.

Command: unchanged `--sboard-camera` provider command in BUILDING.md.
Evidence: `docs/artifacts/recovery-aot/20261005-171115/report.json`,
`texture-batch-reference.json`, `texture-batch-contract.json` (SHA256 09b868f71811e49e021134b12bec509ab3af3c6aecf0768dda51f049329c6b94).
Binary SHA256 2f2c0095167ee750fa7c9febd00698c2d6110e0f2809c98cd88bba73b171166a. 38 current source/input/generated identities
and binary verified; same six patches/CLT pin, no Simulator/game or publication.

Next0B440E (PC0A9682/PR0A96A0/SP8CFFEFBC): original group2 task flags2,
callback0B450E,136-byte work and three-word source copy; verify its behavior and
continue Sonic startup. Heap/selection/call-order fixtures, outer graphics,
normal boot/frame, G1/R3/R4 and production adoption remain open.

## 2026-10-05 — R3/C8 auxiliary queues and original model setup

172737 passes both extended scheduler-prefix cases with the existing57-texture
load/registration/release/reset,12 stage cases and two joined stage updates.
Original0B440E/07B116/07C1FC and model-tree helpers now execute. The second07B116
invocation enters07B8C0; its model traversal and angle helpers return. Subsequent
geometry mode1 explicitly rejects at05A040 (PC07C41E/PR07C426/SP8CFFEEFC).

New source-derived oracle snapshots input geometry before guest mutation, checks
73-node preorder, all34 records,12 stage38-disabled rows,15 copied buffers with
3264 exact vertex/normal bytes and7 index-only records. Missing optional model
targets legitimately produceFF. Task checks cover five occupied nodes: character,
common-work group2 callback0B564E, auxiliary group2 callback0B450E, and group3
callbacks0B3F58/0B3F96. Both queue heads/directions, player0, heap bounds and the
three source pointers in136-byte auxiliary work pass twice.

Discovery171345/171506/171610/171734 reached07B116/07C1FC/0583D8/07B8C0.
172116 reached058474;172213 reached05A040.172410 exposed an oracle assumption
that optional model targets must exist; original0583B2 returns-1, stored asFF.
172513 passed geometry but omitted the earlier common-work auxiliary task in
its queue expectation.172636 records the exact queues; original06DFF0's call at
06E248 confirms that additional task. Corrected oracle172737 passes; no guest
state or source behavior was forced. Animation blend numeric values remain
unverified despite its return; the report preserves that explicit false flag.

Evidence: `docs/artifacts/recovery-aot/20261005-172737/report.json` and
`startup-model-contract.json`, SHA256
c44f3fd5e419277b14ff0acc1f2a91d8264dc2a16f4126f51979ad80ba78751f.
Binary SHA25678ac43c87eabb91448cf8cee7e93420784ed92b094f865c9af3f65c8705f768f.
37 distinct current source/input/generated hashes and binary verified. Callback
closure225 functions/25756 analyzed instructions/1393 external records; static
counts only. Same six patches, public pin and CLT/-O0 lane. Safety/syntax checks
pass; no native probe, Simulator or game remains running. Original inputs intact.

Next: audited05A040 initializes model/motion traversal with a bounded count and
calls63D9B0/63E4B0/059860/63D8D8. Inspect these providers and verify numerical
matrix/motion behavior before continuing toward the first complete group1 call.
No full scheduler tick, normal boot/frame, G1/R3/R4 or production adoption pass.
Heap/selection/call order, outer graphics, task updates/lifetimes and the separate
Katana/PAL acquisition gate remain unresolved. Reproduce with the unchanged
`--sboard-camera` provider command in BUILDING.md.

## 2026-10-05 — R3/C8 first motion pose and character display boundary

173842 passes two extended scheduler-prefix cases through original model/motion
and collision updates, rejecting explicitly at0AFD9A character display entry.
BoundaryPC0AB136/PR0AB13C/SP8CFFEFBC, taskR4=8CE40010. No full group1 return or
scheduler tick, normal boot or Sonic frame is claimed.

173017/173129 exposed a zero-address write in63D9B0 after traversal entered with
its stack producer absent. Source lookup found sole caller056242→63CD24 at05624E
withR4=8C7F0B60/R5=128/R6=0. The harness now calls that original producer before
the original texture-manager setup. Checks cover base/current pointer,128-entry
capacity, depth1, diag(-1,-1,1,1) initial basis, and eventual depth/pointer
restoration. Outer056242 and overall invocation order remain supplied.173317
then completes the73-node traversal and reaches05A0D4.

The new independent motion oracle snapshots original motion0 key0 and model-node
inputs before execution. It verifies19 allocated channels with two frames,
exact position bits and angles modulo65536, mode2/selection0/time0.0625. It
computes parent*T*Rz*Ry*Rx*S in host doubles with original fallback/scale flags,
then compares all1168 finite elements across73 matrices. Declared tolerance is
0.0002+0.00002*abs(expected); observed maximum2.23368e-7 in both repetitions.
173449 passed those checks.173602 adds05A0D4 and reaches0AFD30 after original
geometry-update calls return;173727 adds the collision updater07AB50/0597E0
and reaches display0AFD9A.173842 verifies the collision center against supplied
spawn(1645,-9,219) plus original height5.399999618530273; pitch/roll0, yawC000.
Other copied descriptor bytes retain their prior source/parameter checks.

All prior57-texture load/registration/release/reset, five task nodes/two auxiliary
queues,34 model records/3264 copied bytes,12 stage cases and two joined stage
updates pass. Final deformed-mesh values and arbitrary animation states remain
unverified and are explicit false/open fields; a returned updater alone is not
full geometry correctness. No native override/no-op, new core patch or fabricated
matrix buffer was introduced. Original sources/inputs are unchanged.

Evidence: `docs/artifacts/recovery-aot/20261005-173842/report.json` and
`motion-pose-contract.json`, SHA256
6a1d4328fbbda809035d1a76213de38e3a1a3c8fd99de5e0afcd2265006dcccd.
Binary SHA2561662c86ccb2d4b45ecc8c6145b0e411b11f92dbcdfcd2b30f00cf0bbd2025fa8.
38 distinct current source/input/generated identities plus binary verified.
Callback closure241 routines/27334 analyzed instructions/1363 external records;
static counts only. Safety/diff/syntax pass, no live probe/Simulator/game.
Same CLT lane, diagnostic-O0, public candidate pin and six core patches.

Next: compile audited0AFD9A and resolve its first038FA0 visibility dependency
with original camera/state producers. Preserve the verified pose/texture inputs;
require actual draw submission rather than treating a culled return as rendering
success. Continue toward completed group1 then whole-scheduler execution. G1
Katana/source/PAL acquisition, R3/R4, normal boot and production adoption stay
open. Reproduce with the unchanged `--sboard-camera` command in BUILDING.md.

## 2026-10-05 — R3/C8 original graphics startup, sharded build repair

175004 passes two scheduler-prefix cases after original outer graphics056242.
Checks cover camera origin/direction and matrix,640x480 screen/320x240 center,
projection independently computed from angle31C7, near/far3/3000 and the exact
60-byte background packet. Packet construction is RAM evidence, not display.
All prior57 textures/127776 VRAM bytes, release/reset, auxiliary tasks/model
records,19 animation channels/73 matrices, collision, file and stage checks pass.
Matrix maximum error remains2.23368e-7. Original invocation order outside this
producer, selection and heap remain supplied fixtures.

174117 compiled the display/visibility routines but culled Sonic. Adding outer
graphics produced269 routines, crossing the emitter's sharding threshold.
174237 failed to link because the harness included only generated_program.cpp.
The runner now includes all17 parts plus the root file, with consistent symbol
prefixes/diagnostic-O0 and hashes for each part and the generated header.
174546 then executed to missing665040. Static inspection identified the real
background packet builder and helpers664ECA/664EA0; all were compiled from local
original bytes.174703 completed outer graphics but failed an outdated initial
matrix expectation. Original6440F0/644280/653074 yield the default-camera basis
diag(1,-1,-1,1), not the earlier matrix-stack-only basis.174828 passed the new
matrix expectation and reached094240 after display culling.175004 adds projection,
clipping/background oracles and pins that boundary; both repetitions pass.

Evidence: `docs/artifacts/recovery-aot/20261005-175004/report.json` and
`graphics-startup-contract.json`, SHA256
6766d75000c64b45e9db89769ebb87ca25b078f059360cd437563baeed421119.
Binary SHA256841cdd12fc55af66aa5b4c99f4640fd79fed8410079b770775bbcbaf6ef17c69.
56 distinct input/source/generated identities verified. Callback closure272
routines/31916 analyzed instructions/1451 external records, static counts only.
Same six core patches, CLT, private inputs and no new native overrides/no-ops.

Next: compile audited094240 atPC0AB150/PR0AB16E/SP8CFFEFBC, measure its first
actual dependency, and continue toward group1 return. Then execute the original
gameplay camera update before claiming visibility or drawing. Default-camera
culling is expected for this supplied scene and proves no Sonic draw submission.
Whole scheduler, normal boot, arbitrary motion/deformed mesh correctness and
Sonic frame remain open. G1 Katana/PAL, R3/R4 and production adoption stay open.

## 2026-10-05 — R3/C8 first character return and camera-produced visibility

180247 passes two extended cases. Original09A42C(group1) now returns after
character initialization, ground query, collision registration and final update.
Checks cover ABI/stack/current-callback clearing, source descriptor resets,
collision-table append/count, radius=parameter+velocity norm, and original
position/rotation circular history. Zero ground candidates and +/-1000000
sentinels are explicitly the fixture scope; no ground-hit physics is proved.
All prior57 textures/release/reset,73 matrices, model/task, graphics/file/stage
checks pass. Existing source expectations were extended for original02F780's
record resets and the final radius update rather than retaining pre-return values.

175249 passed094240 and exposed02F7DC.175356 added collision registration and
06E398/06E340, then reached the harness's deliberate unexpected-return guard.
175541 passed both extended native cases but the runner still demanded the old
NEXT-boundary label. Removed that stale requirement and required the new return
checks. This was a harness-contract change, not a guest shortcut.

The original camera file is loaded and compared byte-for-byte, then0196FE
creates the camera in the same pool after the character's five occupied nodes.
Three group0 invocations run the original01993A through0→1→2→2. Fifty initial
position-history records equal the live player position. Return/stack and camera
state/current-callback clearing are checked.175701 exposed dynamic target01E846;
175818 advanced to0246D8, which was absent from the historical resident map.
Its executed PC/PR/bytes justified a separate focused map and analysis. The real
routine copies a selected camera record and calls01DF80.180007 returns all three
camera invocations.180126 re-enters display under the resulting view and reaches
0585C6 withR0=1.180247 pins this boundary and passes both repetitions.

Original038FA0 now returns1 and0AFD9A takes its rendering branch. No camera pose
or visibility result was injected. Recorded view(position,direction) is approximately
(1642.21,17.96,303.17,-0.19996,-0.643905,-0.738514); these pose values are not yet
independently verified numerically. Display rejects atPC0AFDC8/PR0AFDCE/SP8CFFEFD8
before0585C6. No Sonic draw submission or frame follows from this result.

Evidence: `docs/artifacts/recovery-aot/20261005-180247/report.json` and
`character-camera-contract.json`, SHA256
b750bb1fd71855089ba42ec88ee484bf73060566e7174a04b165098d3e5886c8.
Binary SHA2566453d2f11733ada6a786e4cda57b18191ceba77675225b850874f30b45a54bb3.
56 current input/source/generated identities verified; callback285 functions,
33454 analyzed instructions/1481 external records (static only). Six core patches,
CLT and diagnostic-O0 unchanged. No source override, Simulator/game launch or
publication. Camera's newly created tasks and other groups have not yet run as
a complete original frame; supplied startup/invocation order remains explicit.

Next: compile focused0585C6/0585EA/038F8C render-state helpers and continue through
0B3D90 toward actual model submission. First three helpers are9/9/7 instructions;
0B3D90 is180 with057B6E and unresolved indirect calls. Resolve executed targets
from their real inputs; do not grow a speculative inventory. Preserve separate
camera-pose, full scheduler, geometry, normal boot and original G1/PAL gates.

## 2026-10-05 — R3/C8 mesh execution and two bounded translator repairs

181955 passes both extended cases at original material helper0378B8, after
render-state setup, motion-tree traversal, mesh visibility and original vertex
transforms037534/0375F0. Each display invocation records22 SQ commits and0 TA
packets. These counters establish execution, not correct transformed geometry
or submitted polygons. Strict target/PC/PR/SP/model checks pin the boundary at
PC03799A/PR03799E/SP8CFFEF68, model8C1D2274. Earlier character/group1 return,
three camera invocations,57 texture uploads/release/reset,73 pose matrices,
model/task/graphics/file and twelve stage cases still pass.

180542 reached040BE2;180659 reached0408B2;180820 reached03742C. Adding audited
mesh helpers made181005 reject unknown C6dd. Patch0007 adds GBR-relative long
loads through decode/IR/emission and the memory-order barrier.181501 passes
all256 displacements at two bases with distinct data, guarded output/source,
register preservation and changed T, in uncached and active-GBR-cache lanes.
Review then found old-R0 constant retention in call analysis. A source-free
counterexample fails against the seven-patch core. Patch0008 invalidates R0 in
CFG and raw callback heuristics.181921 passes13 analysis controls, twelve
focused upstream tests and eight native execution variants. Unknown/RAW and
missing-target rejection remain enabled; no override or no-op was added.

181534 passed translation but rejected a write to physical0 while037534 used
an uninitialized vertex-workspace pointer8938CC. Original producer612AF0 and
caller056420 establish buffer8C7B0B60 and capacity4096. The fixture now invokes
that producer with the caller's exact arguments before056242 and verifies its
writes. It does not execute all of056420 or prove normal startup.181759 then
reached0378B8;181955 reproduces the strict new boundary in both cases. The old
0585C6 expectation was replaced only after observing this executed target.

0378B8 is absent from the historical resident map. A separate focused map is
bound to the observed call and target bytes. Its nine instructions merge R0
material flags into894384+8 with masks03F800FF/FC07FF00. The historical map is
unchanged and is not complete dynamic coverage.

Evidence: `docs/artifacts/recovery-aot/20261005-181955/report.json` and
`mesh-entry-contract.json`, SHA256 bb9579eaf5b56e4def06186d28d5e35201236b07ca78c922bf2046af407fc783.
Binary SHA256 00c05b308efdd97b164bc9c3e1fc78a6f0aed240564990f464808fa395923a49.
71 current input/source/generated/patch identities plus binary checked.
Callback closure311 routines/34815 instructions/1496 external records (static).
Eight exact patches, pinned public core, CLT and diagnostic-O0; original inputs
unchanged. No Simulator/game launch or publication. Katana/PAL G1, R3/R4 and
production adoption remain open.

Next: compile audited0378B8 and measure the next actually selected material/
polygon helper. Preserve explicit failures. Independently verify transformed
mesh values and eventual packet contents before claiming a Sonic submission.
Full frame scheduling, camera-pose numerical correctness, normal boot and
visible gameplay remain separate unmet gates.


### R3/C8 polygon initialization and first projection —184528

Both supplied-startup repetitions pass the new037C30 boundary and prior
regressions. Original608250 sets0x11C000;605E68 initializes the caller-derived
1 MiB system heap once beforevideo608442. Original60C954/608B0E then allocate
0xA02F0 bytes (uncached bufferACF5FD00), install output records, and return.
Repeated cases use the original605FC8 free path instead of resetting a live heap.
Test stacks moved to8CE3F000; the isolated stage heap moved to8CE10000 to avoid
system-heap allocations and texture-loader scratch. The filename oracle moved
by the same stack delta. These are supplied fixture addresses, not normal boot.

Graphics reservations follow original669A00/65AB6C/6671E0/667060:
frame=640*480*2; bank0 reserves8MiB-4*0x11C000-2*frame=0x264000;
bank1 reserves2*frame=0x12C000. Checks cover extent sizes, flags, owners,
used/free links and accounting. The existing independent PVM oracle retains
all127776 uploaded bytes and outside-range preservation checks across57 textures,
then proves texture release preserves graphics allocations; explicit fixture
reset restores the two4 MiB banks. This does not claim full graphics teardown.

Original material helpers0378B8/0378D8/03791C/0377D8 now run. First-mesh source
vertices are snapshotted before display;43x3 output coordinates agree with a
host double transform/projection (max error3.31671e-5). Guest XF matrix and
projection globals are explicit inputs, leaving independent camera/hierarchy/
lighting/full geometry acceptance open. A nonfinite-coordinate negative control
is rejected, and restoring the original bits passes, in both cases.

Failed probes are preserved:182229→0378D8;182348/182619→zero output buffer;
182806→605F12;182920→65A680;183108→669A00;183234→668092;
183347→669820;183454→65B6A8;183620/184052 expose the obsolete all-free VRAM
assumption.184259 passes revised reservations/textures but exposes the stale
stack-relative filename oracle.184404 reaches037C30;184528 pins that boundary
and passes both complete fixture repetitions. No missing function was replaced
with a no-op, injected output pointer or forced-success return.

Next: original037C30, atPC037A90/PR037A9A/SP8CE3EF68/R4=8C1D2274.
Focused audit is in ignored `work/dreamcast-recovery/loader-review/startup-producers/037C30/`.
It constructs32-byte textured vertex packets via store queues, with original
strip lengths/indices/UVs; a separate branch computes environment UVs. Compile
this existing routine, preserve unknown/missing-target rejection, and verify
packet values before any submission/frame claim. Current22 SQ commits/0 TA
packets prove neither. No normal boot/full scheduler or Sonic frame yet.

Evidence: `docs/artifacts/recovery-aot/20261005-184528/report.json` and
`polygon-projection-contract.json`, SHA256
5af7907aab8e3aed67df2cf332000188ca71b8e3df27abcfcce41f416f4dac89.
Binary SHA2560638d2fcc2d8e85fc6c8d9897fa0694fcf29de430821c0c08a2264c96bf03519.
141 current input/source/generated/patch/evidence hashes plus binary verified;
callback344 functions/39365 instructions/1530 external records (static only).
Same eight core patches/CLT diagnostic-O0; no Simulator/game app or publication.
Original G1/PAL and R3/R4/adoption remain unresolved.


### R3/C8 first textured FIFO packets and interrupt continuation —190025

Original037C30 runs its selected non-environment-texture path and returns.
Before display, the test snapshots the first mesh's12 signed-length/index strips
and98 UV pairs. After execution, it checks98 complete32-byte vertex packets:
bit-exact serialization of independently verified projected XYZ; signed16-bit
UV conversion using the original scale; end-strip flags; lighting serialization;
output cursor; one material header's four written words; exact FIFO length and
preserved prefix. A wrong end-strip flag is rejected and exact restoration passes.
The43-vertex projection oracle and its nonfinite-X negative control still pass.
Lighting and material values are guest inputs, not independently proved shading.
Camera and XF inputs remain explicit limitations of the projection oracle.

184831 first reached037950.185027 falsified the test's assumed translucent RAM
destination. Original609AEC+16/0377D8 actually select record+0, the direct opaque
FIFO at10000000. The existing runtime captures raw32-byte transfers and defers
parsing until a graphics flush. Thus the older0 `ta_packets` metric means0
parsed packets, not0 submissions. Corrected185224 checks99 staged packets
(header+98 vertices), then exposes a real second-case interrupt dependency.

185449 diagnostics pin event0320, VBR0, PC600, SPC8C669C78, SR70000001/SSR1;
pending bank0=f, mask row2=2807ec. Fresh fixture contexts had lost the post-BIOS
VBR, and the original vector producer was omitted. Original010180→606210→649B20
provides the initialization. The provider now executes649B20 with original
source arguments and checks all three56-byte copied templates against source.
Joined startup contexts retain the boot VBR8C000000. A bounded128-step loop uses
the public runner's saved-PC continuation mechanism only after an async redirect;
it does not suppress interrupts or replay completed work.

185708 executes the relocated original handler, resumes8C669C78/669D74/66A680,
then rejects65AC04. That block was emitted but omitted from our joined callback
manifest. The manifest now preserves all5460 resident function/block bindings
from the generated registration table, checks that no function entry was lost,
and keeps loaded-module activation separate.185916 exposed a Python variable
shadowing mistake before execution;190025 fixes that and passes both repetitions.
One event0320 completes through six continuations, with the original graphics
call's stack/PR/preserved GPRs restored and all prior output regressions green.
This is one observed interrupt path, not general interrupt/timing acceptance.

Next037950 atPC037AF4/PR037AF8/SP8CE3EF68/R4=8C1D2274 is the original nine-
instruction output-pointer update. Its focused source audit is preserved under
`work/dreamcast-recovery/loader-review/startup-producers/037950/`. Continue this
original path while retaining the first-strip proof; later meshes require value
checks rather than reusing the first mesh's matrix after traversal changes it.
No complete model, rasterized Sonic frame, normal boot or full scheduler yet.

Evidence: `docs/artifacts/recovery-aot/20261005-190025/report.json` and
`first-strip-contract.json`, SHA256
5aadace6a9e41d376a7550b2f4e3fd7df5a3d5e04f9c6122adb3b01e23247252.
Binary SHA256dddd4d9d7c5b4d46d351d5b6e6e9b1cb2a9c1b19492e2bf058c7bb1d060752ed.
145 current identities plus binary verified. Callback345 functions/39562
instructions/1529 external records (static); same eight patches/CLT diagnostic-O0.
Prior57 textures/release/reset,73 matrices, character/camera/file/stage checks
pass. Original G1/PAL, R3/R4 and production adoption remain unresolved.


### R3/C8 original character display return —202744

Original0AFD9A now returns in both supplied-startup repetitions, with normal
PC/PR/stack and preserved GPRs. Original0585D8 restores the pre-call render
flags/control;051BA0 executes the immediate stage query. The observed model
path completes53 material output-pointer updates and stages819 raw FIFO packets
per invocation (982 SQ commits,0 parsed packets). Those counts are regression
observations, not independent proof of every model/packet value or a frame.

The first43 projected vertices and98 textured vertex packets still pass their
existing independent projection/source-UV/strip-ending checks and both corruption
controls. They now run immediately before the first original037950 pointer update,
so later meshes can overwrite the projection workspace. A scoped test dispatch
wrapper always calls the generated original037950 body, then checks its pointer
write against the incoming cursor/segment. Direct-call bypass is disabled for
that scope; registration and the prior flag are restored at scope exit, including
exception unwinding. This is explicit observation instrumentation, not a missing
game-function replacement or supplied success result. The public core is unchanged.

190759 compiled037950 and reached037B12 after the first checks passed. The
source audit identified original material-alpha handling.190931 added it and
reached037850 after two material completions/114 staged packets. Original037850
builds a header for the buffered translucent path.202534 added it, traversed the
model, and reached0585D8 after53 completions/819 staged packets. Adding that
nine-instruction state restore and the immediately following five-instruction
051BA0 query allows202744 to return. Failed evidence remains intact. The test
now requires a real display return instead of accepting a missing-target exception.

Both cases retain57 texture uploads/release/reset, graphics reservations,73 pose
matrices, character/group1 and three camera invocations. Twelve stage cases,
original file/provider controls, vector copies and one0320/six-continuation
interrupt return pass. No later-mesh numerical oracle, independent camera pose,
lighting, parsed/rasterized Sonic frame, normal boot or full scheduler is claimed.

Next: trace original graphics flush/scene lifecycle and buffered translucent
submission. The owner BOOT contains A05F8014 at literal6558EC; original65568A
writes STARTRENDER at655880. Its caller655900 is recorded at655C40 in the
resident call inventory.65568A requires an original ready-scene state3 in
8A7318, so directly writing readiness or STARTRENDER from the host would bypass
the missing prerequisite. Read the parent/state producers and execute the next
original boundary. Keep parsing and visible presentation as separate checks.

Evidence: `docs/artifacts/recovery-aot/20261005-202744/report.json` and
`display-return-contract.json`, SHA256
0b53bed52818620343ee792426ae36c48109502155f1dd1e3a7a2d6034f77fa9.
Binary SHA256877a6d93fc223e61bfc2ed79e864836a590d78121b9f58342bb7814798f61109.
151 current input/source/generated/patch/evidence identities plus binary verified.
Callback350 functions/39731 instructions/1523 external records and5487 resident
bindings are static counts. Eight core patches and CLT diagnostic-O0 unchanged.
Original Katana/PAL G1, R3/R4 and production adoption remain unresolved. No
Simulator/game app launched, source acquired, global setting changed or publication.


## 2026-10-05 — original frame boundary and controller startup,205334

The prior turn's203235–204816 experiments extended display return through
original608DF0. Original606328 establishes the TMU;646180 closes output lists,
645C40/645B40/654908 perform two real channel2 DMA transfers totaling21216
bytes. The staged stream reaches1483 packets.203702 timed out in the original
scene wait;204027 isolated a disabled runtime device clock (zero cycles/ticks).
Enabling the public guest-cycle clock, without host synchronization, let original
655900→65568A reach STARTRENDER.204816 stopped before controller polling because
8C8AC634 was zero. One logical render/completion, two vblanks and zero page flips
were observed. The saved640x480 render buffer is entirely black: zero nonzero
RGB bytes; visual inspection agrees. This disproves visual-success claims.

205334 resolves that missing producer from original BOOT bytes.010180 calls
60623A after video, then605774 withR4=0,R5=8C6803C0,R6=8C68C3E0.605774 invokes
64A044→6628F0, which writes8C8AB538 to8C8AC634. The harness now executes these
original initializers with those arguments, verifies the return/stack/work area
and all24 controller-record pointers. No guest pointer or ready flag is supplied
in their place. The original frame routine then returns after controller polling,
with five0320 interrupts and fourteen saved-PC resumes; ABI/stack/preserved GPR
checks pass.1483 packets parse, one STARTRENDER occurs, and the deliberate
post-return diagnostic guard keeps the report failed pending render inspection.
Input response, actual controller detection and normal boot remain unproved.

Provider closure459 functions/33497 instructions/202 external records; callback
closure361 functions/41266 instructions/1563 external records. These are static
counts. Eight core patches are unchanged. Original202744 remains the latest
complete passing regression report; later diagnostic reports do not finish
resource cleanup or the second supplied-startup repetition. No Simulator or
game app launched, no publication. G1 Katana/PAL, R3/R4 and adoption remain open.


### Raster/color reduction —205457 /205603 /205723

205457 adds counters but detailed statistics were disabled: its zero detailed
counts cannot establish no geometry.205603 enables only runtime statistics;
986 onscreen and15 offscreen3D triangles,3911 covered samples,2287 depth passes
and1624 depth failures,2287 pixel writes.1003 nondegenerate triangles include
the two background triangles;555 opaque/448 translucent and depth mode6/cull1.
1425 parsed vertices and988 raster triangle completions are separate counters.
All640x480 render RGB pixels are zero. Present/registration retain101018 clear.

205723 captures the exact1483*32-byte stream at savedPC645CD0 before parsing:
55 polygon headers,1425 vertices, three list endings. All55 headers use packed
color format0. Vertex word6 isFF000000 for1096 and00000000 for329; word7 is zero
for all1425. Thus black color is already present before the renderer. This does
not prove texture/shading correctness or the entire black-frame cause. No guest
color, lighting, depth or readiness was overridden. Next trace actual lighting
producer:0375F0 readsGBR+88;038F8C selects039200+index*2048. Source table writer,
caller and initialization inputs must be established before invoking them.

205723/report.json remains passed=false at the deliberate frame-return guard.
frame-input-diagnostic.json SHA256
1dfb7c5f63a0ae8ba825ed69db6dc13498f8e5e0d8c7ae45215342195d1820b8
binds194 current input/source/generated/patch/evidence identities and native
binary SHA25616af5e6be2562a74b9f1bb59b7e5b6e5ccc2b7519274456e9e5c55c0bbfd78fb.
Original full regression202744 remains valid historical evidence; no claim of
current cleanup/second-case pass. Repository safety/diff checks pass. No live
probe process remains, no Simulator/game app launched, no publication.


## 2026-10-05 — original stage lighting and first partial raster,211105

Lowest recovery goal remains R3/C8. The prior turn advanced original controller
startup/frame return and isolated black packed vertex colors. This turn traced
GBR+88/038F8C through the original stage-lighting initializer012240. It reads
stage/act2600 through051540, checks peripheral state, initializes the six palette
banks, loads SL1C0B.BIN and PL1C0B.BIN through0143C2/014488→014360 and the existing
file provider, then establishes original lighting direction/material data. The
unused default palette routine038DE4 was inspected but not substituted. Startup
invocation order remains supplied; this is not normal boot.

Independent owner-disc extraction pins PL1C0B12672 bytes SHA256
003ae42e2f6e5aa1b5cf208baaf8f12d4c94801f27ff43f7ac0c6fdad1414b5b
and SL1C0B1920 bytes SHA256
528ad07114a141852a0af402164e7835b590a7bb39ddb6140550480fa3b20671.
The harness compares every loaded byte at039200 and6B2CC4 in both repetitions.
Expected arrays are read-only oracles, never copied into guest tables. Original
Maple work area/24 record pointers remain checked. No color/depth override or
forced success/readiness was introduced; eight public-core patches are unchanged.

Preserved failures:210251→605C9A,210417→64BFAE,210530→63E744. The final dependency
is an original exponent jump table; its control-flow targets and remaining light
math/copy helpers were audited and compiled. Unknown/RAW/missing-target rejection
remains active. Callback cap448 admits385 functions/42394 instructions/1564
external records;5867 resident block/function bindings. These are static counts.

210721 returns from lighting and the original frame routine. The exact1483 raw
packets contain1425 vertices, now all with nonzero RGB;882 also have nonzero RGB
offset color. It produces1359 nonblack pixels within[x452..486,y0..58]. Visual
inspection shows part of Sonic clipped by the upper edge, against black. This is
partial raster evidence, not correct camera framing, level rendering or gameplay.
210721 still exits1 at the deliberate diagnostic guard, preserved as evidence.

The harness then replaces that guard with explicit frame/DMA checks:1483 parsed
packets, an emptied staging stream, two DMA transfers totaling21216 bytes, one
STARTRENDER/logical-frame/render-completion increment, and bounded nonblack output.
Each frame call checks ABI/stack/preserved GPRs and its original interrupt return.
210923's native execution passes both cases and cleanup; its runner rejects the
historical one-nonframe-interrupt label. With peripheral/lighting startup in place,
there are zero such nonframe continuations and two frame continuations, each with
five0320 interrupts/fourteen saved-PC resumes.211105 requires both actual frame
continuation labels and reports the old single-interrupt boolean false/count0.
No event was injected, skipped or acknowledged by a test override.

211105/report.json passes all existing file/MiniCart, twelve stage cases,
character/camera,57 texture upload/release/reset,73 pose matrices, first43 mesh
projections/98 textured packets and corruption controls,53 pointer updates,
plus two lighting and frame cases. Both full render PPMs are byte-identical.
After the second case, one runtime page flip is observed and its present PPM
matches the first render PPM byte-for-byte. No native window was opened.

lighting-frame-contract.json SHA256
67ab579e598ce982e943059f17392beacd28e4085d4c34705b75c6bf61817ab8
binds218 current input/source/generated/patch/evidence identities and binary
SHA25619afb4e163b0f8ae29ccd768ea57d74855791f3c1a69a7183a460474842f58f4.
Captures, per-frame raw streams and lighting-render-inspection.json are private
under211105. Repository safety, Python syntax and diff checks pass. No Simulator,
SonicPad app or publication; original G1/PAL, full geometry, independently verified
lighting/camera, normal boot/full scheduler and R3/R4/adoption remain unresolved.

Next C8: join the original SBOARD registration/state0→2→3 and live character/camera
into the same pool in the extended fixture, then execute the next original stage
and task-scheduler dependencies. The non-extended check_startup already proves a
state3 update with valid player work; the extended branch currently renders the
character and returns before that join. Preserve both sets of checks when joining.
Do not move the camera or inject a level to make this partial image attractive.


## 2026-10-05 — Live stage join and streamed snowboard texture —213237

R3/C8 makes execution progress; overall report remains passed:false/exit1.
211547 joined original SBOARD registration/state0→2→3/update to the fully
initialized character/camera pool, preserving prior live allocations across the
original147-sector stage reload. It rejected group2 head0B4B06. Source inspection
showed its model pointer0CB37454 and fallback texture list0CB36F94 belong to the
original stage38 S_SBMOT load. Original stage selector09F3A8/09F3B4 loads this pair
for character0/2; the current selector returns0 without a new forced value.

213237 executes both09F664 loads with the original filename/destination arguments:
S_SBMOT.PRS at0CB08000,117125 encoded/261688 decoded bytes, and M_SBMOT.PRS at
0CADC000,74957 encoded/165232 decoded bytes. Independent raw-disc reads and Python
PRS decoding are read-only oracles; all426920 loaded bytes compare equal.95 sector
reads and preservation of the live heap, stage image and camera data are checked.
Encoded/decoded hashes are bound in the report; these loads establish this consumer
path, not all motion formats or a complete original stage-loading sequence.

Original0B4B06 chooses resident texture list1CA0B8 (MIW_B001), loads the texture,
and installs update0B4B66/display0B4C88/cleanup0B4DE6. The constructor's model/list
pointers and speed literal match source. Its original streaming PVR reader uses
real filename normalization, open/read/seek/status/close helpers. MIW_B001.PVR is
10952 bytes, SHA25672551086cbd33c4600140fa18eb8c52d7aedaaa1db5fe4dcbe3645ca3d02a46c.
Checks verify global ID500,64x64 dimensions, mip size10928/flags5, VRAM54B480,
use count1/error0 and permanent-list registration. All10924 payload bytes appear
at VRAM54B484; comparing the entire8MiB against the pre-call snapshot plus only
that payload proves outside-range preservation. Six original sector reads occur.
No reference bytes, successful returns or resource pointers are injected.

Preserved failed probes advance through real missing targets:
212152→65008C;212310→652D0C;212436→652DB4;212600→604BBA;
212711→652998;212830→664C84;213003→0B450E.213237 adds independent
texture-byte/constructor checks and confirms the same next boundary. No core
patch changed. Audited filename/header/cache/streaming helpers were compiled
from original bytes; unknown/RAW and missing-target rejection remain enabled.

At0B450E, PC09A456/PR09A45A/SP8CE3EFEC/task8CE40070, group2 has not returned.
The first extended case has executed the new prefix only; cleanup and the second
extended case remain untested for this version.211105 stays the latest full pass.
The first frame before stage joining still has1483 parsed packets/two DMA transfers,
21216 bytes/one render completion and1359 nonblack pixels. Its capture is byte-
identical to211105's first render: clipped Sonic against black, no full level.
Do not infer snowboard drawing, full scheduler, normal startup or gameplay.

Next exact prerequisite: local S_MOT.PRS, candidate0767,214890 encoded bytes
SHA25652398564204238a3dcaff24a01ad255f0d805a21b334bc48f328a08b620a57ca;
488608 decoded bytes SHA256239a25a00e6314b81bf3c2bc37784efdb6550420b3d31b8c48bc6819c47c0f94.
Original09F5A0(character0) selects filename8C15D110 and destination0CC00000,
then tail-calls09F664.0B450E writes model coordinates at0CC0252C+24/+28;
this memory is not yet loaded. Execute the original selector/loader, compare
bytes and preserve live heap/stage/camera/snowboard memory before compiling that
callback. Also retain missing SET files/other stage dependencies as open; do not
replace them with zero-filled memory or accept this supplied startup as normal boot.

Evidence:213237/report.json and snowboard-startup-contract.json SHA256
02d6a658d7afc00e4b6fc521b888a5c9b2299b447cd4ac3c05c8aaeadec279ee.
232 current identities checked including binary SHA256
30930eb45a63a1cf242fa500cf017bf1cfbb5bc1b85c9b5d02590ec817292c75.
Callback413 routines/47894 instructions/1635 external records; provider461 routines/
33547 instructions/202 external records are static counts, not complete coverage.
Python syntax, repository safety and diff checks pass; no live game/Simulator.
Katana/PAL G1, R3/R4, foundation adoption and physical gates remain unresolved.


## 2026-10-05 — Character motion load and first group2 return —213949

Both supplied-startup repetitions pass; this supersedes213237's failing prefix
as the current regression checkpoint. Original09F5A0 receives the actual character
selector result0 and loads S_MOT at0CC00000 through09F664. All488608 decoded bytes
match independent raw-disc extraction/PRS decoding;105 sectors are read. Existing
heap, stage, camera and two snowboard asset checks pass around the load. No guest
motion data is injected. Source/asset hashes are retained in the new report.

213619 compiled the previously blocked0B450E and reached0B564E. Its original
auxiliary initializer and the two audited selector helpers0446AA/07EAE0 now run.
213949 verifies the entire first group2 invocation returns with stack/ABI/preserved
registers restored and current-callback1C5A78 cleared. Model auxiliary state becomes1;
its installed cleanup is0B44B0, nine coordinates match the three source nodes,
and twelve signed rotation limits match original immediates. All S_MOT bytes remain
identical except the two allowed word destinations0CC0252C+24/+28, whose values
match the original source model. This is a bounded model update, not arbitrary
motion-format or animation coverage. The effect task installs0B573A/0B5B4C/0B571C,
clears its original work fields and selects the matching source table record.

The original snowboard constructor/texture checks still pass twice. Its installed
0B4DE6 destructor calls the original texture release: the additional record becomes
free, VRAM is preserved and the57 character textures remain live. Existing original
character resource release and graphics-reservation checks then pass, followed by
the fixture's original bank reset. This does not exercise complete task-tree
teardown or establish ordinary game lifecycle behavior.

213800 first returned group2 and completed both cases. Review caught an incorrect
address in the newly added callback-clearing assertion; exclude that assertion
from213800 evidence.213949 checks the scheduler's source literal1C5A78 and requires
all six new prefix/result labels twice in the runner. It also checks the destructor's
VRAM/character-texture preservation explicitly. No guest/core shortcut was added.

All previous provider/Mini Cart/failure controls, twelve stage cases, texture/model/
motion/camera checks and both frame/DMA/interrupt returns pass. Both render buffers
are byte-identical to211105's first render; the second present buffer also matches.
Visible scope remains clipped partial Sonic against black. No full stage, snowboard
display, full scheduler tick, normal boot, gameplay or new presentation claim.

Next execute09A42C(group3) before releasing live resources. Actual stage head
0C90554A compares stage/act and tails to09AB38, which traverses child task+12 links
and invokes each child+16 callback. Source audit is private under startup-producers/
90554A and09AB38. Preserve strict missing-target errors and inspect the first actual
child dependency, including source-derived SET/texture prerequisites. Other groups,
repeated update/display order and normal startup remain open.

Evidence:213949/report.json and group2-startup-contract.json SHA256
89c072b3096b7904dc78f009c5e11c344c102e0d17672b42ab64194feba56177.
231 current input/source/generated/patch/evidence/binary identities verified;
private verified-source snapshots preserve this checkpoint's harness inputs.
Binary SHA25628e392dcba86efbe6b4e7805a050782a023f12b054175f79c5aa4c5864c23c43.
Callback418 routines/48407 instructions/1640 external records; provider462 routines/
33606 instructions/202 external records are static scope only. Eight core patches
unchanged. Python syntax, diff and repository-safety checks pass. No Simulator/game
app, external acquisition/message or publication. G1/R3/R4/adoption remain unresolved.


## 2026-10-05 — R3/C8 original group3 and model-link cleanup,215723

The preceding status-only turn was no progress; this turn executed changed
experiments.214320 first reached0A2822;214828 compiled it and047A8A, stopping at
63CDD0.214958 passed that vector operation and the first two stage children,
then rejected07E9B6.215126 compiled that original proximity routine, traversed
all nine stage children and stopped at auxiliary0B3F96. No original instruction,
camera coordinate or guest return was fabricated.

Added original0B3F96/0B3F58 and source-derived checks.215545 completed group3 in
the first case, then failed the second character display's819-packet assertion.
Source analysis identified the omitted0B3F8C destructor, which restores1D1568
from1C9C50;07CD96 restores the attachment model link.215723 calls both installed
original destructors and compares both links with their pre-group3 values.
The first failure is preserved as a regression counterexample.

Both supplied-startup repetitions now pass original group3 return, callback
clearing, nine child links/callbacks, seven original distance triggers remaining
inactive, model phase0→1/destructor installation,32-byte attachment work and four
original model source pointers. Independent distance calculations use live
player collision centers, source positions and original radii. The camera-facing
child takes its nonpositive branch; the effect-count child skips drawing. These
are execution/initialization checks, not scene/display or activated-object coverage.
Both model destructors preserve VRAM. Prior texture release/reset, first motion,
lighting, camera, frame/DMA/interrupt, MiniCart/provider and12 stage cases pass.

Both render files equal213949 exactly; second present equals first render.
Still1359 nonblack pixels of clipped Sonic, no complete level, snowboard display,
normal boot or gameplay. Full task-tree teardown remains false. Original G1,
R3/R4 and production adoption are unresolved.

Exact command remains the documented --sboard-camera provider command. Private
report and group3-startup-contract.json live in20261005-215723; manifest SHA256
7608af0cf8438e5f349b9db324bc0c812ab2c0e7a186ac5593f72a7a00048443.
244 input/source/generated/patch/evidence/binary identities verified; exact
source snapshots preserved under verified-source. Native ARM64 binary SHA256
0da70c3420e7a2ef3956101e80db701ae7c824c62c3be60b455442adfcda277e.
Callback429 functions/49404 instructions/1662 external records; provider462/
33606/202. Counts describe compiled scope, not all-path behavior. The eight core
patches are unchanged.643828/642BF4 are source-audited seeds for the first child's
positive draw branch, which this run does not exercise.

Historical214438 was an unchanged build accidentally launched after an audit
script failed. It was explicitly stopped before execution; terminal exit143 and
interrupted Ninja log are retained. It supplies no pass or progress claim.

Next: original09A544 traverses update group7 then0–6; execute against the joined
live state before resource release.09A564 traverses display in the same group
order via09A4A0. Their source audits are retained under startup-producers. Do not
replace the missing normal scheduler/SET prerequisites with forced state.


## 2026-10-05 — R3/C8 original update traversal,221028

Progress from215723: run original09A544 before resource release, preserving its
update order group7 then0–6. Empty group7 and group0 return; group1 reaches the
player's original movement/collision path. Eight bounded runs stop at successive
missing original functions:220040→0A2B38,220150→0B1B3A,220307→0739E0,
220444→0722BC,220613→63CBEC,220731→074080,220843→0747E4,
221028→076990. Each terminal exit1 is retained; no full scheduler pass claimed.

Original code changes timer8→7, player state1→65 and vertical velocity bits
00000000→BDA3D70A (approximately−0.08). Position bits remain
44CDA000,C1100000,435B0000. Latest failure PC8C074F06/PR8C074F18/
SP8CE3ED04 enters missing8C076990. Local source audit identifies its vector
helpers63CDD0/63E49C/63E68C, already compiled. Next add this audited function and
verify collision/terrain/SET producers before accepting movement. A deliberate
return guard requires inspection even if the complete update returns.

221028 has no full update return, cleanup or second repetition.215723 remains
the latest complete bounded regression. First render matches it, still clipped
partial Sonic against black. No normal boot, complete scene, gameplay, G1/R3
completion or production-foundation decision. No Simulator/game app launched.

Private update-traversal-boundary.json SHA256
9425ea9db89d0c202e04a9e766f4d6cb3f4b937019ca2cd10b3749d78352f9bb
binds232 identities; verified-source retains exact sources. Binary SHA256
79d1cf7112dffe82a41d4acaa35a99520b843a2f51fd3a074940f6307724ef49.
Callback closure445 functions/54294 instructions/1771 external records,
provider462/33606/202. These are static compilation counts, not execution
coverage. Existing448-function cap and unknown/RAW rejection remain in place;
eight core patches unchanged. All runs terminal; goal remains active.


## 2026-10-05 — R3/C8 missing terrain startup,222407

Previous goal turn: no execution progress; checkpoint documentation only.
This turn produced six changed, terminal exit1 experiments:
221610:076990 returns, rejects028D9E;
221731:028D9E/054020 return, rejects076EB4;
221927:076EB4 returns, rejects07AD36;
222111: original terrain task added, rejects053E28;
222247: visibility returns, rejects039040;
222407: terrain drawing reaches mesh pointer0C96882C, rejects037F6C.
No unchanged restart; one native probe at a time, no Simulator/game app.

221731/221927 diagnose zero static/dynamic terrain counts, zero collision list,
zero enabled descriptor. Y changes fromC1100000 toC11147AE on that empty path;
this is not ground collision acceptance. Original0503F2 loads callback053720 and
calls09A1CE withflags2/group1, then0503FA creates Sonic. The test had omitted
terrain startup. Its new tail creates that original task using those arguments,
then invokes053720. This joins after retained baseline checks; original task
creation order remains unproved. No forced descriptor, counts or geometry.

The original task selects0C902DEC, the stage38 descriptor already referenced by
BOOT's1A2734 table. Source descriptor:315 records at0C9001A0, flags12,
texture nameSANDBOARD, texture list0C90A874. Local inventory contains
SANDBOARD.PRS,561126bytes. Original load path returns; additional274sectors
are observed, but exact loaded texture/VRAM correspondence is not yet asserted.
Original053E28 visibility returns and053BB6 reaches0373A8 terrain drawing.
222407 fails at037F6C fromPC037ACC/PR037AD6/SP8CE3EFCC/R4=0C96882C.
Calls2396013/sectors2236. No terrain return, full update, cleanup, second case,
new frame presentation or gameplay. Existing first capture precedes terrain.

Next compile audited037F6C. It was absent from the historical function map;
its observed target receives a separate private focused map, not a historical
map edit. Audit exposes038180/0387B6 branches, not yet execution evidence.
Finish source-derived texture/record selection and collision registration checks
before removing either return guard. Earlier update boundary07AD36 is still
uncompiled.054900 audit identifies original bounds preparation from0501F0;
052944/09B670 audits identify callers preceding09A544 in050474. Their startup
contracts remain unexecuted; do not equate terrain drawing with collision setup.

Callback bound raised448→480 to accommodate the audited additions; actual closure
462functions/56055instructions/1789external records. Unknown/RAW rejection and
eight core patches unchanged. Provider462/33606/202. Counts are static scope.
Private222407/terrain-startup-boundary.json SHA256
1e684f8d3f465ae9cbdba83ef11609dd42824ccd716c84f1a39744aea9839c9a
binds219 verified identities and source snapshots. Binary SHA256
fbcc981053944d99fcf5c63f1d848e0654ce86ca44e593347ffa52aeb8eca8cd.
215723 remains the latest full bounded regression. G1/R3/R4 remain open.


## 2026-10-05 — Terrain clipping bank-state diagnosis,223726

222723 compiles original037F6C and rejects038180.222914 adds audited038180 and
0387B6; the second clipping call fails reading00205710.223153 inherits the full
privileged boot context (SR40000101/FPSCR40001); the same failure persists.
This falsifies the claim that simply retaining privilege fixes the pointer.

223726 adds bounded register-only wrappers around the two original targets;
original functions still execute, and dispatch entries restore on exception.
First mesh entry R2=0C9685EC/SR40000100. First clip return R2=0/SR60000101;
next mesh R2=18, next clip R11=00205710, causing the unmapped16-bit read.
Original038180 clears SR.RB on entry and sets it on return. The next experiment
must establish its original caller bank-state contract before modifying the
supplied harness state. No new core patch, skipped call or fabricated pointer.
Instrumentation changes dispatch: staged packets5 versus6 in223153; timing
and output equivalence are not claimed. Both executions fail at the same UV read.

New audit-recovery-terrain-output.py independently checks50 SANDBOARD textures,
645616 exact payload bytes in decoded scratch/VRAM, dimensions/format/links,
nonoverlapping transfer ranges,315 unchanged terrain records and54 unique valid
selected pointers.222914/223153/223726 pass. Separate one-byte VRAM and record
corruptions are rejected. The selected visibility calculation itself, collision
registration and outside-transfer VRAM preservation remain unverified.

All four runs terminal exit1; current callback closure466functions/57536
instructions/1788external records is static compilation scope. Provider462/
33606/202 and eight core patches unchanged. No terrain return, full update,
cleanup/repetition, new presented scene or gameplay.215723 is last full bounded
regression. Original task creation order is still supplied/joined. G1/R3/R4 and
production adoption stay open; one probe at a time, no Simulator/game app launch.

Private223153/terrain-clipping-boundary.json SHA256
 d4c59b3ec3b80b1305faeef93b46ade6f3a0e1869eea8fd281de3e9a89a68570
binds175 identities. Latest223726/terrain-clipping-boundary.json SHA256
81bc0c6fed14301e4d9eb229ed30013ef334334afc0325e79a6eb6340b243eb9
binds187 identities plus compiled-source snapshots; binary SHA256
7a4522871c4410876fe8744f7f755ed6fcaff4111a36512439b8ed349c991b47.


## 2026-10-05 — Supplied bank1 terrain frame,224845

Previous goal turn made execution progress by tracing the mesh-pointer loss.
This turn produced three changed terminal experiments, all exit1 at explicit
boundaries. No repeated unchanged failed build and no new core patch.

224422 varies supplied startup SR40000001→60000001. Original038180 clearsRB
for scratch work and setsRB on return. With bank1 entry it preserves R2=0C9685EC;
the next mesh uses0C968604 rather than18. The first model returns and execution
rejects missing02E984. Its source is locally available and has19 instructions,
no calls; it restores matrix registers from the original matrix-stack pointer.
Register selection agrees with the [Renesas SH-4 manual, section2.2.1](https://www.renesas.com/en/document/mas/sh-4-software-manual).
This establishes a callee precondition, not the original game's startup producer.
The directly inspected drawing callers do not setRB; normal startup remains open.

224651 adds original02E984 and makes the hypothesis explicit with runner option
`--startup-bank1` (requires `--sboard-camera`). It is recorded in report.json and
in generated compile definitions; the default provider retains40000001.
The original terrain task now returns, selects descriptor0C902DEC, state1,
display053B60/destructor053A98.54 visible pointers and11632 queued drawing packets
are observed. Static/dynamic collision counts and collision list are still0.
An intentional terrain-return guard stops before update acceptance.

224845 checks the returned task state/callbacks, then invokes original608DF0
with the runtime's guest-cycle clock. Five interrupts/14 saved-PC resumes lead
to one new render completion and empty packet queue. Exact return PC, PR, SP
and preserved GPR checks pass. The deliberate guard now stops after this frame
and before full update; it is not a successful complete regression.
Image scene-2-render.ppm SHA256
5ef5c3f689ebd619cb46de557d7000fd8c935ed427595e76bba91a799120cacb
has291583 nonblack pixels at640x480. Inspected PNG shows textured sand and stone
terrain with black upper/background space; no Sonic in this terrain-only image.
The present buffer is byte-identical to the preceding partial character render,
not the new terrain render. No displayed complete scene/gameplay acceptance.
Detailed raster counters were disabled for the second frame; do not attribute
the earlier detailed triangle/depth counters to terrain.

All three captures pass50 texture transfers/645616 bytes,315 source records and
54 selected-pointer bounds using the independent terrain audit.224845's first
character render still matches223726 byte-for-byte (1359 nonblack pixels).
Visibility math, outside-transfer VRAM preservation, cleanup/repetition and
full task ordering remain unverified. R3/R4/G1 and production adoption stay open.
Callback closure467functions/57555instructions/1788external records; provider
462/33606/202; eight core patches unchanged. These are static compile counts.

Next run original collision-neighborhood producer054020 with the now-populated
descriptor and compare selected records/radii against source data before accepting
movement. The prior empty-terrain update did not establish collision behavior.
Keep the normal bank-state producer as an explicit unresolved startup dependency.
Normal setup054900 and outer update/display ordering still need their own proof.

Private manifests and compiled-source snapshots:
224422/terrain-bank1-hypothesis.json SHA256
b4020a2b9da43ccff499214c543780b0360661a297baad02bd336e67a35a5e58 (177identities);
224651/terrain-return-contract.json SHA256
8e7035661ff726ac0bb8c8c742dd6b04da55927f1cd3fd6f16f2e40ecc94a904 (177identities);
224845/terrain-frame-contract.json SHA256
4406d01e47c2ef6ab3b0de3845682c976a8b5fc8d724a088335e484d0ba17c18 (190identities).
Latest binary SHA256
deeee05d9c2ea261a7c35d72d8030b97d75a2b1e1820f33110ae00be7c31969b.
Repository safety/diff checks pass; no Simulator/game app launch or publication.
215723 remains the last complete bounded regression; the goal remains active.


## 2026-10-05 — Collision neighborhood and geometry continuation,230120

Corrected the source interpretation:054020 filters an existing neighborhood;
054278 produces it and029062 consumes it to prepare collision geometry.225456
failed compilation because the capture used rt.ram instead of rt.main_ram;
no execution occurred.225607 then selected12 records and stopped at02D094.
225850 added original029000 and its pool helpers, verified192 object/8192 geometry
free chains, and stopped at028CA0.230120 adds six audited original helpers:
028CA0,02D1AE,028940,028806,02D108,02D160. The callback cap increases480→512;
unknown/RAW rejection stays active. Eight core patches unchanged.

The independent collision oracle verifies one-player sphere overlap at
(1645,-9,219), radius400: indices12,19,27,65,66,67,68,83,90,148,149,150.
All copied records and candidate flags/object links match. Minimum boundary
margin7.5977003 avoids a near-rounding ambiguity. Corrupt selected-record and
candidate-flag captures are rejected. This is static broadphase evidence only;
dynamic count0, triangle correctness and ground contact remain unverified.

230120 runs geometry construction and bucket joining, then fails explicitly at
6447C0, guestPC029480/PR029488/SP8CE3E9C4/SR60000000. Next audit that original
callee and continue preparation, preserving the return/full-update guards.
Do not skip the call or count a nonempty object list as verified collision.
50 terrain payloads/645616 bytes and315 source records/54 selected-pointer bounds
pass again. Normal bank1 production, task order, combined scene, movement,
cleanup/repetition and gameplay remain open.215723 remains latest full bounded
regression. No Simulator/game app launched; all listed runs are terminal.

Private230120/collision-preparation-contract.json SHA256
 afafd94a5d53450e8bb5c91b21ffe6184a8afd7dbdd61bbb819b33432a57c44e
binds176 identities and17 exact compiled-source snapshots. Callback closure480
functions/59092 instructions/1829 external records is static compilation scope,
not execution coverage. Captured RAM SHA256
2b5a41a9bf4ae2bbf34dee5153885dfe68380b5faeb6c2dd7a4b4385bbb43889.
Original G1 and R3/R4 remain unresolved; the goal stays active.


## 2026-10-05 — Collision geometry verified and original update resumed,231156

Progress classification: original preparation returns; an independent geometric
oracle replaces the earlier unverified nonempty-list observation.230729 adds
original6447C0 (diagnostic integer drawing) and reaches the intentional collision
return guard.231003 removes only that guard after geometry verification and
captures pre-update and failed-update RAM separately. Original09A544 reaches
029CA0.231156 compiles that audited query routine and proceeds to07AD36 at
caller0AB0BA/PR0AB0C4/SP8CE3EFB0/SR60000001; missing targets still fail explicitly.
The full-update inspection guard is retained, not reached.

All929 triangle vertices and material flags match unchanged source-model data
across12 selected objects. The new audit supports only this observed leaf-model,
translation-only, triangle-list scope and rejects unsupported data. It checks
finite centroids, original0.3333 centroid scale, squared bounding radii, exact
model centers/Z extents, ordered Z buckets, reverse links, cyclic/disjointness
constraints, and complete partition of192 object/8192 geometry nodes. Maximum
centroid error0.000186426 and squared-radius error0.000976200 are within declared
tolerances. Vertex/material comparison is bit exact. Six corrupt captures
(vertex, material, centroid, forward cycle, reverse link, object bound) are
rejected by the exact saved oracle. Geometry verification is not contact proof.

All three runs repeat the independent12-record neighborhood and50-texture checks.
Pre-update collision RAM is identical: SHA256
 e0c3003a60351727bc89944c633b905bdea1a29ffdefc1e60e71baeaa44f5fc4.
The update's query narrows candidates12→5 and nearby triangle count0→1. Player
state1→65, positionY bitsC1100000→C11147AE, velocityY bits0→BDA3D70A, countdown8→7.
These are captured observations only; no ground/contact or movement pass.
Normal startup bank1, task order, complete scene, full update and cleanup/repeat
remain open.215723 stays the last full bounded pass. Original G1 and R3/R4 remain
unresolved. No Simulator/game app launched; all sessions terminal exit1.

Next finish reviewing07AD36 (source available, direct calls07ABBE/07AAE0 and
63CDD0), compile its original continuation and retain the full-update guard.
Measure original collision response independently before accepting movement.
No no-op provider, injected triangles or runtime-core patch was added.

Exact private `collision-geometry-contract.json` manifests:

| Run | SHA256 | Bound identities |
|---|---|---|
|230729|e237fec8f4b99ff0c5e09701bdb1345c6d267f59f51b338b21d80df45eae2de7|174|
|231003|1685855e8435a17d52cc8d0752437ce2a7b3a7fa0c06e1a999817ab1d2630239|174|
|231156|71fd8b66cc4760eed88b1f163c8e35fab2404e26dbc048049d7682908ee0f208|175|

Each preserves17 exact compiled inputs and three external oracle snapshots.
Latest callback closure482 routines/60465 instructions/1854 external records is
static scope, not complete execution coverage. Cap512/unknown/RAW rejection and
eight core patches unchanged. Repository safety and whitespace checks pass.


## 2026-10-05 — Player callback return and verified collision misses,231711

Previous turn classified as progress.231508 compiles original07AD36; the player
callback returns and update09A544 proceeds to snowboard task0B4B66.231711 adds
that audited original task, reaching05A3CA at guestPC0B4BAE/PR0B4BC0/SP8CE3EFC4/
SR60000001. The source body for05A3CA is extracted locally but needs review next.
No stubs or core patch; all missing targets still fail. Full update has not
returned, and its inspection guard remains installed.

A read-only dispatch observer wraps029CA0 and records actual caller, sphere,
object/flags, return value, contact count and before/after query words. It restores
original dispatch on exit and caps observation at64 calls. Ten calls complete:
five objects in forward/reverse order, callers075006/07530A. All report0, contact
count0 and unchanged query input. Radius5, center(1645,-4.5999999,219), displacement
(0,-0.079999998,0) are directly observed, not inferred from a post-failure buffer.

The independent no-contact oracle uses distance to triangle interiors/edges,
with analytic face/edge/vertex/degenerate controls. A swept sphere is bounded by
radius r+|displacement| at its start. All ten queries clear all929 independently
verified static triangles: minimum distance74.8898058732, bound5.0799999982,
clearance69.8098058750. It verifies triangle vertices/materials remain unchanged
through the update and rejects intersecting queries pending another oracle.
Four negative controls reject a false hit, changed query, intersecting sphere,
and mutated mesh. This proves captured misses only; positive/ground contact,
full movement correctness and general transforms/dynamic terrain remain open.

Both new runs repeat independent12-record selection,929-triangle geometry and
50-texture byte checks. Exact pre-update collision RAM still matches230729.
Normal startup bank1/order, combined character/terrain scene, full update,
cleanup/repetition and gameplay remain open.215723 remains the latest complete
bounded regression. G1 Katana/PAL, R3/R4 and production adoption stay unresolved.
No Simulator/game app launched; both sessions terminal exit1.

Private `collision-query-contract.json` evidence:

| Run | SHA256 | Bound identities |
|---|---|---|
|231508|1ea53f2c571a54eef8cf66a8f3981428f52863f682c0ad6a59f94f98a978f1ab|168|
|231711|054cb73e378ce4eefc48164eddbe8e1c31be6461ed4c5a8146026e36685252b2|172|

Exact compiled inputs and applicable external oracle snapshots are preserved.
Latest callback closure484 routines/60877 instructions/1870 external records is
static compilation scope, not execution coverage. Cap512, unknown/RAW rejection,
eight core patches and private-input boundaries unchanged. Next review05A3CA,
compile its original continuation, and inspect the first changed dependency.


## 2026-10-05 — Snowboard continuation and auxiliary angles,232348

Previous turn was progress.232026 adds original05A3CA and stops at its direct
vector-rotation helper05AA74.232210 adds reviewed05AA74,05A9DC and0586A4; snowboard
motion returns, its display is culled, and auxiliary0B450E rejects07CB98.232348
adds reviewed07CB98 and its angle-step callee0579C0; execution reaches07CC2C at
PC0B4872/PR0B4878/SP8CE3EF94/SR60000001. That original helper is locally extracted;
review its body/indirect callees next. No full update return or cleanup/repetition.

The observed auxiliary angle change matches signed modular distance,4096 step
limit and original bounds: target-16384/current0→-4096, model yaw61440; the second
angle stays0. Input is captured immediately before the helper in232210 and
compared with the changed232348 capture. This checks these two observed values,
not all angle branches or general motion. `auxiliary-angle-audit.json` binds both
RAM identities and expected values.

Snowboard integration in232210 matches captured gravity0.019999999552965164 and
original caller offset-5.21999979019165. Expected/actual velocityY is
-0.019999999552965164 and positionY-0.019999980926513672 after float32 offset/add/
subtract rounding. Other observed position/velocity components remain0. The
constructor left the board at fixture origin(0,0,0); normal placement is unproved.
The collision neighborhood is player-local, so an empty snowboard query is not
accepted as complete scene collision coverage. No positive contact proof.

All three runs repeat12-record selection,929 source-model triangles, ten captured
player misses and50 texture byte checks. Explicit bank1, normal startup/order,
combined scene, complete update, ground contact and gameplay remain open.
215723 remains latest full bounded regression. Original G1 and R3/R4 remain
unresolved; no production-foundation decision. Eight core patches unchanged.
All three sessions terminal exit1; no Simulator or SonicPad app launched.

Private `auxiliary-update-contract.json` manifests:

| Run | SHA256 | Bound identities |
|---|---|---|
|232026|0c2c4cdb71703c4a423fa15edfba99056d0f852a5323f168dd1d4f96dcbc8781|173|
|232210|8f95d6150c0d977463828069efe6757cfea96696323d13c44e8e1e50cb5f3d76|174|
|232348|16448147e2ad54d9aa1f6192d5e3678d4284f77e8a94a4063fb39bf379e10565|175|

Exact compiled-source and external oracle snapshots are retained. Latest callback
closure490 functions/61748 instructions/1881 external records is static scope,
not full execution coverage. Cap512 and unknown/RAW rejection remain active.
Next review07CC2C's indirect targets and continue without bypassing its calls.


## 2026-10-05 — First original full-update return,233831

The preceding status-only turn was no progress; execution resumed from the actual
terminal232821 checkpoint. Two previously undocumented continuations are preserved:
232658 adds07CC2C and returns from auxiliary0B450E;232821 adds0B573A and reaches
0B5B4C after effect activation0→1/timer0→40. Its node list remains empty.

Five changed experiments completed, all terminal exit1:

| Run | Added original routines | Terminal boundary |
|---|---|---|
|233220|0B5B4C,609368,0B50A0,0B500E|Effects return with timer41; stage children return; missing048BC8 in0B3F96.|
|233344|048BC8|Captured provider pointer19FDF0 is0, so original absence path returns; next057A4C.|
|233527|057A4C,10EC28,1101B0|Original inverse-cosine wrappers execute; missing cross-product63D890.|
|233701|63D890|Secondary-motion task returns; next attachment07CDA6.|
|233831|07CDA6,04D948|Full09A544 returns; intentional inspection guard stops execution.|

All routines came from original local input; no no-op providers, forced pointers,
new core patches or host JIT. Latest callback compilation has503 functions,
63364 instruction occurrences and1904 external records. This is static scope,
not general execution coverage; cap512 and unknown/RAW rejection remain active.

The secondary-motion path deserves a narrow claim.048BC8 leaves the caller's
zero vector intact when global19FDF0 is0. Original63E68C normalizes it, producing
NaNs in temporary stack coordinates. Candidate FTRC's explicit NaN result is
80000000; original caller adds1000/masksFFFF. The captured retained motion
magnitudes and model angles finish zero, and phase counters finish4096. The
private `233701/secondary-motion-audit.json` binds the before/after RAM and source;
SHA256 ef09339a426278b8eef8cf2947af22c3d8bce7c4867df0268c3715f18e732f91.
This does not validate all exceptional FPU behavior, nonzero motion or the model
vertex transformation branch (which is not selected in this fixture).

At233831, original09A544 calls group selector8 (mapped by09A42C to group7), then
0–6. The new read-only `audit-recovery-update-state.py` checks all13 root tasks'
next/previous chains and callbacks. Two01B4D8 tasks invoke09A426, which changes
their update callbacks to09A2FC for later destruction; they have not been freed.
The current callback clears, countdown8→7, and player position
(1645,-9.079999923706055,219) agrees with float32 addition of previous position
and retained velocity(0,-0.07999999821186066,0). This is integration consistency,
not an independent proof of all player physics/state transitions.

Effects are active with timer41 and no nodes. Attachment07CDA6 performs state0→1,
installs its work pointer at characterwork+112, and retains zero interpolation
and the original model link. Four independent corruptions are rejected: broken
previous link, lost deferred-destruction callback, nonfinite position and timer8.
All12 terrain selections,929 source-model triangles, ten conservative player
collision misses and50 exact texture transfers pass on every new capture.

Private `effect-update-contract.json` snapshots:

| Run | SHA256 | Identities |
|---|---|---|
|232658|2e9a01c6f4c7a96590b4bf51405bfd9369065756454e5cada4e17ff751558543|241|
|232821|8761b77070de3cc0ae0f6c263a782a729a61223d272d5d0a5216423cc5fa6029|241|
|233220|7cf7c5e6a921f5810d3431b622dfb325a108585076cdbdbe605077f45e777432|242|
|233344|8584740f53e8967095c5e9ef72d93f6fc392011c210e811e2d682321da8c79fb|242|
|233527|26c43ddfbfb105d478b182918d6e602f52a4a07c848f791f905064bbf155f22d|242|
|233701|d8ee9889137ba09fa1f553b5a80b41e6deddb2f1048dfa3443a8abad03630ee3|242|
|233831|d78dcde300d96f000b5e3fed73fe7bb7cd012409ec9760f18eada98b92b95d8d|242|

The final `233831/update-return-contract.json` additionally binds state audits,
negative controls, secondary-motion evidence and relevant disassembly:260 verified
identities; SHA256133b6cfa197a730c09aa096cd6432503dfc9e184eca383c621b0b1350e8c98ea.
Exact compiled-source and oracle snapshots are retained. The provider still fails
intentionally;215723 remains latest full bounded regression. No Simulator or
SonicPad app launched; all sessions are terminal.

Next replace the inspection guard with the verified bounded state assertions and
exercise original cleanup/repetition. Resolve any changed resource/model invariant,
then execute original09A564 for combined display. Normal bank1/startup producer,
positive contact, normal board placement, full scene/gameplay, G1 Katana/PAL,
R3/R4 and production adoption remain unresolved. Do not infer those from a single
update return or the separate pre-update terrain image.


## 2026-10-05 — Two original update/display/frame/release cases,235343

The previous turn was progress (233831 update return plus state audit). This
continuation integrates the checked root/callback/timer/finite-integration/effect/
attachment assertions and proceeds through cleanup. Five changed runs completed:

| Run | Changed cause | Terminal result |
|---|---|---|
|234441|Integrated update assertions; original terrain053A98/05390E release; compare destructor VRAM against current live textures.|All108 texture records free and VRAM reset; old whole-module check rejects retained texture metadata.|
|234721|Verify exactly50 permanent index/pointer pairs and released records; retain all other byte checks; capture released module.|First cleanup passes; second startup rejects obsolete virgin-collision-pool assumption.12,453 queued packets also observed after first cleanup.|
|235028|Original029000 resets each supplied startup; add09A564/09A4A0/09A4E2 and complete frame before release; separate per-case captures.|Full update passes; display executes camera then rejects missing terrain wrapper053B60.|
|235208|Compile audited053B60; require two update/display/release cases and six frame continuations in runner.|Terrain and Sonic display return; missing snowboard display0B4C88.|
|235343|Compile audited0B4C88, retaining original culling/animation branches.|Terminal exit0; both supplied update/display/frame/release cases pass.|

Terrain053720 installs053A98, which invokes05390E to release the stage texture
list and053776 to clear terrain globals. All50 terrain, one board and57 character/
effect records free through original routines; graphics reservations remain
until original reset restores both4 MiB banks. The checks compare against VRAM
with terrain textures already uploaded; the earlier pre-terrain snapshot was not
a valid byte-preservation baseline for destruction. Original09B3F0 copies index
and registration pointer into each12-byte permanent list entry;60C5FC frees the
record without clearing these words. The byte audit checks exact values, not an
ignored region. A separate private234721/released-module-audit.json reconstructs
all508012 bytes from source, PVM indices, known original counters and stage-child
pointers; zero unexplained differences. Stage globals7C030/7C034 denote the stage
task and effect-child work respectively, not stage-task work.

Finishing the original display/frame while textures are live fixes the observed
queued-work lifecycle boundary: both235343 cleanups report queued_packets=0.
Six total original frame completions cover separate character, terrain and joined
renders in each case. Each frame returns through five interrupts/14 saved-PC
resumes with stack/PR/preserved GPR checks. Original collision reset is explicit
supplied-startup behavior; it is not proof of full original scene teardown.

The runtime harness now exits0, but the image boundary stays open. Inspected
scene3/6 show textured terrain and Sonic clipped at the top; no complete camera,
normal spawn, ground contact or playable view. Terrain-only2/5 images are exact.
Character-only1/4 differ in960 pixels at x452–490/y0–37. Joined3/6 differ in475
pixels at x457–487/y0–37. Relative to terrain-only, joined3 adds/changes1495 pixels,
joined6 changes1389; both bounds x452–490/y0–59. Scene4 present equals scene3 render,
while scene6 present equals scene5 render, consistent with the observed previous-
render buffer handoff. No UI or continuous in-game presentation acceptance.

| Capture | SHA256 | Nonblack pixels |
|---|---|---|
|scene-3-render.ppm|e6f136a016badc24a97859e66a18b336f2b8d1b31805223b68c9d6339b545ae9|291583|
|scene-6-render.ppm|7f65cf863b6a24b3a377db1c58227e122dd387dc92b798a2fdb692a659509417|291582|

Resident model1D1B20 yaw is0 before the first update,61440 afterward; it is already
61440 before the second update and stays there afterward. Other two inspected
auxiliary model angles remain0. `repeat-model-observation.json` binds both cases'
RAM. This identifies carryover to investigate, not a complete causal explanation
of image changes. Do not restore arbitrary model bytes or bless a new image hash.
Trace the original producer/reset and ordering relative to pose generation next.

Both archived startup cases independently pass12-record terrain selection,
929-triangle geometry, ten conservative no-contact queries,50 exact texture
transfers and bounded retained update state. Root aliases contain only the last
case's query/RAM; queries are truncated per case, and each completed case is
archived before reuse. Full task-tree destruction, general exceptional FPU
semantics, arbitrary motion/contact and normal boot remain open.

Private `effect-update-contract.json` snapshots:

| Run | SHA256 | Identities |
|---|---|---|
|234441|616667cb2ce142c0c8caea2fc2435d7bfa612e59e667e3fae48d5256bfc32627|242|
|234721|050124d9806261e0afcfd822c47163f57c506f1a769fade1793c69f847de6c3a|251|
|235028|8c6a306844ece2891971aa9225270c58a44bae72abf2bdf880285d8b0a5d5df3|243|
|235208|27f78942a744ddee25c239e04a65b0cc392ec3c86592b13b10d52611c23d2fd4|243|
|235343|3ec442b348e126ce3a4ec94c8e6e3fb403bc1d66bad7a00cb018ce0502437164|263|

These earlier snapshot containers retain conservative overall_pass:false scope.
Final `235343/update-display-contract.json` records runtime_harness_passed:true,
independent per-case audits true and repeat_images_identical:false;311 verified
identities, SHA25611bf63b117ae00f424b5221c2fc3bdcec3fa9158937559fde1c2977bccd2194b.
510 callback functions/63763 instruction occurrences/1923 external records are
static compilation scope, not execution coverage. Cap512/unknown rejection and
eight core patches unchanged.235343 is the latest expanded passing runtime
harness;215723 remains historical narrower evidence. All five sessions terminal;
no Simulator/SonicPad app launched. G1 Katana/PAL, R3/R4 and production adoption
remain unresolved. Source-only acquisition request remains BOOTSTRAP-BLOCKERS.md.


## 2026-10-06 —000204 original angle release fixes repeat rendering

Previous status-only turn was no progress; revalidated235343's311 identities
before continuing R3/C8. Root main/a343a2d and dirty work preserved. Process-local
CLT/ARM64/macOS13 lane, same owner1.004CDI and eight source patches. No saves or
Simulator launched. Simulator enumeration fails on the existing Xcode license.

Hypothesis: the omitted installed auxiliary destructor causes yaw carryover.
0B450E phase0 copies three angle triples into work+100..132 and installs0B44B0.
The47-instruction destructor restores model offsets20/24/28 through work pointers
88/92/96, with no calls or allocation. Added that original routine to the selected
AOT seeds and invoked its installed pointer before task work/resource release.
The harness asserts all nine values and unchanged VRAM; no host angle writes.

Canonical provider command from BUILDING/RECOVERY-PRS with --sboard-camera and
--startup-bank1 exits0 at docs/artifacts/recovery-aot/20261006-000204. Both cases
pass existing checks and all five external audits. New model-release audit proves
saved/restored angles agree with original BOOT bytes; stale-yaw corruptions fail
in both cases. Character1/4,terrain2/5,combined3/6 now match exactly and match the
first235343 images. The only observed changed angle is1D1B38:0→61440→0.
This establishes the cause for the supplied repetition, not arbitrary animation.

511 callback functions/63810 instruction occurrences/1923 external records;
cap512/unknown rejection retained. Contract model-release-contract.json SHA256
8efe2a54e7a61b34c62a544d1d8e58fce4f47f5cb8a9ab1e5b0fbae71564773f,
295 identities including exact compiled-source/oracle snapshots. Safety and
diff checks pass. Run terminal; no owned runtime remains.

Next: extend to a second consecutive update/display/frame. Original050474 wraps
09A544 with additional work and calls029062 later; inspect/retain intervening
contracts rather than label bare scheduler repetition the whole game loop.
Camera clipping, normal bank1/startup, positive contact, full task-tree teardown,
G1/PAL and production adoption remain unresolved. No publication or downloads.


## 2026-10-06 —001309 original outer-loop continuation

Previous turn made progress:000204 corrected original auxiliary cleanup and
render repeatability. Reverified its295 identities. main/a343a2d dirty work and
protected inputs preserved; same ownerCDI/CLT/ARM64/macOS13, eight core patches.
No Simulator/game app or save profile launched. Full Xcode license remains the
Simulator enumeration boundary; initial free storage12GiB.

Hypothesis: original050474 supplies the work omitted by repeated bare task
updates. Source inspection found null coroutine-current global7A4C88 in the
fixture. Original09B620 clears its36-byte static root7A4C8C, self-links the first
two words and publishes it. Added this original producer and an opt-in
--startup-loop provider experiment after the first completed frame. Captures
loop-before/update/display and loop-collision-queries stay separate from the
first-tick evidence. An intentional inspection guard remains after any eventual
second frame return; passing unexamined output cannot silently green the run.

Three changed runs use the canonical provider command plus --startup-loop:

| Run | Terminal boundary | outer-loop-contract SHA256 |
|---|---|---|
|000823|root passes; controller helper6055B4 uncompiled|16db8ee3d2d70fe374673e499df8e78a33faaa64bad3738788f56f8f284ad8ce|
|001019|controller/coroutine prefix returns; second Sonic update needs0B2AB4|b4fec96ba3758e8738da83690c43cb976ea41a962286dd547332522685054edb|
|001309|second Sonic update advances position; effects need10EF54|88ee71ca642750e20b60609b8c55c95b162b4cba6d480eb61435e87434e70bf0|

Each contract binds255 identities and passing first-tick external audits. Sources
are snapshotted before edits.6055B4 is an original three-instruction tail jump to
already-linked605C9A;0B2AB4 follows the original no-contact flag branch. No result,
controller snapshot or character state is forced. Callback limit rose512→576
for the measured523-function outer closure; latest538functions/67056instructions/
1982external records are static scope, not coverage. Unknown/RAW guards remain.

001309 partial second update: y=-9.079999923706055→-9.2391996383667,
vy=-0.15919999778270721. First update countdown7 becomes6 during original display
because the timer installs0A2B38 at both callback offsets16/20; second update then
changes6→5. This is not evidence of a duplicated coroutine continuation. Full
coroutine switching and completion remain unproved.

Reduced next boundary:10EF54 (PC0B57D6,PR0B57DC,SP8CE3EF34) is the locally
available16-instruction LCG. It reads seed5FF840, computes modulo2^32
seed*41C64E6D+3039, writes it back, returns(next>>16)&7FFF and preserves MACL.
Independent expected controls for0,1,FFFFFFFF and live seed are recorded but not
executed. Validate this arithmetic in isolation before another full continuation.

The exact expanded binary302f793ab172f4e9b1741956f1bd1351fa43371d1860c06e8b25c88c61d74748
also ran without SONICPAD_RECOVERY_OUTER_LOOP into an isolated baseline-regression
capture directory; exit0. Both cases pass existing runtime assertions, all five
external audits, angle-restoration corruption controls and exact repeat images.
All six render captures match000204. This is a cached native regression, not a
second wrapper build or acceptance of the failing outer-loop experiment.

Final001309/outer-loop-checkpoint.json SHA256
88ba2eb37b7b0316576c9136f08fe12d39ddb10de0839b838e2604d5c04f3eac,
336 identities. Source/oracle snapshots, old failures and baseline outputs remain
private. All sessions terminal. G1/PAL, original startup/bank1, consecutive frames,
camera clipping/contact, full teardown, gameplay and production adoption stay open.


## 2026-10-06 —002919 original RNG and collision lifecycle

The32-draw isolated original10EF54 test covers0,1,80000000,FFFFFFFF; checks
modulo2^32 arithmetic, return values, MACL/stack/GPR preservation and exact RAM
writes. It does not mutate live state. Live DEAD0CAB is written by original
056242 via10EFBC. The second update advances it toC0CCAF08; no effect spawns.

Changed attempts:001908 compile failure (const read API),002037 RNG passes then
missing033774;002241 missing disc-map argument (no build);002249 missing03353E;
002431 missing03011E;002715 compile failure (out-of-scope descriptor);002919
both outer updates return, outer display rejects08C15A. Failed source snapshots
and logs are preserved. No unchanged repeat of the same failing command.

002431 counterevidence: Sonic's collision record accumulated1→2→3 registrations
across supplied partial updates; original collision paired Sonic with itself.
This is not positive contact or evidence that03011E must be added. Corrected the
opt-in fixture by completing prefix034480→02F9BA, initializing the original
coroutine root, then using050474 for each following update. No direct collision
counter reset, deduplication, forced state or no-op callback. Baseline flag-off
behavior is retained. All nine counters are zero after prefix and both updates.

002919 second outer update returns y=-9.2391996383667,vy=-0.15919999778270721.
Snapshot audit verifies root links and removal of the two deferred group6 tasks
(13→11 roots), one RNG advancement and no spawned effect. Heap/free-list integrity
and actual positive contact are not established. Original outer display executes
its task display then stops08C15A,PC050906/PR05090C/SP8CE3EFE8. No second completed
frame is claimed. Source08C15A is37 instructions; inspect live branch inputs and
compile the original entry next, keeping the explicit next-target failure.

Callback closure547functions/68385instructions/1999external records; bound576
and eight core patches unchanged. The exact binary's separate cached baseline
exits0; five independent audits pass for each startup, original angle-release
and stale-yaw negative controls pass, and all six images match000204 exactly.
First-tick experiment audits also pass. Sonic remains clipped.

Final002919/outer-loop-checkpoint.json SHA256
29d16c202e836185618e8771051d14a63176e257d97d8fd6f34d7c29d740b307,
339 bound identities. All runs terminal, no Simulator/game app running; disk11GiB.
Original G1/Katana/PAL, normal startup/bank1, R3/R4, consecutive rendered frames,
camera/contact, full teardown, gameplay and production adoption remain open.


## 2026-10-06 —004452 original HUD load and sprite selection

Previous goal turn classified as progress:002919 closed RNG arithmetic and prefix
collision ordering, with339 identities reverified before editing. Same dirty
main/a343a2d, private owner1.004 dump, CLT/macOS13 ARM64 and eight core patches.
No game app/Simulator launched; original G1 source/PAL gaps remain unresolved.

08C15A's captured flags are0,stage38,character0. Its original checks select08BF62
and sprite11.003802 compiles those original entries, then rejects60A7C8. Inspecting
that texture selector before adding it revealed empty HUD records: the24-entry
HUD list1C2EDC shares array1C6DA8 with the56-entry list1C7048. Original09AD7E reads
its CON_REGULAR row at1C6198/1C619C; name15C244/list1C7048. The owner dump contains
SONICADV/CON_REGULAR.PRS,extent79900,size27268. Independent PRS decode produces
169664 bytes,SHA2566645bbddc831f0f9c02b931d5421858d655056e9e3f2f24cb738b3b99b5f9640.
No download or guest texture pointer fabrication was required.

The opt-in fixture calls original09B3F0 with that row's exact arguments before
outer display and captures RAM/VRAM on either return or failure. This is a bounded
missing-producer experiment, not the entire09AD7E common startup or normal order.
004203 completes the14-sector load but fails an overly strict unique-slot
assertion. Original archive entries12/13/16 all contain ID3B7 and identical
payloads; the original loader correctly shares slot120 with reference count3.
Corrected the assertion from that source evidence. No game behavior was changed.

004452 passes56 links/54 registrations. The new independent HUD audit verifies
all162016 unique payload bytes, source dimensions/sizes/IDs, duplicate identity
and reference counts, unchanged prior108 records and all outside VRAM. Negative
controls reject a changed link and payload byte. It passes both004203 and004452.
Allocator policy and sprite rasterization are explicitly outside this audit.

Original60A7C8 selects sprite11,record8C766990,ID3A3. Captured cache/header fields
match that actual record. Display next rejects60A748 atPC642C54/PR642C5A/
SP8CE3EFA4,R4=02100028. This41-instruction routine has no callees. Reduced next
check: input header808C000E,C8800000,940984DB,100BD9F4; flags36C; mode destination
893F38. Source masks predict828C0028,C8800000,941184DB,100BD9F4 and mode02100028.
That next-routine oracle is prepared, not executed. Validate it in isolation
before another full continuation, following the three-changed-attempt reduction.

All three runs terminal exit1 with explicit boundaries; no second rendered frame.
First-tick audits remain green. Exact004452 binary's cached flag-off regression
exits0: both startup cases, five external audits each, angle-release corruption
controls and all six render images match000204. Callback scope554functions/
68733instructions/2005external records; cap576 and unknown/RAW rejection retained.

Final004452/hud-checkpoint.json SHA256
7de672a9eb96dab8d9d8355a42b18715d323b0490def8c763e20bb917a54e58d,
361 verified identities. Historical contracts:003802 d8010f5c18124e5e62757d19d1fb6120683e9ef48eda4c6e681de1429497bac1;
004203 13ff5ebe227e0cb29f7030e2b9784cd7c9317cc231e0d54511288e25c3a8937c;
004452 0ed0eb89ce5263cbca76a7ed9fca3b0aab279e827b9b8492da8fd6d6336ddb52.
Source snapshots, failed assertions, source reviews and private archive references
are retained. Latest disk13GiB, no runtime/Simulator/game app running. G1/PAL,
R3/R4, normal startup/bank1, camera/contact, full cleanup, continuous gameplay and
production adoption remain open. No publication or source acquisition claim.


## 2026-10-06 —005752 shared HUD packets and corrected running-game route

Previous turn is progress:004452 HUD checkpoint361 identities reverified. Dirty
main/a343a2d and protected inputs retained; native CLT/macOS13 ARM64, original
owner1.004 dump and eight core patches unchanged. No app/Simulator launched.

004952: added original60A748 and a fresh-runtime check covering40 combinations
of mode/flags/header bits. All flag branches, captured HUD case, exact RAM writes,
stack/argument/MAC/GPR preservation pass without modifying live RAM. Live header
828C0028,C8800000,941184DB,100BD9F4 and mode02100028 match the precomputed oracle.
Display then rejects60A2F0. This is original code, not a replacement header writer.

005138: original60A2F0 and643578 execute and return. Shared HUD sprite11 packet
matches its source descriptor:64x64 at(16,32), unit scale, UV0–1, reciprocal depth
0.5. One32-byte header and four28-byte vertex contents are checked; packet padding,
DMA completion and rasterization are outside scope. Two corruptions (header and
vertex) are rejected. Next call08BBC0 formats the HUD number; this is not an
unavailable source pack. HUD archive bytes/registration audits still pass.

A caller audit then corrected a substantive fixture error:0508DE contains
09A564 plus014EEE/014DCA pause/menu drawing, and04EBBA handles resume/selection
following it. Normal0506BA calls050474 according to758F44, then08C15A and08C3C4;
it does not append0508DE. Prior two-outer-update returns and shared HUD proofs
remain bounded valid observations, but the paired experiment was not normal
frame sequencing. Earlier documentation is superseded by this correction.

0503C0 supplies(2,2) to original0534C0. The corrected opt-in continuation loads
HUD resources after the retained supplied first-frame prefix, calls this cadence
producer, checks its outputs, then invokes0506BA with device clock enabled. No
counter is host-written and no pause path is invoked. First-tick/flag-off fixture
behavior remains unchanged and is still explicitly not normal boot.

005752: cadence2/2/2/0 passes. Original608F74/64968C append callback053570 at
handle11 to event0900's existing two-node chain. Registration words are
053570,0,0,80000058; existing callbacks649D64 and603A30 remain linked.0506BA
completes one update, then053556 waits at05355A for758F40<=1. Counter remains2
and758F48 becomes1.128 continuations comprise64 interrupt entries at000600 and
64 saved-PC resumes at05355A; interrupt event0320. The guard terminates the run.
No normal-wrapper return or completed second rendered frame is established.

Next reduced investigation: record device-clock cycles, PVR ticks/scanline/vblank,
Holly pending/acknowledgments and callback-event routing at053556 entry and
continuation exhaustion. The current captures lack those final clock metrics.
Determine whether the guard is insufficient or event delivery is wrong before
changing limits. Do not force the decrement, omit the wait or disable interrupts.

Both native syntax checks pass. Three changed runs terminal exit1 with distinct
measured boundaries. Latest565functions/69634instructions/2009external records;
cap576, unknown/RAW rejection and core patches unchanged. Cached flag-off005752
binary exits0: two baselines, all five audits each, model-release corruption
controls,40 drawing-header controls per case and all six images match000204.
Latest HUD load passes162016 exact unique bytes,54 registrations/56 links and
two corruption controls. No HUD rasterization or playability acceptance follows.

Contracts:004952 09077d51b5743d8d39b5a3971f03b4a2c69890ba2e1c8091c0824fa80784f4a8;
005138 4785f5365ba850223300327d98dc496c71e323d114757b0f321d96228b873f13;
005752 586b01d544bdba0d41746d15870b12ea81fdb52b2903bd8741590bf897474091.
Final005752/cadence-checkpoint.json SHA256
0aac9f9028982f17fd2414b89d8ee67158f0d590de0835ef230f723159620a26,
371 verified identities. Paused/running source review, snapshots and prior
failures retained privately. Repository safety/diff checks pass; no active
runtime/Simulator/game app; disk13GiB. G1/PAL, normal startup/bank1, R3/R4,
correct camera/contact, full cleanup, gameplay and foundation adoption remain open.


## 2026-10-06 — 011538 clock diagnosis and duplicate initializer repair

Previous turn was status-only (no progress). Revalidated authority/current sources,
then executed the pending R3/C8 experiment. Dirty main/a343a2d preserved. G1/PAL
unchanged; no acquisition retry, Simulator, game app, publication or core patch.

010621 adds observation at053556 entry and each async continuation.65 vblank
clock ticks/IRQ entries (64 handlers completed),215955541 guest cycles, counter2
unchanged.13 RAM/VRAM captures exactly match005752. The enabled source is bit8;
original655900 acknowledges it. Original656294 calls empty default6453DC.
SPG fields are cached last-read values; no continuous scanline claim. Two flag-off
baselines pass with the old reset still present, including five audits each,
model-release controls and six unchanged images. That pass is historical only.

011146 observes the cause directly: before654DE4, work8A6FD4's callback at+238
is64985E; afterward it is6453DC. Event0900 root896B10 is unchanged. Original
654DE4 calls644EA0, which reinstalls default handlers.64968C only installs the
chain dispatcher when its chain is empty; later cadence registration merely
appends a node. All13 captures match010621; this was an observation-only run.

Removed the redundant initializer from RECOVERY_SBOARD_CAMERA, retaining the
already initialized work area and asserting its dispatcher. Non-camera provider
initialization remains unchanged. No callback pointer is host-written.011335's
first frame returns but the oracle rejects final event0360. Source/runtime shows
Maple DMA bit12 and IRQ4 masks; this activity had been suppressed by the reset.

011538 replaces the last-event0320 assumption with validation at each vector
entry: IRQ6 must have an enabled pending source; IRQ4 must have enabled Maple
bit1000 with no enabled IRQ6 source. VBR, stack, PR, preserved registers and
async-return checks remain. Five IRQ6 and one Maple IRQ4 entry pass; first frame
returns after17 continuations. One render completes; this is not gameplay.
Next terrain-frame call608DF0 rejects uncompiled original608DDA at608E3E. The
flag-off exact-binary baseline reproduces the same failure, so baseline acceptance
is explicitly reopened. No full-update audit or six-image pass for this repair.

Next: compile the11-instruction608DDA wrapper, inspect global6771B4 (optional
original callback), restore full baseline and rerun cadence. Its source is local;
no new download is required. Do not restore the reset, force timing/callbacks,
ignore the new event, or claim the cadence wait was retested.

Four changed runs terminal;565 functions/69634 instructions/2009 external records,
cap576 and eight core patches unchanged. Native builds, repository safety and
diff checks pass; no runtime/Simulator/game process;11GiB free. All missing-input,
normal-startup/bank1, camera/contact, cleanup, R3/R4 and adoption gates stay open.

011146 clock-reset-checkpoint.json SHA256
19874b7e667a92b33379fed520816c265e8f0bfb24cc15896db6e81008064502 (356 identities).
011538 callback-repair-checkpoint.json SHA256
ee9d8729f92a109dece43487c2781d447a7dce5e6affacefa15df5abf0745988 (962 identities).
Source snapshots, all failed runs, source-derived callback routing, clock audit,
reset audit and baseline failure are preserved privately.


## 2026-10-06 — 012816 baseline restored and cadence callback delivered

Previous turn was progress: duplicate work-area initialization repaired, precise
baseline failure preserved. Revalidated current PRD/goal/recovery authority,
private dirty main/a343a2d, available disk and no running app/Simulator. G1
acquisition lane unchanged; no new lead, download, source switch or publication.

011955 compiles original11-instruction608DDA. Optional callback6771B4 is zero;
original null branch returns. Active controller polling next rejects605AE6.
012149 adds605AE6,64A8B2,64AB9A,605AC4; reaches64A882. Source review reduces the
next route to that bit-field descriptor reader and663198 (case5 configuration).
012325 adds both; polling reaches original605930, the actual controller decoder.
These are different original dependencies reached with live callback delivery,
not attempts to hide a source-pack failure or patch guest timing.

Reduced605930 to a fresh-runtime contract before continuing startup.168 cases
span seven trigger values around32/64 thresholds and signed-byte boundaries,
four previous latch states, three capability masks and two button polarities.
Independent expected button/trigger/axis values, exact16MiB RAM writes, stack,
return and preserved GPRs are checked. Unsupported axes return zero; one-stick
capabilities exclude the second pair. This tests the original decoder, not the
Apple touch/controller bridge or physical input. No live globals are modified.

012634 passes both sets of168 isolated cases and the first full startup's update,
display and release. Its five external audits pass; three images match000204.
The second startup reaches device-status query64A876→66314E; existing6630D0
supplies its device record.012816 adds both original routines and passes the
full canonical flag-off run. Both startup cases pass all five external audits;
model-angle release and corruption controls pass; all six images match000204.
All six frame callback observations are zero. Both neutral input captures show
capabilities000F06FE/buttons0/analog0; no physical interaction is claimed.

Reused the exact012816 binary for a separate `loop-probe` with the outer-loop
flag enabled. Original cadence producer registers053570; running wrapper reaches
wait053556. At clock tick1/cycle3336409, IRQ0320 enters and original64985E traverses
event0900's chain to053570 at649872. It rejects that uncompiled target explicitly.
This establishes delivery on the first tick, unlike the former65-tick default
callback loop. Counter remains2 because the callback body has not executed.
No wrapper return or completed consecutive frame is accepted. Five loop first-tick
audits and the independent HUD payload/negative controls pass.

Next: compile053570 and inspect6069F8/09EA60 plus their producer state, then measure
counter decrement and original interrupt return.575 functions approach576 bound;
any increase must follow the measured required closure. Keep all unknown/RAW and
missing-target guards, original source semantics, and counter/callback ownership.

Five changed runs and cached loop probe terminal. Latest575 functions/69977
instructions/2016 external records, eight core patches unchanged. Native syntax
check/builds pass; final flag-off runtime/audits pass, loop intentionally fails at
the named missing routine. Safety and diff checks pass; no runtime/Simulator/game
process;11GiB free. G1/PAL, normal startup/bank1, camera/contact, cleanup, R3/R4,
consecutive gameplay and production adoption remain open.

Private012816/input-cadence-checkpoint.json SHA256
1c3234c2e02e7fa45125b042a7eadab2f73c9a6d21bb6ffc210532d1d3c529d2;
431 verified identities bind sources, inputs, patches, generated code, exact
binary, baselines/audits/images, loop output and earlier failures. Original
callback-reset checkpoint reverified; no protected material deleted.

## 2026-10-06 — 013526 original cadence return and isolated VMU baseline

**Latest recovery checkpoint — 20261006-013526: cadence callback returns.**
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

## 2026-10-06 — 015039 original endframe and two continuous supplied cycles

**Latest recovery checkpoint — 20261006-015039: two supplied running cycles return.**
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

Run sequence and changed causes:

| Run | Change | Result |
|---|---|---|
|014015|Compile096540.|38 inactive records match494 independently predicted words; two updates reach missing HUD digits08BBC0.|
|014208|Compile08BBC0 and unconditional wrapper HUD08C3C4.|Both HUD calls return; old one-interrupt assertion rejects a valid graphics+Maple pair.|
|014427|Count validated wrapper vector entries; compile zero-counter helper647CD8.|Wrapper and standalone608DF0 return; visible HUD; baseline and six image checks pass.|
|014758|Compile0534F0/057892; use original endframe and keep clock enabled.|Countdown reaches0 and reloads2; endframe returns at10026427 cycles/tick3, counter1. Render matches014427; baseline passes.|
|015039|Repeat the same original wrapper/endframe pair twice without reset.|Second endframe returns at16699152 cycles/tick5; timer4→2; second rendered HUD00:00:08. Baseline and independent audits pass.|

The first version of the cycle audit incorrectly expected eleven roots before
the first cycle. Source09A2FC and captures show thirteen initial roots, including
two already scheduled for deletion, then eleven retained roots. The corrected
oracle requires exactly those two removals and cleared callbacks; guest execution
was not changed to accommodate the test.

The historical first-HUD packet audit rejects014015 because its fixed offset
F6DD80 contains older geometry. The first matching header is nowF69060, followed
by live packet pointerACF69100. Preserve the rejection; a byte-pattern search is
not a replacement passing packet oracle. Current HUD payload audit and actual
render captures provide their separate, narrower evidence.

Clock source review proves dc_device_clock_enable resets guest cycles and SPG
state even when enabling. The optional loop now enables once for both cycles;
per-call baseline clock behavior is unchanged.0556A0 calls0534F0 at0556FA. Source
0558C0's mode table maps0→05593C,3→055AB4,4→055AE6,5→055BC8,9→055CFA;
mode4 calls04E6C0. Original startup/mode transitions are the next causal gap,
not a reason to force mode4 in the fixture.
