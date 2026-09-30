#include <iostream>
#include <string>
#include <variant>
using Result=std::variant<int,std::string>;
Result divide(int a,int b){ if(b==0)return std::string{"divide by zero"}; return a/b; }
int main(){ Result r=divide(8,0); std::visit([](const auto& x){std::cout<<x<<'\n';},r); }
