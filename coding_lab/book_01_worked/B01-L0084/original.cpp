#include <iostream>
void clamp_to_zero(int& value){ if(value < 0) value = 0; }
int main(){ int balance=-5; clamp_to_zero(balance); std::cout << balance << '\n'; }
