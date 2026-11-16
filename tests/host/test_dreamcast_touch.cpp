/* Source-derived host tests for the RingPad Dreamcast touch layout.
 *
 * Level 1 of the three input acceptance levels (state/unit tests). Covers the
 * game-neutral mechanics reused from the pinned SunPad overlay: normalized
 * placement, hit resolution with priority, stick normalization (+Y up),
 * shoulder slide without a detent, and per-touch ownership. It says nothing
 * about UIKit view-event delivery or physical multitouch. */

#include "DreamcastTouchLayout.h"
#include "DreamcastInputState.h"

#include <cmath>
#include <cstdio>

using namespace ringpad;

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        ++g_checks;                                                            \
        if (!(cond)) {                                                         \
            ++g_failures;                                                      \
            std::printf("FAIL: %s (%s:%d)\n", (msg), __FILE__, __LINE__);      \
        }                                                                      \
    } while (0)

static bool near(float a, float b, float eps = 0.01f) { return std::fabs(a - b) <= eps; }

static const TouchPlacement* find(const TouchLayout& l, TouchControl c) {
    for (const TouchPlacement& p : l) if (p.control == c) return &p;
    return nullptr;
}

static float center_px(const TouchPlacement& p, float w) { return p.cx * w; }
static float center_py(const TouchPlacement& p, float h) { return p.cy * h; }

/* 1. The profile has exactly one movement stick and no C-stick/Z control. */
static void test_layout_shape() {
    for (bool phone : {true, false}) {
        const TouchLayout l = default_touch_layout(phone);
        int moves = 0;
        for (const TouchPlacement& p : l) if (p.control == TouchControl::Move) ++moves;
        CHECK(moves == 1, "exactly one movement stick");
        CHECK(find(l, TouchControl::Start) != nullptr, "Start present");
        CHECK(find(l, TouchControl::A) && find(l, TouchControl::B) &&
                  find(l, TouchControl::X) && find(l, TouchControl::Y), "face buttons present");
        CHECK(find(l, TouchControl::LeftShoulder) && find(l, TouchControl::RightShoulder),
              "both shoulders present");
        CHECK(find(l, TouchControl::DpadUp) && find(l, TouchControl::DpadDown) &&
                  find(l, TouchControl::DpadLeft) && find(l, TouchControl::DpadRight),
              "d-pad present");
    }
}

/* 2. Each placement is hit at its center; empty space resolves to None. */
static void test_hit_resolution() {
    const float w = 844.0f, h = 390.0f;
    const TouchLayout l = default_touch_layout(true);
    for (const TouchPlacement& p : l) {
        CHECK(control_at(l, center_px(p, w), center_py(p, h), w, h) == p.control,
              "center hits its own control");
    }
    CHECK(control_at(l, 0.5f * w, 0.5f * h, w, h) == TouchControl::None,
          "empty space resolves to None");
}

/* 3. Hit priority: a face button inside a larger region still wins. */
static void test_hit_priority() {
    const float w = 400.0f, h = 400.0f;
    TouchLayout l{};
    l[0] = {TouchControl::A, 0.50f, 0.50f, 0.08f};      /* small button, first */
    l[1] = {TouchControl::Move, 0.50f, 0.50f, 0.30f};   /* big region, later */
    CHECK(control_at(l, 0.50f * w, 0.50f * h, w, h) == TouchControl::A,
          "button wins over the overlapping movement region");
    /* A point only inside the big region still resolves to the stick. */
    CHECK(control_at(l, 0.50f * w, 0.78f * h, w, h) == TouchControl::Move,
          "stick still reachable outside the button");
}

/* 4. Stick value: center neutral, right/up full, beyond radius clamped. */
static void test_stick_value() {
    const float w = 844.0f, h = 390.0f, unit = w < h ? w : h;
    const TouchLayout l = default_touch_layout(true);
    const TouchPlacement& m = *find(l, TouchControl::Move);
    const float cx = center_px(m, w), cy = center_py(m, h), r = m.radius * unit;
    float x = 0, y = 0;

    CHECK(stick_value(m, cx, cy, w, h, &x, &y), "stick recognized");
    CHECK(near(x, 0.0f) && near(y, 0.0f), "center is neutral");

    stick_value(m, cx + r, cy, w, h, &x, &y);
    CHECK(near(x, 1.0f) && near(y, 0.0f), "right edge is full right");

    /* UIKit y grows downward, so a point above center must be +Y up. */
    stick_value(m, cx, cy - r, w, h, &x, &y);
    CHECK(near(y, 1.0f) && near(x, 0.0f), "above center is full up");

    stick_value(m, cx + 5.0f * r, cy, w, h, &x, &y);
    CHECK(near(x, 1.0f) && near(y, 0.0f), "beyond radius clamps to full");
}

