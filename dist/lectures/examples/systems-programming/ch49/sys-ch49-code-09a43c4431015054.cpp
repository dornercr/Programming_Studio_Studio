#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
class Ring {
    std::array<int,3> slots_{};
    std::size_t head_ = 0, count_ = 0;
public:
    bool push(int value) {
        if (count_ == slots_.size()) return false;
        slots_[(head_+count_) % slots_.size()] = value;
        ++count_;
        return true;
    }
    std::optional<int> pop() {
        if (count_ == 0) return std::nullopt;
        const int value = slots_[head_];
        head_ = (head_+1) % slots_.size();
        --count_;
        return value;
    }
};
int main() {
    Ring queue;
    if (!queue.push(10) || !queue.push(20)) return 1;
    const auto removed = queue.pop();
    if (!removed || !queue.push(30) || !queue.push(40)) return 2;
    if (queue.push(50)) return 3;
    const auto a = queue.pop(), b = queue.pop(), c = queue.pop();
    if (!a || !b || !c || queue.pop()) return 4;
    std::cout << "removed=" << *removed << " queued="
              << *a << ',' << *b << ',' << *c << '\n';
}
