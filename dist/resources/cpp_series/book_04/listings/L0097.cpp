#include <future>
#include <iostream>
#include <thread>

std::thread::id work(){ return std::this_thread::get_id(); }
int main(){
 auto caller=std::this_thread::get_id();
 auto deferred=std::async(std::launch::deferred,work);
 auto async=std::async(std::launch::async,work);
 std::cout<<(deferred.get()==caller)<<' '<<(async.get()!=caller)<<"\n";
}
