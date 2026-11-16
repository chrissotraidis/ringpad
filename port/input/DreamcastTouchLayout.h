#pragma once

/* RingPad Dreamcast touch layout and per-touch arbitration.
 *
 * Reuses the game-neutral touch mechanics from the pinned SunPad overlay
 * (ref/sunpad/apple/ios/SunPadGameOverlay.mm: normalized centers, stick
 * normalization with +Y up, shoulder slide) with the GameCube-only controls
 * removed: there is ONE movement stick and no C-stick/Z. The Apple view layer
 * applies safe-area insets and pixel scaling; this header holds the
 * game-independent geometry and the ownership rules so they can be tested
 * without UIKit.
 *
 * Controls are circles, as in the donor (cornerRadius = half the smaller
 * dimension). A placement's center is normalized to the view width/height and
 * its radius to the smaller view dimension, so the arrangement stays usable on
 * any aspect ratio. */

#include "DreamcastInputState.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>

namespace ringpad {

enum class TouchControl : uint8_t {
    None = 0,
    Start,
    A, B, X, Y,
    LeftShoulder, RightShoulder,
    DpadUp, DpadDown, DpadLeft, DpadRight,
    Move,
};

struct TouchPlacement {
    TouchControl control;
    float cx, cy;   /* normalized center: fraction of width / height */
    float radius;   /* fraction of the smaller view dimension */
};

inline constexpr std::size_t kTouchPlacementCount = 12;
using TouchLayout = std::array<TouchPlacement, kTouchPlacementCount>;

/* Profile defaults. Ordered highest hit priority first, so a touch that lands
 * on a face button inside the (larger) movement region selects the button.
 * `phone` selects the compact arrangement; otherwise the tablet arrangement.
 * Contains exactly one movement stick and no Z/C-stick. */
TouchLayout default_touch_layout(bool phone) noexcept;

/* Resolve the topmost control under a point in view points. Returns
 * TouchControl::None when the point hits nothing. */
TouchControl control_at(const TouchLayout& layout, float px, float py,
                        float view_w, float view_h) noexcept;

/* Movement-stick value for a point inside/around the stick: normalized to
 * [-1, 1] with +Y up (UIKit is down-positive), clamped to the unit circle.
 * Returns false if the placement is not the movement stick. */
bool stick_value(const TouchPlacement& stick, float px, float py,
                 float view_w, float view_h, float* out_x, float* out_y) noexcept;

/* Shoulder analog value from slide position across the region: 0 at the left
 * edge, 255 at the right edge, with the final stretch a full press. Touch is
 * not assumed to report pressure, so position supplies the analog value. */
uint8_t shoulder_slide_value(const TouchPlacement& shoulder, float px,
                             float view_w, float view_h) noexcept;

/* One touch's latched binding. The control is chosen at begin() and does not
 * change while the touch stays down, so sliding a finger off the stick cannot
 * fire a button under it. */
struct TouchBinding {
    TouchControl control = TouchControl::None;
    float move_x = 0.0f, move_y = 0.0f;
    uint8_t trigger = 0;
};

/* Per-touch identity arbitration. Each active touch owns exactly one control;
 * lifting or cancelling one finger releases only that finger's contribution, so
 * a second finger's button is never released and P1 is never stolen. */
class DreamcastTouchArbiter {
public:
    void begin(int touch_id, float px, float py, const TouchLayout& layout,
               float view_w, float view_h) noexcept;
    void move(int touch_id, float px, float py, const TouchLayout& layout,
              float view_w, float view_h) noexcept;
    void end(int touch_id) noexcept;
    void cancel(int touch_id) noexcept { end(touch_id); }
    void clear() noexcept { active_.clear(); }

    bool is_active(int touch_id) const noexcept { return active_.count(touch_id) != 0; }
    std::size_t active_count() const noexcept { return active_.size(); }

    /* Aggregate the active touches into one canonical state. */
    RingPadDreamcastState state() const noexcept;

private:
    std::map<int, TouchBinding> active_;
};

} /* namespace ringpad */
