#include <map>
#include <string>
#include <iostream>

int main() {
    std::map<std::string,int> seen;int applied=0;
    auto event=[&](const std::string&id,int value){auto it=seen.find(id);if(it!=seen.end())return it->second==value?"duplicate":"conflict";
        seen.emplace(id,value);applied+=value;return "applied";};
    std::cout<<event("reserve7",3)<<' '<<event("reserve7",3)<<' '<<event("reserve7",4)<<" total="<<applied<<'\n';
}
