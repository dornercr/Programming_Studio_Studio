#include <cassert>

int clamp_percent(int x){ return x<0?0:(x>100?100:x); }

int main(){
    const int input = 120;          // arrange
    const int actual = clamp_percent(input); // act
    assert(actual == 100);          // assert
}
