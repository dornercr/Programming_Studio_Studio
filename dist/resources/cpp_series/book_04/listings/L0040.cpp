#include "check.hpp"
#include <limits>
#include <optional>

std::optional<int> checked_add(int a, int b) {
    const int maximum = std::numeric_limits<int>::max();
    const int minimum = std::numeric_limits<int>::min();
    if ((b > 0 && a > maximum - b) || (b < 0 && a < minimum - b))
        return std::nullopt;
    return a + b;
}
int main() {
    CHECK(checked_add(20, 22) == 42);
    CHECK(!checked_add(std::numeric_limits<int>::max(), 1));
    CHECK(!checked_add(std::numeric_limits<int>::min(), -1));
    CHECK(checked_add(std::numeric_limits<int>::min(), 0) == std::numeric_limits<int>::min());
    const unsigned wrapped = std::numeric_limits<unsigned>::max() + 1U;
    CHECK(wrapped == 0);
    std::cout << "plain_char_signed=" << std::numeric_limits<char>::is_signed << "\nPASS\n";
}
