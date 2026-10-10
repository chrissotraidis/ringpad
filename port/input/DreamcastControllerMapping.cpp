/* SonicPad Dreamcast physical-controller mapping implementation.
 *
 * Ported from ref/sunpad/apple/shared/SunPadControllerMapping.mm with the
 * GameCube Z slot and the FLUDD shoulder-pressure helper removed. The
 * Dreamcast analog L/R stay continuous and independent and are not part of
 * this mapping; the right shoulder is an allowed face target only. */

#include "DreamcastControllerMapping.h"

#include "DreamcastInputState.h"

namespace sonicpad {

namespace {
constexpr uint8_t kAllowedPhysical =
    DCPhysical_A | DCPhysical_B | DCPhysical_X | DCPhysical_Y |
    DCPhysical_ShoulderL | DCPhysical_ShoulderR;

uint8_t* slot_for(DreamcastControllerMapping& m, DreamcastFace face) noexcept {
    switch (face) {
        case DreamcastFace::A: return &m.gameA;
        case DreamcastFace::B: return &m.gameB;
        case DreamcastFace::X: return &m.gameX;
        case DreamcastFace::Y: return &m.gameY;
        default: return nullptr;
    }
}
} /* namespace */

DreamcastControllerMapping default_controller_mapping() noexcept {
    return DreamcastControllerMapping{};
}

bool mapping_is_valid(const DreamcastControllerMapping& m) noexcept {
    const uint8_t values[dreamcast_face_count] = {m.gameA, m.gameB, m.gameX, m.gameY};
    uint8_t seen = 0;
    for (uint8_t value : values) {
        if (value == 0 || (value & (value - 1)) != 0 || (value & ~kAllowedPhysical) != 0 ||
            (seen & value) != 0) {
            return false;
        }
        seen |= value;
    }
    /* Four distinct single bits, so all four actions are covered. */
    return seen != 0;
}

uint16_t apply_controller_mapping(const DreamcastControllerMapping& mapping,
                                  uint8_t pressed_physical) noexcept {
    const DreamcastControllerMapping m =
        mapping_is_valid(mapping) ? mapping : default_controller_mapping();
    uint16_t bits = 0;
    if (pressed_physical & m.gameA) bits |= SonicPadDC_A;
    if (pressed_physical & m.gameB) bits |= SonicPadDC_B;
    if (pressed_physical & m.gameX) bits |= SonicPadDC_X;
    if (pressed_physical & m.gameY) bits |= SonicPadDC_Y;
    return bits;
}

DreamcastControllerMapping mapping_by_assigning(DreamcastControllerMapping mapping,
                                                DreamcastFace face,
                                                uint8_t physical_button) noexcept {
    if (!mapping_is_valid(mapping)) mapping = default_controller_mapping();
    uint8_t* destination = slot_for(mapping, face);
    if (destination == nullptr) return mapping;
    const uint8_t previous = *destination;
    if (previous == physical_button) return mapping;
    for (unsigned i = 0; i < dreamcast_face_count; ++i) {
        uint8_t* slot = slot_for(mapping, DreamcastFace(i));
        if (slot != nullptr && *slot == physical_button) {
            *slot = previous;
            break;
        }
    }
    *destination = physical_button;
    return mapping;
}

const char* physical_button_name(uint8_t button) noexcept {
    switch (button) {
        case DCPhysical_A: return "A";
        case DCPhysical_B: return "B";
        case DCPhysical_X: return "X";
        case DCPhysical_Y: return "Y";
        case DCPhysical_ShoulderL: return "Left Shoulder";
        case DCPhysical_ShoulderR: return "Right Shoulder";
        default: return "Unknown";
    }
}

} /* namespace sonicpad */
