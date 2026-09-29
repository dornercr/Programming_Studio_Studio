#include "harbor/check.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
int main() {
    std::vector<int> schedule{1,1,2,3}; unsigned checked=0;
    do {
        std::set<int> receipts;
        unsigned effects=0;
        for (int id:schedule) if (receipts.insert(id).second) ++effects;
        harbor::check(effects==3,"schedule duplicated effect"); ++checked;
    } while (std::next_permutation(schedule.begin(),schedule.end()));
    harbor::check(checked==12,"distinct schedules");
    std::cout<<"Checked all 12 distinct orderings of a bounded duplicate-delivery model.\n";
}
