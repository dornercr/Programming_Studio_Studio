#include <iostream>
#include <span>
void inspect_raw(const int* p) { std::cout << "raw first=" << p[0] << '\n'; }
void inspect_span(std::span<const int> s) { std::cout << "span size=" << s.size() << '\n'; }
int main(){ int a[]{1,2,3,4}; inspect_raw(a); inspect_span(a); }
