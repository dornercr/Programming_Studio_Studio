#include <iostream>
#include <string>
#include <string_view>

std::string trimSpaces(std::string_view text) {
    auto first=text.find_first_not_of(' ');
    if(first==std::string_view::npos) return {};
    auto last=text.find_last_not_of(' ');
    return std::string(text.substr(first,last-first+1));
}
