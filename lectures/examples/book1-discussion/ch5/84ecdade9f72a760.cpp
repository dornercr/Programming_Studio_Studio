#include <iostream>
int main() {
    int temperature = 72;
    bool comfortable = temperature >= 68 && temperature <= 76;
    bool alert = temperature < 50 || temperature > 90;
    std::cout << std::boolalpha << comfortable << ' ' << alert << '\n';
}
