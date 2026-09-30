// LAB: Enumerate defined interleavings
// Enumerate every order of two read/write steps from A and two from B while preserving each participant’s read-before-write order. Count final values in this sequential model.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    int counter = 0;
    const int a_read = counter;
    const int b_read = counter;
    counter = a_read + 1;
    counter = b_read + 1;
    std::cout << counter << '\n';
    assert(counter == 1);
}
