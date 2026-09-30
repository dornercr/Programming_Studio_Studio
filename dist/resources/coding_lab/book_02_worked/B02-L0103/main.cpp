// Exact original book listing B02-L0110
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

// Original book listing B02-L0103
#include <algorithm>
#include <array>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

std::vector<int> selected_snapshot() {
    std::vector<int> local{1, 2, 3, 4, 5, 6};
    auto view = local | std::views::filter([](int value) { return value % 2 == 0; })
                      | std::views::transform([](int value) { return value * value; });
    std::vector<int> owned;
    for (int value : view) owned.push_back(value);
    return owned; // No borrow of local storage escapes.
}
int main() {
    static_assert(std::ranges::borrowed_range<std::span<int>>);
    static_assert(!std::ranges::borrowed_range<std::vector<int>>);
    using UnsafeResult = decltype(std::ranges::find(std::vector<int>{1, 2}, 2));
    static_assert(std::is_same_v<UnsafeResult, std::ranges::dangling>);
    std::array data{2, 4, 6};
    auto found = std::ranges::find(std::span<int>{data}, 4);
    CHECK(found != std::span<int>{data}.end() && *found == 4);
    const auto snapshot = selected_snapshot();
    CHECK(snapshot == std::vector<int>({4, 16, 36}));
    std::vector<int> source{1, 2, 3, 4};
    {
        auto selected = source | std::views::filter([](int value) { return value % 2 == 0; });
        std::vector<int> captured;
        for (int value : selected) captured.push_back(value);
        CHECK(captured == std::vector<int>({2, 4}));
    } // The old view is no longer used after source changes.
    source[0] = 8;
    auto current = source | std::views::filter([](int value) { return value % 2 == 0; });
    CHECK(*current.begin() == 8);
    std::string line{"17|first title"};
    std::string saved;
    {
        const std::string_view field{line.data() + 3, line.size() - 3};
        saved = std::string{field};
    }
    line.assign(200, 'x');
    CHECK(saved == "first title");
    std::vector<int> empty;
    auto empty_view = empty | std::views::filter([](int v) { return v > 0; });
    CHECK(empty_view.begin() == empty_view.end());
    const std::vector<int> none{-3, -2, -1};
    auto no_matches = none | std::views::filter([](int v) { return v > 0; });
    CHECK(no_matches.begin() == no_matches.end());
    course::report();
}
