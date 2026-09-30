#include <iostream>
#include <stdexcept>

int afterDiscount(int subtotal,int percent){ if(subtotal<0||subtotal>1000||percent<0||percent>100)throw std::invalid_argument("range"); return subtotal-percent; }
