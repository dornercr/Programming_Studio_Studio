#include <deque>
#include <iostream>

int main() {
    enum class State{starting,ready,draining,stopped};State state=State::starting;std::deque<int> jobs;
    auto admit=[&](int id){if(state!=State::ready)return false;jobs.push_back(id);return true;};
    bool early=admit(1);bool storage_initialized=true;
    if(storage_initialized)state=State::ready;
    bool accepted=admit(2);state=State::draining;bool late=admit(3);
    unsigned completed=0;while(!jobs.empty()){jobs.pop_front();++completed;}state=State::stopped;
    std::cout<<std::boolalpha<<early<<' '<<accepted<<' '<<late<<" drained="<<completed<<'\n';
}
