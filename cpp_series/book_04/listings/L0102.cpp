#include <atomic>
#include <iostream>
#include <thread>

int main(){
 int payload=0; std::atomic<bool> ready{false};
 std::jthread producer([&]{payload=99; ready.store(true,std::memory_order_release);});
 std::jthread consumer([&]{while(!ready.load(std::memory_order_acquire)){} std::cout<<payload<<"\n";});
}
