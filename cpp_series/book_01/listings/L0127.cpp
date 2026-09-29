#ifndef HARBOR_INVENTORY_HPP
#define HARBOR_INVENTORY_HPP
#include <string>
#include <vector>

namespace harbor {
struct Record {
    int id{};
    std::string name;
    int quantity{};
};

class Inventory {
public:
    static constexpr int maximum_quantity{1000};
    static constexpr std::size_t maximum_records{100};
    bool add(Record record);
    long long total() const;
    const std::vector<Record>& records() const { return records_; }
private:
    std::vector<Record> records_;
};
}
#endif
