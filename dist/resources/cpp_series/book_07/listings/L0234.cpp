#include <array>
#include <string_view>
#include <map>
#include <iostream>

int main() {
    struct Attempt{std::string_view operation;bool success;};
    const std::array attempts{Attempt{"a",false},Attempt{"a",true},Attempt{"b",true}};
    std::map<std::string_view,bool> completed;unsigned failures=0;
    for(auto a:attempts){if(!a.success)++failures;completed[a.operation]|=a.success;}
    unsigned successes=0;for(auto&[id,ok]:completed)successes+=ok;
    std::cout<<"attempts="<<attempts.size()<<" attempt-errors="<<failures<<" successful-operations="<<successes<<'\n';
}
