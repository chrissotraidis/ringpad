# RingPad

<p align="center">
  <img src="port/apple/ios/Assets.xcassets/AppIcon.appiconset/AppIcon.png" width="128" height="128" alt="RingPad icon: a gold ring over sky and sea">
</p>

<p align="center">
  <strong>Sonic Adventure, native on iPhone and iPad, built from your own Dreamcast disc.</strong><br>
  Ahead-of-time static recompilation of the game's SH-4 code, with Dreamcast touch controls, controllers, save states and the original Dreamcast look. No JIT, no BIOS file, no streaming.
</p>

<p align="center">
  <a href="https://github.com/chrissotraidis/ringpad/releases/latest"><img alt="Latest release" src="https://img.shields.io/github/v/release/chrissotraidis/ringpad?label=release&color=FFD60A"></a>
  <img alt="iOS and iPadOS 16 or later" src="https://img.shields.io/badge/iOS%20%2F%20iPadOS-16%2B-0A84FF?logo=apple">
  <img alt="SH-4 static recompilation" src="https://img.shields.io/badge/SH--4-static%20recompilation-FF9F0A">
  <img alt="No JIT required" src="https://img.shields.io/badge/JIT-not%20required-30D158">
  <img alt="Vulkan through MoltenVK on Metal" src="https://img.shields.io/badge/renderer-Vulkan%20%E2%86%92%20Metal-5E5CE6">
  <img alt="Experimental preview" src="https://img.shields.io/badge/preview-experimental-FFD60A">
  <img alt="Game data not included" src="https://img.shields.io/badge/game%20data-not%20included-FF453A">
  <a href="https://github.com/chrissotraidis/padmint"><img alt="Build RingPad with PadMint" src="https://img.shields.io/badge/PadMint-build%20your%20own-3EB489"></a>
  <a href="https://discord.gg/xwHfUD2bxW"><img alt="Join the Discord community" src="https://img.shields.io/badge/Discord-Join%20the%20community-5865F2?logo=discord&amp;logoColor=white"></a>
</p>

