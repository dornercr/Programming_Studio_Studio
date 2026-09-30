#include <cstdint>
#include <iostream>
#include <vector>

struct Datagram { std::uint32_t sequence; int value; };

int main(){
 std::vector<Datagram> received{{10,1},{11,2},{13,4}};
 std::uint32_t expected=10;
 for(const auto& d:received){
   if(d.sequence!=expected) std::cout<<"gap expected="<<expected<<" got="<<d.sequence<<"\n";
   expected=d.sequence+1;
 }
}
