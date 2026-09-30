#include "course_test.hpp"
#include <deque>
#include <optional>
#include <random>
#include <vector>

class RingQueue {
    std::vector<std::optional<int>> slots_;
    std::size_t head_{}, size_{};
public:
    static std::size_t valid_capacity(std::size_t capacity) {
        if (capacity == 0 || capacity > 1000000) throw std::invalid_argument("capacity");
        return capacity;
    }
    explicit RingQueue(std::size_t capacity) : slots_(valid_capacity(capacity)) {}
    std::size_t size() const noexcept { return size_; }
    bool push(int value) {
        if (size_ == slots_.size()) return false;
        slots_[(head_ + size_) % slots_.size()] = value;
        ++size_;
        return true;
    }
    std::optional<int> pop() {
        if (size_ == 0) return std::nullopt;
        const auto result = slots_[head_];
        slots_[head_].reset();
        head_ = (head_ + 1) % slots_.size();
        --size_;
        return result;
    }
};

int main() {
    CHECK(course::throws<std::invalid_argument>([] { RingQueue invalid{0}; }));
    RingQueue ring{8};
    std::deque<int> reference;
    std::mt19937 random{42};
    for (int i = 0; i < 5000; ++i) {
        if (random() % 2 == 0) {
            const bool available = reference.size() < 8;
            CHECK(ring.push(i) == available);
            if (available) reference.push_back(i);
        } else {
            const auto value = ring.pop();
            CHECK(value.has_value() == !reference.empty());
            if (value) { CHECK(*value == reference.front()); reference.pop_front(); }
        }
        CHECK(ring.size() == reference.size());
    }
    std::deque<int> flexible{2};
    flexible.push_front(1); flexible.push_back(3);
    flexible.pop_front(); flexible.pop_back();
    CHECK(flexible.front() == 2);
    course::report();
}
