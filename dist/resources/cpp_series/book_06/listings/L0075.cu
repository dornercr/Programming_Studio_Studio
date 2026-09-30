#include "cuda_support.hpp"
struct Particle {float x,y,z,mass;};
__global__ void aos(const Particle* p,float* y,unsigned n){unsigned i=threadIdx.x;if(i<n)y[i]=2*p[i].mass;}
__global__ void soa(const float* mass,float* y,unsigned n){unsigned i=threadIdx.x;if(i<n)y[i]=2*mass[i];}

int main() {
    try {
        Particle p[8]{};float mass[8]{},a[8]{},b[8]{};
        for(unsigned i=0;i<8;++i){p[i]={1,2,3,float(i+1)};mass[i]=p[i].mass;}
        Device<Particle> dp(8);Device<float> dm(8),da(8),db(8);
        CU(cudaMemcpy(dp.p,p,sizeof p,cudaMemcpyHostToDevice));CU(cudaMemcpy(dm.p,mass,sizeof mass,cudaMemcpyHostToDevice));
        aos<<<1,32>>>(dp.p,da.p,8);CU(cudaGetLastError());soa<<<1,32>>>(dm.p,db.p,8);CU(cudaGetLastError());
        CU(cudaMemcpy(a,da.p,sizeof a,cudaMemcpyDeviceToHost));CU(cudaMemcpy(b,db.p,sizeof b,cudaMemcpyDeviceToHost));
        for(unsigned i=0;i<8;++i)require(a[i]==2*mass[i]&&b[i]==a[i],"layout mismatch");
        std::cout << "mass-only outputs 2 through16 agree\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
