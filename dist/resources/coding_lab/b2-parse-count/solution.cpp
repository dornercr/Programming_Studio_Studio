#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <charconv>
#include <system_error>

std::optional<int> parseCount(std::string_view text){if(text.empty())return std::nullopt;int value=0;auto [end,error]=std::from_chars(text.data(),text.data()+text.size(),value);if(error!=std::errc{}||end!=text.data()+text.size()||value<0||value>1000)return std::nullopt;return value;}
