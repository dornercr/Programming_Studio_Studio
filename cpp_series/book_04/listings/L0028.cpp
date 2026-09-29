#include <cstdint>
#include <iostream>

struct alignas(64) CacheAligned {
    std::uint64_t value;
};

int main() {
    CacheAligned x{};
    auto address = reinterpret_cast<std::uintptr_t>(&x);
    std::cout << "alignof=" << alignof(CacheAligned) << "\n";
    std::cout << "address_mod_64=" << (address % 64) << "\n";
}
