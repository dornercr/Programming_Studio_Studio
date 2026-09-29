// LAB: Turn the instruction trace into a bounded interpreter
// Interpret Load, Add, and Halt in a tiny model. Reject a program that runs past its instruction array without halting. Arithmetic inputs are limited to small values with representable results.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
    const std::array<int, 3> operands{4, 3, 0};
    int accumulator = 0;
    std::size_t pc = 0;
    accumulator = operands[pc++];
    accumulator += operands[pc++];
    std::cout << "pc=" << pc << " acc=" << accumulator << '\n';
    assert(pc == 2 && accumulator == 7);
}
