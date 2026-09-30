#include <algorithm>
#include <iostream>
#include <vector>
int main(){std::vector<int>h{4,1,7,3};std::make_heap(h.begin(),h.end());h.push_back(9);std::push_heap(h.begin(),h.end());std::cout<<h.front()<<' ';std::pop_heap(h.begin(),h.end());std::cout<<h.back()<<'\n';}
