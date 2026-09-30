#include <string>
#include <set>
#include <iostream>

int main() {
    struct Principal{std::string subject,tenant;std::set<std::string> scopes;};
    Principal verified{"worker7","tenantA",{"job.read"}};
    auto allow=[&](const std::string&tenant,const std::string&operation){return verified.tenant==tenant&&verified.scopes.contains(operation);};
    std::cout<<std::boolalpha<<"read-own="<<allow("tenantA","job.read")<<" write="<<allow("tenantA","job.write")<<" other-tenant="<<allow("tenantB","job.read")<<'\n';
}
