/* RingPad Dreamcast touch layout + per-touch arbitration implementation.
 *
 * Game-neutral port of the pinned SunPad overlay mechanics. The FLUDD
 * detent/hysteresis and the C-stick are deliberately removed: the Dreamcast
 * shoulder is a plain slide (no digital-click detent) and there is one stick. */

#include "DreamcastTouchLayout.h"

#include <cmath>

namespace ringpad {

namespace {
float clampf(float v, float lo, float hi) noexcept { return v < lo ? lo : (v > hi ? hi : v); }
float absf(float v) noexcept { return v < 0.0f ? -v : v; }

unsigned control_button_bit(TouchControl c) noexcept {
    switch (c) {
        case TouchControl::Start:     return RingPadDC_Start;
        case TouchControl::A:         return RingPadDC_A;
        case TouchControl::B:         return RingPadDC_B;
        case TouchControl::X:         return RingPadDC_X;
        case TouchControl::Y:         return RingPadDC_Y;
        case TouchControl::DpadUp:    return RingPadDC_Up;
        case TouchControl::DpadDown:  return RingPadDC_Down;
        case TouchControl::DpadLeft:  return RingPadDC_Left;
        case TouchControl::DpadRight: return RingPadDC_Right;
        default:                      return 0u;
    }
}
} /* namespace */

TouchLayout default_touch_layout(bool phone) noexcept {
    /* Ordered highest hit priority first; the movement stick is last because
     * its region is the largest and must not swallow the face buttons. */
    if (phone) {
        return TouchLayout{{
            {TouchControl::Start,         0.44f, 0.10f, 0.048f},
            {TouchControl::A,             0.86f, 0.74f, 0.062f},
            {TouchControl::B,             0.93f, 0.62f, 0.052f},
            {TouchControl::X,             0.79f, 0.62f, 0.052f},
            {TouchControl::Y,             0.86f, 0.50f, 0.052f},
            {TouchControl::LeftShoulder,  0.09f, 0.13f, 0.050f},
            {TouchControl::RightShoulder, 0.91f, 0.13f, 0.050f},
            {TouchControl::DpadUp,        0.34f, 0.52f, 0.042f},
            {TouchControl::DpadDown,      0.34f, 0.76f, 0.042f},
            {TouchControl::DpadLeft,      0.26f, 0.64f, 0.042f},
            {TouchControl::DpadRight,     0.42f, 0.64f, 0.042f},
            {TouchControl::Move,          0.16f, 0.72f, 0.200f},
        }};
    }
    return TouchLayout{{
        {TouchControl::Start,         0.46f, 0.12f, 0.050f},
        {TouchControl::A,             0.88f, 0.74f, 0.055f},
        {TouchControl::B,             0.94f, 0.64f, 0.050f},
        {TouchControl::X,             0.82f, 0.64f, 0.050f},
        {TouchControl::Y,             0.88f, 0.54f, 0.050f},
        {TouchControl::LeftShoulder,  0.11f, 0.16f, 0.050f},
        {TouchControl::RightShoulder, 0.89f, 0.16f, 0.050f},
        {TouchControl::DpadUp,        0.30f, 0.52f, 0.045f},
        {TouchControl::DpadDown,      0.30f, 0.74f, 0.045f},
        {TouchControl::DpadLeft,      0.23f, 0.63f, 0.045f},
        {TouchControl::DpadRight,     0.37f, 0.63f, 0.045f},
        {TouchControl::Move,          0.15f, 0.70f, 0.185f},
    }};
}

TouchControl control_at(const TouchLayout& layout, float px, float py,
                        float view_w, float view_h) noexcept {
    const float unit = view_w < view_h ? view_w : view_h;
    for (const TouchPlacement& p : layout) {
        const float cx = p.cx * view_w, cy = p.cy * view_h;
        const float r = p.radius * unit;
        const float dx = px - cx, dy = py - cy;
        if (dx * dx + dy * dy <= r * r) return p.control;
    }
    return TouchControl::None;
}

bool stick_value(const TouchPlacement& stick, float px, float py,
                 float view_w, float view_h, float* out_x, float* out_y) noexcept {
    if (stick.control != TouchControl::Move) return false;
    const float unit = view_w < view_h ? view_w : view_h;
    const float cx = stick.cx * view_w, cy = stick.cy * view_h;
    const float r = stick.radius * unit;
    if (r <= 0.0f) { if (out_x) *out_x = 0.0f; if (out_y) *out_y = 0.0f; return true; }
    float dx = (px - cx) / r, dy = (py - cy) / r;
    const float len = std::sqrt(dx * dx + dy * dy);
    if (len > 1.0f) { dx /= len; dy /= len; }
    if (out_x) *out_x = dx;
    /* UIKit y grows downward; the canonical state keeps +Y up. */
    if (out_y) *out_y = -dy;
    return true;
}

uint8_t shoulder_slide_value(const TouchPlacement& shoulder, float px,
                             float view_w, float view_h) noexcept {
    const float unit = view_w < view_h ? view_w : view_h;
    const float cx = shoulder.cx * view_w;
    const float r = shoulder.radius * unit;
    if (r <= 0.0f) return 0;
    const float left = cx - r, width = 2.0f * r;
    const float frac = clampf((px - left) / width, 0.0f, 1.0f);
    /* Linear across the whole region: the right edge is a full press, with no
     * GameCube-style digital-click detent or hysteresis. */
    return uint8_t(std::lround(frac * 255.0f));
}

void DreamcastTouchArbiter::begin(int touch_id, float px, float py,
                                  const TouchLayout& layout, float view_w,
                                  float view_h) noexcept {
    TouchBinding binding;
    binding.control = control_at(layout, px, py, view_w, view_h);
    for (const TouchPlacement& p : layout) {
        if (p.control != binding.control) continue;
        if (p.control == TouchControl::Move) {
            stick_value(p, px, py, view_w, view_h, &binding.move_x, &binding.move_y);
        } else if (p.control == TouchControl::LeftShoulder ||
                   p.control == TouchControl::RightShoulder) {
            binding.trigger = shoulder_slide_value(p, px, view_w, view_h);
        }
        break;
    }
    active_[touch_id] = binding;
}

void DreamcastTouchArbiter::move(int touch_id, float px, float py,
                                 const TouchLayout& layout, float view_w,
                                 float view_h) noexcept {
    auto it = active_.find(touch_id);
    if (it == active_.end()) return;
    TouchBinding& binding = it->second;
    /* The control is latched at begin(); only its continuous value follows the
     * finger, so sliding off the stick cannot fire a button underneath. */
    if (binding.control == TouchControl::Move ||
        binding.control == TouchControl::LeftShoulder ||
        binding.control == TouchControl::RightShoulder) {
        for (const TouchPlacement& p : layout) {
            if (p.control != binding.control) continue;
            if (binding.control == TouchControl::Move) {
                stick_value(p, px, py, view_w, view_h, &binding.move_x, &binding.move_y);
            } else {
                binding.trigger = shoulder_slide_value(p, px, view_w, view_h);
            }
            break;
        }
    }
}

void DreamcastTouchArbiter::end(int touch_id) noexcept { active_.erase(touch_id); }

RingPadDreamcastState DreamcastTouchArbiter::state() const noexcept {
    RingPadDreamcastState out{};
    float move_x = 0.0f, move_y = 0.0f;
    for (const auto& entry : active_) {
        const TouchBinding& b = entry.second;
        out.buttons = uint16_t(unsigned(out.buttons) | control_button_bit(b.control));
        if (b.control == TouchControl::LeftShoulder) out.triggerL = b.trigger;
        if (b.control == TouchControl::RightShoulder) out.triggerR = b.trigger;
        if (b.control == TouchControl::Move) {
            /* Strongest-wins if more than one touch somehow owns the stick. */
            if (absf(b.move_x) > absf(move_x)) move_x = b.move_x;
            if (absf(b.move_y) > absf(move_y)) move_y = b.move_y;
        }
    }
    out.moveX = int8_t(std::lround(move_x * 127.0f));
    out.moveY = int8_t(std::lround(move_y * 127.0f));
    out.connected = active_.empty() ? 0 : 1;
    return out;
}

} /* namespace ringpad */
