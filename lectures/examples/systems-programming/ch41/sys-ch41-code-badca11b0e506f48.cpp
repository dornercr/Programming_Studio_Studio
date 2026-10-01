#include <cassert>
#include <iostream>



int main() {
    bool pending = false; unsigned events = 0;
    for(unsigned i=0; i<2; ++i) { pending = true; ++events; }
    unsigned flag_actions = 0;
    if(pending) { pending = false; ++flag_actions; }
    --events;
    assert(flag_actions == 1 && !pending && events == 1);
    std::cout << "flag-actions=" << flag_actions << " counted-events-left=" << events << '\n';
}
