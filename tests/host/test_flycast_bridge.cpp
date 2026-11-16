/* Host tests for the RingPad -> Flycast controller bridge.
 *
 * Checks the button mapping, stick and trigger scaling, the +Y flip, the
 * active-low kcode word and the minimum press hold. Says nothing about UIKit
 * touch delivery or Flycast's maple sampling. */

#include "FlycastPadBridge.h"
#include "DreamcastInputState.h"

#include <cstdio>

using namespace ringpad;

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        ++g_checks;                                                            \
        if (!(cond)) {                                                         \
            ++g_failures;                                                      \
            std::printf("FAIL: %s (%s:%d)\n", (msg), __FILE__, __LINE__);     \
        }                                                                      \
    } while (0)

static void test_buttons() {
    const struct { unsigned dc; uint32_t fc; const char *name; } cases[] = {
        {RingPadDC_Up, kFlycastUp, "up"}, {RingPadDC_Down, kFlycastDown, "down"},
        {RingPadDC_Left, kFlycastLeft, "left"}, {RingPadDC_Right, kFlycastRight, "right"},
        {RingPadDC_Start, kFlycastStart, "start"}, {RingPadDC_A, kFlycastA, "A"},
        {RingPadDC_B, kFlycastB, "B"}, {RingPadDC_X, kFlycastX, "X"}, {RingPadDC_Y, kFlycastY, "Y"},
    };
    for (const auto& c : cases) {
        ActionSnapshot s;
        s.held_mask = c.dc;
        CHECK(to_flycast_pad(s).pressed == c.fc, c.name);
    }
    ActionSnapshot none;
    CHECK(to_flycast_pad(none).pressed == 0, "no buttons when nothing held");
    ActionSnapshot triggers;
    triggers.held_mask = RingPadDC_TriggerL | RingPadDC_TriggerR;
    CHECK(to_flycast_pad(triggers).pressed == 0, "trigger bits are analog only");
}

static void test_kcode() {
    CHECK(flycast_kcode(0) == 0xFFFFFFFFu, "idle kcode is all released");
    CHECK(flycast_kcode(kFlycastA) == ~kFlycastA, "A pressed is active-low");
    CHECK(flycast_kcode(1u << 0) == 0xFFFFFFFFu, "bits outside the profile stay released");
}

static void test_axes() {
    ActionSnapshot up;
    up.move_y = 1.0f;
    FlycastPadState p = to_flycast_pad(up);
    CHECK(p.joyy == -32767 && p.joyx == 0, "stick up is negative Flycast Y");
    ActionSnapshot right;
    right.move_x = 1.0f;
    CHECK(to_flycast_pad(right).joyx == 32767, "stick right is positive X");
    ActionSnapshot over;
    over.move_x = -2.0f;
    CHECK(to_flycast_pad(over).joyx == -32767, "stick clamps");
    RingPadDreamcastState tiny{};
    tiny.connected = 1;
    tiny.moveX = 5;
    CHECK(to_flycast_pad(adapt(tiny)).joyx == 0, "radial deadzone applies before the bridge");
    RingPadDreamcastState full{};
    full.connected = 1;
    full.moveY = -127;
    CHECK(to_flycast_pad(adapt(full)).joyy > 32000, "canonical down is positive Flycast Y");
}

static void test_triggers() {
    ActionSnapshot s;
    s.left_trigger = 1.0f;
    s.right_trigger = 0.5f;
    FlycastPadState p = to_flycast_pad(s);
    CHECK(p.lt == 65535, "full left trigger");
    CHECK(p.rt > 32700 && p.rt < 32800, "half right trigger");
    RingPadDreamcastState raw{};
    raw.connected = 1;
    raw.triggerL = 255;
    raw.triggerR = 0;
    p = to_flycast_pad(adapt(raw));
    CHECK(p.lt == 65535 && p.rt == 0, "triggers stay independent");
}

static void test_hold() {
    FlycastPressHold hold(3);
    CHECK(hold.apply(kFlycastA) == kFlycastA, "tap tick 1");
    CHECK(hold.apply(0) == kFlycastA, "tap held tick 2");
    CHECK(hold.apply(0) == kFlycastA, "tap held tick 3");
    CHECK(hold.apply(0) == 0, "tap released after minimum");
    for (int i = 0; i < 10; i++)
        hold.apply(kFlycastB);
    CHECK(hold.apply(0) == 0, "long press releases immediately");
    hold.apply(kFlycastX);
    hold.clear();
    CHECK(hold.apply(0) == 0, "clear drops pending holds");
    CHECK(hold.apply(kFlycastY) == kFlycastY, "press after clear starts a new hold");
}

int main() {
    test_buttons();
    test_kcode();
    test_axes();
    test_triggers();
    test_hold();
    std::printf("\n%d/%d checks passed\n", g_checks - g_failures, g_checks);
    if (g_failures) {
        std::printf("RESULT: FAIL (%d)\n", g_failures);
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
