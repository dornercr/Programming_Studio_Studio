#include <iostream>
int main() {
    int value = 5;
    while (value > 0) {
        std::cout << value << ' ';
        --value;
    }
    std::cout << "done\n";
}
