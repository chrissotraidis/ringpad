# SonicPad

<p align="center">
  <img src="port/apple/ios/Assets.xcassets/AppIcon.appiconset/AppIcon.png" width="160" alt="SonicPad app icon: a gold ring over sky and sea">
</p>

<p align="center">
  <strong>Sonic Adventure for iPhone and iPad, built from your own Dreamcast disc.</strong><br>
  Ahead-of-time static recompilation, Dreamcast touch controls, controllers, save states, and the original game's graphics.
</p>

<p align="center">
  <img alt="iPhone and iPad" src="https://img.shields.io/badge/iPhone%20%2F%20iPad-iOS%2016%2B-0A84FF?logo=apple">
  <img alt="SH-4 static recompilation" src="https://img.shields.io/badge/SH--4-static%20recompilation-FF9F0A">
  <img alt="No JIT required" src="https://img.shields.io/badge/JIT-not%20required-30D158">
  <img alt="Experimental source release" src="https://img.shields.io/badge/status-experimental-FF9F0A">
  <img alt="Game data not included" src="https://img.shields.io/badge/game%20data-not%20included-FF453A">
  <a href="LICENSE"><img alt="GPL version 3" src="https://img.shields.io/badge/license-GPL--3.0-5E5CE6"></a>
</p>

