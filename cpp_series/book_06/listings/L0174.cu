#include "cuda_support.hpp"
#include <chrono>
__global__ void power_samples(const float* in,float* out,unsigned n){unsigned i=blockIdx.x*blockDim.x+threadIdx.x;if(i<n)out[i]=in[i]*in[i];}

int main() {
    try {
        constexpr unsigned n=1024;Pinned<float> in0(n),in1(n),out0(n),out1(n);
        Device<float> x0(n),x1(n),y0(n),y1(n);Stream s0,s1;
        auto start=std::chrono::steady_clock::now();
        auto submit=[&](float* in,float* out,float* x,float* y,cudaStream_t s,float amplitude){
            for(unsigned i=0;i<n;++i)in[i]=amplitude;
            CU(cudaMemcpyAsync(x,in,n*sizeof(float),cudaMemcpyHostToDevice,s));
            power_samples<<<4,256,0,s>>>(x,y,n);CU(cudaGetLastError());
            CU(cudaMemcpyAsync(out,y,n*sizeof(float),cudaMemcpyDeviceToHost,s));
        };
        submit(in0.p,out0.p,x0.p,y0.p,s0.s,2);submit(in1.p,out1.p,x1.p,y1.p,s1.s,3);
        CU(cudaStreamSynchronize(s0.s));CU(cudaStreamSynchronize(s1.s));
        auto elapsed=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-start).count();
        for(unsigned i=0;i<n;++i)require(out0.p[i]==4&&out1.p[i]==9,"power pipeline mismatch");
        std::cout<<"powers=4,9 end_to_end_us="<<elapsed<<'\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
