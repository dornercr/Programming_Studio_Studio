#include <cassert>
#include <iostream>

enum class State { Ready, Running, Waiting };
bool transition(State& state, State from, State to) {
    if(state != from) return false;
    state = to;
    return true;
}

int main() {
    State process = State::Ready;
    assert(transition(process,State::Ready,State::Running));
    assert(transition(process,State::Running,State::Waiting));
    assert(!transition(process,State::Ready,State::Running) && process == State::Waiting);
    assert(transition(process,State::Waiting,State::Ready));
    std::cout << "ready -> running -> waiting -> ready\n";
}
