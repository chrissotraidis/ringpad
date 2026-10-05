#pragma once

/* SonicPad Dreamcast input mixer.
 *
 * Reuses the pinned SunPad shell mechanism (ref/sunpad/apple/shared/
 * SunPadInputMixer) with GameCube-only semantics removed: the Dreamcast
 * profile has ONE movement stick, so there is no C-stick/right-stick axis to
 * merge. Two sources feed one canonical state:
 *   index 0 = touch overlay, index 1 = physical controller.
 *
 * Merge rules, identical in spirit to the donor:
 *   - buttons are OR'ed with rising-edge latching, so a tap that begins and
 *     ends between game samples is still observed exactly once (no turbo);
 *   - the movement stick is strongest-wins per axis (no stuck axis);
 *   - triggers are max of the two sources, kept independent and continuous;
 *   - connected is OR.
 *
 * The game thread calls consumeMerged() once per frame, which also clears the
 * consumed edges. Pure C++20, no host or guest dependency, so it is testable
 * without a device or the guest runtime. */

#include "DreamcastInputState.h"

#include <mutex>

namespace sonicpad {

class DreamcastInputMixer {
public:
    DreamcastInputMixer() = default;
    DreamcastInputMixer(const DreamcastInputMixer&) = delete;
    DreamcastInputMixer& operator=(const DreamcastInputMixer&) = delete;

    /* Process-wide instance for the shipping shell; tests use local instances. */
    static DreamcastInputMixer& shared() noexcept;

    /* Record one source's snapshot. Rising button edges are latched until the
     * next consumeMerged(), so a sub-sample tap survives. */
    void setState(const SonicPadDreamcastState& state, bool from_touch) noexcept;

    /* Drop one source's held state and pending edges. */
    void clear(bool from_touch) noexcept;

    /* Drop both sources. Use on every modal, layout-edit, owner change, focus
     * loss and lifecycle transition so no held value or queued edge survives. */
    void clearAll() noexcept;

    /* Return the merged snapshot and clear the consumed edges. */
    SonicPadDreamcastState consumeMerged() noexcept;

private:
    void clear_locked(unsigned index) noexcept;

    std::mutex mutex_;
    SonicPadDreamcastState states_[2]{};
    EdgeLatch latches_[2];
};

} /* namespace sonicpad */
