#include <array>
#include <optional>
#include <iostream>

int main() {
    for(int gap=0;gap<4;++gap){bool pending=true,done=false;std::optional<int> receipt;int effects=0;
        auto worker=[&](){if(!receipt){++effects;receipt=49;}return *receipt;};
        if(gap>=1)(void)worker(); // remote commit may already exist
        if(gap>=3){done=true;pending=false;}
        if(pending){int result=worker();if(result==49){done=true;pending=false;}}
        if(!done||pending||effects!=1)return 1;
    }
    std::cout<<"four acknowledgement-gap histories: one effect each\n";
}
