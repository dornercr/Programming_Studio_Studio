#include <algorithm>
#include <iostream>
#include <vector>
void sort(std::vector<int>&a,std::size_t lo,std::size_t hi){if(hi-lo<=1)return;auto mid=lo+(hi-lo)/2;sort(a,lo,mid);sort(a,mid,hi);std::vector<int>tmp;tmp.reserve(hi-lo);std::merge(a.begin()+lo,a.begin()+mid,a.begin()+mid,a.begin()+hi,std::back_inserter(tmp));std::copy(tmp.begin(),tmp.end(),a.begin()+lo);}int main(){std::vector<int>a{8,3,7,1,5};sort(a,0,a.size());for(int x:a)std::cout<<x<<' ';}
