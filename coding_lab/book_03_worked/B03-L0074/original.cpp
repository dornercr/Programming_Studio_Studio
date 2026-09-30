#include <algorithm>
#include <iostream>
#include <vector>
struct Job{int start,end;};
int main(){std::vector<Job>j{{0,6},{1,4},{3,5},{5,7},{5,9},{8,9}};std::sort(j.begin(),j.end(),[](auto a,auto b){return a.end<b.end;});int last=-1,count=0;for(auto x:j)if(x.start>=last){++count;last=x.end;}std::cout<<count<<'\n';}
