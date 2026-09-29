#include <array>
#include <iostream>
#include <numeric>

int main() {
    const std::array<std::size_t,6> next{3,5,4,1,0,2};
    std::size_t index=0;
    std::cout << "dependent path:";
    for(int step=0;step<6;++step) { std::cout << ' ' << index; index=next[index]; }
    const std::array<int,6> contiguous{10,20,30,40,50,60};
    std::cout << "\ncontiguous sum="
              << std::accumulate(contiguous.begin(),contiguous.end(),0) << '\n';
}
