#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <string_view>
#include <sstream>
#include <iterator>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

struct Request{std::size_t vertices=1,source=0,target=0;std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> edges;bool operator==(const Request&)const=default;};
void loadRequest(std::string_view text,Request& accepted){
    std::istringstream input{std::string(text)};
    auto field=[&](std::size_t limit){
        std::string token;if(!(input>>token)) throw std::invalid_argument("missing field");
        std::size_t value=0;
        for(char c:token){
            if(c<'0'||c>'9') throw std::invalid_argument("digits required");
            auto digit=std::size_t(c-'0');
            if(digit>limit||value>(limit-digit)/10) throw std::invalid_argument("field limit");
            value=value*10+digit;
        }
        return value;
    };
    Request candidate;candidate.vertices=field(128);
    if(!candidate.vertices) throw std::invalid_argument("no vertices");
    auto count=field(4096);candidate.source=field(candidate.vertices-1);candidate.target=field(candidate.vertices-1);
    for(std::size_t i=0;i<count;++i){
        auto from=field(candidate.vertices-1);auto to=field(candidate.vertices-1);auto weight=field(1000000);
        candidate.edges.emplace_back(from,to,weight);
    }
    std::string extra;if(input>>extra) throw std::invalid_argument("trailing field");
    static_assert(std::is_nothrow_move_assignable_v<Request>);
    accepted=std::move(candidate);
}
