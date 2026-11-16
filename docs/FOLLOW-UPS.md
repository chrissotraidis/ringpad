# RingPad follow-ups

Updated **31 October 2026**. Open maintainer work, with what is already known
and the next step for each item. Player-facing limits are in
[Known issues](KNOWN-ISSUES.md); recorded measurements are in [Status](STATUS.md).

## Before a public release

### Publish from clean history

The existing repository must stay private. `archive/katana-route` holds
reconstructed original routines and captured game-state values, and Git history
keeps them even though `.gitattributes` leaves `archive/` out of source
exports. Keep this repository as a private history and publish a fresh
repository from the reviewed source export. That changes the repository
identity and needs the maintainer's explicit approval. See the
[release checklist](RELEASE-CHECKLIST.md#publication-status) and
[rights and distribution boundaries](RIGHTS-STATUS.md#historical-research-archive).

### Repeat the PadMint player check on the final commit

The last clean player build used candidate `21bd0df`. Run
`scripts/player-check.sh` from current PadMint `main` against the commit that
will be published, before tagging, and again after publishing. A failure
blocks the release.

## Rendering

### Sonic's face: head polygons over the eyes

Reported by the maintainer: parts of Sonic's face texture sometimes draw over
his eyes. Flycast tracks the same symptom as
[flycast#1443](https://github.com/flyinghead/flycast/issues/1443).

Known so far (31 October 2026):

- **Not the recompiler.** The translator calls Flycast's own handler for every
  floating-point instruction, so vertex math matches the interpreter. On the
  Mac build, the opening Station Square cutscene with recompilation on and with
  `RINGPAD_NO_AOT=1` gave matching close-ups of Sonic's face: 3.5–4.9% RMSE
  between corresponding frames, from a small timing offset, against about 26%
  between neighbouring frames of one run.
- **Not reproduced in the captured shots.** Neither the Mac (OpenGL) runs nor
  the M2 iPad Pro run with Per Triangle sorting showed the glitch in those
  close-ups.
- **Per Pixel sorting is not usable on iPad.** Flycast reports
  `fragmentStoresAndAtomics` through MoltenVK (`hasPerPixel` is true), and its
  order-independent renderer (`RenderType::Vulkan_OIT`) starts, but frames show
  large stray background polygons and missing or shredded eyes. The result was
  the same at 1920p with 4× xBRZ and at 480p with original textures. The
  pinned Flycast `ed953fc` already includes upstream's MoltenVK OIT
  synchronization fix (`e36e9df2d`); upstream `master` had no newer Vulkan
  changes on 31 October 2026. Related: [flycast#2495](https://github.com/flyinghead/flycast/issues/2495)
  and [flycast#2387](https://github.com/flyinghead/flycast/issues/2387).

Next steps:

1. Capture the glitch when it happens, ideally with a screen recording of the
   attached iPad, and note the scene and graphics settings.
2. Write a development state just before that moment (`STATE:` in an input
   script) and replay it on the iPad from `Documents` with
   `RINGPAD_STATE_LOAD`, `RINGPAD_INPUT_SCRIPT` and `SHOT:` events.
3. Compare that scene with **Dreamcast Original** and with Flycast's
   **Per Strip** sorting (`rend.PerStripSorting`). Ship whichever setting
   fixes it, if one does.
4. Revisit Per Pixel only after an upstream Flycast or MoltenVK change, or
   with a smaller `rend.PixelBufferSize`. Exposing it today would show broken
   frames.

When testing on a device, back up its `Documents` VMU files and `emu.cfg`
first, and load a development state instead of driving the file-select menus
over a player's save.

## Hardware acceptance

These remain open; build and Simulator checks do not close them:

- A complete playthrough, credits and Chao flows, and a 60-minute soak.
- Audio quality, natural in-game saving and a cold reload.
- iPhone hardware, other iPads and Bluetooth controllers.
- Sustained stage performance away from the recorded routes.
- Whether Dreamcast Original resolves the earlier report that 1920p / 4× xBRZ
  felt unlike Dreamcast, including input latency.
- iPhone widescreen HUD appearance and edge culling.
- Dismissing the diagnostic share sheet on iPhone and resuming play.

## Engineering notes

- **Interpreter time accounting.** Code that falls back to the interpreter is
  charged one eighth of its cycles, matching Flycast's non-strict interpreter
  underclock; recompiled blocks are charged in full. Timing in rarely
  recompiled code may therefore differ slightly from a fully recompiled route.
- **Stale local builds after Xcode updates.** After an Xcode or SDK update,
  configure `generated/build-ios` again with `cmake --fresh`. A cache from
  the previous SDK points zlib at a removed SDK path. In
  `generated/build-mac`, delete the precompiled header `cmake_pch*.pch` files,
  which clang rejects after a compiler update.
