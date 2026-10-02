#include <iostream>
#include <string>
void inspect(const std::string&){ std::cout<<"lvalue-compatible\n"; }
void inspect(std::string&&){ std::cout<<"rvalue\n"; }
int main(){ std::string s="named"; inspect(s); inspect(std::string{"temp"}); }
