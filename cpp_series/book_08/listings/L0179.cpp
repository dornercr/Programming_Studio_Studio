#include <cstdint>
#include <iostream>

int main() {
    std::uint64_t services=5,outcomes=4,regions=3,users=100000;
    auto bounded=services*outcomes*regions;
    std::cout<<"bounded-series="<<bounded<<" with-user-id="<<bounded*users<<'\n';
}
