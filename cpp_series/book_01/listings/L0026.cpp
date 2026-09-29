#include <iostream>
int main() {
    int age{};
    std::cout << "Age: ";
    if (!(std::cin >> age) || age < 0) {
        std::cerr << "invalid age\n";
        return 1;
    }
    std::cout << "next year=" << age + 1 << '\n';
}
