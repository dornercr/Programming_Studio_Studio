#include <string>
#include <string_view>
#include <iostream>

int main() {
    auto handle=[](std::string_view body,std::string_view token){
        if(body.size()>16)return std::string("event=request_rejected reason=size token=[redacted]");
        return std::string(token.empty()?"event=unauthenticated":"event=request_checked");};
    std::cout<<handle(std::string(17,'x'),"secret-value")<<'\n';
    std::cout<<handle("job7","")<<'\n';
}
