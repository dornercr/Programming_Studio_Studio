#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    int value = 7;
    const pid_t child = fork();
    if (child < 0) throw std::runtime_error("fork");
    if (child == 0) {
        value = 9;
        _exit(value == 9 ? 0 : 1);
    }
    int status = 0;
    pid_t result;
    do { result = waitpid(child, &status, 0); } while (result < 0 && errno == EINTR);
    if (result != child || !WIFEXITED(status))
        throw std::runtime_error("child did not exit normally");
    std::cout << "parent=" << value << " child-status=" << WEXITSTATUS(status) << '\n';
}
