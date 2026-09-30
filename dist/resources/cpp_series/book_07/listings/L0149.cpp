#include <map>
#include <string>
#include <iostream>

int main() {
    using Row=std::map<std::string,std::string>;
    auto old_reader=[](const Row&r){return std::stoi(r.at("amount_cents"));};
    auto new_reader=[&](const Row&r){int cents=old_reader(r);auto it=r.find("memo");return std::pair{cents,it==r.end()?std::string{}:it->second};};
    Row old{{"amount_cents","1250"}},newer{{"amount_cents","1250"},{"memo","invoice"}};
    if(old_reader(newer)!=1250||new_reader(old).first!=1250)return 1;
    std::cout<<"old-on-new=1250 new-on-old-memo-empty="<<std::boolalpha<<new_reader(old).second.empty()<<'\n';
}
