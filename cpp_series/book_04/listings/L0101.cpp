#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(){
 std::atomic<int> counter{0}; std::vector<std::jthread> threads;
 for(int t=0;t<4;++t) threads.emplace_back([&]{for(int i=0;i<10000;++i) counter.fetch_add(1,std::memory_order_relaxed);});
 threads.clear(); std::cout<<counter.load()<<"\n";
}
