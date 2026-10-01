#include <array>
#include <cstddef>
#include <iostream>
struct Field { std::size_t size; std::size_t alignment; };
std::size_t modeled_size(const std::array<Field, 3>& fields) {
    std::size_t offset = 0;
    for (const auto field : fields) {
        offset += (field.alignment - offset % field.alignment) % field.alignment;
        offset += field.size;
    }
    return offset + (8 - offset % 8) % 8; // Model's record alignment is 8.
}
int main() {
    const std::array<Field, 3> first{{{1,1}, {8,8}, {2,2}}};
    const std::array<Field, 3> second{{{8,8}, {2,2}, {1,1}}};
    const auto a = modeled_size(first);
    const auto b = modeled_size(second);
    std::cout << "original model=" << a << '\n';
    std::cout << "reordered model=" << b << '\n';
    std::cout << "saved per record=" << a-b << '\n';
}
