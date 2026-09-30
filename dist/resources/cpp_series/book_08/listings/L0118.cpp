#include <cstddef>
#include <iostream>

int main() {
    constexpr std::size_t budget=64*1024;std::size_t reserved=0;
    auto reserve=[&](std::size_t n){if(n>budget-reserved)return false;reserved+=n;return true;};
    bool a=reserve(48*1024),b=reserve(24*1024);
    std::cout<<std::boolalpha<<"first="<<a<<" second="<<b<<" reserved-bytes="<<reserved<<'\n';
}
