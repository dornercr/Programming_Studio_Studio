#include "cuda_support.hpp"
// Device discovery is a host runtime operation.

int main() {
    try {
        int count=0;CU(cudaGetDeviceCount(&count));require(count>0,"no devices");
        for(int i=0;i<count;++i){cudaDeviceProp p{};CU(cudaGetDeviceProperties(&p,i));
            std::cout << "device=" << i << " name=" << p.name << " compute=" << p.major << '.' << p.minor
                      << " global-bytes=" << p.totalGlobalMem << '\n';}
        const int selected=count-1;CU(cudaSetDevice(selected));
        {Device<int> data(1);CU(cudaMemset(data.p,0,sizeof(int)));CU(cudaDeviceSynchronize());}
        int current=-1;CU(cudaGetDevice(&current));require(current==selected,"wrong active device");
        std::cout << "selected=" << current << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
