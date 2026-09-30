#include <cstdint>
#include <iostream>
#include <vector>
int main(){std::vector<int>v{10,20,30};for(std::size_t i=0;i<v.size();++i)std::cout<<(&v[i]-&v[0])<<' ';}
