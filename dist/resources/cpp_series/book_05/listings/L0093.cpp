#include <algorithm>
#include <iostream>
#include <vector>

int main(){
 std::vector<float> temperatures{20,21,100,22,23};
 std::erase_if(temperatures,[](float t){return t>60;});
 std::transform(temperatures.begin(),temperatures.end(),temperatures.begin(),[](float t){return t*1.8f+32;});
 for(float t:temperatures) std::cout<<t<<' ';
}
