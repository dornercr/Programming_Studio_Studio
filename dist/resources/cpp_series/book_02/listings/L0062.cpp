#include <array>
#include <iostream>
#include <vector>
template<class It> int sum(It first,It last){int s=0;for(;first!=last;++first)s+=*first;return s;}
int main(){std::array<int,3>a{1,2,3};std::vector<int>v{4,5};std::cout<<sum(a.begin(),a.end())<<' '<<sum(v.begin(),v.end())<<'\n';}
