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

std::string failedStage(int compileStatus, int linkStatus, int runStatus) {
    if (compileStatus != 0) return "compile";
    if (linkStatus != 0) return "link";
    if (runStatus != 0) return "run";
    return "none";
}

int main() {
    int c,l,r;
    if(!(std::cin>>c>>l>>r))return 2;
    std::cout<<failedStage(c,l,r)<<"\n";
    }
