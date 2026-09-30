#include "check.hpp"
#include <array>
#include <span>
#include <stdexcept>
long long subtotal(std::span<const int> values) {
    if (values.size() > 1000) throw std::invalid_argument("count");
    long long total = 0;
    for (int value : values) {
        if (value < 0 || value > 1000) throw std::invalid_argument("value");
        total += value;
    }
    return total;
}
long long invoice_total(std::span<const int> values, int shipping) {
    if (shipping < 0 || shipping > 1000) throw std::invalid_argument("shipping");
    const auto items = subtotal(values);
    return items + shipping;
}
int main() {
    const std::array prices{12, 20, 8};
    const auto total = invoice_total(prices, 5);
    CHECK(total == 45);
    std::cout << "invoice=" << total << "\nPASS\n";
}
