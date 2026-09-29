#include <future>
#include <iostream>
#include <thread>

int main(){
 std::promise<int> promise; std::future<int> result=promise.get_future();
 std::jthread worker([p=std::move(promise)]() mutable { p.set_value(6*7); });
 std::cout<<result.get()<<"\n";
}
