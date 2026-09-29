#include "cuda_support.hpp"
__global__ void gather(const float* x,float* y,bool strided) {
    unsigned i=blockIdx.x*blockDim.x+threadIdx.x;
    if(i<4096){unsigned src=strided?(i%128)*32+i/128:i;y[i]=x[src];}
}

int main() {
    try {
        std::vector<float>x(4096),a(4096),b(4096);for(unsigned i=0;i<4096;++i)x[i]=float(i);
        Device<float> dx(4096),da(4096),db(4096);
        CU(cudaMemcpy(dx.p,x.data(),x.size()*sizeof(float),cudaMemcpyHostToDevice));
        gather<<<16,256>>>(dx.p,da.p,false);CU(cudaGetLastError());
        gather<<<16,256>>>(dx.p,db.p,true);CU(cudaGetLastError());
        CU(cudaMemcpy(a.data(),da.p,a.size()*sizeof(float),cudaMemcpyDeviceToHost));
        CU(cudaMemcpy(b.data(),db.p,b.size()*sizeof(float),cudaMemcpyDeviceToHost));
        for(unsigned i=0;i<4096;++i)require(a[i]==x[i]&&b[i]==x[(i%128)*32+i/128],"gather mismatch");
        std::cout << "contiguous first=0 strided second=32\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
