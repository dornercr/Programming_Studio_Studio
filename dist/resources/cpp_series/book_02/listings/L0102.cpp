#include "course_test.hpp"
#include "checked_numeric.hpp"
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
