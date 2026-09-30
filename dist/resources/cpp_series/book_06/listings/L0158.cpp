#include <array>
#include <cstddef>
#include <iostream>

int main() {
    struct MovingSum{std::array<int,4> delay{};std::size_t next=0;int sum=0;
        int process(int x){sum-=delay[next];delay[next]=x;sum+=x;next=(next+1)%delay.size();return sum;}};
    MovingSum filter;
    const std::array<int,6> input{1,2,3,4,5,6};
    std::array<int,6> output{};
    for(std::size_t i=0;i<input.size();++i)output[i]=filter.process(input[i]);
    std::cout << "moving sums:";for(int x:output)std::cout << ' ' << x;std::cout << '\n';
}
