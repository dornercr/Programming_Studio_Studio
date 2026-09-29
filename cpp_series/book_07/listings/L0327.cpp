#include "harbor/net.hpp"
#include <iostream>
int main(int argc,char** argv) {
    try {
        if (argc<3) throw std::invalid_argument("usage: harbor_client PORT COMMAND [ARGUMENTS...]");
        std::string command;
        for (int i=2;i<argc;++i) { if (i>2) command+=' '; command+=argv[i]; }
        std::cout<<harbor::exchange(harbor::port_number(argv[1]),command,
                                  std::chrono::milliseconds(1500))<<'\n';
        return 0;
    } catch (const std::exception& e) { std::cerr<<e.what()<<'\n'; return 2; }
}
