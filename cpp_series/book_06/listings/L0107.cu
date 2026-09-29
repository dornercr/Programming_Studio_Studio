#include "cuda_support.hpp"
__global__ void square(float* x,unsigned n){unsigned i=blockIdx.x*blockDim.x+threadIdx.x;if(i<n)x[i]*=x[i];}

int main() {
    try {
        constexpr unsigned n=1u<<20;Pinned<float> host(n);Device<float> data(n);Stream stream;Event start,copied,computed;
        for(unsigned i=0;i<n;++i)host.p[i]=2;
        CU(cudaFree(nullptr)); // initialize runtime before the measured chain
        CU(cudaEventRecord(start.e,stream.s));
        CU(cudaMemcpyAsync(data.p,host.p,n*sizeof(float),cudaMemcpyHostToDevice,stream.s));
        CU(cudaEventRecord(copied.e,stream.s));
        square<<<n/256,256,0,stream.s>>>(data.p,n);CU(cudaGetLastError());
        CU(cudaEventRecord(computed.e,stream.s));CU(cudaEventSynchronize(computed.e));
        float copy_ms=0,kernel_ms=0;CU(cudaEventElapsedTime(&copy_ms,start.e,copied.e));
        CU(cudaEventElapsedTime(&kernel_ms,copied.e,computed.e));
        CU(cudaMemcpy(host.p,data.p,n*sizeof(float),cudaMemcpyDeviceToHost));
        for(unsigned i=0;i<n;++i)require(host.p[i]==4,"timed square failed");
        std::cout << "copy_ms=" << copy_ms << " kernel_ms=" << kernel_ms << " verified=1048576\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
