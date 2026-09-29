#include "cuda_support.hpp"
__constant__ float taps[3];
__global__ void fir3(const float* input,float* output,unsigned n) {
    unsigned i=blockIdx.x*blockDim.x+threadIdx.x;
    if(i>=n)return;
    float sum=0;
    for(unsigned k=0;k<3;++k)if(i>=k)sum+=taps[k]*input[i-k];
    output[i]=sum;
}

int main() {
    try {
        float weights[3]{0.25f,0.5f,0.25f},input[5]{1,2,3,4,5},output[5]{};
        Device<float> x(5),y(5);
        CU(cudaMemcpyToSymbol(taps,weights,sizeof weights));
        CU(cudaMemcpy(x.p,input,sizeof input,cudaMemcpyHostToDevice));
        fir3<<<1,32>>>(x.p,y.p,5);CU(cudaGetLastError());
        CU(cudaMemcpy(output,y.p,sizeof output,cudaMemcpyDeviceToHost));
        float expected[5]{0.25f,1.0f,2.0f,3.0f,4.0f};
        for(int i=0;i<5;++i)require(output[i]==expected[i],"FIR mismatch");
        std::cout << "FIR: 0.25 1 2 3 4\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
