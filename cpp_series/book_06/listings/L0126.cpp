#include <span>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <numeric>

int main() {
    auto backend=[](std::span<const float> in,std::span<float> out){
        if(in.size()!=out.size())throw std::invalid_argument("shape");
        for(std::size_t i=0;i<in.size();++i)out[i]=in[i]*in[i];
    };
    std::vector<float> batch{1,2,3,4},output(batch.size());
    if(batch.empty()||batch.size()>1024)throw std::invalid_argument("batch limit");
    backend(batch,output);
    std::cout << "accepted=" << batch.size() << " energy="
              << std::accumulate(output.begin(),output.end(),0.0f) << '\n';
}
