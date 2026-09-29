#include "harbor/check.hpp"
#include "harbor/models.hpp"
#include <iostream>
int main() {
    harbor::Discovery registry;
    harbor::check(registry.update(2,{"worker-b","worker-a","worker-a"}),"new revision");
    harbor::check(!registry.update(1,{"retired"}),"stale watch event");
    harbor::check(registry.endpoints.size()==2,"deduplicated membership");
    auto chosen=harbor::placement("job-8",registry.endpoints);
    harbor::check(chosen=="worker-a" || chosen=="worker-b","member routing");
    std::cout<<"Membership revision 2 survived a late revision 1 update.\n";
}
