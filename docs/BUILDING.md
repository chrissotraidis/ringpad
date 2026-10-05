# SonicPad current build and input checks

Updated: **5 October 2026**. Private-only. There is no game/core app build recipe
yet because the required Katana/generated source and supported disc are absent.

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
