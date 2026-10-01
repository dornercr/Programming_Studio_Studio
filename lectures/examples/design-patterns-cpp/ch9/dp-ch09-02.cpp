#include <iostream>
#include <string>

std::string status(const std::string& body) {
    return "WARN " + body.substr(0, 5); // Trim the body, then prefix it.
}
int main() {
    std::cout << "message='" << status("ready now") << "'\n";
}
