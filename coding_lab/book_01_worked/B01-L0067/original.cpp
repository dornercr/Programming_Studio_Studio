#include <iostream>
#include <iterator>
int main(){
    int scores[]{80,90,100};
    int total = 0;
    for (int s : scores) total += s;
    std::cout << total << ' ' << static_cast<double>(total)/std::size(scores) << '\n';
}
