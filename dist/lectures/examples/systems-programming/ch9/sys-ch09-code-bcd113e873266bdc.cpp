#include <iostream>
#include <memory>
#include <stdexcept>
struct Tracked {
    inline static int alive = 0;
    Tracked() { ++alive; }
    ~Tracked() { --alive; }
};
int main() {
    try {
        auto owner = std::make_unique<Tracked>();
        std::cout << "during work=" << Tracked::alive << '\n';
        throw std::runtime_error("later step failed");
    } catch (const std::runtime_error&) {
        std::cout << "after cleanup=" << Tracked::alive << '\n';
    }
}
