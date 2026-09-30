#include <iostream>
#include <memory>
#include <vector>

struct Shape{virtual ~Shape()=default;virtual float area()const=0;};
struct Square:Shape{float s;explicit Square(float x):s(x){}float area()const override{return s*s;}};
struct SquareData{float side;};
int main(){
 std::vector<std::unique_ptr<Shape>> objects;objects.push_back(std::make_unique<Square>(3));
 std::vector<SquareData> dense{{3}};
 std::cout<<objects[0]->area()<<' '<<dense[0].side*dense[0].side<<"\n";
}
