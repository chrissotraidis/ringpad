# SonicPad journal

Append-only. One entry per bounded goal step, in the order the work happened.
Newest entries are appended at the bottom. Every entry uses the Section 8
template from [`goal-loop.md`](goal-loop.md). Records what was actually
observed, including failures and blocked steps.

---

## 2026-10-04 21:10 CDT — G0/G1 bounded source-free work: Dreamcast input contract

```text
Date/time and timezone:  2026-10-04 21:10 CDT
Goal / current matrix row: G0 prerequisite work + G1-independent bounded step; matrix row 20 (input-state contract)
Root revision + dirty patch identity: 3127eb6 ("Add files via upload"); dirty: new port/, tests/, scripts/, config/, .gitignore; root prd.md/goal-loop.md moved into docs/
Source pack / dependency lock / SDK / target: SARecomp pin d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2 in ref/sarecomp (public clone, push DISABLED; insufficient alone for a build); no r354 bundle present; host target = arm64 macOS logic only
Input and isolated profile identity: ref/Sonic Adventure.cdi SHA-256 105b9a8849feeabf4b7b00a6f5d101d18789af82dfeba407952d884a726b9533 (present, incompatible; not used by this step)
Hypothesis / expected observation: A game-independent canonical Dreamcast state (one move stick, D-pad, A/B/X/Y, independent analog L/R, Start) can be converted into SARecomp's guest pad/axis snapshot as a pure function, and an edge latch can deliver a sub-sample tap exactly once.
Change and exact command: Added port/input/DreamcastInputState.h, port/input/DreamcastInputAdapter.cpp (guest mask copied verbatim from ref/sarecomp/src/sonic_input_bindings.hpp and the pad bits confirmed against ref/sarecomp/src/sonic_input.hpp Snapshot::pad), tests/host/test_dreamcast_input.cpp; ran ./scripts/run-host-tests.sh
Actual result / first causal error: 46/46 checks passed; RESULT: PASS; exit status 0. No error. Built with the Homebrew LLVM chain because the Apple toolchain is license-blocked.
Evidence path and artifact SHA-256: docs/artifacts/host-tests/{host-tests.log,build.out,run.out,test_dreamcast_input} (ignored). test_dreamcast_input SHA-256 1d3019f6ca5c9d9f692d9c2366479242235baced7e7ff33b813dda81b287894f
Interpretation and affected regressions: The state/adapter level of the input contract holds. This is a source-derived host-logic result only: it is not a compiled Apple target, not a Simulator view-event test, and not physical-device evidence. Row 20 stays partial; rows 21 and 31 stay open. G1 remains blocked.
Next smallest step / external prerequisite: Verify G1 source/game prerequisites on this machine and pin the donor references.
```

---

## 2026-10-04 22:32 CDT — G0 reference pinning + G1 source/game input inventory

```text
Date/time and timezone:  2026-10-04 22:32 CDT
Goal / current matrix row: Lowest unmet goal G1 (complete source and game inputs); matrix rows 2 and 3 (source closure, exact disc). Also closed the remaining G0 reference-pinning item.
Root revision + dirty patch identity: 3127eb6 unchanged; same dirty set as above plus docs/JOURNAL.md and docs/TEST-MATRIX.md; root prd.md/goal-loop.md remain moved into docs/ (prd.md byte-identical to the committed root copy; goal-loop.md carries the documented Section 7 edit)
Source pack / dependency lock / SDK / target: SARecomp d5288fe64e68711dd9f0e6718c1d0fb2eeca92f2 in ref/sarecomp; SunPad c77dd61b824e46d0c9a57ff4f8eb2682fceefb1b now frozen in new ref/sunpad; PaperPad a6f6999a570156f6794ff3fa79f810cfad71c4fc now frozen in new ref/paperpad; all three push DISABLED. No .local/ tree, no r354 bundle, no Katana runtime/title source present.
Input and isolated profile identity: ref/Sonic Adventure.cdi (SHA-256 105b9a88…726b9533) re-confirmed identical to ~/Documents/Systems/Sega Dreamcast/Sonic Adventure.cdi; no PAL GDI/track files anywhere.
Hypothesis / expected observation: (a) The donor pins are ancestors of the existing clones and can be frozen read-only without disturbing them. (b) A local inventory will confirm the two independent G1 gaps exactly as recorded, and will not accidentally find a usable PAL disc or source pack.
Change and exact command: Created ref/sunpad and ref/paperpad as separate --no-hardlinks clones of the preexisting ~/GitHub clones, checked out detached at the exact pins, origin restored to the public URLs and push set to DISABLED. Re-ran ./scripts/run-host-tests.sh, ./scripts/check-repo-safety.sh, a filesystem sweep for *.gdi/katana_generated.lib/unit-*.cpp/katana-source-*.zip, a check of the three pinned trees, and a hash comparison of the r354 title-source files against the public clone.
Actual result / first causal error: Both pins resolved (SunPad c77dd61, PaperPad a6f6999); safety check now reports all three references push-disabled and VERDICT safe; host tests still 46/46. Blocker A re-confirmed: only the incompatible CDI exists, no GDI. Blocker B re-confirmed with detail: 103 files under ref/sarecomp/src/* consume katana/runtime/* headers that exist nowhere on the machine; the r354 manifest describes 3,951 files / 10,853,522,212 bytes in `.local/baseline/r354/{aot,product,sdk,title-source,native-content,saves,evidence}` plus katana-source-178448be.zip (6,185,732 bytes, SHA-256 head 85e5bd27952552f0…); 1,459 generated/code units and 2026.4 MB of guest product source are absent; the public clone carries 33/39 title-source files byte-identical but 6 title files differ from r354 and the rest of the closure (headers, generated includes, AOT, native-content, FFmpeg) is entirely absent.
Evidence path and artifact SHA-256: docs/artifacts/host-tests/* (unchanged hashes; test binary 1d3019f6…). Raw inventory output kept in this entry; full analysis in docs/BOOTSTRAP-BLOCKERS.md and docs/INPUTS.md.
Interpretation and affected regressions: G0's reference-pinning item is now satisfied. G1 stays blocked by two independent, irreducible prerequisites: an incompatible game container (Blocker A) and an absent private source closure (Blocker B). No local retry can create either. The bounded workstream (input) remains green.
Next smallest step / external prerequisite: Chris (a) accepts the Xcode license (`sudo xcodebuild -license accept`) so the Apple toolchain becomes usable, and (b) supplies the PAL v1.003 GDI with all three tracks and the source-only Katana/runtime/title closure — or authenticated access to a compatible generator. G2 cannot start before (b); even a source-free Apple probe needs (a).
```
