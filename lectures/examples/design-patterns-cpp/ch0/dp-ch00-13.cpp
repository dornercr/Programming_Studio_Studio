#include <iostream>

struct Tally { int count; };

void add_to_copy(Tally value) { value.count += 3; }
void add_to_original(Tally& value) { value.count += 3; }
int observe(const Tally& value) { return value.count; }

int main() {
    Tally tally{5};
    add_to_copy(tally);
    std::cout << "after value=" << observe(tally) << '\n';
    add_to_original(tally);
    std::cout << "after reference=" << observe(tally) << '\n';
}
