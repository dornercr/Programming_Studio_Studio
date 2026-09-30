#include <iostream>

int main() {
    struct State{bool initialized=false;bool event_loop_progress=true;bool draining=false;};State s;
    auto startup=[&]{return s.initialized;};auto live=[&]{return s.event_loop_progress;};auto ready=[&]{return s.initialized&&!s.draining;};
    std::cout<<std::boolalpha<<startup()<<' '<<live()<<' '<<ready()<<'\n';s.initialized=true;s.draining=true;
    std::cout<<startup()<<' '<<live()<<' '<<ready()<<'\n';
}
