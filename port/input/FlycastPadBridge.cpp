/* RingPad -> Flycast controller bridge. See FlycastPadBridge.h. */

#include "FlycastPadBridge.h"

#include <cmath>

namespace ringpad {

namespace {

int16_t to_axis(float v) noexcept {
    if (v > 1.0f) v = 1.0f;
    if (v < -1.0f) v = -1.0f;
    return int16_t(std::lround(v * 32767.0f));
}

uint16_t to_trigger(float v) noexcept {
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;
    return uint16_t(std::lround(v * 65535.0f));
}

} /* namespace */

FlycastPadState to_flycast_pad(const ActionSnapshot& s) noexcept {
    static const struct { unsigned from; uint32_t to; } map[] = {
        {RingPadDC_Up, kFlycastUp},     {RingPadDC_Down, kFlycastDown},
        {RingPadDC_Left, kFlycastLeft}, {RingPadDC_Right, kFlycastRight},
        {RingPadDC_Start, kFlycastStart},
        {RingPadDC_A, kFlycastA}, {RingPadDC_B, kFlycastB},
        {RingPadDC_X, kFlycastX}, {RingPadDC_Y, kFlycastY},
    };
    FlycastPadState out;
    for (const auto& m : map)
        if (s.held_mask & m.from)
            out.pressed |= m.to;
    out.joyx = to_axis(s.move_x);
    out.joyy = to_axis(-s.move_y);   /* canonical +Y up -> Flycast +Y down */
    out.lt = to_trigger(s.left_trigger);
    out.rt = to_trigger(s.right_trigger);
    return out;
}

uint32_t FlycastPressHold::apply(uint32_t pressed) noexcept {
    uint32_t out = pressed;
    for (unsigned bit = 0; bit < 32; bit++) {
        const uint32_t mask = 1u << bit;
        if ((pressed & mask) && !(previous_ & mask))
            remaining_[bit] = uint8_t(min_ticks_);
        if (remaining_[bit] > 0) {
            out |= mask;
            remaining_[bit]--;
        }
    }
    previous_ = pressed;
    return out;
}

} /* namespace ringpad */
