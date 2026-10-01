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
