#include <array>
#include <iostream>

int main() {
    int remaining=100;unsigned attempts=0;
    for(int delay:std::array{10,20,40}){
        constexpr int work=25;if(remaining<work)break;
        remaining-=work;++attempts;
        if(remaining<delay)break;remaining-=delay;
    }
    std::cout<<"attempts="<<attempts<<" remaining-ms="<<remaining<<'\n';
}
