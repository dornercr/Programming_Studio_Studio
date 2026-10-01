// LAB: Make page permissions part of a lookup
// Represent a page entry with present, readable, and writable flags. A read and write request must use the corresponding permission, and an absent page denies both.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const bool present = true;
    const bool readable = true;
    const bool writable = false;
    const auto allowed = [&](bool write) {
        return present && (write ? writable : readable);
    };
    std::cout << std::boolalpha << allowed(false) << ' ' << allowed(true) << '\n';
    assert(allowed(false) && !allowed(true));
}
