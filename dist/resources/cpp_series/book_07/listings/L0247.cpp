#include <array>
#include <algorithm>
#include <iostream>

int main() {
    std::array<unsigned,2> order{1,2};unsigned histories=0;
    do{unsigned current=0;int value=0;for(auto generation:order)if(generation>current){current=generation;value=int(generation)*10;}
        if(current!=2||value!=20)return 1;++histories;
    }while(std::next_permutation(order.begin(),order.end()));
    std::cout<<"histories="<<histories<<" final-generation=2 value=20\n";
}
