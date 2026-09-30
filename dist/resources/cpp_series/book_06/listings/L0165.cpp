#include <deque>
#include <array>
#include <iostream>
#include <algorithm>

int main() {
    std::deque<int> queue;unsigned rejected=0,high_water=0,completed=0;
    for(int id=0;id<5;++id){
        if(queue.size()==2)++rejected;else queue.push_back(id);
        high_water=std::max(high_water,unsigned(queue.size()));
        if(id%2==1&&!queue.empty()){queue.pop_front();++completed;}
    }
    const std::array<int,3> stage_us{100,400,150};
    std::cout<<"completed="<<completed<<" queued="<<queue.size()<<" rejected="<<rejected
             <<" high-water="<<high_water<<" bottleneck_us="<<*std::max_element(stage_us.begin(),stage_us.end())<<'\n';
}
