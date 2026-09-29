#include "cuda_support.hpp"
__global__ void scale(float* x,unsigned n){unsigned i=blockIdx.x*blockDim.x+threadIdx.x;if(i<n)x[i]*=2;}

int main() {
    try {
        constexpr unsigned n=4096;Pinned<float> host0(n),host1(n);Device<float> d0(n),d1(n);Stream s0,s1;
        for(unsigned i=0;i<n;++i){host0.p[i]=1;host1.p[i]=3;}
        auto submit=[&](float* host,float* device,cudaStream_t stream){
            CU(cudaMemcpyAsync(device,host,n*sizeof(float),cudaMemcpyHostToDevice,stream));
            scale<<<16,256,0,stream>>>(device,n);CU(cudaGetLastError());
            CU(cudaMemcpyAsync(host,device,n*sizeof(float),cudaMemcpyDeviceToHost,stream));
        };
        submit(host0.p,d0.p,s0.s);submit(host1.p,d1.p,s1.s);
        CU(cudaStreamSynchronize(s0.s));CU(cudaStreamSynchronize(s1.s));
        for(unsigned i=0;i<n;++i)require(host0.p[i]==2&&host1.p[i]==6,"pipeline mismatch");
        std::cout << "chunk0=2 chunk1=6\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
