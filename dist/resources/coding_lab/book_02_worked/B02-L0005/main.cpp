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

// Original book listing B02-L0005
#include <array>
#include <numeric>
#include <vector>

struct Point { int x{}; int y{}; };
constexpr int square(int value) { return value * value; }

int main() {
    static_assert(square(7) == 49);
    std::vector<Point> original{{1, 2}, {3, 4}};
    auto copy = original;
    copy.front().x = 99;
    CHECK(original.front().x == 1);
    CHECK(copy.front().x == 99);
    const std::array<int, 4> values{1, 2, 3, 4};
    int loop_sum{};
    for (int value : values) loop_sum += value;
    const int algorithm_sum = std::accumulate(values.begin(), values.end(), 0);
    CHECK(loop_sum == 10 && algorithm_sum == loop_sum);
    const Point& borrowed = original.front();
    CHECK(borrowed.y == 2);
    course::report();
}
