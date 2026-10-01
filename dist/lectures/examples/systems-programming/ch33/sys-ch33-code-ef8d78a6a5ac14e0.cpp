#include <cstdio>
#include <iostream>
#include <unistd.h>
int main() {
    FILE* file = std::tmpfile();
    if (!file) return 1;
    bool okay = std::fwrite("ABCDE", 1, 5, file) == 5;
    okay = std::fflush(file) == 0 && okay;
    const int fd = fileno(file);
    if (fd < 0 || lseek(fd, 1, SEEK_SET) != 1) okay = false;
    char bytes[2]{};
    if (okay && pread(fd, bytes, 2, 2) != 2) okay = false;
    const auto position = fd >= 0 ? lseek(fd, 0, SEEK_CUR) : -1;
    okay = position == 1 && okay;
    okay = std::fclose(file) == 0 && okay;
    if (!okay) return 1;
    std::cout << "slice=" << bytes[0] << bytes[1] << '\n';
    std::cout << "position=" << position << '\n';
}
