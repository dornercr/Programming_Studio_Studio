#include <iostream>
enum class Mode { idle, active, done };
enum class Event { begin, finish, reset };
bool send(Mode& m, Event e) {
    if (m == Mode::idle && e == Event::begin) {
        m = Mode::active; return true;
    }
    if (m == Mode::active && e == Event::finish) {
        m = Mode::done; return true;
    }
    if (m == Mode::done && e == Event::reset) {
        m = Mode::idle; return true;
    }
    return false; // Rejection leaves m unchanged.
}
int main() {
    Mode mode = Mode::idle;
    std::cout << std::boolalpha;
    std::cout << send(mode, Event::finish) << '\n';
    std::cout << send(mode, Event::begin) << '\n';
    std::cout << send(mode, Event::finish) << '\n';
    std::cout << send(mode, Event::reset) << '\n';
}
