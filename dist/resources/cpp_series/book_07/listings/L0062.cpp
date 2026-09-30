#include <string>
#include <string_view>
#include <algorithm>
#include <iostream>

int main() {
    std::string pending="RESULT",wire;std::size_t offset=0;bool draining=true;
    auto admit=[&](std::string_view text){if(draining)return false;pending+=text;return true;};
    bool extra=admit("NEW");unsigned attempts=0;
    while(offset<pending.size()){auto n=std::min<std::size_t>(2,pending.size()-offset);
        wire.append(pending,offset,n);offset+=n;++attempts;}
    std::cout<<"wire="<<wire<<" attempts="<<attempts<<" accepted-during-drain="<<std::boolalpha<<extra<<'\n';
}
