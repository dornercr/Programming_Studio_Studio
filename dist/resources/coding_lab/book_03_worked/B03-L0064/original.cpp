#include "course_test.hpp"
#include <algorithm>
#include <optional>
#include <random>
#include <vector>

std::optional<std::size_t> linear_find(const std::vector<int>& values, int key) {
    for (std::size_t i = 0; i < values.size(); ++i) if (values[i] == key) return i;
    return std::nullopt;
}
std::size_t lower_bound_index(const std::vector<int>& values, int key) {
    std::size_t low{}, high = values.size();
    while (low < high) {
        const auto middle = low + (high - low) / 2;
        if (values[middle] < key) low = middle + 1;
        else high = middle;
    }
    return low;
}
std::optional<std::size_t> binary_find(const std::vector<int>& values, int key) {
    const auto index = lower_bound_index(values, key);
    if (index == values.size() || values[index] != key) return std::nullopt;
    return index;
}

int main() {
    std::mt19937 random{42};
    for (int trial = 0; trial < 300; ++trial) {
        std::vector<int> data(static_cast<std::size_t>(trial) % 129);
        for (int& value : data) value = static_cast<int>(random() % 41) - 20;
        std::sort(data.begin(), data.end());
        for (int key = -25; key <= 25; ++key) {
            const auto expected = static_cast<std::size_t>(
                std::lower_bound(data.begin(), data.end(), key) - data.begin());
            CHECK(lower_bound_index(data, key) == expected);
            CHECK(binary_find(data, key) == linear_find(data, key));
        }
    }
    CHECK(lower_bound_index({}, 1) == 0);
    CHECK(binary_find({1, 1, 1}, 1) == 0);
    course::report();
}
