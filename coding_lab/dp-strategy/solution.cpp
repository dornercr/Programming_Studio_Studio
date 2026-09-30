#include <iostream>
#include <functional>

int checkout(int base, const std::function<int(int)>& policy) { return policy(base); }
