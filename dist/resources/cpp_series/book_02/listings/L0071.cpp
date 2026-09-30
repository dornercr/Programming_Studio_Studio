#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
int main(){ std::vector<int> v{1,2,3}; std::vector<int> squares(v.size()); std::transform(v.begin(),v.end(),squares.begin(),[](int x){return x*x;}); std::cout<<std::accumulate(squares.begin(),squares.end(),0)<<'\n'; }
