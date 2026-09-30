#include <sys/wait.h>
#include <unistd.h>
#include <cstdint>
#include <iostream>

int main() {
    int fds[2]; if (::pipe(fds) != 0) return 1;
    pid_t pid = ::fork();
    if (pid == 0) {
        ::close(fds[0]); std::uint32_t value=42; ::write(fds[1], &value, sizeof value); ::close(fds[1]); _exit(0);
    }
    ::close(fds[1]); std::uint32_t value{}; ssize_t n=::read(fds[0], &value, sizeof value); ::close(fds[0]);
    ::waitpid(pid,nullptr,0); std::cout << n << ' ' << value << "\n";
}
