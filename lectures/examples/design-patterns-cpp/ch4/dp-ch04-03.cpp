#include <iostream>
#include <string>
#include <vector>
struct CueList {
    std::vector<std::string> names;
};
int main() {
    CueList original{{"Check", "Begin"}};
    CueList copy = original; // Vector and string values are copied.
    copy.names.front() = "Inspect";
    std::cout << "original=" << original.names.front() << '\n';
    std::cout << "copy=" << copy.names.front() << '\n';
    original.names.clear();
    std::cout << "copy after clear=" << copy.names.front() << '\n';
}
