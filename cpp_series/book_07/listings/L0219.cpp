#include <map>
#include <string>
#include <iostream>

int main() {
    std::map<std::string,int> ledger;
    auto apply=[&](std::string id,int amount){auto[it,inserted]=ledger.emplace(std::move(id),amount);return inserted;};
    apply("charge7",-30);bool refund=apply("refund-charge7",30),retry=apply("refund-charge7",30);
    int net=0;for(auto&[id,amount]:ledger)net+=amount;
    std::cout<<std::boolalpha<<"refund="<<refund<<" repeated="<<retry<<" records="<<ledger.size()<<" net="<<net<<'\n';
}
