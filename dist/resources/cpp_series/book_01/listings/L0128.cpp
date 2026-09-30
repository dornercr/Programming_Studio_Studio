#include "inventory.hpp"
#include <utility>

namespace harbor {
bool Inventory::add(Record record) {
    if (record.id <= 0 || record.name.empty()) return false;
    if (record.quantity < 0 || record.quantity > maximum_quantity) return false;
    if (records_.size() >= maximum_records) return false;
    for (const Record& existing : records_) {
        if (existing.id == record.id) return false;
    }
    records_.push_back(std::move(record));
    return true;
}

long long Inventory::total() const {
    long long result{};
    for (const Record& record : records_) result += record.quantity;
    return result;
}
}
