#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
int main() {
    int ends[2];
    if (pipe(ends) != 0) return 1;
    const int extra_writer = dup(ends[1]);
    if (extra_writer < 0) { close(ends[0]); close(ends[1]); return 1; }
    const int flags = fcntl(ends[0], F_GETFL);
    bool okay = flags >= 0 && fcntl(ends[0], F_SETFL, flags | O_NONBLOCK) == 0;
    okay = close(ends[1]) == 0 && okay;
    if (!okay) { close(extra_writer); close(ends[0]); return 1; }
    char byte = '?';
    const auto first = read(ends[0], &byte, 1);
    const int first_error = errno;
    okay = close(extra_writer) == 0 && okay;
    const auto second = read(ends[0], &byte, 1);
    okay = close(ends[0]) == 0 && okay;
    const bool waiting = first == -1 &&
        (first_error == EAGAIN || first_error == EWOULDBLOCK);
    if (!okay || !waiting || second != 0) return 1;
    std::cout << "retained-writer=would-block\n";
    std::cout << "all-writers-closed=EOF\n";
}
