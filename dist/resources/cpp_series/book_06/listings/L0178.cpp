#include <span>
#include <vector>
#include <stdexcept>
#include <iostream>

int main() {
    struct Processor{virtual~Processor()=default;virtual void apply(std::span<const float>,std::span<float>)=0;};
    struct Cpu final:Processor{void apply(std::span<const float>x,std::span<float>y)override{
        if(x.size()!=y.size())throw std::invalid_argument("shape");
        for(std::size_t i=0;i<x.size();++i)y[i]=x[i]+1;
    }};
    auto run=[](Processor& backend){std::vector<float>x{2,4,6},y(3);backend.apply(x,y);return y;};
    Cpu cpu;auto y=run(cpu);
    std::cout<<"application outputs:";for(auto v:y)std::cout<<' '<<v;std::cout<<'\n';
}
