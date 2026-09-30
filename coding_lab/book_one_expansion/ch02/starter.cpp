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

bool environmentReady(long tag, bool compiler, bool library) {
    // BUG: accepts a compiler alone.
    return compiler || (library && tag >= 202002);
}

int main() {
    long tag;
    int c,l;
    if(!(std::cin>>tag>>c>>l))return 2;
    std::cout<<std::boolalpha<<environmentReady(tag,c!=0,l!=0)<<"\n";
    }
