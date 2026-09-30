// LAB: Generalize a loop with a clear stopping rule
// Sum integers in [0,limit) for limits up to 1000. Reject larger limits so the accumulator’s range is easy to justify.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    int sum = 0;
    int i = 0;
    while (i < 4) {
        sum += i;
        ++i;
    }
    std::cout << "i=" << i << " sum=" << sum << '\n';
    assert(i == 4 && sum == 6);
}
