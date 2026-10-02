#include <compare>
#include <iostream>
struct Cents{ long value; friend Cents operator+(Cents a,Cents b){return {a.value+b.value};} auto operator<=>(const Cents&) const = default; };
int main(){ Cents a{125},b{75}; auto c=a+b; std::cout<<c.value<<' '<<std::boolalpha<<(c>Cents{100})<<'\n'; }
