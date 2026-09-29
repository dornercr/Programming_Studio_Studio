#include <array>
#include <iostream>

int main() {
    const std::array<int,3>x{1,2,3};const std::array<int,2>h{1,-1};
    std::array<int,4>convolution{};
    for(std::size_t n=0;n<convolution.size();++n)
        for(std::size_t k=0;k<h.size();++k)
            if(n>=k && n-k<x.size())convolution[n]+=h[k]*x[n-k];
    std::array<int,3>correlation{};
    for(std::size_t lag=0;lag<3;++lag)
        for(std::size_t i=0;i+lag<3;++i)correlation[lag]+=x[i]*x[i+lag];
    std::cout << "convolution:";for(int v:convolution)std::cout << ' ' << v;
    std::cout << "\ncorrelation:";for(int v:correlation)std::cout << ' ' << v;std::cout << '\n';
}
