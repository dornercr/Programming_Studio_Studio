#include <array>
#include <iomanip>
#include <iostream>
int main() {
    std::cout << std::fixed << std::setprecision(2);
    for (int workers : std::array<int, 4>{1, 2, 4, 8}) {
        const double cost = 25.0 + 75.0 / workers + 5.0 * (workers - 1);
        std::cout << "workers=" << workers << " modeled-time=" << cost << '\n';
    }
}
