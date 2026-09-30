#include <iomanip>
#include <iostream>

int main() {
    int count{};
    if (!(std::cin >> count)) {
        std::cerr << "error: expected an integer\n";
        return 2;
    }
    if (count < 0 || count > 1000) {
        std::cerr << "error: count must be in [0,1000]\n";
        return 3;
    }
    const double watts = count * 2.5;
    std::cout << "devices=" << count << '\n';
    std::cout << std::fixed << std::setprecision(2)
              << "watts=" << watts << '\n';
}
