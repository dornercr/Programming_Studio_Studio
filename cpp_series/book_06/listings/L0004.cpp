#include <array>
#include <iomanip>
#include <iostream>

int main() {
    constexpr double serial=0.20, overhead=0.04;
    std::cout << std::fixed << std::setprecision(3);
    for(int workers:std::array{1,2,4,8}) {
        const double seconds=serial+(1.0-serial)/workers+(workers>1?overhead:0.0);
        std::cout << workers << " workers: " << 1.0/seconds << "x\n";
    }
    std::cout << "ideal infinite-worker ceiling=" << 1.0/serial << "x\n";
}
