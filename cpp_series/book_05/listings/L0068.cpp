#include <chrono>
#include <cstdint>
#include <iostream>

int main(){
 constexpr std::uint64_t N=50'000'000; volatile std::uint64_t sink=0;
 auto start=std::chrono::steady_clock::now();
 std::uint64_t sum=0; for(std::uint64_t i=0;i<N;++i) sum += i;
 sink=sum;
 auto ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count();
 std::cout<<"result="<<sink<<" ns="<<ns<<"\n";
}
