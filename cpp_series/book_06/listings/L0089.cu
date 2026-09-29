#include "cuda_support.hpp"
__global__ void increment(float* x,unsigned n){unsigned i=threadIdx.x;if(i<n)x[i]+=1;}

int main() {
    try {
        constexpr unsigned n=64;Pinned<float> input(n),output(n);Device<float> data(n);Stream stream;
        for(unsigned i=0;i<n;++i)input.p[i]=float(i);
        CU(cudaMemcpyAsync(data.p,input.p,n*sizeof(float),cudaMemcpyHostToDevice,stream.s));
        increment<<<1,64,0,stream.s>>>(data.p,n);CU(cudaGetLastError());
        CU(cudaMemcpyAsync(output.p,data.p,n*sizeof(float),cudaMemcpyDeviceToHost,stream.s));
        unsigned host_work=0;for(unsigned i=0;i<100;++i)host_work+=i;
        CU(cudaStreamSynchronize(stream.s));
        for(unsigned i=0;i<n;++i)require(output.p[i]==float(i+1),"async chain mismatch");
        std::cout << "last=64 independent-host-sum=" << host_work << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
