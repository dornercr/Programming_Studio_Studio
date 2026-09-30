#include <algorithm>
#include <optional>
#include <iostream>

int main() {
    auto negotiate=[](int alo,int ahi,int blo,int bhi)->std::optional<int>{int lo=std::max(alo,blo),hi=std::min(ahi,bhi);if(lo>hi)return{};return hi;};
    auto mixed=negotiate(1,2,1,1),none=negotiate(2,2,1,1);
    std::cout<<"mixed-version="<<*mixed<<" new-feature="<<std::boolalpha<<(*mixed>=2)<<" incompatible="<<!none<<'\n';
}
