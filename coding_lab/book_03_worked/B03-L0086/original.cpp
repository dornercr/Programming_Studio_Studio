#include "course_test.hpp"
#include <array>
#include <cstddef>
#include <map>
#include <optional>
#include <random>

// All keys deliberately share one starting bucket. No growing or concurrency.
class CollisionTable {
    enum class State { empty, occupied, deleted };
    struct Slot { State state{State::empty}; int key{}; int value{}; };
    std::array<Slot, 7> slots_{};
public:
    bool put(int key, int value) {
        std::optional<std::size_t> reusable;
        for (std::size_t i = 0; i < slots_.size(); ++i) {
            auto& slot = slots_[i];
            if (slot.state == State::occupied && slot.key == key) {
                slot.value = value;
                return true;
            }
            if (slot.state != State::occupied && !reusable) reusable = i;
            if (slot.state == State::empty) break;
        }
        if (!reusable) return false;
        slots_[*reusable] = {State::occupied, key, value};
        return true;
    }
    std::optional<int> get(int key) const {
        for (const auto& slot : slots_) {
            if (slot.state == State::empty) return std::nullopt;
            if (slot.state == State::occupied && slot.key == key)
                return slot.value;
        }
        return std::nullopt;
    }
    bool erase(int key) {
        for (auto& slot : slots_) {
            if (slot.state == State::empty) return false;
            if (slot.state == State::occupied && slot.key == key) {
                slot.state = State::deleted;
                return true;
            }
        }
        return false;
    }
};
int main() {
    CollisionTable table;
    std::map<int, int> model;
    std::mt19937 random{317};
    for (int step = 0; step < 5000; ++step) {
        const int key = static_cast<int>(random() % 13) - 6;
        if (random() % 3 == 0) {
            CHECK(table.erase(key) == (model.erase(key) != 0));
        } else {
            const bool accepted = model.contains(key) || model.size() < 7;
            CHECK(table.put(key, step) == accepted);
            if (accepted) model[key] = step;
        }
        for (int candidate = -6; candidate <= 6; ++candidate) {
            const auto found = model.find(candidate);
            const auto expected = found == model.end()
                ? std::optional<int>{} : std::optional<int>{found->second};
            CHECK(table.get(candidate) == expected);
        }
    }
    course::report();
}
