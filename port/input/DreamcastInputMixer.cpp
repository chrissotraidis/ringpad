/* SonicPad Dreamcast input mixer implementation.
 *
 * Mirrors the pinned SunPad mixer (ref/sunpad/apple/shared/SunPadInputMixer.mm)
 * with the GameCube C-stick axis removed. The EdgeLatch provides the donor's
 * rising-edge latching; see DreamcastInputState.h. */

#include "DreamcastInputMixer.h"

#include <cstdlib>

namespace sonicpad {

namespace {
int abs_i8(int8_t v) noexcept { return v < 0 ? -int(v) : int(v); }
uint8_t max_u8(uint8_t a, uint8_t b) noexcept { return a > b ? a : b; }
} /* namespace */

DreamcastInputMixer& DreamcastInputMixer::shared() noexcept {
    static DreamcastInputMixer instance;
    return instance;
}

void DreamcastInputMixer::clear_locked(unsigned index) noexcept {
    states_[index] = SonicPadDreamcastState{};
    latches_[index].clear();
}

void DreamcastInputMixer::setState(const SonicPadDreamcastState& state,
                                   bool from_touch) noexcept {
    std::scoped_lock lock(mutex_);
    const unsigned index = from_touch ? 0u : 1u;
    /* Latch rising edges so a fast tap survives until the game thread polls. */
    latches_[index].press(unsigned(state.buttons) & ~unsigned(states_[index].buttons));
    states_[index] = state;
}

void DreamcastInputMixer::clear(bool from_touch) noexcept {
    std::scoped_lock lock(mutex_);
    clear_locked(from_touch ? 0u : 1u);
}

void DreamcastInputMixer::clearAll() noexcept {
    std::scoped_lock lock(mutex_);
    clear_locked(0u);
    clear_locked(1u);
}

SonicPadDreamcastState DreamcastInputMixer::consumeMerged() noexcept {
    std::scoped_lock lock(mutex_);
    const SonicPadDreamcastState& touch = states_[0];
    const SonicPadDreamcastState& controller = states_[1];

    SonicPadDreamcastState merged{};
    /* Held bits plus this sample's latched edges. */
    merged.buttons = uint16_t(unsigned(touch.buttons) | unsigned(controller.buttons) |
                              latches_[0].consume(0u) | latches_[1].consume(0u));
    merged.moveX = abs_i8(controller.moveX) > abs_i8(touch.moveX)
                       ? controller.moveX : touch.moveX;
    merged.moveY = abs_i8(controller.moveY) > abs_i8(touch.moveY)
                       ? controller.moveY : touch.moveY;
    merged.triggerL = max_u8(touch.triggerL, controller.triggerL);
    merged.triggerR = max_u8(touch.triggerR, controller.triggerR);
    merged.connected = (touch.connected || controller.connected) ? 1 : 0;
    return merged;
}

} /* namespace sonicpad */
