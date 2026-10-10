/* Source-derived host tests for the SonicPad Dreamcast input contract.
 *
 * Level 1 of the three input acceptance levels (state/adapter unit tests).
 * These assert the adapter reproduces SARecomp's retained guest boundary
 * (ref/sarecomp/src/sonic_input_bindings.hpp) rather than a guessed mask. They
 * say nothing about Simulator view-event routing or physical hardware. */

#include "DreamcastInputState.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace sonicpad;

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

static bool near(float a, float b, float eps = 0.002f) { return std::fabs(a - b) <= eps; }

static SonicPadDreamcastState make(int8_t x, int8_t y, uint16_t buttons,
                                   uint8_t l = 0, uint8_t r = 0, int connected = 1) {
    SonicPadDreamcastState s{};
    s.moveX = x; s.moveY = y; s.buttons = buttons;
    s.triggerL = l; s.triggerR = r; s.connected = connected;
    return s;
}

/* 1. Guest mask constants match the retained boundary. */
static void test_mask_identity() {
    CHECK(SonicPadDC_Up == (1u << 0), "up bit");
    CHECK(SonicPadDC_Down == (1u << 1), "down bit");
    CHECK(SonicPadDC_Left == (1u << 2), "left bit");
    CHECK(SonicPadDC_Right == (1u << 3), "right bit");
    CHECK(SonicPadDC_Start == (1u << 4), "start bit");
    CHECK(SonicPadDC_A == (1u << 10), "A bit");
    CHECK(SonicPadDC_B == (1u << 11), "B bit");
    CHECK(SonicPadDC_X == (1u << 12), "X bit");
    CHECK(SonicPadDC_Y == (1u << 13), "Y bit");
    CHECK(SonicPadDC_TriggerL == (1u << 14), "L trigger bit");
    CHECK(SonicPadDC_TriggerR == (1u << 15), "R trigger bit");
}

/* 2. Neutral and disconnected states produce no motion or held bits. */
static void test_neutral_and_disconnected() {
    const auto neutral = adapt(make(0, 0, 0));
    CHECK(neutral.held_mask == 0u, "neutral holds nothing");
    CHECK(near(neutral.move_x, 0.0f) && near(neutral.move_y, 0.0f), "neutral centered");

    const auto off = adapt(make(127, 127, 0xFFFF, 255, 255, 0));
    CHECK(off.held_mask == 0u, "disconnected holds nothing");
    CHECK(near(off.move_x, 0.0f) && near(off.move_y, 0.0f), "disconnected centered");
    CHECK(near(off.left_trigger, 0.0f) && near(off.right_trigger, 0.0f), "disconnected triggers 0");
}

/* 3. Cardinal movement maps to full-range axes. */
static void test_cardinals() {
    const auto right = adapt(make(127, 0, 0));
    CHECK(near(right.move_x, 1.0f) && near(right.move_y, 0.0f), "full right");
    const auto up = adapt(make(0, 127, 0));
    CHECK(near(up.move_y, 1.0f) && near(up.move_x, 0.0f), "full up");
    const auto left = adapt(make(-127, 0, 0));
    CHECK(near(left.move_x, -1.0f), "full left");
    const auto down = adapt(make(0, -127, 0));
    CHECK(near(down.move_y, -1.0f), "full down");
}

/* 4. Radial deadzone: center dead, no stuck axis, diagonals preserved. */
static void test_deadzone_and_diagonals() {
    const auto jitter = adapt(make(6, 6, 0)); /* ~6.7% magnitude, inside 12% */
    CHECK(near(jitter.move_x, 0.0f) && near(jitter.move_y, 0.0f), "inside deadzone is zero");

    const auto diag = adapt(make(127, 127, 0));
    CHECK(near(diag.move_x, 0.7071f, 0.01f) && near(diag.move_y, 0.7071f, 0.01f),
          "diagonal preserves 45 degrees");
    const float mag = std::sqrt(diag.move_x * diag.move_x + diag.move_y * diag.move_y);
    CHECK(near(mag, 1.0f, 0.01f), "diagonal reaches full magnitude");

    /* One axis fully deflected, other just outside deadzone: direction intact. */
    float ox = 0, oy = 0;
    apply_radial_deadzone(1.0f, 0.05f, 12, &ox, &oy);
    CHECK(oy > 0.0f && ox > 0.0f, "near-axis keeps a small orthogonal component");
}

