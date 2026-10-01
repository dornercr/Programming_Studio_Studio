#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
enum class Kind { word, pipe };
struct Token { Kind kind; std::string text; };
int main() {
    const std::vector<Token> tokens{{Kind::word,"print"},
        {Kind::word,"|"}, {Kind::pipe,"|"}, {Kind::word,"count"}};
    std::vector<std::vector<std::string>> stages(1);
    for (const auto& token : tokens) {
        if (token.kind == Kind::pipe) {
            if (stages.back().empty()) throw std::runtime_error("empty stage");
            stages.emplace_back();
        } else stages.back().push_back(token.text);
    }
    if (stages.back().empty()) throw std::runtime_error("trailing pipe");
    std::cout << "stages=" << stages.size() << '\n';
    std::cout << "first arguments=" << stages[0].size() << '\n';
    std::cout << "literal argument=" << stages[0][1] << '\n';
}
