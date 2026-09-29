#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

int main(){
 std::vector<int> data(1'000'000,1); constexpr int workers=4; long long partial[workers]{}; std::vector<std::jthread> ts;
 for(int w=0;w<workers;++w) ts.emplace_back([&,w]{auto first=data.begin()+w*(data.size()/workers);auto last=(w==workers-1)?data.end():first+(data.size()/workers);partial[w]=std::accumulate(first,last,0LL);});
 ts.clear(); std::cout<<std::accumulate(std::begin(partial),std::end(partial),0LL)<<"\n";
}
