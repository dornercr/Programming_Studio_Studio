// LAB: Detach a shared value before a private write
// Implement a single-threaded copy-on-write update for shared_ptr<int>. Reject a null handle, copy only when another owner shares the object, and preserve the other owner’s value.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
