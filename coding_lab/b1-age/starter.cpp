#include <iostream>
#include <stdexcept>

int nextAge(int age){ if(age>130)throw std::invalid_argument("invalid age"); return age+1; }
