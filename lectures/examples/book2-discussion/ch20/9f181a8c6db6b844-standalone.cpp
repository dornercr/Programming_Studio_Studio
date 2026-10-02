#include <chrono>
#include <iostream>
#include <random>
int main(){ std::mt19937 rng{1234}; std::uniform_int_distribution<int>d(1,6); auto start=std::chrono::steady_clock::now(); int total=0; for(int i=0;i<1000;++i)total+=d(rng); auto end=std::chrono::steady_clock::now(); std::cout<<total<<' '<<(end >= start)<<'\n'; }