#include "cuda_support.hpp"
__global__ void average2(const float* x,float* y,unsigned n) {
    __shared__ float tile[129];
    unsigned local=threadIdx.x, global=blockIdx.x*blockDim.x+local;
    tile[local+1]=global<n?x[global]:0;
    if(local==0)tile[0]=global>0?x[global-1]:0;
    __syncthreads();
    if(global<n)y[global]=0.5f*(tile[local]+tile[local+1]);
}

int main() {
    try {
        constexpr unsigned n=131;std::vector<float>x(n),y(n);
        for(unsigned i=0;i<n;++i)x[i]=float(i+1);
        Device<float> dx(n),dy(n);
        CU(cudaMemcpy(dx.p,x.data(),n*sizeof(float),cudaMemcpyHostToDevice));
        average2<<<2,128>>>(dx.p,dy.p,n);CU(cudaGetLastError());
        CU(cudaMemcpy(y.data(),dy.p,n*sizeof(float),cudaMemcpyDeviceToHost));
        for(unsigned i=0;i<n;++i)require(y[i]==0.5f*(x[i]+(i?x[i-1]:0)),"tile mismatch");
        std::cout << "first=0.5 block-boundary=128.5 last=130.5\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
