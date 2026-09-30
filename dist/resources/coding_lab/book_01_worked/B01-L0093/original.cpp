#include <iostream>
#include <memory>
int main(){
    int automatic = 1;
    auto dynamic = std::make_unique<int>(2);
    std::cout << automatic << ' ' << *dynamic << '\n';
}
