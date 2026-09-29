#include <iostream>
struct Trace{ int value; Trace(int v):value(v){} Trace(const Trace& other):value(other.value){std::cout<<"copy\n";} };
int main(){ Trace a{5}; Trace b=a; std::cout<<b.value<<'\n'; }