RingPad translates Sonic Adventure's Dreamcast machine code into C++ on your
Mac and compiles it into a native ARM64 app.
[Flycast](https://github.com/flyinghead/flycast) supplies the Dreamcast's
graphics, sound, disc and controller hardware, rendering through
[MoltenVK](https://github.com/KhronosGroup/MoltenVK) on Metal. RingPad adds the
iPhone and iPad app around it: touch controls, controller support, menus,
saves and disc import.

> [!IMPORTANT]
> **Bring your own disc.** RingPad needs your own **Sonic Adventure PAL v1.003
> (MK-5100050)** as a three-track GDI. No game data, translated code or
> prebuilt app is distributed; every player builds their own copy.
> RingPad is unofficial and is not affiliated with or endorsed by SEGA.
>
> **AI disclosure:** RingPad uses substantial AI assistance for code, tests,
> documentation, debugging and maintenance. There is no audited percentage of
> AI-generated code. Build, test and device records describe what was checked.
> This disclosure concerns RingPad's workflow, not the authorship of its
> upstream projects.

## Get RingPad

RingPad is free. On an Apple Silicon Mac, [PadMint](https://github.com/chrissotraidis/padmint)
builds it from your disc in about eight minutes; you then install the IPA on
your iPhone or iPad.

| You want RingPad on | Get it | Then |
|---|---|---|
| **iPhone or iPad** (the easy way) | Build it with [PadMint](https://github.com/chrissotraidis/padmint) | [With PadMint](#with-padmint) |
| **iPhone or iPad** (by hand) | Run one script from this repository | [By hand](#by-hand) |

**You need:**

- An **Apple Silicon Mac** with full **Xcode** and its iOS platform, and
  **CMake 3.24 or newer** (`brew install cmake`).
- About **6 GB** free for the build.
- An iPhone or iPad on **iOS / iPadOS 16 or newer**, and AltStore Classic,
  SideStore, Sideloadly or Xcode to install a personal IPA.
- Your own [supported disc](#supported-disc).

### With PadMint

1. Download [PadMint](https://github.com/chrissotraidis/padmint/releases/latest),
   unzip it and double-click `PadMint.command`.
2. In the page that opens, choose **RingPad**, pick your `.gdi` file (its three
   tracks must be in the same folder) and click **Make my copy**.
3. Install the IPA from your Downloads folder with your signing tool.
4. Copy the whole GDI folder to Files on the device. Open RingPad, tap
   **Import Game** and choose that folder. RingPad checks every track, then
   starts the game.

### By hand

```sh
git clone https://github.com/chrissotraidis/ringpad.git
cd ringpad
/usr/bin/python3 scripts/build-personal.py \
  --gdi "/path/to/Sonic Adventure.gdi" \
  --output "$HOME/Desktop/RingPad.ipa"
```

The script checks your disc, downloads pinned Flycast and MoltenVK, translates
the game and packages an unsigned IPA. Then install and import as in steps 3
and 4 above. The [install guide](docs/INSTALL.md) covers Xcode setup,
troubleshooting and updates.

> [!NOTE]
> The IPA contains code translated from your disc. Keep it for yourself.

**Updating:** export your save with **••• → Game Files → Export VMU Save…**,
then install the new build over the old app with the same signing setup. Your
disc, saves and settings stay. Deleting RingPad deletes them.

## Current status

**0.1.0 is an experimental preview.** On a 12.9-inch iPad Pro (M2), recorded
runs reach the Chaos 0 fight, Emerald Coast and Windy Valley at full speed,
with 99.9% or more of the game's instructions running as recompiled code. A
full playthrough, audio quality, save and reload, iPhone hardware, other iPads
and Bluetooth controllers still need a recorded pass. See
[known issues](docs/KNOWN-ISSUES.md) and the [status record](docs/STATUS.md).

## Features

| | |
|---|---|
| **Recompiled CPU** | The game's SH-4 code runs as ahead-of-time compiled ARM64. Flycast's interpreter covers anything the translator missed; every Flycast JIT is compiled out. |
| **Touch controls** | Floating or fixed analog stick, eight-way D-pad, A/B/X/Y with slide chords, independent analog L/R and Start. Move, resize and fade them. |
| **Controllers** | Xbox, PlayStation and MFi controllers, with face-button remapping. Touch controls hide when one connects. |
| **Graphics** | **Dreamcast Original** (480p, original textures) by default; up to 4× resolution and xBRZ texture upscaling; widescreen on iPhone. |
| **Saves** | In-game VMU saves, three save-state slots with thumbnails, VMU import and export. |
| **Extras** | Max Rings, Infinite Lives and Unlock All Characters. See [cheats](docs/CHEATS.md). |

## Supported disc

| | |
|---|---|
| Game | **Sonic Adventure PAL v1.003 (1999)**, product **MK-5100050** |
| Files | `.gdi` with `track01.bin`, `track02.raw` and `track03.bin` in the same folder, about 1.23 GB |
| Check | Every track is SHA-256 checked against [`config/game-identity.json`](config/game-identity.json) |
| Not supported | Other regions and revisions, Sonic Adventure DX, CDI and CHD images |

## Controls

| Dreamcast | Touch | Controller |
|---|---|---|
| Analog stick | Floating stick, lower left (fixed mode in settings) | Left thumbstick |
| D-pad | Eight-way cross | D-pad |
| A / B / X / Y | Diamond: A bottom, B right, X left, Y top | Face buttons, remappable |
| L / R | Analog sliders | Analog triggers |
| Start | Start pill | Menu button |
| RingPad menu | **•••** | **•••** on screen |

**••• → Controls** changes opacity, size and stick travel, moves the controls
and opens controller mapping. **•••** never sends Start to the game. Details:
[input contract](docs/INPUT.md) and [DCPad](port/dcpad/README.md).

## The ••• menu

| Menu | What it does |
|---|---|
| **Save State / Load State** | Three slots with thumbnails and dates |
| **Graphics** | Dreamcast Original, resolution (480p to 1920p), xBRZ upscaling, widescreen (iPhone), Show FPS |
| **Sound** | Full, half or off |
| **Cheats** | Max Rings, Infinite Lives, Unlock All Characters |
| **Game Files** | Import a game, export or import VMU saves |
| **Help** | Share Diagnostic Log…, About This Build |

If a stage slows down, try **Dreamcast Original** first.

## Frequently asked questions

<details>
<summary><strong>Is this an emulator or a decompilation?</strong></summary>

Neither, exactly. RingPad is a static recompilation of the supported disc's
machine code: the game's own CPU instructions become native ARM64. Flycast
emulates the rest of the Dreamcast (graphics, sound, disc, controllers), and
its interpreter runs any code the translator missed. There is no source
decompilation.
</details>

<details>
<summary><strong>Does it need JIT or a connection to my Mac?</strong></summary>

No. Everything runs on the device, and Flycast's JITs are compiled out.
</details>

<details>
<summary><strong>Where is the IPA download?</strong></summary>

There isn't one. The app contains code translated from the game, so each
player builds their own from their own disc.
</details>

<details>
<summary><strong>Can I use Sonic Adventure DX, another region or a CDI/CHD?</strong></summary>

No. Only the hash-checked PAL v1.003 GDI is supported.
</details>

<details>
<summary><strong>Can I build on Windows or play on a Mac?</strong></summary>

Not yet. The build needs an Apple Silicon Mac with Xcode. The Mac build in
this repository is a development tool, not a player app.
</details>

<details>
<summary><strong>Sonic's face sometimes shows polygons over his eyes. Is that RingPad?</strong></summary>

It comes from Flycast's transparency sorting
([flycast#1443](https://github.com/flyinghead/flycast/issues/1443)), not the
recompiled code. See [known issues](docs/KNOWN-ISSUES.md#sonics-face-head-polygons-over-the-eyes).
</details>

## Help and community

**••• → Help → Share Diagnostic Log…** exports recent app and runtime events;
**About This Build** shows the version and commit. Include your device, OS
version, graphics settings and steps to reproduce. Never attach game files,
generated code, IPAs or saves.

[Issues](https://github.com/chrissotraidis/ringpad/issues) ·
[Discord](https://discord.gg/xwHfUD2bxW) ·
[Install guide](docs/INSTALL.md) ·
[Known issues](docs/KNOWN-ISSUES.md) ·
[Security](SECURITY.md)

## For developers

| Path | Purpose |
|---|---|
| [Building](docs/BUILDING.md) | Development builds, tracing, static discovery, Simulator and device runs, releasing |
| [Status](docs/STATUS.md) · [Follow-ups](docs/FOLLOW-UPS.md) | What is verified, and what is next |
| [`scripts/recomp/`](scripts/recomp/) | Block discovery and C++ generation |
| [`port/recomp/`](port/recomp/) | The executor that runs recompiled blocks inside Flycast |
| [`port/apple/ios/`](port/apple/ios/) | iPhone/iPad app: menus, import and diagnostics |
| [`port/dcpad/`](port/dcpad/) | Dreamcast touch controls and controller input |
| [`padmint.json`](padmint.json) | PadMint build recipe |
| [`tests/`](tests/) | Host tests, iOS UI tests and scripted acceptance routes |

## Credits and legal

Built on [Flycast](https://github.com/flyinghead/flycast) (Dreamcast hardware,
high-level BIOS and SH-4 interpreter) and
[MoltenVK](https://github.com/KhronosGroup/MoltenVK) (Vulkan on Metal). Touch
input and menu mechanisms are adapted from
[SunPad](https://github.com/chrissotraidis/sunpad). Versions are pinned in
[`dependencies.lock.json`](dependencies.lock.json); attributions are in
[third-party notices](THIRD_PARTY_NOTICES.md).

RingPad is an unofficial project and is not affiliated with or endorsed by
SEGA. Sonic Adventure, Sonic the Hedgehog and Dreamcast belong to SEGA.
RingPad's source is licensed under the [GNU GPL, version 3](LICENSE); this
grants no rights to SEGA's game data or to code translated from it (see
[rights and distribution boundaries](docs/RIGHTS-STATUS.md)). The app icon is
original artwork with no character, logo or game imagery.
