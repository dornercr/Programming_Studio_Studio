#include <charconv>
#include <string_view>
#include <stdexcept>
#include <iostream>
#include <cstdint>

int main() {
    auto square=[](std::int64_t x){if(x<0||x>1000000)throw std::invalid_argument("domain range");return x*x;};
    auto handle=[&](std::string_view text){std::int64_t x=0;auto[p,e]=std::from_chars(text.data(),text.data()+text.size(),x);
        if(e!=std::errc{}||p!=text.data()+text.size())throw std::invalid_argument("syntax");return square(x);};
    std::cout<<"result="<<handle("12")<<'\n';
    try{(void)handle("12x");return 1;}catch(const std::invalid_argument&e){std::cout<<e.what()<<'\n';}
}
