#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

int main() {
    std::vector<std::uint8_t> buffer;std::vector<std::string> messages;
    auto feed=[&](std::uint8_t byte){buffer.push_back(byte);
        while(buffer.size()>=2){std::size_t n=(std::size_t(buffer[0])<<8)|buffer[1];
            if(n>8)throw std::length_error("frame limit");if(buffer.size()<n+2)return;
            messages.emplace_back(buffer.begin()+2,buffer.begin()+2+n);buffer.erase(buffer.begin(),buffer.begin()+2+n);}};
    for(auto byte:std::vector<std::uint8_t>{0,2,'O','K',0,1,'!'})feed(byte);
    if(!buffer.empty()||messages!=std::vector<std::string>{"OK","!"})return 1;
    std::cout<<"decoded=OK,! residual=0\n";
}
