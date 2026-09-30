#include "cuda_support.hpp"
__global__ void add_offset(int* out,int begin,int count){int i=int(threadIdx.x);if(i<count)out[i]=(begin+i)*2;}

int main() {
    try {
        int devices=0;CU(cudaGetDeviceCount(&devices));require(devices>=2,"two visible GPUs are required");
        int combined[10]{};
        for(int ordinal=0;ordinal<2;++ordinal){
            CU(cudaSetDevice(ordinal));
            Device<int> shard(5);
            add_offset<<<1,32>>>(shard.p,ordinal*5,5);CU(cudaGetLastError());
            CU(cudaMemcpy(combined+ordinal*5,shard.p,5*sizeof(int),cudaMemcpyDeviceToHost));
        }
        for(int i=0;i<10;++i)require(combined[i]==2*i,"shard placement mismatch");
        std::cout << "two-device result: 0 2 4 6 8 10 12 14 16 18\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
