#include <iostream>
struct UserId{ int value; };
bool operator==(UserId a, UserId b){ return a.value==b.value; }
int main(){ UserId a{7}, b{7}; std::cout << std::boolalpha << (a==b) << '\n'; }
