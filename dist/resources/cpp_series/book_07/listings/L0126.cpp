#include <array>
#include <map>
#include <string>
#include <iostream>

int main() {
    struct Order{int tenant,order;};const std::array orders{Order{10,1},Order{10,2},Order{10,3},Order{11,4}};
    std::map<int,unsigned> load;
    for(auto o:orders){int partition=o.tenant%2;++load[partition];}
    std::cout<<"tenant10 partition="<<10%2<<" work="<<load[0]<<" tenant11 work="<<load[1]<<'\n';
}
