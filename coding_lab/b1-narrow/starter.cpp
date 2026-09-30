#include <iostream>
#include <limits>
#include <stdexcept>

int narrow(long long value){ return static_cast<int>(value); // BUG: unchecked narrowing
}
