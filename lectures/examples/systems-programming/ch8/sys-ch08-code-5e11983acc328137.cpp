#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

int main() {
    std::unique_ptr<int> owner;
    {
        auto temporary = std::make_unique<int>(42);
        owner = std::move(temporary);
        assert(!temporary);
    }
    std::cout << *owner << '\n';
    assert(*owner == 42);
}
