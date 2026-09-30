#include <iostream>
#include <string>
#include <string_view>

enum class Health { starting, ready, draining, stopped };
Health transition(Health current, std::string_view event) {
    if(current==Health::starting&&event=="ready") return Health::ready;
    if(current==Health::ready&&event=="stop") return Health::draining;
    if(current==Health::draining&&event=="drained") return Health::stopped;
    return current;
}
