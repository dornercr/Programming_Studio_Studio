#include <iostream>
int main(){
    int scores[4]{90, 85, 88, 92};
    scores[1] = 87;
    std::cout << scores[0] << ' ' << scores[1] << ' ' << scores[3] << '\n';
}
