#include <cassert>
#include <iostream>
#include <memory>

int main() {
    auto parent = std::make_shared<int>(7);
    auto child = parent;
    if (!child.unique()) child = std::make_shared<int>(*child);
    *child = 9;
    std::cout << *parent << ' ' << *child << '\n';
    assert(*parent == 7 && *child == 9);
}
