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
