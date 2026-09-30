#include <iostream>
#include <memory>

struct Shape{virtual ~Shape()=default;virtual int area()const{return 0;}};class Rectangle:public Shape{int w_,h_;public:Rectangle(int w,int h):w_(w),h_(h){}int area()const override{return 0;}};
