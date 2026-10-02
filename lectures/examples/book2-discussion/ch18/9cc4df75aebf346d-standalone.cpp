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

// Original book listing B02-L0076
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
