#include "cuda_support.hpp"
#include <cufft.h>
void fftcheck(cufftResult r){if(r!=CUFFT_SUCCESS)throw std::runtime_error("cuFFT error");}
struct Plan{cufftHandle p{};Plan(){fftcheck(cufftPlan1d(&p,4,CUFFT_C2C,1));}
    ~Plan(){(void)cufftDestroy(p);}};
int main(){try{
    cufftComplex input[4]{{1,0},{0,0},{0,0},{0,0}},output[4]{};
    Device<cufftComplex>x(4),y(4);Plan plan;
    CU(cudaMemcpy(x.p,input,sizeof input,cudaMemcpyHostToDevice));
    fftcheck(cufftExecC2C(plan.p,x.p,y.p,CUFFT_FORWARD));
    CU(cudaMemcpy(output,y.p,sizeof output,cudaMemcpyDeviceToHost));
    for(auto v:output)require(std::abs(v.x-1)<1e-6f&&std::abs(v.y)<1e-6f,"FFT mismatch");
    std::cout<<"cuFFT impulse bins=1+0i\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
