#include <iostream>
#include <limits>

int main() {
    std::cout << "C++ mode: " << __cplusplus << '\n';
    std::cout << "int bytes: " << sizeof(int) << '\n';
    std::cout << "pointer bytes: " << sizeof(void*) << '\n';
    std::cout << "double digits: "
              << std::numeric_limits<double>::digits << '\n';
    if (__cplusplus < 202002L) return 1;
    std::cout << "PASS\n";
}
