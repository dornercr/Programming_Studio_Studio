#include <chrono>
#include <iostream>
#include <vector>
long sum_indexed(const std::vector<int>&v){long s=0;for(std::size_t i=0;i<v.size();++i)s+=v[i];return s;}
long sum_ranged(const std::vector<int>&v){long s=0;for(int x:v)s+=x;return s;}
int main(){std::vector<int>v(100000,1);auto a=sum_indexed(v),b=sum_ranged(v);std::cout<<a<<' '<<b<<'\n';}
