# Known issues and acceptance limits

Updated **29 October 2026**. SonicPad is experimental. The [status record](STATUS.md)
contains supporting measurements; the [release checklist](RELEASE-CHECKLIST.md)
records source-release preparation and the publication gate.

## Gameplay and device coverage

- Selected routes reach Chaos 0, Emerald Coast's opening and Windy Valley.
  Completion of all seven stories, credits, Chao flows and a 60-minute soak
  remains unverified.
- Static discovery compiles code from the rest of the disc's modules, but
  computed jumps and other missed blocks still use Flycast's interpreter.
  Coverage percentages describe a particular route, not the whole game.
- Historical physical checks used a 12.9-inch iPad Pro (M2). Broader iPad
  coverage, iPhone hardware, Bluetooth controller acceptance and sustained
  stage performance remain open. The iOS 16 deployment minimum is not a list
  of accepted devices.
- Audio quality, a complete new-game route through Station Square and Emerald
  Coast, natural in-game saving and cold reload still need a recorded
  acceptance pass. Host and Simulator checks do not close these items.
- The maintainer reported that an earlier 1920p / 4× xBRZ session felt unlike
  Dreamcast. Dreamcast Original restores 480p and original textures; whether
  it resolves that subjective feel, including input latency, still needs
  hands-on confirmation.

## Widescreen

Phone widescreen uses Flycast's camera patch and a 4:3-to-16:9 stretch. HUD
appearance and edge pop-in/culling have not been accepted in SonicPad. The
option is not offered on iPad. Turn it off under **••• → Graphics → Widescreen
(16:9)** for the original aspect ratio, then restart when ready.
**Dreamcast Original** changes resolution and textures, not widescreen.

## Performance and graphics

Higher resolution and xBRZ texture upscaling add work. Use **Graphics →
Dreamcast Original** when diagnosing a slowdown. Full-speed intro or demo
measurements on an M2 iPad do not establish performance in every stage or on
other devices. Startup and stage loads can pause frame presentation; include
the affected route and a diagnostic log in a report.

## Builds, imports and saves

- Only Sonic Adventure PAL v1.003 / MK-5100050 with the exact hash-checked GDI
  descriptor and tracks is supported. SADX, other revisions, CDI and CHD are
  outside this release.
- The player route needs an Apple Silicon Mac and full Xcode. macOS is also
  a development/regression target, not a packaged player release. There is no
  Windows, Linux, Android or Apple TV player route.
- SonicPad is not yet in PadMint's public catalog. Use the
  [personal build instructions](INSTALL.md#build-your-app).
- Back up the VMU before updates or imports. Save states are not a substitute
  for normal game saves; compatibility across builds is not guaranteed.

Report reproducible problems through
[GitHub issues](https://github.com/chrissotraidis/sonicpad/issues), with the build
identity, device, settings and relevant diagnostic log. Never upload game
data, generated game code, an IPA or a save.
