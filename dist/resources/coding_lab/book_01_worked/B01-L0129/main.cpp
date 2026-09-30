// Original book listing B01-L0127
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

// Original book listing B01-L0128
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

// Original book listing B01-L0129
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void report(const harbor::Inventory& inventory) {
    for (const auto& record : inventory.records()) {
        std::cout << record.id << ' ' << std::quoted(record.name)
                  << ' ' << record.quantity << '\n';
    }
}

int self_test() {
    harbor::Inventory inventory;
    if (!inventory.add({1, "bench scope", 2})) return 1;
    if (!inventory.add({2, "meter", 3})) return 2;
    if (inventory.add({1, "duplicate", 4})) return 3;
    if (inventory.add({3, "invalid", -1})) return 4;
    if (inventory.records().size() != 2 || inventory.total() != 5) return 5;
    report(inventory);
    std::cout << "total=" << inventory.total() << '\n';
    std::cout << "PASS\n";
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1 || (argc == 2 && std::string{argv[1]} == "--self-test")) {
        return self_test();
    }
    if (argc != 2 || std::string{argv[1]} != "--interactive") {
        std::cerr << "usage: inventory [--self-test|--interactive]\n";
        return 2;
    }
    harbor::Inventory inventory;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream input{line};
        std::string command;
        if (!(input >> command)) continue;
        if (command == "add") {
            harbor::Record candidate;
            std::string extra;
            if (!(input >> candidate.id >> std::quoted(candidate.name)
                        >> candidate.quantity) || (input >> extra)) {
                std::cout << "error: malformed add\n";
                continue;
            }
            std::cout << (inventory.add(candidate) ? "added\n" : "rejected\n");
        } else {
            std::string extra;
            if (input >> extra) {
                std::cout << "error: unexpected arguments\n";
                continue;
            }
            if (command == "list") report(inventory);
            else if (command == "total") std::cout << "total=" << inventory.total() << '\n';
            else if (command == "quit") break;
            else std::cout << "error: unknown command\n";
        }
    }
    if (std::cin.bad()) return 3;
    return 0;
}
