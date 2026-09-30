#include <cctype>
#include <iostream>
int main() {
    constexpr char delimiter = ':';
    char ch = 'A';
    bool uppercase = std::isupper(static_cast<unsigned char>(ch)) != 0;
    std::cout << ch << delimiter << std::boolalpha << uppercase << '\n';
}
