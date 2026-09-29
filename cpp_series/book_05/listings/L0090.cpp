#include <cstddef>

void scale(float* __restrict out,const float* __restrict in,std::size_t n,float factor){
 for(std::size_t i=0;i<n;++i) out[i]=in[i]*factor;
}
