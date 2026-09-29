#include "cuda_support.hpp"
__global__ void index_values(int* out,unsigned n) {
    unsigned i=blockIdx.x*blockDim.x+threadIdx.x;
    if(i<n) out[i]=int(i);
}

int main() {
    try {
        constexpr unsigned n=259, block=128;
        std::vector<int> host(n+1,-99); Device<int> data(n+1);
        CU(cudaMemcpy(data.p,host.data(),host.size()*sizeof(int),cudaMemcpyHostToDevice));
        index_values<<<(n+block-1)/block,block>>>(data.p,n); CU(cudaGetLastError());
        CU(cudaMemcpy(host.data(),data.p,host.size()*sizeof(int),cudaMemcpyDeviceToHost));
        for(unsigned i=0;i<n;++i) require(host[i]==int(i),"index mismatch");
        require(host[n]==-99,"logical-end canary changed");
        std::cout << "last=258 canary=-99\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
