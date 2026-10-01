#include <cerrno>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
bool observe(int expected) {
    const pid_t child = fork();
    if (child < 0) return false;
    if (child == 0) _exit(expected);
    int status = 0; pid_t result;
    do { result = waitpid(child, &status, 0); } while (result < 0 && errno == EINTR);
    return result == child && WIFEXITED(status) && WEXITSTATUS(status) == expected;
}
int main() {
    if (!observe(0) || !observe(6)) return 1;
    std::cout << "success code=0\n";
    std::cout << "intentional failure code=6\n";
    std::cout << "supervisor check=passed\n";
}
