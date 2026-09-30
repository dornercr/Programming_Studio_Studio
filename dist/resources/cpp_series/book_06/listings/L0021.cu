#include "cuda_support.hpp"
__global__ void add4(const int* a,const int* b,int* c) {
    unsigned i=threadIdx.x;
    if(i<4) c[i]=a[i]+b[i];
}

int main() {
    try {
        int a[4]{1,2,3,4}, b[4]{10,20,30,40}, c[4]{};
        Device<int> da(4),db(4),dc(4);
        CU(cudaMemcpy(da.p,a,sizeof a,cudaMemcpyHostToDevice));
        CU(cudaMemcpy(db.p,b,sizeof b,cudaMemcpyHostToDevice));
        add4<<<1,32>>>(da.p,db.p,dc.p); CU(cudaGetLastError());
        CU(cudaMemcpy(c,dc.p,sizeof c,cudaMemcpyDeviceToHost));
        for(int i=0;i<4;++i) require(c[i]==a[i]+b[i],"addition mismatch");
        std::cout << c[0] << ' ' << c[1] << ' ' << c[2] << ' ' << c[3] << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
