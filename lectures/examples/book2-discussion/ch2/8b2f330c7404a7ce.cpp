#include <stdexcept>
#include <iostream>
class Rectangle{ int w_,h_; public: Rectangle(int w,int h):w_(w),h_(h){ if(w<=0||h<=0) throw std::invalid_argument("dimensions"); } int area() const{return w_*h_;} };
int main(){ Rectangle r{3,4}; std::cout << r.area() << '\n'; }
