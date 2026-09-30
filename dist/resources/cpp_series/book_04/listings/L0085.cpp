#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main(){
 std::mutex m; int counter=0; std::vector<std::jthread> threads;
 for(int t=0;t<4;++t) threads.emplace_back([&]{ for(int i=0;i<10000;++i){ std::scoped_lock lock(m); ++counter; }});
 threads.clear();
 std::cout<<counter<<"\n";
}
