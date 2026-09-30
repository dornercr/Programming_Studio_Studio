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

// Exact original book listing B02-L0101
#ifndef HARBOR_CHECKED_NUMERIC_HPP
#define HARBOR_CHECKED_NUMERIC_HPP
#include <concepts>
#include <limits>
#include <span>
#include <stdexcept>
#include <type_traits>

namespace harbor::numeric {
template<class T>
concept Integer = std::integral<T> && !std::same_as<std::remove_cv_t<T>, bool>;

template<Integer T>
constexpr T checked_add(T left, T right) {
    constexpr T high = std::numeric_limits<T>::max();
    if constexpr (std::is_signed_v<T>) {
        constexpr T low = std::numeric_limits<T>::min();
        if ((right > 0 && left > high - right) ||
            (right < 0 && left < low - right)) {
            throw std::overflow_error("sum is not representable");
        }
    } else if (left > high - right) {
        throw std::overflow_error("sum is not representable");
    }
    return static_cast<T>(left + right);
}

template<Integer T>
T checked_total(std::span<const T> values) {
    T result{};
    for (T value : values) result = checked_add(result, value);
    return result;
}
}
#endif

// Original book listing B02-L0102
#include <array>
#include <limits>
#include <span>
#include <vector>

int main() {
    using harbor::numeric::Integer;
    using harbor::numeric::checked_add;
    using harbor::numeric::checked_total;
    static_assert(Integer<int> && Integer<unsigned long>);
    static_assert(!Integer<bool> && !Integer<const bool> && !Integer<double>);
    static_assert(checked_add(2, 3) == 5);
    CHECK(checked_total<int>({}) == 0);
    const std::array mixed{10, -4, 7, -3};
    CHECK(checked_total<int>(mixed) == 10);
    constexpr int high = std::numeric_limits<int>::max();
    constexpr int low = std::numeric_limits<int>::min();
    CHECK(checked_add(high, 0) == high);
    CHECK(checked_add(low, 0) == low);
    CHECK(checked_add(high, -1) == high - 1);
    CHECK(checked_add(low, 1) == low + 1);
    CHECK(course::throws<std::overflow_error>([&] { (void)checked_add(high, 1); }));
    CHECK(course::throws<std::overflow_error>([&] { (void)checked_add(low, -1); }));
    const std::vector<int> overflow{high - 2, 1, 2};
    CHECK(course::throws<std::overflow_error>([&] { (void)checked_total<int>(overflow); }));
    CHECK(overflow == std::vector<int>({high - 2, 1, 2}));
    CHECK(course::throws<std::overflow_error>([] {
        (void)checked_add(std::numeric_limits<unsigned>::max(), 1U);
    }));
    // Independent wider arithmetic oracle over a small exhaustive domain.
    for (int left = -40; left <= 40; ++left) {
        for (int right = -40; right <= 40; ++right) {
            const auto actual = checked_add(static_cast<signed char>(left),
                                            static_cast<signed char>(right));
            const int expected = left + right; // Always within [-80,80].
            CHECK(static_cast<int>(actual) == expected);
        }
    }
    course::report();
}
