#include <map>
#include <string>
#include <iostream>

int main() {
    std::map<int,int> old{{1,100},{2,250}};
    std::map<int,std::string> newer;
    for(auto[id,cents]:old)newer[id]=std::to_string(cents);
    bool equivalent=true;for(auto[id,cents]:old)equivalent &= std::stoi(newer.at(id))==cents;
    if(!equivalent)return 1;
    bool use_new=true;
    auto read=[&](int id){return use_new?std::stoi(newer.at(id)):old.at(id);};
    std::cout<<"new-read="<<read(2);use_new=false;std::cout<<" rollback-read="<<read(2)<<'\n';
}
