#include <iomanip>
#include <iostream>
#include <algorithm>

int main() {
    struct Work {const char* name;double flops,bytes;};
    const Work workloads[]{{"saxpy",2,12},{"reused-tile",512,16}};
    constexpr double bandwidth_gbs=100, compute_gflops=2000;
    for(auto w:workloads){
        double intensity=w.flops/w.bytes;
        double bound=std::min(compute_gflops,bandwidth_gbs*intensity);
        std::cout << w.name << " intensity=" << std::fixed << std::setprecision(3)
                  << intensity << " bound_GFLOP/s=" << bound << '\n';
    }
}
