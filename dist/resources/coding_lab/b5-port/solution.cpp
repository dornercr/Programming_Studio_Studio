#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>

int parsePort(std::string_view text) {
    if(text.empty()) throw std::invalid_argument("invalid port");
    int port=0;
    for(char ch:text){
        if(ch<'0'||ch>'9') throw std::invalid_argument("invalid port");
        port=port*10+(ch-'0');
        if(port>65535) throw std::invalid_argument("invalid port");
    }
    if(port==0) throw std::invalid_argument("invalid port");
    return port;
}
