#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

int main(){
 std::vector<int> v(1000,1); long long left=0,right=0;
 std::jthread a([&]{left=std::accumulate(v.begin(),v.begin()+500,0LL);});
 std::jthread b([&]{right=std::accumulate(v.begin()+500,v.end(),0LL);});
 a.join(); b.join(); std::cout<<left+right<<"\n";
}
