#include "course_test.hpp"
#include <bit>
#include <cstddef>
#include <numeric>
#include <vector>

std::size_t linear_comparisons(const std::vector<int>& values, int wanted) {
    std::size_t comparisons{};
    for (int value : values) {
        ++comparisons;
        if (value == wanted) break;
    }
    return comparisons;
}
std::size_t binary_comparisons(const std::vector<int>& values, int wanted) {
    std::size_t low{}, high = values.size(), comparisons{};
    while (low < high) {
        const auto middle = low + (high - low) / 2;
        ++comparisons;
        if (values[middle] < wanted) low = middle + 1;
        else high = middle;
    }
    return comparisons;
}

int main() {
    std::vector<int> data(1024);
    std::iota(data.begin(), data.end(), 0);
    CHECK(linear_comparisons(data, -1) == 1024);
    CHECK(linear_comparisons(data, 0) == 1);
    CHECK(binary_comparisons(data, -1) <= static_cast<std::size_t>(std::bit_width(data.size())));
    std::size_t capacity{}, size{}, moved{};
    for (std::size_t n = 1; n <= 1000; ++n) {
        if (size == capacity) {
            moved += size;
            capacity = capacity ? capacity * 2 : 1;
        }
        ++size;
        CHECK(moved < 2 * n);
    }
    std::cout << "linear_missing=" << linear_comparisons(data, -1)
              << " binary_missing=" << binary_comparisons(data, -1) << '\n';
    course::report();
}
