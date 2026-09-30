#include <map>
#include <iostream>

int main() {
    bool accepting=true;std::map<int,bool> active{{1,false},{2,false}};
    active[1]=true; // request1 cancelled
    accepting=false;
    bool late=accepting;unsigned completed=0,cancelled=0;
    for(auto[id,stop]:active){if(stop)++cancelled;else++completed;}active.clear();
    std::cout<<"late-accepted="<<std::boolalpha<<late<<" completed="<<completed<<" cancelled="<<cancelled<<" remaining="<<active.size()<<'\n';
}
