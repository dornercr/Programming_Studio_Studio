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

// Original book listing B02-L0060
#include <algorithm>
#include <vector>

auto make_counter(int initial) {
    return [value = initial]() mutable { return ++value; };
}

int main() {
    int cutoff{3};
    const auto fixed = [cutoff](int value) { return value >= cutoff; };
    const auto live = [&cutoff](int value) { return value >= cutoff; };
    cutoff = 5;
    CHECK(fixed(4) && !live(4));
    auto counter = make_counter(10);
    CHECK(counter() == 11);
    CHECK(counter() == 12);
    auto independent = counter;
    CHECK(independent() == 13 && counter() == 13);
    const auto add = [](auto left, auto right) { return left + right; };
    CHECK(add(2, 3) == 5 && add(2.0, 0.5) == 2.5);
    std::vector<int> data{1, 2, 3};
    std::transform(data.begin(), data.end(), data.begin(),
                   [](int value) { return value * value; });
    CHECK(data == std::vector<int>({1, 4, 9}));
    course::report();
}
