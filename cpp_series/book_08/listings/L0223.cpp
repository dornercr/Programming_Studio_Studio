#include <cstdint>
#include <iostream>

int main() {
    std::uint64_t total=100000,allowed=total/1000,failed=120;
    auto remaining=failed<=allowed?allowed-failed:0;
    auto excess=failed>allowed?failed-allowed:0;
    std::cout<<"allowed="<<allowed<<" remaining="<<remaining<<" excess="<<excess<<'\n';
}
