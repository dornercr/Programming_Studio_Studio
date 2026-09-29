#include <iostream>
#include <string>
#include <string_view>
#include <vector>

enum class State { offline, ready, failed };

struct Device {
    int id{};
    std::string name;
    State state{State::offline};
};

std::string_view label(State state) {
    switch (state) {
    case State::offline: return "offline";
    case State::ready: return "ready";
    case State::failed: return "failed";
    }
    return "invalid";
}

int main() {
    const std::vector<Device> devices{
        {1, "scope", State::ready}, {2, "meter", State::failed}};
    int ready{};
    for (const Device& device : devices) {
        if (device.state == State::ready) ++ready;
        std::cout << device.id << ':' << device.name
                  << ':' << label(device.state) << '\n';
    }
    if (ready != 1 || label(State::offline) != "offline") return 1;
    std::cout << "PASS\n";
}
