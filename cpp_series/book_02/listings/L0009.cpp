#include <iostream>
#include <string>
#include <vector>
struct Report{ std::string title; std::vector<int> values; };
int main(){ Report a{"run",{1,2,3}}; Report b=a; b.values[0]=9; std::cout << a.values[0] << ' ' << b.values[0] << '\n'; }
