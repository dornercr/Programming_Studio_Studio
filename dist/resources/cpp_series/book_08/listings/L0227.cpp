#include <array>
#include <iostream>

int main() {
    unsigned consecutive=0,pages=0;bool firing=false;
    for(bool symptom:std::array{false,true,false,true,true,true}){
        consecutive=symptom?consecutive+1:0;
        bool next=consecutive>=3;if(next&&!firing)++pages;firing=next;
    }
    std::cout<<"page-transitions="<<pages<<" firing="<<std::boolalpha<<firing<<'\n';
}
