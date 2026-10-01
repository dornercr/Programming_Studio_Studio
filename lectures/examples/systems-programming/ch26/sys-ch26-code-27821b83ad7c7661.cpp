#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

void private_write(std::shared_ptr<int>& owner, int value) {
    if(!owner) throw std::invalid_argument("null owner");
    if(!owner.unique()) owner = std::make_shared<int>(*owner);
    *owner = value;
}

int main() {
    auto parent = std::make_shared<int>(7);
    auto child = parent;
    private_write(child,9);
    assert(*parent == 7 && *child == 9 && parent != child);
    const int* address = child.get();
    private_write(child,11);
    assert(child.get() == address && *child == 11);
    std::shared_ptr<int> empty; bool rejected = false;
    try { private_write(empty,1); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "parent=" << *parent << " child=" << *child << " detached=yes\n";
}
