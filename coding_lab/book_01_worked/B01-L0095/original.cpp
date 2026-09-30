#include <iostream>
#include <memory>
struct Resource{ ~Resource(){ std::cout << "released\n"; } };
void work(){ auto r = std::make_unique<Resource>(); std::cout << "working\n"; }
int main(){ work(); }
