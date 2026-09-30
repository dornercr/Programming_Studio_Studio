#include "harbor/check.hpp"
#include "harbor/models.hpp"
#include <iostream>
int main() {
    std::vector<std::string> before{"a","b","c"},after=before; after.push_back("d");
    unsigned moved=0;
    for (int i=0;i<200;++i) {
        auto key="job-"+std::to_string(i);
        auto old=harbor::placement(key,before),next=harbor::placement(key,after);
        if (old!=next) { ++moved; harbor::check(next=="d","unrelated reassignment"); }
    }
    std::cout<<moved<<" of 200 teaching keys moved to the added member.\n";
}
