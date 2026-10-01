#include <csignal>
#include <iostream>
#include <signal.h>
volatile std::sig_atomic_t observed = 0;
extern "C" void notice(int) { observed = 1; }
int main() {
    struct sigaction action{}, previous{};
    action.sa_handler = notice;
    if (sigemptyset(&action.sa_mask) == -1) return 1;
    if (sigaction(SIGUSR1, &action, &previous) == -1) return 2;
    const int raised = raise(SIGUSR1);
    const int restored = sigaction(SIGUSR1, &previous, nullptr);
    if (raised != 0 || restored == -1) return 3;
    std::cout << "observed=" << std::boolalpha << (observed == 1) << '\n';
}
