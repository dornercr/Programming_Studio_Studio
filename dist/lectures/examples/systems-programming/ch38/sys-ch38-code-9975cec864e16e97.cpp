#include <cerrno>
#include <iostream>
#include <unistd.h>
int main() {
    char name[] = "unused";
    char* args[]{name, nullptr};
    char* environment[]{nullptr};
    const int result = execve("", args, environment);
    const int saved_error = errno;
    if (result != -1 || saved_error != ENOENT) return 1;
    std::cout << "exec failed=true\n";
    std::cout << "old code continues=true\n";
}
