#include <array>
#include <iostream>
#include <vector>
int main(){
    std::array<int,3> rgb{255,128,0};
    std::vector<int> samples{4,8,15};
    samples.push_back(16);
    std::cout << rgb.size() << ' ' << samples.size() << '\n';
}
