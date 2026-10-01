// LAB: Validate an Amdahl calculation
// Compute whole-program speedup from a fraction in [0,1] and a finite positive local speedup. Reject invalid fractions and nonpositive factors.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const double fraction = 0.4;
    const double local_speedup = 2.0;
    const double new_time = (1 - fraction) + fraction / local_speedup;
    std::cout << "speedup=" << 1 / new_time << '\n';
    assert(new_time > 0.79 && new_time < 0.81);
}
