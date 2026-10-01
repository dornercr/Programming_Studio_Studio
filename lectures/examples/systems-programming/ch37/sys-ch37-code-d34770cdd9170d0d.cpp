#include <array>
#include <cerrno>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    std::array<int, 2> values{2, 4};
    const pid_t child = fork();
    if (child < 0) return 1;
    if (child == 0) {
        values[0] = 9;
        _exit(values[0] + values[1] == 13 ? 0 : 1);
    }
    int status = 0;
    pid_t result;
    do { result = waitpid(child, &status, 0); }
    while (result < 0 && errno == EINTR);
    if (result != child || !WIFEXITED(status) || WEXITSTATUS(status) != 0) return 1;
    std::cout << "parent=" << values[0] << ',' << values[1] << '\n';
    std::cout << "child-private-check=passed\n";
}
