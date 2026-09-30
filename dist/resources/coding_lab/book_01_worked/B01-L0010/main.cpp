#include <iostream>

#ifndef __cplusplus
#error This file must be compiled as C++.
#endif

int main() {
    std::cout << "A source file becomes an executable.\n";
    std::cout << "__cplusplus=" << __cplusplus << '\n';
    if (__cplusplus < 202002L) {
        std::cerr << "Select C++20 or later for this series.\n";
        return 1;
    }
    std::cout << "PASS\n";
}
