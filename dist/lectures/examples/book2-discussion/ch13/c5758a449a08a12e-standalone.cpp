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

// Original book listing B02-L0056
#include <concepts>
#include <limits>
#include <type_traits>

template<class T>
concept Integer = std::integral<T> && !std::same_as<std::remove_cv_t<T>, bool>;

template<Integer T>
T checked_add(T left, T right) {
    const T high = std::numeric_limits<T>::max();
    if constexpr (std::is_signed_v<T>) {
        const T low = std::numeric_limits<T>::min();
        if ((right > 0 && left > high - right) ||
            (right < 0 && left < low - right)) {
            throw std::overflow_error("signed addition");
        }
    } else {
        if (left > high - right) throw std::overflow_error("unsigned addition");
    }
    return static_cast<T>(left + right);
}
template<class T> requires Integer<T>
constexpr bool accepted_type() { return true; }

int main() {
    static_assert(Integer<int> && Integer<unsigned>);
    static_assert(!Integer<bool> && !Integer<double>);
    static_assert(accepted_type<long>());
    CHECK(checked_add(2, 3) == 5);
    CHECK(checked_add(-2, -3) == -5);
    CHECK(checked_add(2U, 3U) == 5U);
    CHECK(course::throws<std::overflow_error>([] {
        (void)checked_add(std::numeric_limits<int>::max(), 1);
    }));
    CHECK(course::throws<std::overflow_error>([] {
        (void)checked_add(std::numeric_limits<int>::min(), -1);
    }));
    CHECK(course::throws<std::overflow_error>([] {
        (void)checked_add(std::numeric_limits<unsigned>::max(), 1U);
    }));
    course::report();
}
