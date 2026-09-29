#include <deque>
#include <string>
#include <iostream>

int main() {
    std::deque<std::string> pending;unsigned dropped=0;
    auto emit=[&](std::string event){if(pending.size()==2){++dropped;return false;}pending.push_back(std::move(event));return true;};
    emit("span1");emit("span2");bool third=emit("span3");
    std::cout<<"queued="<<pending.size()<<" dropped="<<dropped<<" third="<<std::boolalpha<<third<<'\n';
}
