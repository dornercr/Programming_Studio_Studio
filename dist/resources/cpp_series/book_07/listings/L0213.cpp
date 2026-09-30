#include <iostream>

int main() {
    enum class State{created,reserved,charged,completed,compensating};State s=State::created;
    auto reserve=[&](){if(s!=State::created)return false;s=State::reserved;return true;};
    auto charge=[&](bool ok){if(s!=State::reserved)return false;s=ok?State::charged:State::compensating;return true;};
    bool out_of_order=charge(true);reserve();charge(false);
    std::cout<<"early-charge="<<std::boolalpha<<out_of_order<<" compensation-needed="<<(s==State::compensating)<<'\n';
}
