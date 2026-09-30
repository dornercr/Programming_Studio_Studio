#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

int main() {
    pid_t pid = ::fork();
    if (pid == 0) {
        ::execlp("printf", "printf", "child image\\n", static_cast<char*>(nullptr));
        _exit(127);
    }
    if (pid < 0) return 1;
    int status{}; ::waitpid(pid, &status, 0);
    std::cout << "child exited=" << WIFEXITED(status) << " code=" << WEXITSTATUS(status) << "\n";
}
