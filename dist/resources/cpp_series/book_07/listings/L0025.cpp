#include <variant>
#include <string>
#include <iostream>

int main() {
    struct Rejected{std::string reason;};struct Failed{std::string reason;};
    struct Unknown{std::string operation;};struct Completed{int result;};
    using Outcome=std::variant<Rejected,Failed,Unknown,Completed>;
    Outcome result=Unknown{"op42"};
    if(auto* u=std::get_if<Unknown>(&result))std::cout<<"reconcile="<<u->operation<<" do-not-create-new-id\n";
    result=Completed{144};std::cout<<"confirmed="<<std::get<Completed>(result).result<<'\n';
}
