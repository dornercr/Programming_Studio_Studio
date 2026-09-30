#include <iostream>
struct Meters{ double value; };
struct Seconds{ double value; };
double speed(Meters d, Seconds t){ return d.value / t.value; }
int main(){ std::cout << speed(Meters{100.0}, Seconds{9.58}) << '\n'; }
