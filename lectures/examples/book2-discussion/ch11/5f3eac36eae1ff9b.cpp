#include "course_test.hpp"
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
