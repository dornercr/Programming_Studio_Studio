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

std::string cleanupTrace(bool fail){
 // BUG: invents owner destruction for an incomplete object.
 return fail ? "A+ B+ body owner- B- A- caught" : "A+ B+ body owner- B- A-";
}

int main() {
    std::cout<<cleanupTrace(true)<<"\n";
    }
