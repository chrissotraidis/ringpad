# SonicPad

Private Sonic Adventure Apple-port investigation. **No playable Apple build is
currently verified.** Required Katana/title sources and the supported PAL dump
remain missing; isolated recovery experiments do not clear that gate.

Start with [the review and reorientation handoff](docs/HANDOFF-REORIENTATION.md).
It separates proven work, historical experiments, missing local dependencies
and the decisions the next reviewer needs to make.

- [Product requirements](docs/prd.md)
- [Exact bootstrap blockers and source-only request](docs/BOOTSTRAP-BLOCKERS.md)
- [Single-stick Dreamcast input contract](docs/INPUT.md)

The recovery `work/` directory is absent as of 10 October 2026. Older run commands
and checkpoint reports must be revalidated before use. Game data, private source
packages, generated code and runtime captures are deliberately excluded from Git.
