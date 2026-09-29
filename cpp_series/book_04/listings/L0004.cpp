#include <unistd.h>
#include <cstring>

int main() {
    const char* msg = "kernel boundary\n";
    const std::size_t n = std::strlen(msg);
    return ::write(STDOUT_FILENO, msg, n) == static_cast<ssize_t>(n) ? 0 : 1;
}
