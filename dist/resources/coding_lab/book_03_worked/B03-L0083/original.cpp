#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>
int main(){std::vector<int>v(100000);std::iota(v.begin(),v.end(),0);long long check=0;std::vector<long long>times;for(int trial=0;trial<20;++trial){auto s=std::chrono::steady_clock::now();long long total=std::accumulate(v.begin(),v.end(),0LL);auto e=std::chrono::steady_clock::now();check=total;times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(e-s).count());}std::sort(times.begin(),times.end());std::cout<<"sum="<<check<<" median_ns="<<times[times.size()/2]<<'\n';}
