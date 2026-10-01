#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

bool copy_text(char* destination, std::size_t capacity, const std::string& source) {
    if(capacity == 0 || source.size() >= capacity) return false;
    std::copy(source.begin(),source.end(),destination);
    destination[source.size()] = '\0';
    return true;
}

int main() {
    std::array<char,4> destination{'x','x','x','x'};
    const auto before = destination;
    assert(!copy_text(destination.data(),3,"cat") && destination == before);
    assert(copy_text(destination.data(),4,"cat"));
    assert(std::string(destination.data()) == "cat");
    std::array<char,1> empty{'x'};
    assert(copy_text(empty.data(),1,"") && empty[0] == '\0');
    assert(!copy_text(nullptr,0,""));
    std::cout << destination.data() << " rejected=unchanged\n";
}
