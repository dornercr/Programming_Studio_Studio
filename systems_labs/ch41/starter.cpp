// LAB: Separate pending state from an event count
// Model both a Boolean notification and a counted event queue. Deliver two events before consuming anything, then explain why the two representations produce different amounts of information.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    bool pending = false;
    pending = true;
    pending = true;
    int handled = 0;
    if (pending) { pending = false; ++handled; }
    std::cout << "handled=" << handled << '\n';
    assert(handled == 1);
}
