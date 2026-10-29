#pragma once

/* Cheats for Sonic Adventure PAL v1.003 (MK-5100050), the only disc SonicPad
 * imports. Each cheat is a set of RAM writes repeated at every vertical blank
 * while it is on. The addresses were found on this exact disc: the published
 * CodeBreaker codes for the PAL "Version 1.0" release sit 0xC0 bytes higher,
 * and each value was confirmed in a RAM dump and on screen before it was
 * added (docs/CHEATS.md). Writes run on the emulator thread; on/off flags are
 * atomic, so the UI may change them at any time. */

#include <cstddef>
#include <cstdint>
#include <string>

namespace sonicpad {

struct CheatInfo {
    const char *id;        /* stable key for settings, e.g. "rings" */
    const char *title;     /* menu title */
    const char *subtitle;  /* one line under the title */
};

size_t cheat_count();
const CheatInfo& cheat_info(size_t index);
bool cheat_enabled(size_t index);
void set_cheat_enabled(size_t index, bool enabled);
/* Turns on the cheats named in a comma-separated list of ids; for scripted
 * verification runs (SONICPAD_CHEATS). Returns how many matched. */
int enable_cheats_from_list(const std::string& ids);
/* Starts applying cheats at every vertical blank. Safe to call more than once. */
void install_cheats();

} // namespace sonicpad
