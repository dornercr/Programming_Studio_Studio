#include <iostream>
#include <memory>
struct Task{ int id; };
std::unique_ptr<Task> make_task(int id){ return std::make_unique<Task>(Task{id}); }
void consume(std::unique_ptr<Task> t){ std::cout<<t->id<<'\n'; }
int main(){ auto t=make_task(9); consume(std::move(t)); std::cout<<std::boolalpha<<(t==nullptr)<<'\n'; }
