#include "course_test.hpp"
#include <algorithm>
#include <ranges>
#include <vector>
#include <functional>

int main() {
    std::vector<int> source{1, 2, 3, 4, 5, 6};
    auto selected = source
        | std::views::filter([](int value) { return value % 2 == 0; })
        | std::views::transform([](int value) { return value * value; });
    std::vector<int> result;
    for (int value : selected) result.push_back(value);
    CHECK(result == std::vector<int>({4, 16, 36}));
    CHECK(source == std::vector<int>({1, 2, 3, 4, 5, 6}));
    std::ranges::sort(result, std::greater<>{});
    CHECK(result == std::vector<int>({36, 16, 4}));
    source[1] = 8;
    // Rebuild the view after structural/predicate-relevant source mutation.
    auto current = source | std::views::filter([](int v) { return v % 2 == 0; });
    CHECK(*current.begin() == 8);
    course::report();
}
