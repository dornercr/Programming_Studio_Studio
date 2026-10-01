#include <iostream>
#include <string>
struct Descriptors {
    std::string out = "terminal";
    std::string err = "terminal";
};
int main() {
    // Names model relationships; no real file is opened.
    Descriptors first;
    first.out = "file";
    first.err = first.out;
    Descriptors second;
    second.err = second.out;
    second.out = "file";
    std::cout << "output-then-duplicate=" << first.out << '/' << first.err << '\n';
    std::cout << "duplicate-then-output=" << second.out << '/' << second.err << '\n';
}
