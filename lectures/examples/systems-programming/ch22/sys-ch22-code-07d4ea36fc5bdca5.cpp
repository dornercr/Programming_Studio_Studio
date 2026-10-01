#include <iostream>
#include <optional>
std::optional<unsigned> location(unsigned base, unsigned offset) {
    if (offset >= 16) return std::nullopt;
    return base + offset; // Fixed small bases make this addition safe.
}
int main() {
    const auto a = location(1000, 12);
    const auto b = location(5000, 12);
    std::cout << "A=" << *a << " B=" << *b << " offset=12\n";
    std::cout << "past-end=" << (location(1000,16) ? "accepted" : "rejected") << '\n';
}
