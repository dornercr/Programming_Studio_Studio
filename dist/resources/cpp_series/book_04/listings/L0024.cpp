#include <iostream>
#include <type_traits>

struct PacketHeader {
    unsigned type;
    unsigned length;
};

struct Polymorphic {
    virtual ~Polymorphic() = default;
    unsigned value{};
};

int main() {
    std::cout << std::boolalpha;
    std::cout << std::is_standard_layout_v<PacketHeader> << ' '
              << std::is_trivially_copyable_v<PacketHeader> << "\n";
    std::cout << std::is_standard_layout_v<Polymorphic> << "\n";
}
