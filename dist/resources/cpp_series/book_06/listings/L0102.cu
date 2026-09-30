#include "cuda_support.hpp"
__global__ void set_one(int* out){if(threadIdx.x==0)out[0]=1;}

int main() {
    try {
        cudaDeviceProp prop{};CU(cudaGetDeviceProperties(&prop,0));Device<int> result(1);
        set_one<<<1,unsigned(prop.maxThreadsPerBlock+1)>>>(result.p);
        cudaError_t launch=cudaGetLastError();require(launch!=cudaSuccess,"oversized block accepted");
        set_one<<<1,1>>>(result.p);CU(cudaGetLastError());
        CU(cudaDeviceSynchronize());int value=0;
        CU(cudaMemcpy(&value,result.p,sizeof value,cudaMemcpyDeviceToHost));
        require(value==1,"valid kernel failed");std::cout << "invalid launch rejected; valid execution completed\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
