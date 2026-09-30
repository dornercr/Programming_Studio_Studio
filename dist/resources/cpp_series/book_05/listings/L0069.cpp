#include <algorithm>
#include <iostream>
#include <vector>

int main(){
 std::vector<long long> ns{102,99,101,500,100,98,103,97,105,100};
 std::sort(ns.begin(),ns.end());
 auto median=(ns[4]+ns[5])/2;
 auto p90=ns[8];
 std::cout<<"median="<<median<<" p90="<<p90<<" max="<<ns.back()<<"\n";
}
