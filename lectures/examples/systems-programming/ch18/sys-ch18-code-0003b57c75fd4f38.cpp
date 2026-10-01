#include <array>
#include <cstddef>
#include <iostream>
#include <span>
unsigned scalar(std::span<const unsigned> values) {
    unsigned sum = 0;
    for (unsigned value : values) sum += value;
    return sum;
}
unsigned paired(std::span<const unsigned> values) {
    unsigned sum = 0;
    std::size_t i = 0;
    for (; values.size() - i >= 2; i += 2) sum += values[i] + values[i + 1];
    if (i < values.size()) sum += values[i];
    return sum;
}
int main() {
    const std::array<unsigned, 5> values{2, 4, 6, 8, 10};
    std::cout << "scalar=" << scalar(values) << " paired=" << paired(values) << '\n';
}
