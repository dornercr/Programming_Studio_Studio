#include <iostream>
int main() {
    const unsigned flags = 0b0100u;
    const unsigned required = 0b1100u;
    const bool any = (flags & required) != 0u;
    const bool all = (flags & required) == required;
    std::cout << std::boolalpha << "any=" << any << '\n';
    std::cout << "all=" << all << '\n';
    std::cout << "logical and=" << (flags && required) << '\n';
}
