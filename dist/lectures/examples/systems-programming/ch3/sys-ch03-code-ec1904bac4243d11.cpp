#include <iostream>
int main() {
    const unsigned old_word = 0b1011u;
    const unsigned mask = 0b0011u;
    const unsigned new_field = 1u;
    const unsigned wrong = old_word | new_field;
    const unsigned correct = (old_word & ~mask) | new_field;
    std::cout << "OR only=" << wrong << '\n';
    std::cout << "replace=" << correct << '\n';
    std::cout << std::boolalpha << "outside preserved="
              << ((correct & ~mask) == (old_word & ~mask)) << '\n';
}
