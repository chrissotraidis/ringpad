# Install and update RingPad

RingPad's player route builds a personal app from your own **Sonic Adventure
PAL v1.003 (MK-5100050)** disc on an Apple Silicon Mac. The intended public
release provides source and a recipe, excluding prebuilt IPAs and game data.

## Prepare your Mac and disc

Install full Xcode, open it once to finish setup and accept its license, and
install the iOS platform in Xcode's settings. The Command Line Tools alone
cannot build an iPhone/iPad app. Install CMake 3.24 or newer; with Homebrew,
use `brew install cmake`.

Check the tools from Terminal:

```sh
xcodebuild -version
xcrun --sdk iphoneos --show-sdk-version
cmake --version
/usr/bin/python3 --version
git --version
```

If Xcode is installed at `/Applications/Xcode.app` but Terminal selects the
Command Line Tools, select Xcode for the current Terminal session:

```sh
export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
```

Keep your disc's `.gdi`, `track01.bin`, `track02.raw` and `track03.bin`
together. The builder checks every track and the descriptor against
[`config/game-identity.json`](../config/game-identity.json). Other releases or
formats cannot be substituted. The source and app do not download game data.

Allow at least 6 GB for the build, separately from Xcode and your disc. The
three disc tracks total about 1.23 GB. On the device, allow room for the app,
the imported disc and a temporary second disc copy during import. Copies on
the same APFS volume may share storage, but cloud or external imports may not.

## Build your app

```sh
git clone https://github.com/chrissotraidis/ringpad.git
cd ringpad
/usr/bin/python3 scripts/build-personal.py \
  --gdi "/path/to/Sonic Adventure.gdi" \
  --output "$HOME/Desktop/RingPad.ipa"
```

The first build downloads pinned Flycast and MoltenVK dependencies. It then
verifies and translates the disc, compiles the iOS app and creates an
**unsigned IPA**. Use `--jobs 2` to reduce parallel build work on a busy Mac.
The output path must not already exist; choose a new filename for an update.

All disc-derived files remain in ignored local build directories. The IPA
contains translated game code and is for your own devices. Keep it private.

The repository also includes a [PadMint-compatible recipe](../padmint.json).
RingPad is not yet listed in PadMint's public catalog. The command above is
the available player build route.

## Sign, install and import

1. Use your own IPA signing and installation tool to sign `RingPad.ipa` for
   your iPhone or iPad. Follow that tool's account, trust and renewal steps.
   RingPad itself does not request your Apple ID or signing credentials.
2. Copy the complete disc folder to Files on the device. Make sure all four
   files are downloaded locally before you continue.
3. Open RingPad and tap **Import Game**, or **••• → Game Files → Import
   Game…**. Choose the **folder containing the GDI and tracks**.
4. Wait for verification and import to finish. RingPad retains its own game
   copy and uses it on later launches.

The app targets iOS and iPadOS 16 or newer. Historical hardware checks used
an M2 iPad Pro; iPhone hardware and older devices remain unverified.
See [Known issues](KNOWN-ISSUES.md).

## Graphics and controls

New installs use **Graphics → Dreamcast Original**: 480p and original
textures. You can increase resolution or enable texture upscaling separately.
For performance problems, return to Dreamcast Original first. Existing
installations retain their previous graphics settings.

On iPhone, **Widescreen (16:9)** defaults on. This is a separate setting;
Dreamcast Original does not disable it. Turning it off offers **Restart Now**
or **Later**. Save in-game first because restarting loses unsaved progress.
The patch can stretch the HUD and has unverified edge-culling behavior.

Under **Controls**, adjust touch-control opacity, size and stick travel, move
control groups, or remap a connected controller's face buttons. The floating
stick's base stays where your thumb first lands; move past the rim to hold
full tilt. **Start** is the game's button; **•••** opens RingPad's menu.
On a landscape iPhone, scroll inside the menu to reach **Help** at the bottom.

## Updates and saves

Use the game's normal save function, then choose **••• → Game Files → Export
VMU Save** and save a backup outside RingPad before updating. Save states
are convenient checkpoints; use the VMU backup to preserve your game progress.
Compatibility of save states between builds is not guaranteed.

Build the update, sign it with the same app identity and install it over the
existing app. Previously tested in-place installs preserved saves, settings
and imported data. Uninstalling the app, changing its bundle ID or changing
signing identity may remove or disconnect that container, so keep the backup.

**Import VMU Save…** accepts a 128 KB card image or a `.VMS` and its matching
`.VMI` selected together. Import replaces the relevant save or whole card,
backs up the old card in Documents and restarts after confirmation.
See [Cheats, saves and widescreen](CHEATS.md) for details.

## Troubleshooting

| Problem | What to check |
|---|---|
| `xcodebuild` cannot find Xcode or the iOS SDK | Finish full Xcode setup, install the iOS platform and select its developer directory as shown above. |
| Disc validation fails | Check the region and revision, all four files, original track names and descriptor. Renaming a different release will not work. |
| Existing output file is kept | Choose a new `--output` filename; the builder does not overwrite an existing IPA. |
| Import cannot finish | Download cloud files locally, select the containing folder, and leave enough free storage for the import. |
| Game slows down | Use Dreamcast Original and collect a diagnostic log from the affected scene. Uncompiled paths use the slower interpreter. |
| An update appears to lose saves | Check that the signing tool retained the same app identity, then restore your exported VMU backup if needed. |

For a reproducible problem, open a
[RingPad issue](https://github.com/chrissotraidis/ringpad/issues). Include
**Help → About This Build**, your device/OS, settings and steps. Export
**Help → Share Diagnostic Log**, review it for personal information, then
attach only the relevant log. Do not attach disc files, generated code, IPAs,
VMU saves or credentials.
