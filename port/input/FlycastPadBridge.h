#pragma once

/* SonicPad -> Flycast controller bridge.
 *
 * Converts the adapter's canonical ActionSnapshot into the values Flycast's
 * Dreamcast controller reads for port A: button bits (stored active-low in
 * kcode[]), a signed 16-bit stick with +Y down, and 16-bit analog triggers.
 * Pure C++ with no Flycast dependency; the iOS shell static_asserts that the
 * button constants below match Flycast's core/input/gamepad.h. */

#include "DreamcastInputState.h"

#include <array>
#include <cstdint>

namespace sonicpad {

/* Flycast DreamcastKey bits (core/input/gamepad.h). */
enum : uint32_t {
    kFlycastB     = 1u << 1,
    kFlycastA     = 1u << 2,
    kFlycastStart = 1u << 3,
    kFlycastUp    = 1u << 4,
    kFlycastDown  = 1u << 5,
    kFlycastLeft  = 1u << 6,
    kFlycastRight = 1u << 7,
    kFlycastY     = 1u << 9,
    kFlycastX     = 1u << 10,
};

/* Every button the SonicPad profile drives; other kcode bits stay released. */
inline constexpr uint32_t kFlycastProfileButtons = kFlycastB | kFlycastA | kFlycastStart |
    kFlycastUp | kFlycastDown | kFlycastLeft | kFlycastRight | kFlycastY | kFlycastX;

struct FlycastPadState {
    uint32_t pressed = 0;   /* Flycast button bits, active-high here */
    int16_t joyx = 0;       /* -32767..32767, +X right */
    int16_t joyy = 0;       /* -32767..32767, +Y down (Flycast convention) */
    uint16_t lt = 0;        /* 0..65535 */
    uint16_t rt = 0;        /* 0..65535 */
};

/* Map one adapted snapshot to Flycast's controller values. */
FlycastPadState to_flycast_pad(const ActionSnapshot& snapshot) noexcept;

/* Flycast's kcode word for a set of pressed profile buttons (active-low). */
inline uint32_t flycast_kcode(uint32_t pressed) noexcept { return ~(pressed & kFlycastProfileButtons); }

/* Keeps each newly pressed button down for at least min_ticks display ticks.
 * The display runs at 60 Hz while the PAL game samples input at 50 Hz, so a
 * tap that lasts a single tick could fall between two game samples. A held
 * button releases as soon as the minimum has passed. */
class FlycastPressHold {
public:
    explicit FlycastPressHold(unsigned min_ticks = 3) noexcept : min_ticks_(min_ticks) {}
    uint32_t apply(uint32_t pressed) noexcept;
    void clear() noexcept { remaining_.fill(0); previous_ = 0; }

private:
    std::array<uint8_t, 32> remaining_{};
    uint32_t previous_ = 0;
    unsigned min_ticks_;
};

} /* namespace sonicpad */
