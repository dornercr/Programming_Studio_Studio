#include <array>
#include <iostream>

int main() {
    constexpr double cpu_ns=10,gpu_ns=1,setup_ns=50000;
    for(int n:std::array{100,1000,10000}){
        double cpu=n*cpu_ns,gpu=setup_ns+n*gpu_ns;
        std::cout << n << " elements -> " << (gpu<cpu?"gpu":"cpu") << '\n';
    }
    std::cout << "break-even above=" << setup_ns/(cpu_ns-gpu_ns) << " elements\n";
}
