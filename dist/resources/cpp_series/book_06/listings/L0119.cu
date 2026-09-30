#include "cuda_support.hpp"
__global__ void divide(const float* x,float* y,float divisor,unsigned n){unsigned i=threadIdx.x;if(i<n)y[i]=x[i]/divisor;}
__global__ void multiply(const float* x,float* y,float reciprocal,unsigned n){unsigned i=threadIdx.x;if(i<n)y[i]=x[i]*reciprocal;}

int main() {
    try {
        float input[32],a[32]{},b[32]{};for(unsigned i=0;i<32;++i)input[i]=float(i)-7;
        Device<float> x(32),da(32),db(32);CU(cudaMemcpy(x.p,input,sizeof input,cudaMemcpyHostToDevice));
        divide<<<1,32>>>(x.p,da.p,3.0f,32);CU(cudaGetLastError());
        multiply<<<1,32>>>(x.p,db.p,1.0f/3.0f,32);CU(cudaGetLastError());
        CU(cudaMemcpy(a,da.p,sizeof a,cudaMemcpyDeviceToHost));CU(cudaMemcpy(b,db.p,sizeof b,cudaMemcpyDeviceToHost));
        float error=0;for(unsigned i=0;i<32;++i)error=std::max(error,std::abs(a[i]-b[i]));
        require(error<=1e-6f,"reciprocal approximation exceeds fixture budget");
        std::cout << "maximum error <= 1e-6 for32 values\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
