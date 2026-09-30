#include <array>
#include <algorithm>
#include <iostream>

int main() {
    struct Span{int start,end;};const std::array children{Span{10,50},Span{20,60}};int sum=0;
    for(auto s:children)sum+=s.end-s.start;
    std::cout<<"child-duration-sum="<<sum<<" parent-elapsed="<<60-0<<" overlap="<<50-20<<'\n';
}
