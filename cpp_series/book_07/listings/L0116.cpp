#include <array>
#include <iostream>

int main() {
    struct Entry{unsigned index;int delta;};const std::array log{Entry{1,5},Entry{2,-2},Entry{3,100}};
    unsigned committed=2,applied=0;int value=0;
    for(auto e:log)if(e.index<=committed&&e.index>applied){value+=e.delta;applied=e.index;}
    std::cout<<"received="<<log.size()<<" applied="<<applied<<" value="<<value<<'\n';
}
