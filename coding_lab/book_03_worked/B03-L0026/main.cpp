#include <deque>
#include <iostream>
int main(){std::deque<int>w;for(int x:{1,2,3,4,5}){w.push_back(x);if(w.size()>3)w.pop_front();std::cout<<'[';for(int v:w)std::cout<<v;std::cout<<"] ";}}
