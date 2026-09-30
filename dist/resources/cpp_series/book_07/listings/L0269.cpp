#include "harbor/check.hpp"
#include <iostream>
int main() {
    static_assert(__cplusplus>=202002L);
    harbor::check(sizeof(void*)>=4,"supported word size");
    std::cout<<"language="<<__cplusplus<<" pointer_bits="<<sizeof(void*)*8<<'\n';
#ifdef __clang__
    std::cout<<"compiler=clang "<<__clang_major__<<'.'<<__clang_minor__<<'\n';
#elif defined(__GNUC__)
    std::cout<<"compiler=gcc "<<__GNUC__<<'.'<<__GNUC_MINOR__<<'\n';
#endif
    std::cout<<"Record dependencies and artifact digests separately; a version string is not provenance.\n";
}
