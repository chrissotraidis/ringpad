#pragma once

/* RingPad Dreamcast input contract.
 *
 * The shell (Apple overlay, physical controller, keyboard) produces one
 * canonical, game-independent Dreamcast controller state. A single adapter
 * converts it once into SARecomp's native action snapshot at the verified
 * guest boundary (ref/sarecomp/src/sonic_input.hpp: Snapshot::pad, with pad
 * bits 14/15 carrying the analog L/R triggers).
 *
 * Canonical profile: ONE analog movement stick, a D-pad, A/B/X/Y, independent
 * analog L/R and Start. There is deliberately no second analog stick; optional
 * modern-camera input is a separate, explicitly enabled host extension, not a
 * Dreamcast axis. The guest pad mask and the trigger rule are copied from the
 * retained input boundary rather than guessed from GameCube/Z/FLUDD. */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Normalized Dreamcast state at the shell/adapter boundary.
 * - moveX/moveY: analog movement stick, int8 [-127, 127]; +Y is up.
 * - triggerL/triggerR: independent analog triggers, uint8 [0, 255].
 * - buttons: bitmask of the RingPadDC_* constants below (bits 0-13 only).
 * - connected: 1 when a sampled source owns P1, else 0. */
typedef struct RingPadDreamcastState {
    int8_t   moveX, moveY;
    uint8_t  triggerL, triggerR;
    uint16_t buttons;
    int      connected;
} RingPadDreamcastState;

/* Guest pad bits, identical to SARecomp src/sonic_input_bindings.hpp. */
enum {
    RingPadDC_Up       = 1u << 0,
    RingPadDC_Down     = 1u << 1,
    RingPadDC_Left     = 1u << 2,
    RingPadDC_Right    = 1u << 3,
    RingPadDC_Start    = 1u << 4,
    RingPadDC_A        = 1u << 10,
    RingPadDC_B        = 1u << 11,
    RingPadDC_X        = 1u << 12,
    RingPadDC_Y        = 1u << 13,
    RingPadDC_TriggerL = 1u << 14, /* adapter-derived from triggerL */
    RingPadDC_TriggerR = 1u << 15  /* adapter-derived from triggerR */
};

#ifdef __cplusplus
} /* extern "C" */

namespace ringpad {

/* Guest trigger pad bit set when a trigger's raw value exceeds this. Copied
 * from the retained boundary (SARecomp sets bits 14/15 above raw value 64). */
inline constexpr int kDefaultTriggerThreshold = 64;

/* Radial movement deadzone, as a whole percent of full stick radius. */
inline constexpr unsigned kDefaultMoveDeadzonePercent = 12;

/* Canonical Dreamcast actions for held-state queries. */
enum class DreamcastAction : unsigned {
    Up, Down, Left, Right, A, B, X, Y, Start, LeftTrigger, RightTrigger, Count
};
inline constexpr unsigned dreamcast_action_count = unsigned(DreamcastAction::Count);

/* Adapter output: the exact fields written into the guest snapshot. */
struct ActionSnapshot {
    unsigned held_mask = 0;        /* pad bits held, post trigger-threshold */
    float    move_x = 0.0f;        /* normalized [-1, 1] */
    float    move_y = 0.0f;        /* normalized [-1, 1], +Y up */
    float    left_trigger = 0.0f;  /* normalized [0, 1] */
    float    right_trigger = 0.0f; /* normalized [0, 1] */
};

/* Convert one canonical state into the guest-facing action snapshot.
 * Purely functional: no hidden state, no host I/O, no timing. */
ActionSnapshot adapt(const RingPadDreamcastState& state,
                     unsigned deadzone_percent = kDefaultMoveDeadzonePercent,
                     int trigger_threshold = kDefaultTriggerThreshold) noexcept;

/* Apply a radial (circular) deadzone while preserving direction. Writes the
 * rescaled axes through out_x/out_y and returns the resulting magnitude in
 * [0, 1]. Never leaves a stuck axis at center; keeps diagonals consistent. */
float apply_radial_deadzone(float x, float y, unsigned deadzone_percent,
                            float* out_x, float* out_y) noexcept;

/* True when an action's held state is asserted in a snapshot. */
bool held(const ActionSnapshot&, DreamcastAction) noexcept;

/* Guest pad bit for a canonical action, or 0 for LeftTrigger/RightTrigger
 * (which are analog values, not a single held bit). */
unsigned action_pad_bit(DreamcastAction) noexcept;

/* Latch that guarantees an edge observed between game samples is delivered
 * exactly once. The shell records a press when a touch begins (press), even if
 * the finger lifts before the next game sample. consume() folds pending edges
 * into the current held mask and clears them. Modal, handoff, layout-edit and
 * lifecycle transitions call clear() so no queued edge survives. */
class EdgeLatch {
public:
    void press(unsigned pad_bit) noexcept { pending_ |= pad_bit; }
    unsigned consume(unsigned held_mask) noexcept {
        const unsigned out = held_mask | pending_;
        pending_ = 0;
        return out;
    }
    void clear() noexcept { pending_ = 0; }

private:
    unsigned pending_ = 0;
};

} /* namespace ringpad */
#endif /* __cplusplus */
