#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string live = "version=1";
    std::string candidate = "broken";
    if (candidate.rfind("version=", 0) == 0) live.swap(candidate);
    std::cout << live << '\n';
    candidate = "version=2";
    if (candidate.rfind("version=", 0) == 0) live.swap(candidate);
    std::cout << live << '\n';
    assert(live == "version=2");
}
