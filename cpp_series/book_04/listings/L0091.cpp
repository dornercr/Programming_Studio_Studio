#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

class Queue{
 std::mutex m; std::condition_variable not_empty,not_full; std::deque<int> q; const std::size_t cap=2;
public:
 void push(int v){std::unique_lock l(m);not_full.wait(l,[&]{return q.size()<cap;});q.push_back(v);not_empty.notify_one();}
 int pop(){std::unique_lock l(m);not_empty.wait(l,[&]{return !q.empty();});int v=q.front();q.pop_front();not_full.notify_one();return v;}
};
int main(){Queue q; std::jthread p([&]{q.push(1);q.push(2);q.push(3);}); std::cout<<q.pop()+q.pop()+q.pop()<<"\n";}
