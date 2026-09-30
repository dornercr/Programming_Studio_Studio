#include "harbor/check.hpp"
#include <iostream>
#include <string>
#include <set>
struct Principal { std::string tenant; std::set<std::string> permissions; };
bool allowed(const Principal& verified, const std::string& resource_tenant,
             const std::string& action) {
    return verified.tenant==resource_tenant && verified.permissions.contains(action);
}
int main() {
    // Principal must come from a verified transport/authentication boundary.
    Principal p{"tenant-a",{"status:read"}};
    harbor::check(allowed(p,"tenant-a","status:read"),"own tenant read");
    harbor::check(!allowed(p,"tenant-b","status:read"),"cross tenant denial");
    harbor::check(!allowed(p,"tenant-a","job:submit"),"action denial");
    std::cout<<"Identity and authorization are different checks.\n";
}
