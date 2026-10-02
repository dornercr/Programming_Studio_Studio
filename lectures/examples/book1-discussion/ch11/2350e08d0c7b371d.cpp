#include <charconv>
#include <iostream>
#include <string>
int main(){
    std::string text = "42";
    int value{};
    auto [ptr, ec] = std::from_chars(text.data(), text.data()+text.size(), value);
    if (ec == std::errc{} && ptr == text.data()+text.size())
        std::cout << std::to_string(value + 8) << '\n';
}