SonicPad translates the supported Dreamcast game's SH-4 code into C++ on your
Mac, then compiles it for ARM64. [Flycast](https://github.com/flyinghead/flycast)
provides the Dreamcast hardware layer and interpreter fallback. On iPhone and
iPad, its Vulkan renderer runs through MoltenVK on Metal. No JIT or Dreamcast
BIOS file is needed.

> [!IMPORTANT]
> **Bring your own game.** SonicPad supports **Sonic Adventure PAL v1.003
> (MK-5100050)** as a three-track GDI. The intended public release supplies
> source and a build recipe. Game files, translated game code and prebuilt
> IPAs are excluded. You build your own app and import your disc on the device.
>
> **Experimental.** Selected routes have run on an M2 iPad Pro, but a complete
> playthrough, broad device coverage, audio and save/reload acceptance remain
> open. Build and Simulator checks do not establish those results.
>
> **AI disclosure:** SonicPad uses substantial AI assistance for code, tests,
> documentation and debugging. Build, test and device records state what was
> checked. This describes SonicPad's workflow, not its upstream projects'
> authorship; no audited percentage of AI-generated code is claimed.

## Get SonicPad

| You want to | What is available |
|---|---|
| Play on **iPhone or iPad** | Build a personal unsigned IPA on an Apple Silicon Mac, sign it with your own signing tool, then import your game. |
| Develop on **Mac** | An Apple Silicon development build for tracing, regression runs and code generation. See [Building](docs/BUILDING.md#macos-development-build). |
| Use **PadMint** | This repository includes a compatible [build recipe](padmint.json). SonicPad is not yet in PadMint's public catalog; use the direct build below. |

**You need:**

- An **Apple Silicon Mac** with full Xcode and its iOS platform installed,
  CMake **3.24 or newer**, Git and Python **3.9 or newer**. Apple's
  `/usr/bin/python3` works. Allow at least **6 GB** of build space in addition
  to Xcode and your disc.
- An iPhone or iPad running **iOS / iPadOS 16 or newer**, plus your own IPA
  signing and installation tool. This is the deployment minimum; it does not
  mean every compatible device has been tested.
- Your own supported `.gdi` with **`track01.bin`, `track02.raw` and
  `track03.bin` in the same folder**. Other regions, revisions, CDI and CHD
  images are not supported.

### Build and install

```sh
git clone https://github.com/chrissotraidis/sonicpad.git
cd sonicpad
/usr/bin/python3 scripts/build-personal.py \
  --gdi "/path/to/Sonic Adventure.gdi" \
  --output "$HOME/Desktop/SonicPad.ipa"
```

The builder verifies your disc, downloads pinned dependencies, translates the
game and packages the app. It keeps generated game code on your Mac.

1. Sign and install `SonicPad.ipa` using your own signing tool.
2. Copy the whole GDI folder to Files on your iPhone or iPad. Download any
   cloud placeholders before importing.
3. Open SonicPad, choose **Import Game**, and select that folder. You can
   also find it under **••• → Game Files → Import Game…**.
4. Let verification finish, then play. Tap **•••** for controls, graphics,
   saves and help.

See the [installation and update guide](docs/INSTALL.md) for prerequisites,
save backups and troubleshooting. The personal IPA contains code translated
from your disc; keep it and the generated files private.

## Features

- **Dreamcast controls.** A floating or fixed analog stick, eight-way D-pad,
  A/B/X/Y, independent analog triggers and a separate Start button. Move and
  resize controls, change opacity and stick travel, or connect an Apple
  GameController-compatible controller.
- **Original graphics by default.** **Graphics → Dreamcast Original** uses
  480p and original textures. Optional rendering up to 4× and xBRZ texture
  upscaling are available. Existing installations keep their settings.
- **iPhone widescreen.** A 16:9 camera patch, on by default on phones. It is
  separate from the graphics preset and has
  [known limitations](docs/KNOWN-ISSUES.md#widescreen).
- **Saves.** In-game VMU saves, three save-state slots with thumbnails, and
  VMU import/export. [Back up your VMU](docs/INSTALL.md#updates-and-saves)
  before updating or replacing a save.
- **Cheats.** Max Rings, Infinite Lives and Unlock All Characters for the
  supported PAL disc. See [cheats and save import](docs/CHEATS.md).
- **Diagnostics.** About This Build shows the version and commit;
  **Help → Share Diagnostic Log** exports recent runtime and app events.

## What has been checked?

Recorded routes reach the Chaos 0 fight, Emerald Coast's opening and Windy
Valley. Static discovery also compiles code from the game's stage, event,
boss and other modules, with the interpreter handling missed blocks.
Compiling a module does not mean its whole stage has been played through.

On the tested **12.9-inch iPad Pro (M2)**, recorded intro, attract-demo and
Windy Valley runs held full game speed. The Windy Valley route executed
**99.94%** of instructions as recompiled code. These are scoped measurements,
not a whole-game or all-device performance guarantee.

The [status record](docs/STATUS.md) separates historical measurements from
[release preparation](docs/RELEASE-CHECKLIST.md).
[Known issues](docs/KNOWN-ISSUES.md) lists the remaining coverage and hardware
work. The release checklist also records the source-publication gate.

## Questions

**Is this a decompilation or SARecomp port?** SonicPad is a static
recompilation of the supported disc's machine code. It does not use SARecomp
or a matching source decompilation.

**Is it streaming, or does it need JIT?** It runs on the device. All Flycast
JITs are compiled out; untranslated blocks use the interpreter.

**Can I use Sonic Adventure DX or another Dreamcast release?** Only the
hash-checked PAL v1.003 GDI is supported. Renaming a different image does not
make it compatible.

**Can I download an IPA or build on Windows?** The release scope is source
and a recipe. The player build needs an Apple Silicon Mac and full Xcode;
there is no prebuilt IPA, Windows build route or App Store release.

## Project map

| Path | Purpose |
|---|---|
| [Installation](docs/INSTALL.md) | Personal build, signing, first launch, updates and save backups |
| [Building](docs/BUILDING.md) | Developer setup, tracing, static discovery and Simulator checks |
| [Known issues](docs/KNOWN-ISSUES.md) | Limitations, workarounds and outstanding acceptance |
| [Status](docs/STATUS.md) | Recorded build, route and device evidence |
| [Release checklist](docs/RELEASE-CHECKLIST.md) | Source-release validation and publication steps |
| [Plan](docs/PLAN.md) / [PRD](docs/prd.md) | Project scope and completion gates |
| [Input contract](docs/INPUT.md) / [DCPad](port/dcpad/README.md) | Touch controls and the Flycast input bridge |
| [`scripts/build-personal.py`](scripts/build-personal.py) | Build a private unsigned IPA from your disc |
| [`padmint.json`](padmint.json) | PadMint-compatible build recipe |

## Credits and license

SonicPad builds on Flycast's Dreamcast hardware implementation, MoltenVK's
Vulkan-to-Metal support, and input and menu mechanisms adapted from
[SunPad](https://github.com/chrissotraidis/sunpad). Dependency versions and
attributions are in [Third-party notices](THIRD_PARTY_NOTICES.md).

SonicPad's source is licensed under **GNU GPL version 3** ([LICENSE](LICENSE)).
Dependencies retain their own licenses. The source license does not grant
rights to Sega's game data or translated game code; see
[Rights and release boundaries](docs/RIGHTS-STATUS.md).

Sonic Adventure, Sonic the Hedgehog and Dreamcast belong to SEGA. SonicPad is
an independent project and is not affiliated with or endorsed by SEGA.

## Contributing and support

[Open an issue](https://github.com/chrissotraidis/sonicpad/issues) with your
version and commit from **About This Build**, device and OS, graphics settings,
reproduction steps and what happened. A diagnostic log is useful; review it
for personal paths and filenames before attaching it. Do not upload game
files, generated code, IPAs, saves, signing material or credentials.

Focused fixes and reproducible reports against the known issues are welcome.
Run the [host checks](docs/BUILDING.md#host-checks) for relevant changes and
describe exactly which build or device was tested.
