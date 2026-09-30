#include <iostream>
#include <string>
void print_length(const std::string& s){ std::cout << s.size() << '\n'; }
int main(){ std::string name="C++"; print_length(name); }
