# SonicPad status

Updated: **5 October 2026, Astra audit**. Private-only; publication not approved.
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

Highest observed behavior: native macOS Foundation probe and input logic tests.
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

Next external input: an accessible authenticated Katana/runtime/generated-source
package or generator repository/path, plus the owner's supported PAL dump.
The exact source-only request is in [BOOTSTRAP-BLOCKERS.md](BOOTSTRAP-BLOCKERS.md).
For full Xcode/iOS tools Chris must accept its newer license once:
`sudo xcodebuild -license accept`. macOS work does not require that step.
