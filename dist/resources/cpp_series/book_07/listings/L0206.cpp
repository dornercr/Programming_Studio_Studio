#include <deque>
#include <memory>
#include <iostream>

int main() {
    std::deque<std::unique_ptr<int>> q;bool closed=false;
    auto push=[&](std::unique_ptr<int>&item){if(closed||q.size()==1)return false;q.push_back(std::move(item));return true;};
    auto a=std::make_unique<int>(7),b=std::make_unique<int>(9);bool accepted=push(a),full=push(b);closed=true;
    int drained=*q.front();q.pop_front();
    std::cout<<std::boolalpha<<"accepted="<<accepted<<" full="<<full<<" caller-retains="<<bool(b)<<" drained="<<drained<<'\n';
}
