#include <immintrin.h>
#include <iostream>

int main(){
 alignas(16) float a[4]{1,2,3,4}, b[4]{10,20,30,40}, out[4]{};
 __m128 va=_mm_load_ps(a); __m128 vb=_mm_load_ps(b);
 __m128 vc=_mm_add_ps(va,vb); _mm_store_ps(out,vc);
 std::cout<<out[0]<<' '<<out[3]<<"\n";
}
