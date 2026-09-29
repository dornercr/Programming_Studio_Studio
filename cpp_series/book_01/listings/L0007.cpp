#include <iostream>
int main() {
    std::cout << "__cplusplus=" << __cplusplus << '\n';
#if defined(__clang__)
    std::cout << "compiler=Clang\n";
#elif defined(__GNUC__)
    std::cout << "compiler=GCC\n";
#elif defined(_MSC_VER)
    std::cout << "compiler=MSVC\n";
#endif
}
