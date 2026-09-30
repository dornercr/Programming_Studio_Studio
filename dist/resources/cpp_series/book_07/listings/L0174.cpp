#include <set>
#include <string>
#include <iostream>

int main() {
    std::set<std::string> receipts;int balance=0;unsigned deliveries=0;
    auto deliver=[&](const std::string&id){++deliveries;if(receipts.contains(id))return;balance+=5;receipts.insert(id);};
    deliver("m1");bool ack_lost=true;if(ack_lost)deliver("m1");
    std::cout<<"deliveries="<<deliveries<<" effect="<<balance<<" receipts="<<receipts.size()<<'\n';
}
