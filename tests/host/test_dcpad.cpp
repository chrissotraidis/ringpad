/* Host tests for the DCPad layout and touch engine (port/dcpad/core). */

#include "DCPad.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace dcpad;

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        ++g_checks;                                                            \
        if (!(cond)) {                                                         \
            ++g_failures;                                                      \
            std::printf("FAIL: %s (%s:%d)\n", std::string(msg).c_str(), __FILE__, __LINE__); \
        }                                                                      \
    } while (0)

static const Control kAll[] = {Control::Stick, Control::DPad, Control::Face, Control::LTrigger,
                               Control::RTrigger, Control::Start};
static const char *name(Control c) {
    switch (c) {
        case Control::Stick: return "stick";
        case Control::DPad: return "dpad";
        case Control::Face: return "face";
        case Control::LTrigger: return "L";
        case Control::RTrigger: return "R";
        case Control::Start: return "start";
        default: return "none";
    }
}

struct Device { const char *name; Rect safe; bool phone; };
/* Landscape safe areas in points. */
static const Device kDevices[] = {
    {"iPad Air 11", {0, 24, 1180, 776}, false},
    {"iPad mini", {0, 24, 1133, 720}, false},
    {"iPad Pro 13", {0, 24, 1376, 1008}, false},
    {"iPhone 16 Pro", {62, 0, 750, 381}, true},
    {"iPhone SE", {0, 0, 667, 375}, true},
    {"iPhone 16 Pro Max", {62, 0, 832, 419}, true},
};

static bool overlaps(const Rect& a, const Rect& b, float gap) {
    return a.x < b.x + b.w + gap && b.x < a.x + a.w + gap && a.y < b.y + b.h + gap && b.y < a.y + a.h + gap;
}

static bool inside(const Rect& f, const Rect& s) {
    return f.x >= s.x - 0.5f && f.y >= s.y - 0.5f && f.x + f.w <= s.x + s.w + 0.5f && f.y + f.h <= s.y + s.h + 0.5f;
}

/* Layout sanity on every device class. */
static void test_layouts() {
    Settings s;
    for (const Device& d : kDevices) {
        Layout l = resolve_layout(d.safe, d.phone, s);
        const Rect menu{d.safe.x + d.safe.w - 56, d.safe.y, 56, 56};
        for (Control c : kAll) {
            const Rect& f = l.frame(c);
            CHECK(inside(f, d.safe), std::string(d.name) + ": " + name(c) + " inside safe area");
            CHECK(std::min(f.w, f.h) >= 34.0f, std::string(d.name) + ": " + name(c) + " at least 34 pt");
            CHECK(!overlaps(f, menu, 0), std::string(d.name) + ": " + name(c) + " clear of the menu button");
            for (Control o : kAll)
                if (int(o) > int(c))
                    CHECK(!overlaps(f, l.frame(o), 4), std::string(d.name) + ": " + name(c) + " vs " + name(o));
        }
        CHECK(l.face_radius * 2 >= 44.0f || d.phone, std::string(d.name) + ": face buttons at least 44 pt on tablets");
        CHECK(l.face_spacing * std::sqrt(2.0f) > l.face_radius * 2, std::string(d.name) + ": face buttons do not touch");
        for (Control c : kAll) {
            const Rect& f = l.frame(c);
            float x = f.midX(), y = f.midY();
            if (c == Control::Face) l.face_center(FaceA, &x, &y);
            CHECK(control_at(l, s, x, y) == c, std::string(d.name) + ": centre of " + name(c) + " resolves to it");
        }
        CHECK(control_at(l, s, d.safe.midX(), d.safe.y + d.safe.h * 0.4f) == Control::None,
              std::string(d.name) + ": the middle of the screen is free");
    }
}

