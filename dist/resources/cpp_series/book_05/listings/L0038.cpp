#include <charconv>
#include <optional>
#include <string_view>
#include <cassert>

std::optional<int> parse_port(std::string_view s){
 int v{}; auto [p,ec]=std::from_chars(s.data(),s.data()+s.size(),v);
 if(ec!=std::errc{} || p!=s.data()+s.size() || v<1 || v>65535) return std::nullopt;
 return v;
}
int main(){assert(parse_port("1"));assert(parse_port("65535"));assert(!parse_port("0"));assert(!parse_port("65536"));assert(!parse_port("abc"));}
