#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

int main() {
    ::setenv("DEMO_MODE", "safe", 1);
    pid_t pid = ::fork();
    if (pid == 0) {
        const char* value = std::getenv("DEMO_MODE");
        _exit(value && std::string(value)=="safe" ? 23 : 24);
    }
    int status{}; ::waitpid(pid, &status, 0);
    if (WIFEXITED(status)) std::cout << WEXITSTATUS(status) << "\n";
}
