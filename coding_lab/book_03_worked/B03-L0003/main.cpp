#include <iostream>
#include <vector>
int maximum(const std::vector<int>& v){ int best=v.at(0); for(std::size_t i=1;i<v.size();++i) if(v[i]>best) best=v[i]; return best; }
int main(){ std::cout<<maximum({4,9,2,7})<<'\n'; }
