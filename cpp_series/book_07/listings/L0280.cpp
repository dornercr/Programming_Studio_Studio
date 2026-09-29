#include <cstdint>
#include <iostream>

int main() {
    constexpr std::uint64_t requests=10000,failed=7; // illustrative evaluation window
    constexpr std::uint64_t allowed_per_thousand=1;
    auto budget=requests*allowed_per_thousand/1000;
    std::cout<<"allowed-failures="<<budget<<" observed="<<failed<<" remaining="<<(failed<=budget?budget-failed:0)<<'\n';
}
