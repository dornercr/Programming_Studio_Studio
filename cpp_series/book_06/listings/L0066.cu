#include "cuda_support.hpp"
template<int Pitch> __global__ void transpose(const float* x,float* y) {
    __shared__ float tile[32][Pitch];
    unsigned row=threadIdx.y,col=threadIdx.x;
    tile[row][col]=x[row*32+col];
    __syncthreads();
    y[row*32+col]=tile[col][row];
}

int main() {
    try {
        cudaDeviceProp prop{};CU(cudaGetDeviceProperties(&prop,0));
        require(prop.maxThreadsPerBlock>=1024,"fixture needs 1024-thread blocks");
        std::vector<float>x(1024),a(1024),b(1024);
        for(unsigned i=0;i<x.size();++i)x[i]=float(i);
        Device<float> dx(1024),da(1024),db(1024);
        CU(cudaMemcpy(dx.p,x.data(),4096,cudaMemcpyHostToDevice));
        transpose<32><<<1,dim3(32,32)>>>(dx.p,da.p);CU(cudaGetLastError());
        transpose<33><<<1,dim3(32,32)>>>(dx.p,db.p);CU(cudaGetLastError());
        CU(cudaMemcpy(a.data(),da.p,4096,cudaMemcpyDeviceToHost));
        CU(cudaMemcpy(b.data(),db.p,4096,cudaMemcpyDeviceToHost));
        for(unsigned r=0;r<32;++r)for(unsigned c=0;c<32;++c)
            require(a[r*32+c]==x[c*32+r] && b[r*32+c]==a[r*32+c],"transpose mismatch");
        std::cout << "both tile pitches transpose 1024 values\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
