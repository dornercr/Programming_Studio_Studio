#include <iostream>
int main(){ int value=10; int& alias=value; alias=25; std::cout << value << ' ' << alias << '\n'; }
