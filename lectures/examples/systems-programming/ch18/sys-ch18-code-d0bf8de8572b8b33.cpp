// LAB: Test an alias-sensitive rewrite
// Implement write_then_read(a,b) and verify both separate objects and two references to the same object. Do not move the read before the write.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    int x = 4;
    int& a = x;
    int& b = x;
    const int remembered = b;
    a = 9;
    const int after = b;
    std::cout << remembered << ' ' << after << '\n';
    assert(remembered == 4 && after == 9);
}
