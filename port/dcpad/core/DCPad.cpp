/* DCPad layout and touch engine. See DCPad.h. */

#include "DCPad.h"

#include <algorithm>
#include <cmath>

namespace dcpad {

namespace {

constexpr float kPi = 3.14159265f;

/* Default centres are anchored to the bottom corners, in points at size 1:
 * x is the distance from the near side edge and y the height above the bottom
 * edge. Thumbs do not grow with the screen, so a 13-inch iPad keeps the same
 * reach as an 11-inch one and the top of the screen stays clear.
 *
 * The arrangement follows the Dreamcast controller and a landscape grip: the
 * stick sits where the left thumb rests, the D-pad below and inward of it (as
 * on the real pad), L and R at the top of each thumb's arc, the face diamond
 * under the right thumb and Start just inward of it. tests/host/test_dcpad.cpp
 * checks every control against thumb_pivot() in millimetres. */
struct Spot { float x, y; };
struct Defaults {
    Spot stick, dpad, face, l, r, start;
    float stick_d, dpad_d, face_d, face_s, trig_w, trig_h, start_w, start_h;
};

constexpr Defaults kTablet = {
    {150.f, 310.f}, {240.f, 120.f}, {175.f, 310.f}, {118.f, 470.f}, {118.f, 470.f}, {250.f, 104.f},
    200.f, 150.f, 84.f, 76.f, 156.f, 58.f, 104.f, 42.f,
};

/* Phones are laid out on an 800 x 380 reference and scaled down to fit. */
constexpr Defaults kPhone = {
    {92.f, 210.f}, {226.f, 76.f}, {108.f, 196.f}, {84.f, 340.f}, {132.f, 340.f}, {232.f, 52.f},
    136.f, 108.f, 60.f, 55.f, 112.f, 46.f, 88.f, 44.f,
};

float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

/* Keep a frame inside the safe area (frames larger than it are centred). */
Rect clamp_into(Rect f, const Rect& safe) {
    f.x = f.w >= safe.w ? safe.midX() - f.w * 0.5f : clampf(f.x, safe.x, safe.x + safe.w - f.w);
    f.y = f.h >= safe.h ? safe.midY() - f.h * 0.5f : clampf(f.y, safe.y, safe.y + safe.h - f.h);
    return f;
}

float dist(float ax, float ay, float bx, float by) { return std::hypot(ax - bx, ay - by); }

bool in_circle(const Rect& f, float x, float y, float slop) {
    const float r = std::min(f.w, f.h) * 0.5f * slop;
    return dist(x, y, f.midX(), f.midY()) <= r;
}

bool in_rect_slop(const Rect& f, float x, float y, float pad) {
    return x >= f.x - pad && x <= f.x + f.w + pad && y >= f.y - pad && y <= f.y + f.h + pad;
}

} /* namespace */

void Layout::face_center(Face f, float* x, float* y) const {
    const Rect& c = frame(Control::Face);
    float dx = 0, dy = 0;
    switch (f) {
        case FaceA: dy = face_spacing; break;
        case FaceB: dx = face_spacing; break;
        case FaceX: dx = -face_spacing; break;
        case FaceY: dy = -face_spacing; break;
    }
    *x = c.midX() + dx;
    *y = c.midY() + dy;
}

Layout resolve_layout(const Rect& safe, bool phone, const Settings& settings) {
    Layout out;
    out.safe = safe;
    out.phone = phone;
    const Defaults& d = phone ? kPhone : kTablet;
    /* Phones scale to an 800 x 380 reference safe area, as SunPad does. */
    const float base = phone ? std::min(1.0f, std::min(safe.w / 800.0f, safe.h / 380.0f)) : 1.0f;
    const float global = clampf(settings.size, 0.7f, 1.4f) * base;

    /* Offsets scale with the global size so a larger pad grows out of the
     * corners without its groups running into each other. */
    auto place = [&](Control c, Spot spot, bool right, float w, float h) {
        const ControlOverride& o = settings.over(c);
        const float s = global * clampf(o.scale, 0.6f, 1.75f);
        float cx, cy;
        if (o.has_position) {
            cx = safe.x + clampf(o.nx, 0, 1) * safe.w;
            cy = safe.y + clampf(o.ny, 0, 1) * safe.h;
        } else {
            cx = right ? safe.x + safe.w - spot.x * global : safe.x + spot.x * global;
            cy = safe.y + safe.h - spot.y * global;
        }
        out.frame(c) = clamp_into({cx - w * s * 0.5f, cy - h * s * 0.5f, w * s, h * s}, safe);
        return s;
    };
    const float ss = place(Control::Stick, d.stick, false, d.stick_d, d.stick_d);
    out.stick_radius = d.stick_d * ss * 0.5f;
    const float ds = place(Control::DPad, d.dpad, false, d.dpad_d, d.dpad_d);
    out.dpad_radius = d.dpad_d * ds * 0.5f;
    const float cluster = 2.0f * d.face_s + d.face_d;
    const float fs = place(Control::Face, d.face, true, cluster, cluster);
    out.face_spacing = d.face_s * fs;
    out.face_radius = d.face_d * fs * 0.5f;
    place(Control::LTrigger, d.l, false, d.trig_w, d.trig_h);
    place(Control::RTrigger, d.r, true, d.trig_w, d.trig_h);
    place(Control::Start, d.start, true, d.start_w, d.start_h);

    /* Floating stick: the lower-left part of the screen, from the top of the
     * stick's home position down, up to 42% of the width. */
    const Rect& st = out.frame(Control::Stick);
    const float top = std::min(st.y, safe.y + safe.h * 0.45f);
    const float right = std::max(st.x + st.w, safe.x + safe.w * 0.42f);
    out.stick_zone = {safe.x, top, right - safe.x, safe.y + safe.h - top};
    return out;
}

void thumb_pivot(const Layout& l, bool left_hand, float* x, float* y) {
    *x = left_hand ? l.safe.x : l.safe.x + l.safe.w;
    /* A larger tablet is still gripped at about the same height above its
     * bottom edge, so the tablet pivot is capped at the iPad Air's 233 pt. */
    const float above = l.phone ? l.safe.h * 0.12f : std::min(l.safe.h * 0.30f, 233.0f);
    *y = l.safe.y + l.safe.h - above;
}

Control control_at(const Layout& l, const Settings& settings, float x, float y) {
    const float slop = l.phone ? 8.0f : 10.0f;
    if (in_rect_slop(l.frame(Control::Start), x, y, slop)) return Control::Start;
    if (in_rect_slop(l.frame(Control::LTrigger), x, y, slop)) return Control::LTrigger;
    if (in_rect_slop(l.frame(Control::RTrigger), x, y, slop)) return Control::RTrigger;
    if (in_rect_slop(l.frame(Control::DPad), x, y, slop * 0.5f)) return Control::DPad;
    const Rect& face = l.frame(Control::Face);
    if (dist(x, y, face.midX(), face.midY()) <= l.face_spacing + l.face_radius * 1.25f) return Control::Face;
    if (in_circle(l.frame(Control::Stick), x, y, 1.2f)) return Control::Stick;
    if (settings.floating_stick && l.stick_zone.contains(x, y)) return Control::Stick;
    return Control::None;
}

uint8_t dpad_direction(const Layout& l, float x, float y) {
    const Rect& f = l.frame(Control::DPad);
    const float dx = x - f.midX(), dy = y - f.midY();
    if (std::hypot(dx, dy) < l.dpad_radius * 0.2f) return 0;
    /* Cardinals own 60 degrees each, diagonals 30: easy to hold a direction,
     * still possible to roll into a diagonal. */
    float a = std::atan2(-dy, dx) * 180.0f / kPi;   /* 0 = right, 90 = up */
    if (a < 0) a += 360.0f;
    /* Twelve 30-degree sectors from 0 degrees: each cardinal spans
     * [-30, 30) around its axis and each diagonal the 30 degrees between. */
    const int sector = std::min(11, int(a / 30.0f));
    static const uint8_t map[12] = {
        DPadRight, DPadRight | DPadUp, DPadUp, DPadUp, DPadUp | DPadLeft, DPadLeft,
        DPadLeft, DPadLeft | DPadDown, DPadDown, DPadDown, DPadDown | DPadRight, DPadRight,
    };
    return map[sector];
}

uint8_t face_buttons_at(const Layout& l, float x, float y) {
    const Face faces[4] = {FaceA, FaceB, FaceX, FaceY};
    /* Adjacent buttons are face_spacing * sqrt(2) apart; a hit radius of 0.78
     * spacing gives a small overlap in each gap, so a thumb resting between
     * two buttons presses both (A+B and similar chords). */
    const float hit = l.face_spacing * 0.78f;
    uint8_t bits = 0;
    float best = 1e9f;
    uint8_t nearest = 0;
    for (Face f : faces) {
        float cx, cy;
        l.face_center(f, &cx, &cy);
        const float dd = dist(x, y, cx, cy);
        if (dd <= hit) bits |= f;
        if (dd < best) { best = dd; nearest = f; }
    }
    return bits ? bits : nearest;
}

uint8_t trigger_value(const Layout& l, Control trigger, float x, bool was_full, bool* full) {
    const Rect& f = l.frame(trigger);
    float pos = f.w > 0 ? clampf((x - f.x) / f.w, 0, 1) : 0;
    if (trigger == Control::LTrigger) pos = 1.0f - pos;   /* both grow toward the screen edge */
    constexpr float kEnter = kTriggerFullAt, kExit = kTriggerFullAt - 0.06f, kMin = 0.45f, kMax = 0.89f;
    const bool is_full = was_full ? pos >= kExit : pos >= kEnter;
    if (full) *full = is_full;
    const float v = is_full ? 1.0f : std::min(kMax, kMin + (pos / kEnter) * (kMax - kMin));
    return uint8_t(std::lround(v * 255.0f));
}

void TouchEngine::count_new(uint8_t before, uint8_t after) {
    const uint8_t added = uint8_t(after & ~before);
    for (uint8_t b = added; b; b &= uint8_t(b - 1)) press_events_++;
}

void TouchEngine::update(Touch& t, float x, float y, const Layout& l) {
    switch (t.control) {
    case Control::Stick: {
        /* The base stays where the thumb landed. Past the rim the knob stops
         * at the edge and the stick holds full tilt in the thumb's direction,
         * so the stick never wanders across the screen with the thumb. */
        const float dx = x - t.base_x, dy = y - t.base_y;
        const float r = std::max(1.0f, l.stick_radius);
        const float reach = r * clampf(t.travel, 0.5f, 1.0f);
        float vx = dx / reach, vy = dy / reach;
        const float m = std::hypot(vx, vy);
        if (m > 1.0f) { vx /= m; vy /= m; }
        t.x = vx;
        t.y = vy;
        break;
    }
    case Control::DPad: {
        const uint8_t before = t.bits;
        t.bits = dpad_direction(l, x, y);
        count_new(before, t.bits);
        break;
    }
    case Control::Face: {
        const uint8_t before = t.bits;
        t.bits = face_buttons_at(l, x, y);
        count_new(before, t.bits);
        break;
    }
    case Control::LTrigger:
    case Control::RTrigger: {
        const bool was = t.full;
        t.trigger = trigger_value(l, t.control, x, was, &t.full);
        if (t.full && !was) press_events_++;
        break;
    }
    default:
        break;
    }
}

void TouchEngine::begin(int id, float x, float y, const Layout& l, const Settings& s) {
    Touch t;
    t.control = control_at(l, s, x, y);
    /* One touch per single-owner control: a second finger landing on a stick,
     * D-pad or trigger that is already held is ignored. */
    if (t.control == Control::Stick || t.control == Control::DPad ||
            t.control == Control::LTrigger || t.control == Control::RTrigger) {
        for (const auto& kv : touches_)
            if (kv.second.control == t.control) t.control = Control::None;
    }
    if (t.control == Control::Stick) {
        const Rect& home = l.frame(Control::Stick);
        t.floating = s.floating_stick;
        t.travel = s.stick_travel;
        if (s.floating_stick && !in_circle(home, x, y, 1.0f)) {
            /* Keep the whole base on screen. */
            t.base_x = clampf(x, l.safe.x + l.stick_radius, l.safe.x + l.safe.w - l.stick_radius);
            t.base_y = clampf(y, l.safe.y + l.stick_radius, l.safe.y + l.safe.h - l.stick_radius);
        } else {
            t.base_x = home.midX();
            t.base_y = home.midY();
        }
    }
    if (t.control == Control::Start) press_events_++;
    if (t.control == Control::LTrigger || t.control == Control::RTrigger) press_events_++;
    update(t, x, y, l);
    touches_[id] = t;
}

void TouchEngine::move(int id, float x, float y, const Layout& l, const Settings&) {
    auto it = touches_.find(id);
    if (it != touches_.end()) update(it->second, x, y, l);
}

void TouchEngine::end(int id) { touches_.erase(id); }

void TouchEngine::clear() {
    touches_.clear();
    press_events_ = 0;
}

Control TouchEngine::control_for(int id) const {
    auto it = touches_.find(id);
    return it == touches_.end() ? Control::None : it->second.control;
}

unsigned TouchEngine::take_press_events() {
    const unsigned n = press_events_;
    press_events_ = 0;
    return n;
}

SonicPadDreamcastState TouchEngine::state() const {
    SonicPadDreamcastState out{};
    float sx = 0, sy = 0;
    for (const auto& kv : touches_) {
        const Touch& t = kv.second;
        switch (t.control) {
        case Control::Stick: sx = t.x; sy = t.y; break;
        case Control::DPad:
            if (t.bits & DPadUp) out.buttons |= SonicPadDC_Up;
            if (t.bits & DPadDown) out.buttons |= SonicPadDC_Down;
            if (t.bits & DPadLeft) out.buttons |= SonicPadDC_Left;
            if (t.bits & DPadRight) out.buttons |= SonicPadDC_Right;
            break;
        case Control::Face:
            if (t.bits & FaceA) out.buttons |= SonicPadDC_A;
            if (t.bits & FaceB) out.buttons |= SonicPadDC_B;
            if (t.bits & FaceX) out.buttons |= SonicPadDC_X;
            if (t.bits & FaceY) out.buttons |= SonicPadDC_Y;
            break;
        case Control::LTrigger: out.triggerL = std::max(out.triggerL, t.trigger); break;
        case Control::RTrigger: out.triggerR = std::max(out.triggerR, t.trigger); break;
        case Control::Start: out.buttons |= SonicPadDC_Start; break;
        default: break;
        }
    }
    out.moveX = int8_t(std::lround(clampf(sx, -1, 1) * 127.0f));
    out.moveY = int8_t(std::lround(clampf(-sy, -1, 1) * 127.0f));   /* canonical +Y is up */
    out.connected = 1;
    return out;
}

Visual TouchEngine::visual(const Layout& l) const {
    Visual v;
    const Rect& home = l.frame(Control::Stick);
    v.stick_base_x = home.midX();
    v.stick_base_y = home.midY();
    for (const auto& kv : touches_) {
        const Touch& t = kv.second;
        switch (t.control) {
        case Control::Stick:
            v.stick_active = true;
            v.stick_base_x = t.base_x;
            v.stick_base_y = t.base_y;
            v.stick_x = t.x;
            v.stick_y = t.y;
            break;
        case Control::DPad: v.dpad |= t.bits; break;
        case Control::Face: v.face |= t.bits; break;
        case Control::LTrigger: v.l = std::max(v.l, t.trigger); v.l_full = v.l_full || t.full; break;
        case Control::RTrigger: v.r = std::max(v.r, t.trigger); v.r_full = v.r_full || t.full; break;
        case Control::Start: v.start = true; break;
        default: break;
        }
    }
    return v;
}

} /* namespace dcpad */