static void test_dpad() {
    Settings s;
    Layout l = resolve_layout(kDevices[0].safe, false, s);
    const Rect& f = l.frame(Control::DPad);
    const float r = l.dpad_radius * 0.8f;
    struct { float deg; uint8_t want; const char *what; } cases[] = {
        {0, DPadRight, "right"}, {90, DPadUp, "up"}, {180, DPadLeft, "left"}, {270, DPadDown, "down"},
        {45, DPadUp | DPadRight, "up-right"}, {135, DPadUp | DPadLeft, "up-left"},
        {225, DPadDown | DPadLeft, "down-left"}, {315, DPadDown | DPadRight, "down-right"},
        {20, DPadRight, "20 degrees stays right"}, {70, DPadUp, "70 degrees stays up"},
    };
    for (auto& c : cases) {
        const float a = c.deg * 3.14159265f / 180.0f;
        CHECK(dpad_direction(l, f.midX() + std::cos(a) * r, f.midY() - std::sin(a) * r) == c.want, c.what);
    }
    CHECK(dpad_direction(l, f.midX() + 2, f.midY()) == 0, "centre of the D-pad is neutral");

    TouchEngine e;
    e.begin(1, f.midX(), f.midY() - r, l, s);
    CHECK(e.state().buttons == SonicPadDC_Up, "touch D-pad up");
    e.move(1, f.midX() + r, f.midY(), l, s);
    CHECK(e.state().buttons == SonicPadDC_Right, "rolling the thumb changes direction without lifting");
    e.move(1, f.midX() + r * 2.5f, f.midY() - r * 2.5f, l, s);
    CHECK(e.state().buttons == (SonicPadDC_Up | SonicPadDC_Right), "the D-pad keeps tracking outside its frame");
    e.end(1);
    CHECK(e.state().buttons == 0, "D-pad releases on lift");
}

static void test_face() {
    Settings s;
    Layout l = resolve_layout(kDevices[0].safe, false, s);
    const struct { Face f; unsigned bit; const char *n; } faces[] = {
        {FaceA, SonicPadDC_A, "A"}, {FaceB, SonicPadDC_B, "B"}, {FaceX, SonicPadDC_X, "X"}, {FaceY, SonicPadDC_Y, "Y"}};
    for (auto& f : faces) {
        float x, y;
        l.face_center(f.f, &x, &y);
        CHECK(face_buttons_at(l, x, y) == f.f, std::string(f.n) + " centre presses only " + f.n);
    }
    float ax, ay, bx, by;
    l.face_center(FaceA, &ax, &ay);
    l.face_center(FaceB, &bx, &by);
    CHECK(face_buttons_at(l, (ax + bx) / 2, (ay + by) / 2) == (FaceA | FaceB), "between A and B presses both");
    const Rect& c = l.frame(Control::Face);
    CHECK(face_buttons_at(l, c.midX(), c.midY()) != 0, "the cluster centre still presses something");

    float yx, yy;
    l.face_center(FaceY, &yx, &yy);
    CHECK(face_buttons_at(l, (ax + yx) / 2, (ay + yy) / 2) != (FaceA | FaceY), "opposite buttons never chord");

    TouchEngine e;
    e.begin(1, ax, ay, l, s);
    CHECK(e.state().buttons == SonicPadDC_A, "press A");
    e.move(1, bx, by, l, s);
    CHECK(e.state().buttons == SonicPadDC_B, "slide from A to B");
    e.begin(2, yx, yy, l, s);
    CHECK(e.state().buttons == (SonicPadDC_B | SonicPadDC_Y), "second thumb adds Y");
    e.end(1);
    CHECK(e.state().buttons == SonicPadDC_Y, "lifting one thumb keeps the other's button");
    e.end(2);
    unsigned events = e.take_press_events();
    CHECK(events == 3, "A, B and Y each produce one press event");
    CHECK(e.take_press_events() == 0, "press events are consumed");
}

