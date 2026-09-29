#include <charconv>
#include <string_view>
#include <optional>
#include <iostream>
#include <stdexcept>

int main() {
    auto duration=[](std::string_view s){if(!s.ends_with("ms"))throw std::invalid_argument("unit");s.remove_suffix(2);int n=0;
        auto[p,e]=std::from_chars(s.data(),s.data()+s.size(),n);if(e!=std::errc{}||p!=s.data()+s.size()||n<1||n>5000)throw std::invalid_argument("range");return n;};
    std::string_view file="500ms";std::optional<std::string_view> override="250ms";
    std::cout<<"timeout_ms="<<duration(override.value_or(file))<<" source=override\n";
    try{(void)duration("250");return 1;}catch(const std::invalid_argument&){std::cout<<"missing unit rejected\n";}
}
