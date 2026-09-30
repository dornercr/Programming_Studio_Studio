#include <atomic>
#include <thread>
#include <iostream>

int main() {
    std::atomic<bool> entered=false;int owned_result=0;
    std::jthread child([&](std::stop_token token){entered.store(true,std::memory_order_release);
        while(!token.stop_requested())std::this_thread::yield();owned_result=7;});
    while(!entered.load(std::memory_order_acquire))std::this_thread::yield();
    child.request_stop();child.join();
    std::cout<<"joined cleanup-result="<<owned_result<<'\n';
}
