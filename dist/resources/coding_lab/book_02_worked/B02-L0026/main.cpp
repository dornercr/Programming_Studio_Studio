#include <iostream>
#include <memory>
struct State{ int value=5; };
int main(){ auto a=std::make_shared<State>(); auto b=a; b->value=8; std::cout<<a->value<<' '<<a.use_count()<<'\n'; }
