#include <map>
#include <string>
#include <vector>
#include <iostream>

int main() {
    std::map<unsigned,std::string> completed;unsigned next=1;std::vector<std::string> applied;
    auto finish=[&](unsigned seq,std::string value){completed.emplace(seq,std::move(value));
        while(completed.contains(next)){applied.push_back(completed.at(next));completed.erase(next++);}};
    finish(2,"second");finish(1,"first");finish(3,"third");
    std::cout<<"application order:";for(auto&s:applied)std::cout<<' '<<s;std::cout<<'\n';
}
