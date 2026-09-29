#include "check.hpp"
#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>
#if defined(__SSE2__) || defined(_M_X64)
#include <emmintrin.h>
#define HARBOR_HAS_SSE 1
#else
#define HARBOR_HAS_SSE 0
#endif
void add_scalar(std::span<const float> a,std::span<const float> b,std::span<float> output) {
    if (a.size()!=b.size() || a.size()!=output.size()) throw std::invalid_argument("length mismatch");
    for (std::size_t i=0;i<a.size();++i) output[i]=a[i]+b[i];
}
void add_selected(std::span<const float> a,std::span<const float> b,std::span<float> output) {
    if (a.size()!=b.size() || a.size()!=output.size()) throw std::invalid_argument("length mismatch");
    std::size_t i=0;
#if HARBOR_HAS_SSE
    for (;i+4<=a.size();i+=4) {
        const auto x=_mm_loadu_ps(a.data()+i), y=_mm_loadu_ps(b.data()+i);
        _mm_storeu_ps(output.data()+i,_mm_add_ps(x,y));
    }
#endif
    for (;i<a.size();++i) output[i]=a[i]+b[i];
}
int main() {
    for (std::size_t size=0;size<65;++size) {
        std::vector<float> a(size),b(size),expected(size),actual(size);
        for (std::size_t i=0;i<size;++i) { a[i]=static_cast<float>(i); b[i]=static_cast<float>(2*i); }
        add_scalar(a,b,expected); add_selected(a,b,actual);
        CHECK(actual==expected);
    }
    std::cout << "compiled_sse_path=" << HARBOR_HAS_SSE << "\nPASS\n";
}
