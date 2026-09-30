#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::vector<std::unique_ptr<std::byte[]>> blocks;
    for (std::size_t i = 1; i <= 8; ++i)
        blocks.push_back(std::make_unique<std::byte[]>(i * 1024));
    for (std::size_t i = 1; i < blocks.size(); i += 2)
        blocks[i].reset();
    auto large = std::make_unique<std::byte[]>(12 * 1024);
    std::cout << (large != nullptr) << "\n";
}
