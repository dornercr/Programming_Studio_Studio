#include <iostream>
#include <stdexcept>

void clamp(int& value,int lower,int upper){ if(lower>upper)throw std::invalid_argument("bounds"); if(value<lower)value=lower; else if(value>upper)value=upper; }
