#include <charconv>
#include <iostream>
#include <optional>
#include <string_view>
std::optional<int> parse(std::string_view s){int v{};auto [p,e]=std::from_chars(s.data(),s.data()+s.size(),v);if(e!=std::errc{}||p!=s.data()+s.size())return std::nullopt;return v;}
int main(){ auto v=parse("42"); std::cout<<(v?*v:-1)<<'\n'; }
