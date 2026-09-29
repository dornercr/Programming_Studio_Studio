#include "cuda_support.hpp"
__global__ void initialize(int* x){unsigned i=threadIdx.x;if(i<16)x[i]=int(i);}
__global__ void consume(const int* x,int* y){unsigned i=threadIdx.x;if(i<16)y[i]=x[i]+10;}

int main() {
    try {
        Device<int> x(16),y(16);Stream producer,consumer;Event ready;int host[16]{};
        initialize<<<1,32,0,producer.s>>>(x.p);CU(cudaGetLastError());
        CU(cudaEventRecord(ready.e,producer.s));CU(cudaStreamWaitEvent(consumer.s,ready.e,0));
        consume<<<1,32,0,consumer.s>>>(x.p,y.p);CU(cudaGetLastError());
        CU(cudaStreamSynchronize(consumer.s));
        CU(cudaMemcpy(host,y.p,sizeof host,cudaMemcpyDeviceToHost));
        for(int i=0;i<16;++i)require(host[i]==i+10,"cross-stream order failed");
        std::cout << "consumer observed10 through25\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
