#include "harbor/check.hpp"
#include "harbor/wire.hpp"
#include <iostream>
int main() {
    const std::string expected("\0\0\0\3abc",7);
    harbor::check(harbor::frame("abc")==expected,"independent golden bytes");
    for (std::size_t split=0;split<=expected.size();++split) {
        harbor::Decoder d;
        d.feed(std::string_view(expected).substr(0,split));
        d.feed(std::string_view(expected).substr(split));
        d.finish(); harbor::check(d.payload()=="abc","fragmentation");
    }
    std::cout<<"Every two-fragment partition preserves the message.\n";
}
