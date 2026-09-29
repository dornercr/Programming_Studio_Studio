#include <cstddef>
#include <iostream>

int main() {
    unsigned connections=0;std::size_t bytes=0;
    auto admit=[&](std::size_t size){if(connections==3||size>100-bytes)return false;++connections;bytes+=size;return true;};
    bool a=admit(60),b=admit(50),c=admit(30);
    std::cout<<std::boolalpha<<a<<' '<<b<<' '<<c<<" connections="<<connections<<" bytes="<<bytes<<'\n';
}
