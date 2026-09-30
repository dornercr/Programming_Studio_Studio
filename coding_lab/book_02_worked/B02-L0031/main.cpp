#include <iostream>
struct Point{ int x,y; };
std::ostream& operator<<(std::ostream& out,const Point& p){ return out << '(' << p.x << ',' << p.y << ')'; }
int main(){ std::cout << Point{2,3} << '\n'; }
