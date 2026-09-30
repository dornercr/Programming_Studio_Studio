#include <array>
#include <string_view>
#include <iostream>

int main() {
    auto write_policy=[](unsigned reachable)->std::string_view{return reachable>=2?"may-propose":"unavailable";};
    for(unsigned reachable:std::array{1u,2u,3u})std::cout<<reachable<<" reachable: "<<write_policy(reachable)<<'\n';
}
