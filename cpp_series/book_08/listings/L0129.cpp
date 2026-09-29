#include <cmath>
#include <algorithm>
#include <iostream>

int main() {
    int current=3,min_replicas=2,max_replicas=8;double utilization=90,target=60;
    int raw=int(std::ceil(current*utilization/target));int desired=std::clamp(raw,min_replicas,max_replicas);
    std::cout<<"raw="<<raw<<" bounded-desired="<<desired<<'\n';
}
