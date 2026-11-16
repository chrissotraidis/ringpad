# Cheats, saves and widescreen

Updated **29 October 2026**; verification below was recorded on 22 October. Everything here applies to the PAL v1.003 disc
(MK-5100050), the only one RingPad imports.

## Cheats (••• ▸ Cheats)

| Cheat | RAM writes (main RAM offset) | Verified |
|---|---|---|
| Max Rings | 16-bit 999 at `0x752B24` | 999 rings on the Chaos 0 HUD |
| Infinite Lives | 16-bit 9 at `0x752B20` | 9 lives on the HUD after taking hits |
| Unlock All Characters | 16-bit `0x0101` at `0x7998A2`, 32-bit `0x01010101` at `0x7998A4` | Character select shows all six plus Super Sonic |

Cheats are written at every vertical blank while on
(`port/game/GameCheats.cpp`) and stay on across launches.

**How they were found.** ConsoleDatabase lists CodeBreaker codes for two PAL
releases. Neither set's addresses hold those values on this disc: the RAM at
the "Version 1.1" addresses stayed zero through a full run. In dumps taken
during the Chaos 0 route (`DUMP:` in an input script), the ring counter (0
then 1 after the first pickup) and the lives counter (4, 3, 2 as Sonic took
hits) sit at `0x752B24` and `0x752B20`: the "Version 1.0" addresses minus
0xC0. The character flags follow the same rule. Each cheat then ran the whole
route from boot (`RINGPAD_CHEATS=rings,lives,characters`) with screenshots.

**Left out.** Max Emblems at the shifted address changed nothing on the file
screen. Debug Mode at the shifted address turned on Sega's development
overlay (polygon counters and a rainbow debug floor), which is not useful for
play; neither is in the menu.

## Saves further into the game (••• ▸ Game Files ▸ Import VMU Save)

Import accepts either a whole memory card (a 128 KB `.bin`, the format
Export VMU Save writes) or one save as a `.VMS` file with its `.VMI`
(select both in Files). A `.VMS` save is written into the current card,
replacing the same-named save already there and leaving other files.
Export a VMU backup outside the app first. A whole-card import replaces all
files on that card; a single-save import replaces only the matching filename.
The previous card is first copied to
`Documents/MK-5100050_vmu_save_A1.backup-<time>.bin`, then the game
restarts. `port/game/VMUImage.cpp` does the card work and has host tests.

Checked with El Bucanero's "Perfect save. All 130 emblems and super sonic"
(`v34162.VMS`/`.vmi`): imported through the app in the Simulator, File 1
shows every character including Super Sonic, 130 emblems, Perfect Chaos
completed and 100% clear. Downloaded saves are not stored in this repository.

## Widescreen (iPhone)

Graphics ▸ Widescreen (16:9) turns on Flycast's built-in widescreen patch for
this disc and stretches the picture to 16:9. It is on by default on phones
and not offered on iPad. Turning it off offers **Restart Now** or **Later**,
because the patched camera values remain in memory until restart. Save in-game
first; restarting discards unsaved progress. The **Dreamcast Original** preset
changes resolution and textures, not this setting. HUD appearance and edge
culling remain unverified; see [Known issues](KNOWN-ISSUES.md#widescreen).
