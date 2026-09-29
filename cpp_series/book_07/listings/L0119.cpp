#include <cstdint>
#include <string>
#include <iostream>

int main() {
    struct Resource{std::uint64_t fence=0;std::string value;
        bool write(std::uint64_t token,std::string next){if(token<fence)return false;fence=token;value=std::move(next);return true;}};
    Resource r;r.write(10,"old");r.write(11,"new-owner");bool late=r.write(10,"late-old-write");
    std::cout<<"late-accepted="<<std::boolalpha<<late<<" value="<<r.value<<'\n';
}
