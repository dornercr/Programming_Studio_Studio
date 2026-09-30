#include "cuda_support.hpp"
__global__ void scratch_kernel(float* out){
    extern __shared__ float scratch[];
    unsigned t=threadIdx.x;scratch[t]=float(t);__syncthreads();
    if(t==0)out[blockIdx.x]=scratch[blockDim.x-1];
}

int main() {
    try {
        cudaDeviceProp prop{};CU(cudaGetDeviceProperties(&prop,0));
        int small=0,large=0;constexpr int threads=128;
        std::size_t larger=std::min<std::size_t>(16384,prop.sharedMemPerBlock);
        CU(cudaOccupancyMaxActiveBlocksPerMultiprocessor(&small,scratch_kernel,threads,threads*sizeof(float)));
        CU(cudaOccupancyMaxActiveBlocksPerMultiprocessor(&large,scratch_kernel,threads,larger));
        require(small>0 && large>0,"kernel has no resident blocks");
        std::cout << "resident-blocks small=" << small << " large=" << large << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
