// LAB: Return an owner across a function boundary
// Create a unique owner in a factory function, return it, move it to a second owner, and show the original owner is empty afterward.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
