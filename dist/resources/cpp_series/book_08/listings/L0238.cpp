#include <array>
int square(int x){return x*x;}
int main(){for(auto p:std::array{std::array{0,0},std::array{7,49},std::array{12,144}})if(square(p[0])!=p[1])return 1;}
