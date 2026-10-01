#include <cerrno>
#include <csignal>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    const pid_t child = fork();
    if (child < 0) return 1;
    if (child == 0) {
        struct sigaction action{};
        action.sa_handler = SIG_DFL;
        sigemptyset(&action.sa_mask);
        sigset_t unblock;
        sigemptyset(&unblock);
        sigaddset(&unblock, SIGTERM);
        if (sigaction(SIGTERM, &action, nullptr) != 0 ||
            sigprocmask(SIG_UNBLOCK, &unblock, nullptr) != 0) _exit(90);
        raise(SIGTERM);
        _exit(91);
    }
    int status = 0; pid_t result;
    do { result = waitpid(child, &status, 0); } while (result < 0 && errno == EINTR);
    if (result != child || !WIFSIGNALED(status) || WTERMSIG(status) != SIGTERM) return 1;
    std::cout << "category=signal\nexpected-signal=true\n";
}
