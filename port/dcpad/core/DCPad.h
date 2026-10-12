#pragma once

/* DCPad: reusable Dreamcast touch controls.
 *
 * Game-independent layout and touch engine for any Dreamcast port. The engine
 * works in view points, owns per-touch identity and produces one canonical
 * Dreamcast controller state (port/input/DreamcastInputState.h). The UIKit
 * overlay in port/dcpad/apple draws what this engine reports; nothing here
 * depends on UIKit, so every rule is host-tested.
 *
 * Controls follow the Dreamcast controller: one analog stick, a D-pad, the
 * A/B/X/Y diamond (A bottom, B right, X left, Y top), analog L/R triggers and
 * Start. Mechanisms are adapted from SunPad's overlay (normalized placement,
 * grouped D-pad, analog trigger slider with a full-press detent, per-device
 * layout overrides) with three touch improvements: a floating stick, an
 * eight-way D-pad and a face-button cluster that supports sliding and chords. */

#include "DreamcastInputState.h"

#include <array>
#include <cstdint>
#include <map>

namespace dcpad {

enum class Control : uint8_t { None = 0, Stick, DPad, Face, LTrigger, RTrigger, Start, Count };
inline constexpr int kControlCount = int(Control::Count) - 1;   /* excludes None */

/* Face buttons in diamond order. */
enum Face : uint8_t { FaceA = 1u << 0, FaceB = 1u << 1, FaceX = 1u << 2, FaceY = 1u << 3 };

/* D-pad directions (combinable for diagonals). */
enum DPadDir : uint8_t { DPadUp = 1u << 0, DPadDown = 1u << 1, DPadLeft = 1u << 2, DPadRight = 1u << 3 };

struct Rect {
    float x = 0, y = 0, w = 0, h = 0;
    float midX() const { return x + w * 0.5f; }
    float midY() const { return y + h * 0.5f; }
    bool contains(float px, float py) const { return px >= x && px <= x + w && py >= y && py <= y + h; }
};

/* User override for one control group: normalized center inside the safe
 * area and a size multiplier. has_position == false keeps the default spot. */
struct ControlOverride {
    bool has_position = false;
    float nx = 0.5f, ny = 0.5f;
    float scale = 1.0f;
};

struct Settings {
    float opacity = 0.75f;        /* 0.25 .. 1 */
    float size = 1.0f;            /* global size multiplier, 0.7 .. 1.4 */
    float stick_travel = 0.85f;   /* full tilt at this fraction of the stick radius, 0.5 .. 1 */
    bool floating_stick = true;   /* stick re-centres where the thumb lands */
    bool haptics = true;
    bool hide_with_controller = true;
    bool dim_when_idle = true;    /* fade the controls back after a few seconds untouched */
    std::array<ControlOverride, kControlCount> overrides{};

    ControlOverride& over(Control c) { return overrides[int(c) - 1]; }
    const ControlOverride& over(Control c) const { return overrides[int(c) - 1]; }
};

/* Resolved geometry for one frame of layout. */
struct Layout {
    Rect safe;
    bool phone = false;
    std::array<Rect, kControlCount> frames{};   /* visual frame per control */
    float stick_radius = 0;                     /* base radius */
    float dpad_radius = 0;                      /* half size of the cross */
    float face_spacing = 0;                     /* cluster centre to button centre */
    float face_radius = 0;                      /* visual button radius */
    Rect stick_zone;                            /* floating-stick start region */

    Rect& frame(Control c) { return frames[int(c) - 1]; }
    const Rect& frame(Control c) const { return frames[int(c) - 1]; }
    /* Face button centre for one of FaceA/B/X/Y. */
    void face_center(Face f, float* x, float* y) const;
};

/* Build the layout for a safe area (points) on a phone or tablet. */
Layout resolve_layout(const Rect& safe, bool phone, const Settings& settings);

/* Ergonomic reference for the default layout: where the base of a thumb sits
 * when the device is held in landscape. On a tablet held by its sides that is
 * the side edge, 30% of the height above the bottom but no more than 233 pt
 * (about 45 mm); on a phone cradled in the palms it is near the lower corner
 * (12% up). */
void thumb_pivot(const Layout& layout, bool left_hand, float* x, float* y);

/* Topmost control group whose hit region contains the point (the floating
 * stick zone counts as Stick only when floating is enabled). */
Control control_at(const Layout& layout, const Settings& settings, float x, float y);

/* Pure hit rules, exposed for tests. */
uint8_t dpad_direction(const Layout& layout, float x, float y);
uint8_t face_buttons_at(const Layout& layout, float x, float y);
/* Trigger value 0..255 and full-press state from a point in the slider,
 * with hysteresis when was_full is set. Pressure grows toward the outer edge. */
uint8_t trigger_value(const Layout& layout, Control trigger, float x, bool was_full, bool* full);

/* What the overlay draws. */
struct Visual {
    bool stick_active = false;
    float stick_base_x = 0, stick_base_y = 0;   /* points */
    float stick_x = 0, stick_y = 0;             /* -1..1, +y down (screen) */
    uint8_t dpad = 0;
    uint8_t face = 0;
    uint8_t l = 0, r = 0;
    bool l_full = false, r_full = false;
    bool start = false;
};

/* Per-touch engine. Each touch latches one control group at begin(); its
 * value then follows the finger (stick, D-pad, face glide, trigger slide)
 * until that touch ends. Lifting one finger never releases another's input. */
class TouchEngine {
public:
    void begin(int id, float x, float y, const Layout& layout, const Settings& settings);
    void move(int id, float x, float y, const Layout& layout, const Settings& settings);
    void end(int id);
    void clear();

    std::size_t active_count() const { return touches_.size(); }
    Control control_for(int id) const;

    SonicPadDreamcastState state() const;
    Visual visual(const Layout& layout) const;

    /* Number of new presses (button down, new D-pad direction, trigger full)
     * since the last call; the overlay turns these into haptic taps. */
    unsigned take_press_events();

private:
    struct Touch {
        Control control = Control::None;
        float base_x = 0, base_y = 0;   /* stick base for this touch */
        float x = 0, y = 0;             /* stick vector -1..1, +y down */
        float travel = 0.85f;           /* full-tilt fraction latched at touch-down */
        uint8_t bits = 0;               /* D-pad dirs or face buttons */
        uint8_t trigger = 0;
        bool full = false;
        bool floating = false;
    };
    void update(Touch& t, float x, float y, const Layout& layout);
    void count_new(uint8_t before, uint8_t after);

    std::map<int, Touch> touches_;
    unsigned press_events_ = 0;
};

/* Default stick travel: full scale at this fraction of the base radius. */
inline constexpr float kStickFullTilt = 0.85f;
/* Trigger sliders become a full press at this fraction of their length. */
inline constexpr float kTriggerFullAt = 0.72f;

} /* namespace dcpad */
