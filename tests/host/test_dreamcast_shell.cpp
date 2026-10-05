/* Source-derived host tests for the SonicPad Dreamcast shell layer.
 *
 * Level 1 of the three input acceptance levels (state/unit tests). These cover
 * the mechanisms reused from the pinned SunPad donor, adapted to the
 * single-stick Dreamcast profile: the touch/controller mixer, the narrow
 * face-button mapping and the player-slot bookkeeping. They say nothing about
 * Simulator view-event routing or physical hardware. */

#include "DreamcastControllerMapping.h"
#include "DreamcastControllerSlots.h"
#include "DreamcastInputMixer.h"
#include "DreamcastInputState.h"

#include <cstdio>

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

static SonicPadDreamcastState make(int8_t x, int8_t y, uint16_t buttons,
                                   uint8_t l = 0, uint8_t r = 0, int connected = 1) {
    SonicPadDreamcastState s{};
    s.moveX = x; s.moveY = y; s.buttons = buttons;
    s.triggerL = l; s.triggerR = r; s.connected = connected;
    return s;
}

/* 1. A single source merges to itself; connected is reported. */
static void test_single_source() {
    DreamcastInputMixer mixer;
    mixer.setState(make(127, 0, SonicPadDC_A, 200, 0), /*from_touch=*/true);
    const auto merged = mixer.consumeMerged();
    CHECK(merged.moveX == 127, "touch stick passes through");
    CHECK((merged.buttons & SonicPadDC_A) != 0u, "touch button passes through");
    CHECK(merged.triggerL == 200, "touch trigger passes through");
    CHECK(merged.connected == 1, "connected reported");
}

/* 2. Two sources OR their buttons and take the strongest stick / max trigger. */
static void test_merge_two_sources() {
    DreamcastInputMixer mixer;
    mixer.setState(make(40, 0, SonicPadDC_A, 10, 0), /*from_touch=*/true);
    mixer.setState(make(0, 90, SonicPadDC_B, 0, 220), /*from_touch=*/false);
    const auto merged = mixer.consumeMerged();
    CHECK((merged.buttons & SonicPadDC_A) != 0u, "touch button present");
    CHECK((merged.buttons & SonicPadDC_B) != 0u, "controller button present");
    CHECK(merged.moveX == 40, "stronger X axis wins (touch)");
    CHECK(merged.moveY == 90, "stronger Y axis wins (controller)");
    CHECK(merged.triggerL == 10, "L is max of the two");
    CHECK(merged.triggerR == 220, "R is max of the two");
}

/* 3. A sub-sample tap on either source is delivered exactly once. */
static void test_mixer_edge_latch() {
    DreamcastInputMixer mixer;
    mixer.setState(make(0, 0, SonicPadDC_X), true); /* press */
    mixer.setState(make(0, 0, 0), true);            /* released before poll */
    const auto first = mixer.consumeMerged();
    CHECK((first.buttons & SonicPadDC_X) != 0u, "tap observed once");
    const auto second = mixer.consumeMerged();
    CHECK((second.buttons & SonicPadDC_X) == 0u, "tap not repeated as turbo");
}

/* 4. clearAll drops held values and queued edges on every transition. */
static void test_mixer_clear() {
    DreamcastInputMixer mixer;
    mixer.setState(make(127, 127, SonicPadDC_Start, 255, 255), true);
    mixer.setState(make(0, 0, SonicPadDC_A), false);
    mixer.clearAll();
    const auto merged = mixer.consumeMerged();
    CHECK(merged.buttons == 0u, "clearAll drops held buttons");
    CHECK(merged.moveX == 0 && merged.moveY == 0, "clearAll drops stick");
    CHECK(merged.triggerL == 0 && merged.triggerR == 0, "clearAll drops triggers");
    CHECK(merged.connected == 0, "clearAll drops connection");
}

/* 5. Default mapping is valid and maps physical A/B/X/Y to the guest bits. */
static void test_mapping_default() {
    const auto m = default_controller_mapping();
    CHECK(mapping_is_valid(m), "default mapping valid");
    const uint16_t bits = apply_controller_mapping(m, DCPhysical_A | DCPhysical_Y);
    CHECK((bits & SonicPadDC_A) != 0u, "physical A -> guest A");
    CHECK((bits & SonicPadDC_Y) != 0u, "physical Y -> guest Y");
    CHECK((bits & SonicPadDC_B) == 0u, "unpressed B clear");
}

/* 6. An invalid mapping falls back to the default rather than losing input. */
static void test_mapping_invalid_fallback() {
    DreamcastControllerMapping bad{};
    bad.gameA = DCPhysical_A;
    bad.gameB = DCPhysical_A; /* duplicate */
    bad.gameX = DCPhysical_X;
    bad.gameY = DCPhysical_Y;
    CHECK(!mapping_is_valid(bad), "duplicate mapping rejected");
    const uint16_t bits = apply_controller_mapping(bad, DCPhysical_A);
    CHECK((bits & SonicPadDC_A) != 0u, "invalid mapping uses default A");
}

/* 7. Assigning swaps with the previous owner; shoulders are allowed targets. */
static void test_mapping_assign() {
    auto m = default_controller_mapping();
    m = mapping_by_assigning(m, DreamcastFace::A, DCPhysical_ShoulderR);
    CHECK(m.gameA == DCPhysical_ShoulderR, "A now on right shoulder");
    CHECK(m.gameB == DCPhysical_B, "B unchanged");
    CHECK(mapping_is_valid(m), "swapped mapping still valid");
    m = mapping_by_assigning(m, DreamcastFace::B, DCPhysical_ShoulderR);
    CHECK(m.gameB == DCPhysical_ShoulderR, "B takes right shoulder");
    CHECK(m.gameA == DCPhysical_B, "A takes the freed physical B");
    CHECK(mapping_is_valid(m), "swap mapping still valid");
}

/* 8. Slots keep stable ownership; a second controller does not steal P1. */
static void test_controller_slots() {
    DreamcastControllerSlots slots;
    auto r = slots.reconcile({0x1000});
    CHECK(r.assigned.size() == 1 && r.assigned[0].slot == 0, "first controller gets slot 0");
    r = slots.reconcile({0x1000, 0x2000});
    CHECK(r.assigned.size() == 1 && r.assigned[0].instance == 0x2000 && r.assigned[0].slot == 1,
          "second controller gets slot 1");
    CHECK(slots.slot_for(0x1000) == 0, "P1 ownership stable");
    r = slots.reconcile({0x2000});
    CHECK(r.removed.size() == 1 && r.removed[0].instance == 0x1000 && r.removed[0].slot == 0,
          "disconnect frees exactly one slot");
    CHECK(slots.slot_for(0x2000) == 1, "surviving controller keeps its slot");
    r = slots.reconcile({0x2000, 0x3000});
    CHECK(r.assigned.size() == 1 && r.assigned[0].slot == 0, "freed slot 0 is reused");
}

int main() {
    test_single_source();
    test_merge_two_sources();
    test_mixer_edge_latch();
    test_mixer_clear();
    test_mapping_default();
    test_mapping_invalid_fallback();
    test_mapping_assign();
    test_controller_slots();

    std::printf("\n%d/%d checks passed\n", g_checks - g_failures, g_checks);
    if (g_failures) {
        std::printf("RESULT: FAIL (%d)\n", g_failures);
        return 1;
    }
    std::printf("RESULT: PASS\n");
    return 0;
}
