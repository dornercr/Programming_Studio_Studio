#include <iostream>
class Thermostat{
    double target_{21.0};
public:
    explicit Thermostat(double t):target_(t){}
    bool should_heat(double current) const { return current < target_; }
};
int main(){ Thermostat t{22}; std::cout << std::boolalpha << t.should_heat(20.5) << '\n'; }