static void test_triggers() {
    Settings s;
    Layout l = resolve_layout(kDevices[0].safe, false, s);
    const Rect& r = l.frame(Control::RTrigger);
    const Rect& lt = l.frame(Control::LTrigger);
    bool full = false;
    const uint8_t inner = trigger_value(l, Control::RTrigger, r.x + 1, false, &full);
    CHECK(inner > 64 && !full, "a tap anywhere on R is past the digital threshold");
    CHECK(trigger_value(l, Control::RTrigger, r.x + r.w - 1, false, &full) == 255 && full, "R outer edge is a full press");
    CHECK(trigger_value(l, Control::RTrigger, r.x + r.w * 0.69f, true, &full) == 255 && full, "full press holds inside the hysteresis band");
    trigger_value(l, Control::RTrigger, r.x + r.w * 0.69f, false, &full);
    CHECK(!full, "the same spot is not full without hysteresis");
    CHECK(trigger_value(l, Control::LTrigger, lt.x + 1, false, &full) == 255 && full, "L grows toward the left edge");
    CHECK(trigger_value(l, Control::LTrigger, lt.x + lt.w * 0.6f, false, &full) < trigger_value(l, Control::LTrigger, lt.x + lt.w * 0.2f, false, &full),
          "L increases toward its outer edge");

    TouchEngine e;
    e.begin(1, r.x + r.w * 0.2f, r.midY(), l, s);
    e.begin(2, lt.x + lt.w * 0.9f, lt.midY(), l, s);
    SonicPadDreamcastState st = e.state();
    CHECK(st.triggerR > 64 && st.triggerL > 64 && st.triggerR < 255, "both triggers analog and independent");
    e.move(1, r.x + r.w + 40, r.midY(), l, s);
    CHECK(e.state().triggerR == 255, "sliding past the edge clamps to full");
    e.move(1, l.safe.midX(), l.safe.midY(), l, s);
    CHECK(e.state().triggerR > 64, "R stays held while the finger wanders");
    e.end(1);
    CHECK(e.state().triggerR == 0 && e.state().triggerL > 0, "releasing R leaves L");
}

static void test_stick() {
    Settings s;
    Layout l = resolve_layout(kDevices[0].safe, false, s);
    const Rect& home = l.frame(Control::Stick);
    const float r = l.stick_radius;

    TouchEngine e;
    e.begin(1, home.midX(), home.midY(), l, s);
    CHECK(e.state().moveX == 0 && e.state().moveY == 0, "touching the stick centre is neutral");
    e.move(1, home.midX() + r * kStickFullTilt, home.midY(), l, s);
    CHECK(e.state().moveX == 127, "full tilt before the rim");
    e.move(1, home.midX(), home.midY() - r * 0.5f, l, s);
    SonicPadDreamcastState st = e.state();
    CHECK(st.moveY > 60 && st.moveX == 0, "dragging up is positive Y");
    e.end(1);

    /* Stick travel: a shorter travel reaches full tilt sooner; it is latched
     * at touch-down so changing the setting never jolts a held stick. */
    Settings shortTravel = s;
    shortTravel.stick_travel = 0.5f;
    e.begin(4, home.midX(), home.midY(), l, shortTravel);
    e.move(4, home.midX() + r * 0.5f, home.midY(), l, shortTravel);
    CHECK(e.state().moveX == 127, "short travel reaches full tilt at half the radius");
    e.move(4, home.midX() + r * 0.25f, home.midY(), l, s);
    CHECK(std::abs(e.state().moveX - 64) <= 1, "travel stays latched for the held touch");
    e.end(4);
    Settings tooShort = s;
    tooShort.stick_travel = 0.1f;
    e.begin(5, home.midX(), home.midY(), l, tooShort);
    e.move(5, home.midX() + r * 0.25f, home.midY(), l, tooShort);
    CHECK(std::abs(e.state().moveX - 64) <= 1, "travel is clamped to at least half the radius");
    e.end(5);

    /* Floating: a touch elsewhere in the zone becomes the stick centre. */
    const float fx = l.stick_zone.x + l.stick_zone.w * 0.85f, fy = l.stick_zone.y + l.stick_zone.h * 0.15f;
    CHECK(control_at(l, s, fx, fy) == Control::Stick, "the floating zone starts the stick");
    e.begin(2, fx, fy, l, s);
    Visual v = e.visual(l);
    CHECK(v.stick_active && std::fabs(v.stick_base_x - fx) < 1 && std::fabs(v.stick_base_y - fy) < 1,
          "the base re-centres under the thumb");
    CHECK(e.state().moveX == 0, "a floating touch starts neutral");
    e.move(2, fx - r * 3, fy, l, s);
    v = e.visual(l);
    CHECK(e.state().moveX == -127 && std::fabs(v.stick_base_x - (fx - r * 2)) < 1, "the base follows past the rim");
    e.move(2, fx - r * 2 + r * 0.85f, fy, l, s);
    CHECK(e.state().moveX == 127, "reversing direction is immediate after the base followed");
    e.end(2);
    CHECK(e.visual(l).stick_active == false && e.visual(l).stick_base_x == home.midX(), "the base returns home");

    /* Fixed mode: no floating zone, base never moves. */
    Settings fixed;
    fixed.floating_stick = false;
    Layout lf = resolve_layout(kDevices[0].safe, false, fixed);
    CHECK(control_at(lf, fixed, fx, fy) == Control::None || fx < home.x + home.w, "no floating zone in fixed mode");
    e.begin(3, home.midX(), home.midY(), lf, fixed);
    e.move(3, home.midX() + r * 4, home.midY(), lf, fixed);
    CHECK(e.visual(lf).stick_base_x == home.midX() && e.state().moveX == 127, "fixed base stays put and saturates");
    e.end(3);

    /* A second thumb on the stick is ignored, not a takeover. */
    e.begin(4, home.midX() + r * 0.5f, home.midY(), l, s);
    e.begin(5, home.midX() - r * 0.5f, home.midY(), l, s);
    CHECK(e.control_for(5) == Control::None && e.state().moveX > 0, "second stick touch ignored");
    e.clear();
    CHECK(e.active_count() == 0 && e.state().buttons == 0 && e.state().moveX == 0, "clear drops everything");
}

