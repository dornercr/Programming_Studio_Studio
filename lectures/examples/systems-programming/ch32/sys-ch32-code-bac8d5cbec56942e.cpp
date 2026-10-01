// LAB: Use local miss rates in a two-level cost model
// Compute h1 + m1×(h2 + m2×memory). Miss rates must lie in [0,1]; times must be nonnegative. Assume serial costs and incremental penalties.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const double hit_time = 1;
    const double miss_rate = 0.05;
    const double miss_penalty = 40;
    const double amat = hit_time + miss_rate * miss_penalty;
    std::cout << amat << " ns\n";
    assert(amat == 3);
}
