#include <cassert>
#include <iostream>
#include <utility>

int main() {
    const auto order = [](int from, int to) {
        if (from > to) std::swap(from, to);
        return std::pair<int, int>{from, to};
    };
    const auto forward = order(3, 8);
    const auto reverse = order(8, 3);
    std::cout << forward.first << ',' << forward.second << ' '
              << reverse.first << ',' << reverse.second << '\n';
    assert(forward == reverse);
}
