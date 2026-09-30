// Shared test support from Book II, B02-L0110.
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

#include <cstddef>
#include <functional>
#include <vector>

unsigned long long queens(std::size_t n) {
    if (n > 12) throw std::invalid_argument("teaching limit: n <= 12");
    if (n == 0) return 1; // One empty placement.
    std::vector<bool> column(n), descending(2 * n - 1), ascending(2 * n - 1);
    unsigned long long solutions{};
    std::function<void(std::size_t)> place = [&](std::size_t row) {
        if (row == n) { ++solutions; return; }
        for (std::size_t c = 0; c < n; ++c) {
            const auto d = row + c, a = row + n - 1 - c;
            if (column[c] || descending[d] || ascending[a]) continue;
            column[c] = descending[d] = ascending[a] = true;
            place(row + 1);
            column[c] = descending[d] = ascending[a] = false;
        }
    };
    place(0);
    return solutions;
}

int main() {
    CHECK(queens(0) == 1); CHECK(queens(1) == 1);
    CHECK(queens(2) == 0); CHECK(queens(3) == 0);
    CHECK(queens(4) == 2); CHECK(queens(8) == 92);
    CHECK(course::throws<std::invalid_argument>([] { (void)queens(13); }));
    course::report();
}
