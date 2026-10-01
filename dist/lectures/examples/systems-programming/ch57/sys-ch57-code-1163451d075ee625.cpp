#include <cstdint>
#include <iostream>
#include <limits>
int main() {
    const unsigned payload = 250, header = 10;
    const auto narrowed = static_cast<std::uint8_t>(payload + header);
    const unsigned maximum = std::numeric_limits<std::uint8_t>::max();
    const bool allowed = header <= maximum && payload <= maximum - header;
    std::cout << "wrapped-total=" << unsigned(narrowed) << '\n';
    std::cout << "checked=" << (allowed ? "accept" : "reject") << '\n';
    // No allocation or out-of-bounds copy is attempted.
}
