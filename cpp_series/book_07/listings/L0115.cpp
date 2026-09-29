#include "harbor/check.hpp"
#include <iostream>
#include <vector>
struct Replica { std::vector<int> log; int read() const { return log.empty()?0:log.back(); } };
int main() {
    Replica leader,follower;
    leader.log.push_back(7); // local commit model, not a consensus implementation
    harbor::check(leader.read()==7 && follower.read()==0,"stale read witness");
    follower.log=leader.log;
    harbor::check(follower.read()==7,"eventual catch-up in this schedule");
    std::cout<<"A copied log converges after delivery; it does not prove linearizability.\n";
}
