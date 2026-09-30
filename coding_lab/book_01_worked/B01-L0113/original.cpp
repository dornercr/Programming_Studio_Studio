// math.hpp
#pragma once
int add(int a, int b);

// math.cpp
#include "math.hpp"
int add(int a, int b) { return a + b; }

// main.cpp
#include "math.hpp"
#include <iostream>
int main(){ std::cout << add(2,3) << '\n'; }
