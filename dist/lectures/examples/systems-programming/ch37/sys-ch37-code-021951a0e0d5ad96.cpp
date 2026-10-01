#include <cerrno>
#include <cstdio>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    FILE* file = std::tmpfile();
    if (!file) return 1;
    if (std::fwrite("ABC", 1, 3, file) != 3 || std::fflush(file) != 0) {
        std::fclose(file); return 1;
    }
    const int fd = fileno(file);
    if (fd < 0 || lseek(fd, 0, SEEK_SET) != 0) { std::fclose(file); return 1; }
    const pid_t child = fork();
    if (child < 0) { std::fclose(file); return 1; }
    if (child == 0) {
        char byte = '?';
        const bool okay = read(fd, &byte, 1) == 1 && byte == 'A';
        const bool closed = close(fd) == 0;
        _exit(okay && closed ? 0 : 1);
    }
    int status = 0; pid_t result;
    do { result = waitpid(child, &status, 0); } while (result < 0 && errno == EINTR);
    bool okay = result == child && WIFEXITED(status) && WEXITSTATUS(status) == 0;
    char next = '?';
    if (okay && read(fd, &next, 1) != 1) okay = false;
    const auto offset = lseek(fd, 0, SEEK_CUR);
    okay = std::fclose(file) == 0 && okay;
    if (!okay || next != 'B' || offset != 2) return 1;
    std::cout << "child-read=A parent-read=" << next << " offset=" << offset << '\n';
}
