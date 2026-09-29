#include <cstddef>
#include <iostream>

struct Record {
    char tag;
    int count;
    char state;
};

int main() {
    std::cout << "sizeof=" << sizeof(Record) << "\n";
    std::cout << "count offset=" << offsetof(Record, count) << "\n";
    std::cout << "state offset=" << offsetof(Record, state) << "\n";
}
