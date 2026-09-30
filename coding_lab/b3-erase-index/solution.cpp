#include <iostream>
#include <vector>
#include <cstddef>

bool eraseIndex(std::vector<int>& v,std::size_t i){
    if(i>=v.size()) return false;
    for(std::size_t j=i+1;j<v.size();++j) v[j-1]=v[j];
    v.pop_back();
    return true;
}
