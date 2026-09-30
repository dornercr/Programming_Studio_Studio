#include "course_test.hpp"
#include <algorithm>
#include <functional>
#include <optional>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

class ChainedTable {
    std::vector<std::vector<std::pair<int, int>>> buckets_;
public:
    explicit ChainedTable(std::size_t capacity) : buckets_(capacity) {
        if (capacity == 0) throw std::invalid_argument("zero buckets");
    }
    void put(int key, int value) {
        auto& bucket = buckets_[std::hash<int>{}(key) % buckets_.size()];
        for (auto& item : bucket) if (item.first == key) { item.second = value; return; }
        bucket.emplace_back(key, value);
    }
    std::optional<int> get(int key) const {
        const auto& bucket = buckets_[std::hash<int>{}(key) % buckets_.size()];
        for (const auto& item : bucket) if (item.first == key) return item.second;
        return std::nullopt;
    }
    bool erase(int key) {
        auto& bucket = buckets_[std::hash<int>{}(key) % buckets_.size()];
        return std::erase_if(bucket, [key](const auto& item) { return item.first == key; }) != 0;
    }
};
class OpenTable {
    enum class State { empty, occupied, tombstone };
    struct Slot { State state{State::empty}; int key{}; int value{}; };
    std::vector<Slot> slots_;
public:
    static std::size_t valid_capacity(std::size_t capacity) {
        if (capacity == 0 || capacity > 1000000) throw std::invalid_argument("capacity");
        return capacity;
    }
    explicit OpenTable(std::size_t capacity) : slots_(valid_capacity(capacity)) {}
    bool put(int key, int value) {
        const auto first = std::hash<int>{}(key) % slots_.size();
        std::optional<std::size_t> available;
        for (std::size_t step = 0; step < slots_.size(); ++step) {
            const auto i = (first + step) % slots_.size();
            Slot& slot = slots_[i];
            if (slot.state == State::occupied && slot.key == key) {
                slot.value = value; return true;
            }
            if (slot.state != State::occupied && !available) available = i;
            if (slot.state == State::empty) break;
        }
        if (!available) return false;
        slots_[*available] = Slot{State::occupied, key, value};
        return true;
    }
    std::optional<int> get(int key) const {
        const auto first = std::hash<int>{}(key) % slots_.size();
        for (std::size_t step = 0; step < slots_.size(); ++step) {
            const Slot& slot = slots_[(first + step) % slots_.size()];
            if (slot.state == State::empty) return std::nullopt;
            if (slot.state == State::occupied && slot.key == key) return slot.value;
        }
        return std::nullopt;
    }
    bool erase(int key) {
        const auto first = std::hash<int>{}(key) % slots_.size();
        for (std::size_t step = 0; step < slots_.size(); ++step) {
            Slot& slot = slots_[(first + step) % slots_.size()];
            if (slot.state == State::empty) return false;
            if (slot.state == State::occupied && slot.key == key) {
                slot.state = State::tombstone; return true;
            }
        }
        return false;
    }
};

int main() {
    CHECK(course::throws<std::invalid_argument>([] { OpenTable invalid{1000001}; }));
    CHECK(course::throws<std::invalid_argument>([] { OpenTable invalid{0}; }));
    ChainedTable chained{3};
    OpenTable open{17};
    std::unordered_map<int, int> expected;
    std::mt19937 random{42};
    for (int step = 0; step < 2000; ++step) {
        const int key = static_cast<int>(random() % 16);
        if (random() % 3 == 0) {
            const bool existed = expected.erase(key) != 0;
            CHECK(chained.erase(key) == existed);
            CHECK(open.erase(key) == existed);
        } else {
            expected[key] = step; chained.put(key, step); CHECK(open.put(key, step));
        }
        for (int k = 0; k < 16; ++k) {
            const auto it = expected.find(k);
            const auto want = it == expected.end() ? std::optional<int>{} : it->second;
            CHECK(chained.get(k) == want && open.get(k) == want);
        }
    }
    OpenTable one{1};
    CHECK(one.put(1, 10)); CHECK(!one.put(2, 20));
    CHECK(one.erase(1)); CHECK(one.put(2, 20)); CHECK(one.get(2) == 20);
    course::report();
}
