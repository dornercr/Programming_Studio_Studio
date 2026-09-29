#include "cuda_support.hpp"
__global__ void negate(int* values,unsigned n) {
    unsigned i=threadIdx.x;if(i<n)values[i]=-values[i];
}

int main() {
    try {
        int input[8]{0,1,2,3,4,5,6,7},output[8]{};
        Device<int> data(8);
        CU(cudaMemcpy(data.p,input,sizeof input,cudaMemcpyHostToDevice));
        negate<<<1,32>>>(data.p+2,3); CU(cudaGetLastError());
        CU(cudaMemcpy(output,data.p,sizeof output,cudaMemcpyDeviceToHost));
        int expected[8]{0,1,-2,-3,-4,5,6,7};
        for(unsigned i=0;i<8;++i)require(output[i]==expected[i],"slice mismatch");
        for(auto x:output)std::cout << x << ' ';
        std::cout << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
