#include "cuda_support.hpp"
__global__ void twice(const float* x,float* t,unsigned n) {
    unsigned i=blockIdx.x*blockDim.x+threadIdx.x;if(i<n)t[i]=2*x[i];
}
__global__ void offset(const float* t,float* y,unsigned n) {
    unsigned i=blockIdx.x*blockDim.x+threadIdx.x;if(i<n)y[i]=t[i]+3;
}
__global__ void fused(const float* x,float* y,unsigned n) {
    unsigned i=blockIdx.x*blockDim.x+threadIdx.x;if(i<n)y[i]=2*x[i]+3;
}

int main() {
    try {
        constexpr unsigned n=513; std::vector<float> x(n),a(n),b(n);
        for(unsigned i=0;i<n;++i)x[i]=float(i);
        Device<float> dx(n),temp(n),da(n),db(n);
        CU(cudaMemcpy(dx.p,x.data(),n*sizeof(float),cudaMemcpyHostToDevice));
        twice<<<3,256>>>(dx.p,temp.p,n); CU(cudaGetLastError());
        offset<<<3,256>>>(temp.p,da.p,n); CU(cudaGetLastError());
        fused<<<3,256>>>(dx.p,db.p,n); CU(cudaGetLastError());
        CU(cudaMemcpy(a.data(),da.p,n*sizeof(float),cudaMemcpyDeviceToHost));
        CU(cudaMemcpy(b.data(),db.p,n*sizeof(float),cudaMemcpyDeviceToHost));
        require(a==b && b.back()==1027.0f,"fusion mismatch");
        std::cout << "513 equal outputs, last=1027\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
