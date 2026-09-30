#include <cstdlib>
#include <iostream>
#if defined(__GLIBC__)
#include <malloc.h>
#endif

int main() {
    void* p = std::malloc(13);
    if (!p) return 1;
    std::cout << "requested=13";
#if defined(__GLIBC__)
    std::cout << " usable=" << malloc_usable_size(p);
#endif
    std::cout << "\n";
    std::free(p);
}
