#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

std::string band(int score){
 if(score<0||score>100)return "invalid";
 // BUG: broader range catches high scores.
 if(score>=50)return "pass";
 if(score>=80)return "high";
 return "retry";
}

int main() {
    int n;
    if(!(std::cin>>n))return 2;
    std::cout<<band(n)<<"\n";
    }
