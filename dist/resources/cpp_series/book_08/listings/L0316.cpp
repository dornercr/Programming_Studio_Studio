#include <string>
#include <string_view>
#include <iostream>

int main() {
    struct Request{std::string id,authorization,payload;};
    Request r{"req-17","Bearer SECRET","private customer data"};
    auto incident_event=[](const Request&x){return "request="+x.id+" category=dependency_timeout";};
    const auto event=incident_event(r);
    if(event.find("SECRET")!=std::string::npos||event.find("private")!=std::string::npos)return 1;
    std::cout<<event<<'\n';
}
