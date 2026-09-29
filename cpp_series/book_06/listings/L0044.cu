#include "cuda_support.hpp"
__global__ void matvec(const float* a,const float* x,float* y,unsigned rows,unsigned cols) {
    unsigned row=blockIdx.x*blockDim.x+threadIdx.x;
    if(row>=rows) return;
    float sum=0;
    for(unsigned col=0;col<cols;++col) sum+=a[row*cols+col]*x[col];
    y[row]=sum;
}

int main() {
    try {
        float a[15]{1,2,3, 4,5,6, 7,8,9, 10,11,12, 13,14,15};
        float x[3]{1,0,-1},y[5]{};
        Device<float> da(15),dx(3),dy(5);
        CU(cudaMemcpy(da.p,a,sizeof a,cudaMemcpyHostToDevice));
        CU(cudaMemcpy(dx.p,x,sizeof x,cudaMemcpyHostToDevice));
        matvec<<<1,32>>>(da.p,dx.p,dy.p,5,3); CU(cudaGetLastError());
        CU(cudaMemcpy(y,dy.p,sizeof y,cudaMemcpyDeviceToHost));
        for(auto v:y) require(v==-2.0f,"dot product mismatch");
        std::cout << "five row dot products=-2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
