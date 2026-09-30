#include <algorithm>
#include <cstdint>
#include <iostream>

int main() {
    std::uint64_t a=0,b=0;
    auto local=[](std::uint64_t&c){return ++c;};
    local(a);auto sent=local(a);local(b);
    b=std::max(b,sent)+1;auto reply=local(b);a=std::max(a,reply)+1;
    std::cout<<"send="<<sent<<" receive="<<reply-1<<" reply="<<reply<<" final-a="<<a<<'\n';
}
