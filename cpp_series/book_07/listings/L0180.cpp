#include "harbor/check.hpp"
#include <iostream>
#include <map>
#include <vector>
int main() {
    std::map<std::string,int> inbox; int total=0;
    for (const auto& event:std::vector<std::pair<std::string,int>>{{"e1",5},{"e1",5},{"e2",3}}) {
        auto [it,inserted]=inbox.emplace(event);
        harbor::check(it->second==event.second,"identity conflict");
        if (inserted) total+=event.second;
    }
    harbor::check(total==8,"duplicate delivery effect");
    std::cout<<"Three deliveries caused two effects; this in-memory inbox is not crash durable.\n";
}
