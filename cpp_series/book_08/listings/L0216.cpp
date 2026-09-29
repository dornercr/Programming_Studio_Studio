#include <array>
#include <iostream>

int main() {
    struct Request{bool user,successful;};const std::array trace{Request{true,true},Request{false,false},Request{true,false},Request{true,true}};
    unsigned eligible=0,good=0;for(auto r:trace)if(r.user){++eligible;good+=r.successful;}
    std::cout<<"good="<<good<<" eligible="<<eligible<<" health-probes-excluded\n";
}
