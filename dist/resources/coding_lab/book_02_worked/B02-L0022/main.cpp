#include <iostream>
#include <string>
#include <utility>
int main(){ std::string s="payload"; auto&& r=std::move(s); std::cout<<s<<' '<<r<<'\n'; std::string destination=std::move(s); std::cout<<destination<<'\n'; }
