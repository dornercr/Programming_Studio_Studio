#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

long long baseline(const std::vector<int>& v){return std::accumulate(v.begin(),v.end(),0LL);}
long long candidate(const std::vector<int>& v){long long a=0,b=0;for(std::size_t i=0;i<v.size();++i)(i&1?b:a)+=v[i];return a+b;}
int main(){std::vector<int> v(100000,3);assert(candidate(v)==baseline(v));}
