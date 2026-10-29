// See SonicCheats.h.
#include "SonicCheats.h"

#include "types.h"
#include "emulator.h"
#include "hw/sh4/sh4_mem.h"

#include <atomic>
#include <cstdio>
#include <sstream>

namespace sonicpad {
namespace {

constexpr const char *kGameId = "MK-5100050";

struct Write {
    uint32_t address;   /* offset in main RAM (0x0C000000) */
    uint32_t value;
    int bits;
};

struct Cheat {
    CheatInfo info;
    Write writes[4];
};

const Cheat kCheats[] = {
    {{"rings", "Max Rings", "999 rings"}, {{0x752B24, 999, 16}}},
    {{"lives", "Infinite Lives", "Lives stay at 9"}, {{0x752B20, 9, 16}}},
    {{"characters", "Unlock All Characters", "Tails, Knuckles, Amy, E-102, Big and Super Sonic"},
        {{0x7998A2, 0x0101, 16}, {0x7998A4, 0x01010101, 32}}},
};
constexpr size_t kCount = sizeof(kCheats) / sizeof(kCheats[0]);
std::atomic<bool> gEnabled[kCount];
std::atomic<bool> gInstalled{false};

void write(const Write& w)
{
    const uint32_t a = 0x8C000000 + w.address;
    switch (w.bits) {
    case 8: WriteMem8_nommu(a, uint8_t(w.value)); break;
    case 16: WriteMem16_nommu(a, uint16_t(w.value)); break;
    default: WriteMem32_nommu(a, w.value); break;
    }
}

void onVBlank(Event, void *)
{
    if (settings.content.gameId != kGameId)
        return;
    for (size_t i = 0; i < kCount; i++) {
        if (!gEnabled[i].load(std::memory_order_relaxed))
            continue;
        for (const Write& w : kCheats[i].writes)
            if (w.bits != 0)
                write(w);
    }
}

} // namespace

size_t cheat_count() { return kCount; }
const CheatInfo& cheat_info(size_t index) { return kCheats[index].info; }
bool cheat_enabled(size_t index) { return index < kCount && gEnabled[index].load(); }

void set_cheat_enabled(size_t index, bool enabled)
{
    if (index < kCount)
        gEnabled[index] = enabled;
}

int enable_cheats_from_list(const std::string& ids)
{
    std::stringstream in(ids);
    std::string id;
    int matched = 0;
    while (std::getline(in, id, ','))
        for (size_t i = 0; i < kCount; i++)
            if (id == kCheats[i].info.id) {
                gEnabled[i] = true;
                matched++;
                std::fprintf(stderr, "SONICPAD_CHEAT on %s\n", id.c_str());
            }
    return matched;
}

void install_cheats()
{
    if (!gInstalled.exchange(true))
        EventManager::listen(Event::VBlank, onVBlank);
}

} // namespace sonicpad
