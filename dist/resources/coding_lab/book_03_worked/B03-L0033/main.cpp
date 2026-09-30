#include <iostream>
#include <set>
int main(){std::set<int>s;for(int x:{4,2,4,1,2})s.insert(x);for(int x:s)std::cout<<x<<' ';std::cout<<"size="<<s.size()<<'\n';}
