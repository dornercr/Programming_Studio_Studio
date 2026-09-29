#include <iostream>
#include <memory>
struct Job{ int id; };
int main(){ auto owner=std::make_unique<Job>(Job{4}); std::cout << owner->id << '\n'; }
