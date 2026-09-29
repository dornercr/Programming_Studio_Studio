#include "cuda_support.hpp"
__host__ __device__ int affine(int x) { return 3*x+2; }
__global__ void evaluate(int* result) { result[0]=affine(7); }

int main() {
    try {
        Device<int> result(1); int observed=0;
        evaluate<<<1,1>>>(result.p); CU(cudaGetLastError());
        CU(cudaMemcpy(&observed,result.p,sizeof observed,cudaMemcpyDeviceToHost));
        require(observed==affine(7),"host/device disagreement");
        std::cout << "host=" << affine(7) << " device=" << observed << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
