// Shared test support from Book II, B02-L0110.
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

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
