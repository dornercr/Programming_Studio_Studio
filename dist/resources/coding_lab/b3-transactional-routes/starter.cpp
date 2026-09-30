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
void loadRequest(std::string_view text,Request& accepted){(void)text;accepted=Request{};}