static void test_combined() {
    Settings s;
    Layout l = resolve_layout(kDevices[3].safe, true, s);
    const Rect& home = l.frame(Control::Stick);
    float ax, ay;
    l.face_center(FaceA, &ax, &ay);
    const Rect& r = l.frame(Control::RTrigger);
    TouchEngine e;
    e.begin(1, home.midX(), home.midY(), l, s);
    e.move(1, home.midX() + l.stick_radius, home.midY(), l, s);
    e.begin(2, ax, ay, l, s);
    e.begin(3, r.midX(), r.midY(), l, s);
    SonicPadDreamcastState st = e.state();
    CHECK(st.moveX == 127 && (st.buttons & SonicPadDC_A) && st.triggerR > 64, "run, jump and hold R together");
    e.end(2);
    st = e.state();
    CHECK(st.moveX == 127 && !(st.buttons & SonicPadDC_A) && st.triggerR > 64, "lifting A keeps run and R");
    const Rect& start = l.frame(Control::Start);
    e.begin(4, start.midX(), start.midY(), l, s);
    CHECK(e.state().buttons & SonicPadDC_Start, "Start");
}

static void test_overrides() {
    Settings s;
    s.over(Control::Face).has_position = true;
    s.over(Control::Face).nx = 0.5f;
    s.over(Control::Face).ny = 0.5f;
    s.over(Control::Stick).scale = 1.5f;
    s.over(Control::Start).has_position = true;
    s.over(Control::Start).nx = 2.0f;   /* out of range */
    Layout base = resolve_layout(kDevices[0].safe, false, Settings{});
    Layout l = resolve_layout(kDevices[0].safe, false, s);
    CHECK(std::fabs(l.frame(Control::Face).midX() - kDevices[0].safe.midX()) < 1, "moved face cluster");
    CHECK(std::fabs(l.frame(Control::Stick).w - base.frame(Control::Stick).w * 1.5f) < 1, "per-control size");
    CHECK(inside(l.frame(Control::Start), kDevices[0].safe), "out-of-range position is clamped on screen");
    Settings big;
    big.size = 1.4f;
    Layout lb = resolve_layout(kDevices[0].safe, false, big);
    CHECK(lb.frame(Control::DPad).w > base.frame(Control::DPad).w * 1.3f, "global size");
    CHECK(lb.face_spacing > base.face_spacing * 1.3f, "global size scales the cluster spacing");
}

int main() {
    test_layouts();
    test_dpad();
    test_face();
    test_triggers();
    test_stick();
    test_combined();
    test_overrides();
    std::printf("\n%d/%d checks passed\n", g_checks - g_failures, g_checks);
    if (g_failures) {
        std::printf("RESULT: FAIL (%d)\n", g_failures);
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
