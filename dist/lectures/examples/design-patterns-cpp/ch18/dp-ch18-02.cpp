#include <iostream>
#include <stdexcept>
void publish(int reading, int& total, int& latest) {
    if (reading < 0 || reading > 100) {
        throw std::out_of_range("reading range");
    }
    total += reading;
    latest = reading;
}
int main() {
    int total = 0;
    int latest = 0;
    publish(4, total, latest);
    publish(3, total, latest);
    std::cout << "total: " << total << '\n';
    std::cout << "latest: " << latest << '\n';
}
