#include <iostream>
struct Point{ int x; int y; };
int main(){ Point a{1,2}; Point b=a; b.x=9; std::cout << a.x << ' ' << b.x << '\n'; }
