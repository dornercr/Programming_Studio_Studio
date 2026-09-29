#include "cuda_support.hpp"
// No device kernel is needed to test API-return handling.

int main() {
    try {
        int count=0;CU(cudaGetDeviceCount(&count));require(count>0,"a CUDA device is required");
        bool rejected=false;
        try { CU(cudaSetDevice(count)); }
        catch(const std::runtime_error& e) {
            rejected=std::string(e.what()).find("cudaSetDevice")!=std::string::npos;
        }
        require(rejected,"invalid ordinal was not diagnosed");
        CU(cudaSetDevice(0));std::cout << "invalid device ordinal rejected with operation context\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
