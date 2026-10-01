#include <iostream>
#include <limits>
#include <optional>
std::optional<int> increment(int value) {
    if (value == std::numeric_limits<int>::max()) return std::nullopt;
    return value + 1;
}
int main() {
    const auto ordinary = increment(7);
    const auto boundary = increment(std::numeric_limits<int>::max());
    std::cout << "ordinary=" << *ordinary << '\n';
    std::cout << std::boolalpha << "maximum rejected="
              << !boundary.has_value() << '\n';
    const unsigned wrapped = std::numeric_limits<unsigned>::max() + 1u;
    std::cout << "unsigned wrap=" << wrapped << '\n';
}
