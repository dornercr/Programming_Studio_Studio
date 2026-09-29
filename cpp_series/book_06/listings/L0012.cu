#include "cuda_support.hpp"
__global__ void ramp(float* out,std::size_t n) {
    for(std::size_t i=std::size_t(blockIdx.x)*blockDim.x+threadIdx.x;
        i<n;i+=std::size_t(blockDim.x)*gridDim.x)
        out[i]=float(i)*0.25f;
}

int main() {
    try {
        constexpr std::size_t n=100003;
        Device<float> out(n); std::vector<float> host(n);
        ramp<<<80,256>>>(out.p,n); CU(cudaGetLastError());
        CU(cudaMemcpy(host.data(),out.p,n*sizeof(float),cudaMemcpyDeviceToHost));
        for(std::size_t i=0;i<n;++i) require(host[i]==float(i)*0.25f,"ramp mismatch");
        std::cout << "verified 100003 independent outputs\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
