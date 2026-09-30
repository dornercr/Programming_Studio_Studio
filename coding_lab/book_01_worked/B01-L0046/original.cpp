#include <iostream>
int main() {
    int choice = 2;
    switch (choice) {
        case 1: std::cout << "open\n"; break;
        case 2: std::cout << "save\n"; break;
        case 3: std::cout << "quit\n"; break;
        default: std::cout << "unknown\n"; break;
    }
}
