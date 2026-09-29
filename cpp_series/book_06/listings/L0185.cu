#include <cuda_runtime.h>
#include <iostream>
int main(){int count=0;auto e=cudaGetDeviceCount(&count);
  if(e!=cudaSuccess||count==0){std::cerr<<"no usable CUDA device\n";return 1;}
  int runtime=0,driver=0;
  if(cudaRuntimeGetVersion(&runtime)!=cudaSuccess||cudaDriverGetVersion(&driver)!=cudaSuccess)return 2;
  std::cout<<"devices="<<count<<" runtime="<<runtime<<" driver="<<driver<<'\n';
}
