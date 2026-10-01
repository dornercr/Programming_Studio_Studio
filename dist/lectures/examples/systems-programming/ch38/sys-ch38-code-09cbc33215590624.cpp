#include <cerrno>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    const pid_t child = fork();
    if (child < 0) return 1;
    if (child == 0) {
        char name[] = "true";
        char locale[] = "LC_ALL=C";
        char* args[]{name, nullptr};
        char* environment[]{locale, nullptr};
        execve("/bin/true", args, environment);
        _exit(91); // Reached only if replacement failed.
    }
    int status = 0; pid_t result;
    do { result = waitpid(child, &status, 0); } while (result < 0 && errno == EINTR);
    if (result != child || !WIFEXITED(status) || WEXITSTATUS(status) != 0) return 1;
    std::cout << "replacement completed=true\n";
    std::cout << "fallback path reached=false\n";
}
