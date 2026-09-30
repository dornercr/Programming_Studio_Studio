#include "cuda_support.hpp"
__global__ void ballot_demo(unsigned* masks) {
    unsigned lane=threadIdx.x%warpSize;
    unsigned mask=__ballot_sync(0xffffffffu,lane<4);
    masks[threadIdx.x]=mask;
}

int main() {
    try {
        cudaDeviceProp prop{}; CU(cudaGetDeviceProperties(&prop,0));
        require(prop.warpSize==32,"this ballot fixture requires 32-lane warps");
        Device<unsigned> data(32); std::vector<unsigned> host(32);
        ballot_demo<<<1,32>>>(data.p); CU(cudaGetLastError());
        CU(cudaMemcpy(host.data(),data.p,32*sizeof(unsigned),cudaMemcpyDeviceToHost));
        for(auto mask:host) require(mask==15u,"ballot mismatch");
        std::cout << "32 lanes observed mask=15\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
