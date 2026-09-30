#include <iostream>
#include <optional>
#include <string_view>

std::optional<int> parse_request(std::string_view line){
 if(!line.starts_with("ADD ")) return std::nullopt;
 int value=0; for(char c:line.substr(4)){ if(c<'0'||c>'9') return std::nullopt; value=value*10+(c-'0'); }
 return value;
}
int main(){ auto r=parse_request("ADD 42"); std::cout<<(r?*r:-1)<<"\n"; }
