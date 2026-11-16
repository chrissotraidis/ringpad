/* RingPad Dreamcast input adapter implementation.
 *
 * Pure C++20, no host or guest dependencies. The guest mask values mirror
 * SARecomp src/sonic_input_bindings.hpp so this adapter is a verified copy of
 * the retained boundary. */

#include "DreamcastInputState.h"

#include <cmath>

namespace ringpad {

namespace {
unsigned clamped_trigger_bits(const RingPadDreamcastState& s, int threshold) noexcept {
    unsigned bits = 0;
    const int t = threshold < 0 ? 0 : (threshold > 255 ? 255 : threshold);
    if (int(s.triggerL) > t) bits |= RingPadDC_TriggerL;
    if (int(s.triggerR) > t) bits |= RingPadDC_TriggerR;
    return bits;
}
} /* namespace */

float apply_radial_deadzone(float x, float y, unsigned deadzone_percent,
                            float* out_x, float* out_y) noexcept {
    const float dz = (deadzone_percent >= 100 ? 0.99f : float(deadzone_percent) / 100.0f);
    const float mag = std::sqrt(x * x + y * y);
    if (mag <= dz || mag <= 0.0f) {
        if (out_x) *out_x = 0.0f;
        if (out_y) *out_y = 0.0f;
        return 0.0f;
    }
    const float scaled = (mag - dz) / (1.0f - dz);
    const float capped = scaled > 1.0f ? 1.0f : scaled;
    const float nx = x / mag, ny = y / mag;
    if (out_x) *out_x = nx * capped;
    if (out_y) *out_y = ny * capped;
    return capped;
}

ActionSnapshot adapt(const RingPadDreamcastState& state,
                     unsigned deadzone_percent, int trigger_threshold) noexcept {
    ActionSnapshot out;
    if (!state.connected) return out;

    /* Digital/latching buttons are copied verbatim; only bits we own are kept. */
    const unsigned digital = unsigned(state.buttons) & 0x3FFFu; /* bits 0..13 */
    out.held_mask = digital | clamped_trigger_bits(state, trigger_threshold);

    /* Movement stick: normalized int8 -> [-1, 1], then circular deadzone. */
    float mx = float(state.moveX) / 127.0f;
    float my = float(state.moveY) / 127.0f;
    if (mx > 1.0f) mx = 1.0f; if (mx < -1.0f) mx = -1.0f;
    if (my > 1.0f) my = 1.0f; if (my < -1.0f) my = -1.0f;
    apply_radial_deadzone(mx, my, deadzone_percent, &out.move_x, &out.move_y);

    /* Triggers stay independent and continuous; the held bit is only the
     * threshold crossing, so analog range is preserved for actions that read
     * the value rather than the bit. */
    out.left_trigger = float(state.triggerL) / 255.0f;
    out.right_trigger = float(state.triggerR) / 255.0f;
    return out;
}

unsigned action_pad_bit(DreamcastAction action) noexcept {
    switch (action) {
        case DreamcastAction::Up:    return RingPadDC_Up;
        case DreamcastAction::Down:  return RingPadDC_Down;
        case DreamcastAction::Left:  return RingPadDC_Left;
        case DreamcastAction::Right: return RingPadDC_Right;
        case DreamcastAction::A:     return RingPadDC_A;
        case DreamcastAction::B:     return RingPadDC_B;
        case DreamcastAction::X:     return RingPadDC_X;
        case DreamcastAction::Y:     return RingPadDC_Y;
        case DreamcastAction::Start: return RingPadDC_Start;
        default: return 0;
    }
}

bool held(const ActionSnapshot& snap, DreamcastAction action) noexcept {
    unsigned bit = action_pad_bit(action);
    if (action == DreamcastAction::LeftTrigger) bit = RingPadDC_TriggerL;
    if (action == DreamcastAction::RightTrigger) bit = RingPadDC_TriggerR;
    return bit != 0 && (snap.held_mask & bit) != 0;
}

} /* namespace ringpad */
