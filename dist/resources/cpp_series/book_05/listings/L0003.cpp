// include/calc/add.hpp
#pragma once
int add(int a, int b);

// src/add.cpp
#include "calc/add.hpp"
int add(int a, int b) { return a + b; }

// app/main.cpp
#include "calc/add.hpp"
#include <iostream>
int main() { std::cout << add(20, 22) << "\n"; }
