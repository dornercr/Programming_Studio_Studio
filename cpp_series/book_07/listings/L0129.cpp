#include <array>
#include <cstdint>
#include <string_view>
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    auto score=[](std::string_view key,char node){std::uint64_t h=14695981039346656037ull;
        for(unsigned char c:key){h^=c;h*=1099511628211ull;}h^=unsigned(node);h*=1099511628211ull;return h;};
    auto owner=[&](std::string_view key,const std::vector<char>&nodes){return *std::max_element(nodes.begin(),nodes.end(),[&](char a,char b){return score(key,a)<score(key,b);});};
    std::vector<char> nodes{'a','b','c'};char first=owner("tenant42",nodes);
    char removed=first=='a'?'b':'a';nodes.erase(std::find(nodes.begin(),nodes.end(),removed));
    char second=owner("tenant42",nodes);if(first!=second)return 1;
    std::cout<<"non-owner removal preserves placement\n";
}
