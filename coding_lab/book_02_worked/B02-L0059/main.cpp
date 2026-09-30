#include <iostream>
int main(){ int total=0; auto add=[&total](auto value){ total += static_cast<int>(value); return total; }; std::cout<<add(2)<<' '<<add(3.5)<<'\n'; }
