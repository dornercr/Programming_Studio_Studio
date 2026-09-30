#include "course_test.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>

std::optional<std::size_t> first_maximum(const std::vector<int>& values) {
    if (values.empty()) return std::nullopt;
    std::size_t best{};
    // Before each iteration, best is the first maximum in [0, i).
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[best] < values[i]) best = i;
    }
    return best;
}

int main() {
    CHECK(!first_maximum({}));
    CHECK(first_maximum({7}) == 0);
    CHECK(first_maximum({3, 9, 9, 2}) == 1);
    CHECK(first_maximum({-4, -1, -3}) == 1);
    for (int a = -2; a <= 2; ++a) {
        for (int b = -2; b <= 2; ++b) {
            for (int c = -2; c <= 2; ++c) {
                const std::vector<int> data{a, b, c};
                const auto expected = static_cast<std::size_t>(
                    std::max_element(data.begin(), data.end()) - data.begin());
                CHECK(first_maximum(data) == expected);
            }
        }
    }
    course::report();
}
