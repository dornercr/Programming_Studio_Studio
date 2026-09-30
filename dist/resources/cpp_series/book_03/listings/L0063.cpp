#include <iostream>
#include <vector>
bool binary(const std::vector<int>&v,int x){std::size_t lo=0,hi=v.size();while(lo<hi){auto mid=lo+(hi-lo)/2;if(v[mid]<x)lo=mid+1;else hi=mid;}return lo<v.size()&&v[lo]==x;}
int main(){std::cout<<std::boolalpha<<binary({1,3,5,7,9},7)<<' '<<binary({1,3,5,7,9},6)<<'\n';}
