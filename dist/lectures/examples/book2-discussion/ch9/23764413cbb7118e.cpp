#include <iostream>
struct Shape{ virtual ~Shape()=default; virtual double area() const=0; };
struct Square:Shape{ double side; explicit Square(double s):side(s){} double area() const override{return side*side;} };
void print_area(const Shape& s){ std::cout<<s.area()<<'\n'; }
int main(){ Square s{4}; print_area(s); }
