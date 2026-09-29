// point.hpp
#pragma once
struct Point { int x; int y; };

// a.hpp
#pragma once
#include "point.hpp"

// main.cpp
#include "point.hpp"
#include "a.hpp"
int main(){ Point p{1,2}; return p.x+p.y==3 ? 0 : 1; }
