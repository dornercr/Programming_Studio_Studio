#include <array>
#include <cmath>
#include <iostream>
#include <algorithm>

int main() {
    const std::array<double,4> reference{0,1,1000,-2};
    const std::array<double,4> observed{1e-9,1.0000001,1000.5,-2};
    auto close=[](double a,double b){return std::isfinite(a)&&std::isfinite(b)&&std::abs(a-b)<=1e-8+1e-6*std::max(std::abs(a),std::abs(b));};
    std::size_t mismatch=reference.size();
    for(std::size_t i=0;i<reference.size();++i)if(!close(reference[i],observed[i])){mismatch=i;break;}
    if(mismatch!=2)return 1;
    std::cout<<"mismatch index="<<mismatch<<" expected="<<reference[mismatch]<<" observed="<<observed[mismatch]<<'\n';
}
