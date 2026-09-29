#include <iostream>
#include <string_view>

std::string_view classify(int temperature) {
    if (temperature < 0 || temperature > 150) return "invalid";
    if (temperature >= 80) return "critical";
    if (temperature >= 60) return "warning";
    return "normal";
}

int main() {
    if (classify(-1) != "invalid") return 1;
    if (classify(59) != "normal") return 2;
    if (classify(60) != "warning") return 3;
    if (classify(79) != "warning") return 4;
    if (classify(80) != "critical") return 5;
    if (classify(151) != "invalid") return 6;
    std::cout << "59=" << classify(59) << " 80=" << classify(80) << '\n';
    std::cout << "PASS\n";
}