/* 5. Shoulder slide spans released..full with no detent jump. */
static void test_shoulder_slide() {
    const float w = 844.0f, h = 390.0f, unit = w < h ? w : h;
    const TouchLayout l = default_touch_layout(true);
    const TouchPlacement& s = *find(l, TouchControl::LeftShoulder);
    const float cx = center_px(s, w), r = s.radius * unit;

    CHECK(shoulder_slide_value(s, cx - r, w, h) == 0, "left edge released");
    CHECK(shoulder_slide_value(s, cx + r, w, h) == 255, "right edge full");
    const uint8_t mid = shoulder_slide_value(s, cx, w, h);
    CHECK(mid >= 120 && mid <= 136, "midpoint is about half");
    /* Monotonic: no detent discontinuity around three-quarters. */
    const uint8_t a = shoulder_slide_value(s, cx + r * 0.70f, w, h);
    const uint8_t b = shoulder_slide_value(s, cx + r * 0.80f, w, h);
    CHECK(b > a && (b - a) < 40, "value rises smoothly across the former detent point");
}

/* 6. Per-touch ownership: controls latch at begin and lifting one finger does
 * not release another finger's button. */
static void test_per_touch_ownership() {
    const float w = 844.0f, h = 390.0f;
    const TouchLayout l = default_touch_layout(true);
    DreamcastTouchArbiter arbiter;

    const TouchPlacement& a = *find(l, TouchControl::A);
    const TouchPlacement& m = *find(l, TouchControl::Move);
    arbiter.begin(1, center_px(a, w), center_py(a, h), l, w, h);
    arbiter.begin(2, center_px(m, w), center_py(m, h), l, w, h);
    auto s = arbiter.state();
    CHECK((s.buttons & RingPadDC_A) != 0u, "touch 1 holds A");
    CHECK(arbiter.active_count() == 2, "two touches active");

    arbiter.end(1); /* lift the button finger */
    s = arbiter.state();
    CHECK((s.buttons & RingPadDC_A) == 0u, "lifting touch 1 releases A");
    CHECK(arbiter.active_count() == 1, "touch 2 still active");
    CHECK(s.connected == 1, "still connected via the remaining finger");

    /* Slide the stick finger toward a face button: control must not change. */
    arbiter.move(2, center_px(a, w), center_py(a, h), l, w, h);
    s = arbiter.state();
    CHECK((s.buttons & RingPadDC_A) == 0u, "sliding off the stick does not fire a button");

    arbiter.end(2);
    s = arbiter.state();
    CHECK(s.buttons == 0u && s.connected == 0, "all fingers up clears the state");
}

/* 7. cancel and clear drop everything, leaving no stuck input. */
static void test_cancel_and_clear() {
    const float w = 844.0f, h = 390.0f;
    const TouchLayout l = default_touch_layout(true);
    DreamcastTouchArbiter arbiter;

    const TouchPlacement& m = *find(l, TouchControl::Move);
    const TouchPlacement& r = *find(l, TouchControl::RightShoulder);
    arbiter.begin(1, center_px(m, w) + 10.0f, center_py(m, h), l, w, h);
    arbiter.begin(2, center_px(r, w) + r.radius * (w < h ? w : h), center_py(r, h), l, w, h);
    auto s = arbiter.state();
    CHECK(s.moveX != 0, "stick deflected before cancel");
    CHECK(s.triggerR == 255, "shoulder at full before cancel");

    arbiter.cancel(1);
    s = arbiter.state();
    CHECK(s.moveX == 0 && s.moveY == 0, "cancel recenters the stick");
    CHECK(s.triggerR == 255, "cancel does not touch the other finger's shoulder");

    arbiter.clear();
    s = arbiter.state();
    CHECK(s.buttons == 0u && s.moveX == 0 && s.triggerR == 0 && s.connected == 0,
          "clear drops every held value");
}

int main() {
    test_layout_shape();
    test_hit_resolution();
    test_hit_priority();
    test_stick_value();
    test_shoulder_slide();
    test_per_touch_ownership();
    test_cancel_and_clear();

    std::printf("\n%d/%d checks passed\n", g_checks - g_failures, g_checks);
    if (g_failures) {
        std::printf("RESULT: FAIL (%d)\n", g_failures);
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
