#include <iostream>
int main() {
    unsigned accumulator = 0;
    unsigned memory = 0;
    unsigned pc = 0;
    accumulator = 3; ++pc; // Modeled LOAD immediate.
    std::cout << "pc=" << pc << " acc=" << accumulator << '\n';
    accumulator *= 4; ++pc; // Modeled MULTIPLY immediate.
    std::cout << "pc=" << pc << " acc=" << accumulator << '\n';
    memory = accumulator; ++pc; // Modeled STORE.
    std::cout << "pc=" << pc << " memory=" << memory << '\n';
}
