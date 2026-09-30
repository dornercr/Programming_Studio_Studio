#include <iostream>
#include <map>
#include <string>
#include <vector>
int main(){std::map<std::string,int>count;for(const auto&w:std::vector<std::string>{"red","blue","red"})++count[w];for(const auto&[k,v]:count)std::cout<<k<<'='<<v<<' ';}
