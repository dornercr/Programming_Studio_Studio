#include <iostream>
#include <memory>
#include <utility>
struct Resource {
    static inline int live = 0;
    Resource() { ++live; }
    ~Resource() { --live; }
};
int main() {
    std::cout << "before=" << Resource::live << '\n';
    {
        auto first = std::make_unique<Resource>();
        auto second = std::move(first); // Transfer the one owning handle.
        std::cout << std::boolalpha << "first=" << bool(first)
                  << " second=" << bool(second) << " live=" << Resource::live << '\n';
    }
    std::cout << "after=" << Resource::live << '\n';
}