/* 5. Face / start / d-pad bits pass through unchanged. */
static void test_buttons() {
    const uint16_t all = SonicPadDC_Up | SonicPadDC_A | SonicPadDC_B | SonicPadDC_X |
                         SonicPadDC_Y | SonicPadDC_Start;
    const auto snap = adapt(make(0, 0, all));
    CHECK(held(snap, DreamcastAction::A), "A held");
    CHECK(held(snap, DreamcastAction::B), "B held");
    CHECK(held(snap, DreamcastAction::X), "X held");
    CHECK(held(snap, DreamcastAction::Y), "Y held");
    CHECK(held(snap, DreamcastAction::Start), "Start held");
    CHECK(held(snap, DreamcastAction::Up), "Up held");
    CHECK(!held(snap, DreamcastAction::Down), "Down not held");
}

/* 6. Triggers are independent, continuous, and threshold-crossing to bits. */
static void test_triggers() {
    const auto below = adapt(make(0, 0, 0, 64, 64));
    CHECK((below.held_mask & SonicPadDC_TriggerL) == 0u, "L at threshold: no bit");
    CHECK(!held(below, DreamcastAction::LeftTrigger), "L at 64 reads released");

    const auto l_only = adapt(make(0, 0, 0, 200, 0));
    CHECK((l_only.held_mask & SonicPadDC_TriggerL) != 0u, "L above threshold: bit set");
    CHECK((l_only.held_mask & SonicPadDC_TriggerR) == 0u, "R stays clear");
    CHECK(near(l_only.left_trigger, 200.0f / 255.0f), "L analog value preserved");
    CHECK(near(l_only.right_trigger, 0.0f), "R analog value zero");

    const auto r_only = adapt(make(0, 0, 0, 0, 255));
    CHECK((r_only.held_mask & SonicPadDC_TriggerR) != 0u, "R full: bit set");
    CHECK((r_only.held_mask & SonicPadDC_TriggerL) == 0u, "L stays clear");

    /* No digital-click detent: a value just over the threshold is a held bit. */
    const auto edge = adapt(make(0, 0, 0, 65, 0));
    CHECK(held(edge, DreamcastAction::LeftTrigger), "65 crosses threshold");
}

/* 7. Edge latch: a tap between samples is delivered exactly once. */
static void test_edge_latch() {
    EdgeLatch latch;
    /* Press and release happen between two game samples. */
    latch.press(SonicPadDC_A);
    const auto tap = adapt(make(0, 0, 0)); /* finger already lifted */
    const unsigned delivered = latch.consume(tap.held_mask);
    CHECK((delivered & SonicPadDC_A) != 0u, "short tap observed once");

    const unsigned next = latch.consume(tap.held_mask);
    CHECK((next & SonicPadDC_A) == 0u, "tap not repeated as turbo");

    /* A modal transition clears a queued edge. */
    latch.press(SonicPadDC_Start);
    latch.clear();
    const unsigned after_clear = latch.consume(0);
    CHECK((after_clear & SonicPadDC_Start) == 0u, "cleared edge does not survive");
}

/* 8. Holding a button is visible every sample; edge latch must not merge. */
static void test_hold_then_release() {
    EdgeLatch latch;
    const auto hold = adapt(make(0, 0, SonicPadDC_B));
    CHECK((latch.consume(hold.held_mask) & SonicPadDC_B) != 0u, "hold visible sample 1");
    CHECK((latch.consume(hold.held_mask) & SonicPadDC_B) != 0u, "hold visible sample 2");
    const auto release = adapt(make(0, 0, 0));
    CHECK((latch.consume(release.held_mask) & SonicPadDC_B) == 0u, "release visible");
}

int main() {
    test_mask_identity();
    test_neutral_and_disconnected();
    test_cardinals();
    test_deadzone_and_diagonals();
    test_buttons();
    test_triggers();
    test_edge_latch();
    test_hold_then_release();

    std::printf("\n%d/%d checks passed\n", g_checks - g_failures, g_checks);
    if (g_failures) {
        std::printf("RESULT: FAIL (%d)\n", g_failures);
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
