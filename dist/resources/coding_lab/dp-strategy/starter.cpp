#include <iostream>
#include <functional>

int checkout(int base, const std::function<int(int)>& policy) {
    // TODO: delegate this decision to the selected policy.
    return base;
}
