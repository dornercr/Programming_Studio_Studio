#include <iostream>
#include <string>
int main(){
    std::string record = "user:charles";
    auto pos = record.find(':');
    if (pos != std::string::npos) std::cout << record.substr(pos + 1) << '\n';
}
