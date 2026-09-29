#include <iostream>
#include <optional>
#include <vector>
std::optional<std::size_t>find(const std::vector<int>&v,int target){for(std::size_t i=0;i<v.size();++i)if(v[i]==target)return i;return std::nullopt;}
int main(){auto i=find({4,8,2,8},2);std::cout<<(i?std::to_string(*i):"none")<<'\n';}
