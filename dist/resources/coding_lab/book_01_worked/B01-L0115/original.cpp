// counter.hpp
#pragma once
int next_value();

// counter.cpp
#include "counter.hpp"
int next_value(){ static int n=0; return ++n; }

// main.cpp
#include "counter.hpp"
#include <iostream>
int main(){ std::cout << next_value() << '\n'; }
