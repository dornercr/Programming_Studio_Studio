#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    const std::string line = "emit | count";
    std::istringstream input(line);
    std::vector<std::string> tokens;
    for (std::string word; input >> word;) tokens.push_back(word);
    assert(tokens.size() == 3 && tokens[1] == "|");
    std::cout << "left=" << tokens[0] << " right=" << tokens[2] << '\n';
}
