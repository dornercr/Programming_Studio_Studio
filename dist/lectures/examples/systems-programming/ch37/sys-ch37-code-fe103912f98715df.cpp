// LAB: Separate a descriptor handle from its open-file state
// Model two inherited handles referring to one open-file description. Closing the child’s handle must not close the parent’s handle or reset their shared offset.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <memory>

int main() {
    int parent_value = 7;
    int child_value = parent_value;
    auto parent_offset = std::make_shared<int>(0);
    auto child_offset = parent_offset;
    child_value = 9;
    *child_offset += 2;
    std::cout << parent_value << ' ' << child_value << " offset=" << *parent_offset << '\n';
    assert(parent_value == 7 && *parent_offset == 2);
}
