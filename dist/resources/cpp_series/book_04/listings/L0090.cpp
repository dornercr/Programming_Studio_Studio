#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main(){
 std::mutex m; std::condition_variable cv; bool ready=false;
 std::jthread worker([&]{ std::unique_lock lock(m); cv.wait(lock,[&]{return ready;}); std::cout<<"ready\n"; });
 { std::lock_guard lock(m); ready=true; }
 cv.notify_one();
}
