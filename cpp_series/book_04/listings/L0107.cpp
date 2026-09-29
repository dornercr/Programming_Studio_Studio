#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main(){
 using clock=std::chrono::steady_clock; std::mutex m; std::condition_variable cv; std::deque<int> jobs; bool done=false; std::atomic<int> processed{0};
 std::vector<std::jthread> workers; for(int w=0;w<2;++w) workers.emplace_back([&]{for(;;){int job;{std::unique_lock l(m);cv.wait(l,[&]{return done||!jobs.empty();});if(jobs.empty()&&done)break;job=jobs.front();jobs.pop_front();}(void)job;++processed;}});
 auto start=clock::now(); {std::lock_guard l(m);for(int i=0;i<10000;++i)jobs.push_back(i);done=true;} cv.notify_all(); workers.clear();
 auto us=std::chrono::duration_cast<std::chrono::microseconds>(clock::now()-start).count(); std::cout<<processed<<" jobs in "<<us<<" us\n";
}
