#include <iostream>
int main(){ int value=42; int* p=&value; std::cout << "value=" << *p << " same=" << std::boolalpha << (p==&value) << '\n'; }
