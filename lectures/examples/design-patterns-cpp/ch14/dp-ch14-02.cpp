#include <iostream>
#include <stdexcept>
void set_position(int& position, int target) {
    // Validate before changing the caller's integer.
    if (target < 0 || target > 10) {
        throw std::out_of_range("marker range");
    }
    position = target;
}
int main() {
    int position = 0;
    set_position(position, 3);
    std::cout << "first: " << position << '\n';
    set_position(position, 7);
    std::cout << "second: " << position << '\n';
}
