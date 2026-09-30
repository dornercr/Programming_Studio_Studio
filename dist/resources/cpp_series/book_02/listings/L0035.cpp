#include <iostream>
struct Base{ Base(){std::cout<<"Base construct\n";} ~Base(){std::cout<<"Base destroy\n";} };
struct Derived:Base{ Derived(){std::cout<<"Derived construct\n";} ~Derived(){std::cout<<"Derived destroy\n";} };
int main(){ Derived d; }
