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

// Original book listing B02-L0048
#include <string>
#include <type_traits>

template<class T>
T larger(T left, T right) { return left < right ? right : left; }

// Give C strings content ordering rather than accidental pointer ordering.
std::string larger(const char* left, const char* right) {
    return larger(std::string{left}, std::string{right});
}
template<class T>
T clamp_value(T value, T low, T high) {
    static_assert(std::is_arithmetic_v<T>, "clamp_value requires arithmetic");
    if (high < low) throw std::invalid_argument("inverted bounds");
    return value < low ? low : (high < value ? high : value);
}

int main() {
    CHECK(larger(3, 9) == 9);
    CHECK(larger(2.5, 1.5) == 2.5);
    CHECK(larger("alpha", "beta") == "beta");
    CHECK(clamp_value(12, 0, 10) == 10);
    CHECK(clamp_value(-1, 0, 10) == 0);
    CHECK(clamp_value(5, 0, 10) == 5);
    CHECK(course::throws<std::invalid_argument>([] { (void)clamp_value(1, 10, 0); }));
    course::report();
}
