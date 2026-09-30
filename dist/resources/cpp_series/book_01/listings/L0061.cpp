#include <iostream>
int main() {
    int value = 10;
    {
        int value = 20;
        std::cout << value << ' ';
    }
    std::cout << value << '\n';
}
