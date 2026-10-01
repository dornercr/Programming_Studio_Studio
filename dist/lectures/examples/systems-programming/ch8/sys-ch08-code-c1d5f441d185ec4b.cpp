#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

std::unique_ptr<int> make_value(int value) { return std::make_unique<int>(value); }

int main() {
    auto first = make_value(42);
    assert(first && *first == 42);
    auto second = std::move(first);
    assert(!first && second && *second == 42);
    std::cout << "value=" << *second << " first-empty=" << !first << '\n';
}
