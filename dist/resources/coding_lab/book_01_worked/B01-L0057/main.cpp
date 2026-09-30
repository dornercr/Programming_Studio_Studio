#include <iostream>
int area(int side) { return side * side; }
int area(int width, int height) { return width * height; }
int scale(int value, int factor = 2) { return value * factor; }
int main(){ std::cout << area(4) << ' ' << area(4,5) << ' ' << scale(3) << '\n'; }
