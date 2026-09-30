#include <chrono>
#include <iostream>
#include <thread>

int main(){
 int iterations=0;
 {
   std::jthread worker([&](std::stop_token stop){
     while(!stop.stop_requested()){ ++iterations; std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
   });
   std::this_thread::sleep_for(std::chrono::milliseconds(5));
 }
 std::cout << (iterations>0) << "\n";
}
