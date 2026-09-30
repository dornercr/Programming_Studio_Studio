#include <iostream>
struct Trace {
    Trace(){ std::cout << "construct\n"; }
    ~Trace(){ std::cout << "destroy\n"; }
};
int main(){
    std::cout << "before\n";
    { Trace t; std::cout << "inside\n"; }
    std::cout << "after\n";
}
