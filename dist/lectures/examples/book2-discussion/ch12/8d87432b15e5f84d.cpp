#include <iostream>
template<class T> struct Kind{ static const char* name(){return "value";} };
template<class T> struct Kind<T*>{ static const char* name(){return "pointer";} };
int main(){ std::cout<<Kind<int>::name()<<' '<<Kind<int*>::name()<<'\n'; }
