#include "check.hpp"
#include <array>
#include <cstddef>
#include <optional>
#include <span>
[[nodiscard]] std::optional<int> at_checked(std::span<const int> values, std::ptrdiff_t index) {
    if (index < 0) return std::nullopt;
    const auto converted = static_cast<std::size_t>(index);
    if (converted >= values.size()) return std::nullopt;
    return values[converted];
}
int main() {
    const std::array values{4, 8, 12};
    CHECK(at_checked(values, 0) == 4);
    CHECK(at_checked(values, 2) == 12);
    CHECK(!at_checked(values, -1) && !at_checked(values, 3));
    CHECK(!at_checked({}, 0));
    std::cout << "PASS\n";
}
