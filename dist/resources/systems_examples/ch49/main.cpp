#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> queue;
    const std::size_t capacity = 2;
    const auto push = [&](int value) {
        if (queue.size() == capacity) return false;
        queue.push_back(value);
        return true;
    };
    const bool first = push(1), second = push(2), full = push(3);
    assert(first && second && !full);
    queue.erase(queue.begin());
    const bool after_pop = push(3);
    std::cout << std::boolalpha << full << ' ' << after_pop
              << " size=" << queue.size() << '\n';
}
