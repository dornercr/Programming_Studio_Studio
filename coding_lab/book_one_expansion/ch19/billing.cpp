#include "billing.hpp"
#include <stdexcept>
int billing::subtotal(int unit, int count) {
    if (unit < 0 || unit > 100 || count < 0 || count > 100) throw std::invalid_argument("range");
    return unit * count;
}
