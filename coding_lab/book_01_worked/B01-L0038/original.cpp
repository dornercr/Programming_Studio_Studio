#include <iostream>
int main() {
    int items = 17;
    int boxes = 5;
    std::cout << "full=" << items / boxes << " leftover=" << items % boxes << '\n';
    std::cout << "average=" << static_cast<double>(items) / boxes << '\n';
}
