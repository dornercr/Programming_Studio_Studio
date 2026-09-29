#include <iostream>
#include <mutex>
#include <thread>

int main(){
 std::mutex a,b; int x=0;
 auto work=[&]{ for(int i=0;i<1000;++i){ std::scoped_lock lock(a,b); ++x; }};
 std::jthread t1(work), t2(work); t1.join(); t2.join();
 std::cout<<x<<"\n";
}
