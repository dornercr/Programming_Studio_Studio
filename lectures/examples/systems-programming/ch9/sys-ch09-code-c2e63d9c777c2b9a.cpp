#include <cstddef>
#include <cstdlib>
#include <iostream>
void* grow_under_limit(void* owner, std::size_t bytes, std::size_t limit) {
    if (bytes == 0 || bytes > limit) return nullptr;
    return std::realloc(owner, bytes);
}
int main() {
    auto* owner = static_cast<unsigned char*>(std::malloc(4));
    if (!owner) return 1;
    owner[0] = 42;
    // A policy rejection exercises the same preserve-owner branch.
    void* candidate = grow_under_limit(owner, 8, 4);
    if (candidate) {
        owner = static_cast<unsigned char*>(candidate);
        std::cout << "growth=accepted\n";
    } else {
        std::cout << "growth=rejected\n";
    }
    std::cout << "preserved=" << static_cast<unsigned>(owner[0]) << '\n';
    std::free(owner);
    std::cout << "owner released\n";
}
