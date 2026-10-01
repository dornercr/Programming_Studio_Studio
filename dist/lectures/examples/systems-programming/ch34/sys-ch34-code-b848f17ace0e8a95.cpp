#include <cstdio>
#include <iostream>
#include <unistd.h>
int main() {
    FILE* file = std::tmpfile();
    if (!file) return 1;
    bool okay = std::fwrite("XYZ", 1, 3, file) == 3;
    okay = std::fflush(file) == 0 && okay;
    const int fd = fileno(file);
    if (fd < 0 || lseek(fd, 0, SEEK_SET) != 0) okay = false;
    const int other = okay ? dup(fd) : -1;
    char first = '?', second = '?';
    if (other < 0) okay = false;
    if (okay && read(fd, &first, 1) != 1) okay = false;
    if (okay && read(other, &second, 1) != 1) okay = false;
    const auto position = fd >= 0 ? lseek(fd, 0, SEEK_CUR) : -1;
    if (other >= 0 && close(other) != 0) okay = false;
    okay = std::fclose(file) == 0 && okay;
    if (!okay || position != 2) return 1;
    std::cout << "bytes=" << first << second << " offset=" << position << '\n';
}
