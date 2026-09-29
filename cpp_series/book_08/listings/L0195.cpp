#include <string_view>
#include <algorithm>
#include <iostream>

int main() {
    auto hex=[](std::string_view s){return std::all_of(s.begin(),s.end(),[](char c){return(c>='0'&&c<='9')||(c>='a'&&c<='f');});};
    auto valid=[&](std::string_view s){if(s.size()!=55||s.substr(0,3)!="00-"||s[35]!='-'||s[52]!='-')return false;
        auto trace=s.substr(3,32),parent=s.substr(36,16);return hex(trace)&&hex(parent)&&hex(s.substr(53,2))&&trace.find_first_not_of('0')!=trace.npos&&parent.find_first_not_of('0')!=parent.npos;};
    std::string_view good="00-4bf92f3577b34da6a3ce929d0e0e4736-00f067aa0ba902b7-01";
    std::cout<<std::boolalpha<<"valid="<<valid(good)<<" truncated="<<valid(good.substr(0,54))<<'\n';
}
