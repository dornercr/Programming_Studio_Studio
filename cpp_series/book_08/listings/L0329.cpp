#include <deque>
#include <iostream>

int main() {
    std::deque<int> queue;unsigned offered=0,accepted=0,rejected=0,completed=0;
    for(int id=0;id<10;++id){++offered;if(queue.size()==3)++rejected;else{queue.push_back(id);++accepted;}}
    while(!queue.empty()){queue.pop_front();++completed;}
    if(offered!=accepted+rejected||accepted!=completed)return 1;
    std::cout<<"offered="<<offered<<" accepted="<<accepted<<" rejected="<<rejected<<" completed="<<completed<<'\n';
}
