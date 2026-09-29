#include "cuda_support.hpp"
__global__ void reduce64(const int* input,int* output) {
    __shared__ int work[64];unsigned t=threadIdx.x;
    work[t]=input[t];__syncthreads();
    for(unsigned stride=32;stride;stride/=2){
        if(t<stride)work[t]+=work[t+stride];
        __syncthreads();
    }
    if(t==0)output[0]=work[0];
}

int main() {
    try {
        int x[64];for(int i=0;i<64;++i)x[i]=i+1;
        Device<int> dx(64),dy(1);int sum=0;
        CU(cudaMemcpy(dx.p,x,sizeof x,cudaMemcpyHostToDevice));
        reduce64<<<1,64>>>(dx.p,dy.p);CU(cudaGetLastError());
        CU(cudaMemcpy(&sum,dy.p,sizeof sum,cudaMemcpyDeviceToHost));
        require(sum==2080,"reduction mismatch");std::cout << "sum=" << sum << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
