#include "cuda_support.hpp"
__global__ void coordinates(int* image,unsigned width,unsigned height) {
    unsigned x=blockIdx.x*blockDim.x+threadIdx.x;
    unsigned y=blockIdx.y*blockDim.y+threadIdx.y;
    if(x<width && y<height) image[y*width+x]=int(100*y+x);
}

int main() {
    try {
        constexpr unsigned w=19,h=11;
        Device<int> image(w*h); std::vector<int> host(w*h);
        dim3 block(8,4),grid((w+7)/8,(h+3)/4);
        coordinates<<<grid,block>>>(image.p,w,h); CU(cudaGetLastError());
        CU(cudaMemcpy(host.data(),image.p,host.size()*sizeof(int),cudaMemcpyDeviceToHost));
        for(unsigned y=0;y<h;++y) for(unsigned x=0;x<w;++x)
            require(host[y*w+x]==int(100*y+x),"coordinate mismatch");
        std::cout << "grid=3x3 bottom-right=" << host.back() << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
