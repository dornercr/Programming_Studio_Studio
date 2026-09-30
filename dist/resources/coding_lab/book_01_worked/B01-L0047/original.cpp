#include <iostream>
bool can_ship(bool paid, bool address_valid, int stock) {
    if (!paid) return false;
    if (!address_valid) return false;
    return stock > 0;
}
int main(){ std::cout << std::boolalpha << can_ship(true,true,3) << '\n'; }
