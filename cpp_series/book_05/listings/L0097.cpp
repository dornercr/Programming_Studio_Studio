#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main(){
 std::mutex m; long long total=0; std::vector<std::jthread> threads;
 for(int t=0;t<4;++t) threads.emplace_back([&]{long long local=0;for(int i=0;i<100000;++i)++local;std::lock_guard lock(m);total+=local;});
 threads.clear(); std::cout<<total<<"\n";
}
