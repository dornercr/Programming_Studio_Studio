// LAB: Distinguish recoverable and fatal faults
// Model a load with separate permitted and present flags. A permitted absent page may be resolved and retried; a forbidden access leaves the program counter unchanged and returns failure.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    bool present = false;
    int pc = 8;
    int faults = 0;
    if (!present) { ++faults; present = true; }
    assert(present);
    const int loaded = 42;
    ++pc;
    std::cout << "faults=" << faults << " pc=" << pc << " value=" << loaded << '\n';
}
