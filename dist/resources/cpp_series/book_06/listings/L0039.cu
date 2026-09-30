#include "cuda_support.hpp"
__global__ void piecewise(float* out,bool grouped) {
    unsigned lane=threadIdx.x;
    unsigned logical=grouped ? (lane<16?2*lane:2*(lane-16)+1) : lane;
    float x=float(logical);
    out[logical]=(logical%2==0)?x*x:-x;
}

int main() {
    try {
        Device<float> a(32),b(32); std::vector<float> ha(32),hb(32);
        piecewise<<<1,32>>>(a.p,false); CU(cudaGetLastError());
        piecewise<<<1,32>>>(b.p,true); CU(cudaGetLastError());
        CU(cudaMemcpy(ha.data(),a.p,128,cudaMemcpyDeviceToHost));
        CU(cudaMemcpy(hb.data(),b.p,128,cudaMemcpyDeviceToHost));
        require(ha==hb,"reordering changed logical values");
        std::cout << "same values: even30=" << ha[30] << " odd31=" << ha[31] << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
