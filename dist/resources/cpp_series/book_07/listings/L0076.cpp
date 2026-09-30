#include <variant>
#include <string>
#include <iostream>

int main() {
    struct TransportFailure{std::string reason;};struct Reply{bool accepted;std::string detail;};
    std::variant<TransportFailure,Reply> outcome=Reply{false,"quota"};
    if(auto*r=std::get_if<Reply>(&outcome))std::cout<<"transport=received application="<<(r->accepted?"accepted":"rejected")<<" reason="<<r->detail<<'\n';
    else return 1;
}
