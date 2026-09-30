// Online single-file adaptation of cpp_series/book_01/projects/B01-L0115/counter.hpp
int next_value();

// Online single-file adaptation of cpp_series/book_01/projects/B01-L0115/counter.cpp
int next_value(){ static int n=0; return ++n; }

// Online single-file adaptation of cpp_series/book_01/projects/B01-L0115/main.cpp
#include <iostream>
int main(){ std::cout << next_value() << '\n'; }
