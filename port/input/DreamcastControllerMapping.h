#pragma once

/* RingPad Dreamcast physical-controller mapping.
 *
 * Reuses the pinned SunPad narrow remapping mechanism (ref/sunpad/apple/
 * shared/SunPadControllerMapping) with GameCube-only semantics removed: the
 * Dreamcast profile remaps only the four face buttons A/B/X/Y. There is no Z
 * and no C-stick, and the analog L/R shoulders, D-pad, Start and the single
 * movement stick keep their direct mappings and stay outside this store.
 *
 * Pure C++20: the donor persisted the mapping through NSUserDefaults; the
 * Apple shell keeps that persistence, while this contract only validates and
 * applies the mapping so it can be tested without a device. */

#include <stdint.h>

namespace ringpad {

/* Physical buttons a face action may be assigned to. The Apple GameController
 * layer maps its own buttons into this small set before calling apply(). */
enum DreamcastPhysicalButton : uint8_t {
    DCPhysical_A      = 1u << 0,
    DCPhysical_B      = 1u << 1,
    DCPhysical_X      = 1u << 2,
    DCPhysical_Y      = 1u << 3,
    DCPhysical_ShoulderL = 1u << 4,
    DCPhysical_ShoulderR = 1u << 5,
};

/* One physical button per Dreamcast face action. Defaults mirror the donor's
 * positional default (physical A/B/X/Y -> game A/B/X/Y). */
struct DreamcastControllerMapping {
    uint8_t gameA = DCPhysical_A;
    uint8_t gameB = DCPhysical_B;
    uint8_t gameX = DCPhysical_X;
    uint8_t gameY = DCPhysical_Y;
};

/* The four assignable face actions, in canonical order. */
enum class DreamcastFace : unsigned { A, B, X, Y, Count };
inline constexpr unsigned dreamcast_face_count = unsigned(DreamcastFace::Count);

DreamcastControllerMapping default_controller_mapping() noexcept;

/* True when each action maps to a distinct, single, allowed face button. */
bool mapping_is_valid(const DreamcastControllerMapping&) noexcept;

/* Convert pressed physical buttons into the guest face-button bits
 * (RingPadDC_A/B/X/Y). An invalid mapping falls back to the default. */
uint16_t apply_controller_mapping(const DreamcastControllerMapping&,
                                  uint8_t pressed_physical) noexcept;

/* Assign a physical button to a face action, swapping with whichever action
 * held it. Mirrors the donor's by-assigning behavior. */
DreamcastControllerMapping mapping_by_assigning(DreamcastControllerMapping,
                                                DreamcastFace face,
                                                uint8_t physical_button) noexcept;

/* Human-readable physical-button name, for the settings UI. */
const char* physical_button_name(uint8_t button) noexcept;

} /* namespace ringpad */
