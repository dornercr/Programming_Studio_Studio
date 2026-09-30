#include <array>
#include <cstdint>
#include <iostream>

int main() {
    struct Frame{std::uint64_t sequence;std::uint64_t start_ns;std::array<float,4> samples;};
    auto acquire=[](){return Frame{7,1000000,{1,2,3,4}};};
    auto process=[](Frame f){for(auto&v:f.samples)v*=0.5f;return f;};
    auto output=[](const Frame&f){float sum=0;for(float v:f.samples)sum+=v;
        std::cout<<"sequence="<<f.sequence<<" sample-time="<<f.start_ns<<" sum="<<sum<<'\n';};
    output(process(acquire()));
}
