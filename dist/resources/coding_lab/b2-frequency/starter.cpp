#include <iostream>
#include <map>
#include <string>
#include <vector>

std::map<std::string,int> frequencies(const std::vector<std::string>& words){std::map<std::string,int> counts;for(const auto& w:words)counts[w]=1;return counts;}
