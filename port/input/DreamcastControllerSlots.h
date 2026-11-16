#pragma once

/* RingPad Dreamcast controller-slot bookkeeping.
 *
 * Reuses the pinned SunPad mechanism (ref/sunpad/apple/ios/
 * SunPadControllerSlots.h) unchanged in intent: Apple's GameController layer
 * remains the connection authority; object addresses are process-local
 * instance IDs used only for reconciliation and logs. A second controller
 * must not steal P1, and a disconnect frees exactly one slot. */

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace ringpad {

struct ControllerSlotChange {
    uintptr_t instance = 0;
    std::size_t slot = 0;
};

struct ControllerReconcileResult {
    std::vector<ControllerSlotChange> removed;
    std::vector<ControllerSlotChange> assigned;
};

class DreamcastControllerSlots {
public:
    static constexpr std::size_t kMaxPlayers = 4;

    ControllerReconcileResult reconcile(const std::vector<uintptr_t>& current_instances) {
        std::vector<uintptr_t> current;
        current.reserve(current_instances.size());
        for (uintptr_t instance : current_instances) {
            if (instance != 0 &&
                std::find(current.begin(), current.end(), instance) == current.end()) {
                current.push_back(instance);
            }
        }

        ControllerReconcileResult result;
        for (std::size_t slot = 0; slot < slots_.size(); ++slot) {
            const uintptr_t instance = slots_[slot];
            if (instance != 0 &&
                std::find(current.begin(), current.end(), instance) == current.end()) {
                result.removed.push_back({instance, slot});
                slots_[slot] = 0;
            }
        }

        for (uintptr_t instance : current) {
            if (slot_for(instance) >= 0) continue;
            auto free_slot = std::find(slots_.begin(), slots_.end(), 0);
            if (free_slot == slots_.end()) continue;
            const std::size_t slot = static_cast<std::size_t>(free_slot - slots_.begin());
            slots_[slot] = instance;
            result.assigned.push_back({instance, slot});
        }
        return result;
    }

    int slot_for(uintptr_t instance) const {
        auto slot = std::find(slots_.begin(), slots_.end(), instance);
        return slot == slots_.end() ? -1 : static_cast<int>(slot - slots_.begin());
    }

    uintptr_t instance_at(std::size_t slot) const {
        return slot < slots_.size() ? slots_[slot] : 0;
    }

private:
    std::array<uintptr_t, kMaxPlayers> slots_{};
};

} /* namespace ringpad */
