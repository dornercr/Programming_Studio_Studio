#include <iostream>
#include <unistd.h>
int main() {
    int ends[2];
    if (pipe(ends) != 0) return 1;
    bool okay = write(ends[1], "cat", 3) == 3;
    okay = close(ends[1]) == 0 && okay;
    char buffer[8]{};
    const auto count = read(ends[0], buffer, sizeof buffer);
    char extra = '?';
    const auto final_count = read(ends[0], &extra, 1);
    okay = close(ends[0]) == 0 && okay;
    if (!okay || count != 3 || final_count != 0) return 1;
    std::cout << "data=";
    std::cout.write(buffer, count);
    std::cout << "\nnext=EOF\n";
}
