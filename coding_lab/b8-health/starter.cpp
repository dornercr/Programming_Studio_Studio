#include <iostream>
#include <string>
#include <string_view>

enum class Health { starting, ready, draining, stopped };
Health transition(Health current, std::string_view event) {
    // TODO: allow only the three stated transitions.
    return current;
}
