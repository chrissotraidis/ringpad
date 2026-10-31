# SonicPad

<p align="center">
  <img src="port/apple/ios/Assets.xcassets/AppIcon.appiconset/AppIcon.png" width="160" alt="SonicPad app icon: a gold ring over sky and sea">
</p>

SonicPad brings **Sonic Adventure to iPhone and iPad**, built from your own
Dreamcast disc. It translates the game's SH-4 machine code into C++ ahead of
time on your Mac and compiles it for ARM64, with
[Flycast](https://github.com/flyinghead/flycast) as the Dreamcast hardware
layer. It adds Dreamcast touch controls, controller support, save states and a
native ••• menu.

**AI disclosure:** SonicPad uses substantial AI assistance for code, tests,
documentation, debugging and maintenance. There is no audited percentage of
AI-generated code; build, test and device records state what has actually been
checked. This describes SonicPad's workflow, not the authorship of its upstream
projects.

<p align="center">
  <strong>Sonic Adventure on iPhone and iPad, from your own disc.</strong><br>
  Ahead-of-time static recompilation. No JIT, no BIOS file, no streaming.<br>
  Touch or controller, save states, and the original Dreamcast look.
</p>

<p align="center">
  <img alt="iOS and iPadOS 16 or later" src="https://img.shields.io/badge/iOS%20%2F%20iPadOS-16%2B-0A84FF?logo=apple">
  <img alt="SH-4 static recompilation" src="https://img.shields.io/badge/SH--4-static%20recompilation-FF9F0A">
  <img alt="No JIT required" src="https://img.shields.io/badge/JIT-not%20required-30D158">
  <img alt="Vulkan through MoltenVK on Metal" src="https://img.shields.io/badge/renderer-Vulkan%20%E2%86%92%20Metal-5E5CE6">
  <img alt="Game data not included" src="https://img.shields.io/badge/game%20data-not%20included-FF453A">
  <a href="LICENSE"><img alt="GPL version 3" src="https://img.shields.io/badge/license-GPL--3.0-5E5CE6"></a>
  <a href="https://discord.gg/xwHfUD2bxW"><img alt="Join the Discord community" src="https://img.shields.io/badge/Discord-Join%20the%20community-5865F2?logo=discord&amp;logoColor=white"></a>
</p>

<p align="center">
  <a href="#get-sonicpad">Get SonicPad</a> ·
  <a href="#supported-game">Game requirements</a> ·
  <a href="#controls">Controls</a> ·
  <a href="docs/INSTALL.md">Install guide</a> ·
  <a href="docs/KNOWN-ISSUES.md">Known issues</a> ·
  <a href="#frequently-asked-questions">FAQ</a>
</p>

> [!IMPORTANT]
> **Build your own copy on an Apple Silicon Mac.** SonicPad releases contain
> source and a build recipe. No prebuilt app, translated game code or game data
> is distributed. You need your own **Sonic Adventure PAL v1.003
> (MK-5100050)** disc as a three-track GDI.

## Highlights

- **Recompiled, not emulated, CPU.** The game's SH-4 code runs as
  ahead-of-time compiled ARM64 code. Flycast supplies the graphics, sound,
  disc and controller hardware, and its interpreter covers any block the
  translator missed. All of Flycast's JITs are compiled out.
- **Dreamcast touch controls.** A floating or fixed analog stick, eight-way
  D-pad, A/B/X/Y with chords, independent analog L/R and a separate Start.
  Move, resize and fade the controls to suit your hands.
- **Controllers.** Apple GameController-compatible pads, such as Xbox,
  PlayStation and MFi controllers, with face-button remapping. Touch controls
  hide when a controller connects.
- **Original graphics by default.** **Dreamcast Original** renders at 480p with
  the original textures. Up to 4× resolution and xBRZ texture upscaling are
  optional.
- **Saves and backups.** In-game VMU saves, three save-state slots with
  thumbnails, and VMU import and export.
- **Extras.** iPhone widescreen, plus Max Rings, Infinite Lives and Unlock All
  Characters cheats for the supported disc.

## What is available now?

| Platform or route | Status |
|---|---|
| iPhone / iPad, iOS 16+ | Personal unsigned IPA built on an Apple Silicon Mac |
| PadMint | Compatible [recipe](padmint.json) in this repository; not yet in PadMint's public catalog |
| Apple Silicon Mac | Development and regression build only; no packaged Mac player |
| Windows, Linux, Android, Apple TV | Not offered |
| App Store, TestFlight, prebuilt IPAs | Not offered |

**SonicPad 0.1.0 is an experimental preview.** On a 12.9-inch iPad Pro (M2),
recorded runs reached the Chaos 0 fight, Emerald Coast's opening and Windy
Valley, and the intro, attract demo and Windy Valley held full game speed.
The Windy Valley route ran 99.94% of its instructions as recompiled code. A
complete playthrough, audio, save-and-reload, iPhone hardware and other devices
are not yet accepted. See [Known issues](docs/KNOWN-ISSUES.md) and the
[status record](docs/STATUS.md) for the exact scope.

## Get SonicPad

You need:

- An **Apple Silicon Mac** with full **Xcode and its iOS platform** installed,
  **CMake 3.24 or newer** (`brew install cmake`), Git and Python 3.9 or newer.
  Apple's `/usr/bin/python3` works.
- At least **6 GB** of build space, separate from Xcode and your disc.
- An iPhone or iPad running **iOS / iPadOS 16 or newer**, and a way to sign and
  install a personal IPA, such as AltStore Classic, SideStore, Sideloadly or
  Xcode.
- Your own supported disc. See [Supported game](#supported-game).

### Build and install

```sh
git clone https://github.com/chrissotraidis/sonicpad.git
cd sonicpad
/usr/bin/python3 scripts/build-personal.py \
  --gdi "/path/to/Sonic Adventure.gdi" \
  --output "$HOME/Desktop/SonicPad.ipa"
```

The builder verifies your disc, downloads pinned Flycast and MoltenVK,
translates the game and packages an unsigned IPA. A clean build took about
eight minutes on the maintainer's Apple Silicon Mac. Generated game code stays
in ignored folders on your Mac.

1. Sign and install `SonicPad.ipa` with your signing tool.
2. Copy the whole GDI folder to Files on your iPhone or iPad. Download any
   cloud placeholders first.
3. Open SonicPad, tap **Import Game** and choose that folder. You can also use
   **••• → Game Files → Import Game…**.
4. Let verification finish, then play. Tap **•••** for controls, graphics,
   saves and help.

The personal IPA contains code translated from your disc. Keep it, and the
generated files, private. The [install guide](docs/INSTALL.md) covers Xcode
setup, troubleshooting and updates.

**Updating:** back up your VMU with **••• → Game Files → Export VMU Save…**,
then install the new build over the existing app with the same signing setup.
Deleting SonicPad removes its saves, settings and imported disc.

## Supported game

| Input | Requirement |
|---|---|
| Game | **Sonic Adventure PAL v1.003 (1999)**, product **MK-5100050** |
| Format | `.gdi` with `track01.bin`, `track02.raw` and `track03.bin` in the same folder |
| Size | About 1.23 GB for the three tracks |
| Identity | Every track and the descriptor are SHA-256 checked against [`config/game-identity.json`](config/game-identity.json) |
| Not supported | Other regions and revisions, Sonic Adventure DX, CDI and CHD images |

Renaming a different image does not make it compatible. SonicPad copies the
disc into its own storage during import and leaves your files unchanged. No
disc image, BIOS, extracted game files or saves are included in this
repository or its releases.

## Controls

| Dreamcast | Touch | Controller |
|---|---|---|
| Analog stick | Floating stick in the lower left; fixed mode in settings | Left thumbstick |
| D-pad | Eight-way cross | D-pad |
| A / B / X / Y | Colored diamond: A bottom, B right, X left, Y top; slide for chords | Positional face buttons, remappable |
| L / R | Independent analog sliders | Analog triggers; shoulders act as full presses |
| Start | Start pill | Menu button |
| SonicPad menu | **•••** | **•••** on screen |

**••• → Controls** sets touch opacity, size and stick travel, moves control
groups, and opens controller mapping. The **•••** button never sends Start to
the game. See the [input contract](docs/INPUT.md) and
[DCPad](port/dcpad/README.md) for details.

## Graphics and settings

| Menu | What it does |
|---|---|
| **Graphics → Dreamcast Original** | 480p with the original textures; the default for new installs |
| **Graphics → Resolution** | Original (480p), 2×, 3× or 4× (1920p) |
| **Graphics → Texture Upscaling** | Off, 2× or 4× xBRZ |
| **Graphics → Widescreen (16:9)** | iPhone only, on by default; turning it off restarts the game |
| **Graphics → Show FPS** | Frame counter at the top left |
| **Sound** | Full, half or off |
| **Save State / Load State** | Three slots with thumbnails and dates |
| **Cheats** | Max Rings, Infinite Lives, Unlock All Characters |
| **Game Files** | Import a game, export or import VMU saves |
| **Help** | Share Diagnostic Log…, About This Build |

Use **Dreamcast Original** first if a stage slows down. Widescreen can stretch
the HUD; see [Known issues](docs/KNOWN-ISSUES.md#widescreen). Cheats and VMU
import are described in [Cheats, saves and widescreen](docs/CHEATS.md).

## Frequently asked questions

### Is this an emulator, SARecomp or a decompilation?

SonicPad is a static recompilation of the supported disc's machine code. Flycast
emulates the Dreamcast's other hardware, and its interpreter runs any code the
translator missed. It does not use SARecomp or a source decompilation.

### Does it need JIT or a connection to my Mac?

No. Everything runs on the device. Flycast's JITs are compiled out, so no JIT
entitlement or debugger connection is needed.

### Where is the IPA download?

There is none. The app would contain code translated from the game, so each
player builds their own from their own disc using [the steps above](#get-sonicpad).

### Can I use Sonic Adventure DX or another release?

No. Only the hash-checked PAL v1.003 GDI is supported.

### Can I build on Windows or play on Mac?

Not in this release. The player build needs an Apple Silicon Mac with full
Xcode. The Mac build is a development tool for tracing and regression runs.

### Does everything work?

Not yet. This is an early preview: a full playthrough, audio quality,
save-and-reload, iPhone hardware, other iPads, Bluetooth controllers and long
sessions remain open.
Sonic's face can occasionally show head polygons over his eyes; see
[Known issues](docs/KNOWN-ISSUES.md#sonics-face-head-polygons-over-the-eyes).

## Help and community

Use **••• → Help → Share Diagnostic Log…** to export recent app and runtime
events, and **About This Build** for the version and commit. Review the log for
personal paths and filenames before you share it. Include your device, OS
version, graphics settings and steps to reproduce. Never attach game files,
generated code, IPAs, saves, signing files or credentials.

[GitHub issues](https://github.com/chrissotraidis/sonicpad/issues) ·
[Discord community](https://discord.gg/xwHfUD2bxW) ·
[Install guide](docs/INSTALL.md) ·
[Known issues](docs/KNOWN-ISSUES.md) · [Security](SECURITY.md)

Focused fixes and reproducible reports against the known issues are welcome.
Run the [host checks](docs/BUILDING.md#host-checks) for relevant changes and
say exactly which build or device you tested.

## Project map

| Path | Purpose |
|---|---|
| [Install guide](docs/INSTALL.md) | Personal build, signing, first launch, updates and save backups |
| [Building](docs/BUILDING.md) | Developer setup, tracing, static discovery, Simulator and device runs |
| [Known issues](docs/KNOWN-ISSUES.md) | Limitations, workarounds and outstanding acceptance |
| [Follow-ups](docs/FOLLOW-UPS.md) | Open maintainer work and next steps |
| [Status](docs/STATUS.md) | Recorded build, route and device evidence |
| [Release checklist](docs/RELEASE-CHECKLIST.md) | Release checks and publication steps |
| [Rights](docs/RIGHTS-STATUS.md) | Licenses and game-content boundaries |
| [`scripts/build-personal.py`](scripts/build-personal.py) | Build a private unsigned IPA from your disc |
| [`scripts/recomp/`](scripts/recomp/) | Block discovery and C++ generation |
| [`port/recomp/`](port/recomp/) | The executor that runs recompiled blocks inside Flycast |
| [`port/apple/ios/`](port/apple/ios/) | iPhone/iPad shell, menus, import and diagnostics |
| [`port/dcpad/`](port/dcpad/) | Dreamcast touch controls and controller input |
| [`padmint.json`](padmint.json) | PadMint-compatible build recipe |
| [`tests/`](tests/) | Host tests, iOS UI tests and scripted acceptance routes |

## Research and credits

SonicPad builds on [Flycast](https://github.com/flyinghead/flycast) for the
Dreamcast hardware layer, HLE BIOS and SH-4 interpreter, and on
[MoltenVK](https://github.com/KhronosGroup/MoltenVK) for Vulkan on Metal. Its
touch input and menu mechanisms are adapted from
[SunPad](https://github.com/chrissotraidis/sunpad). Exact dependency versions
are pinned in [`dependencies.lock.json`](dependencies.lock.json), with
attributions in [Third-party notices](THIRD_PARTY_NOTICES.md).

## Legal and provenance

SonicPad is an unofficial project and is not affiliated with or endorsed by
SEGA. Sonic Adventure, Sonic the Hedgehog and Dreamcast belong to SEGA.

SonicPad's source is licensed under the **GNU General Public License, version
3** ([LICENSE](LICENSE)). Dependencies keep their own licenses. The source
license grants no rights to Sega's game data or to code translated from it; see
[Rights and distribution boundaries](docs/RIGHTS-STATUS.md). The app icon is
original project artwork with no character, logo or game imagery.
