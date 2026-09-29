#include <iostream>
class Counter{ int n_{}; public: void increment(){++n_;} int value() const{return n_;} };
int main(){ Counter c; c.increment(); const Counter& view=c; std::cout << view.value() << '\n'; }
