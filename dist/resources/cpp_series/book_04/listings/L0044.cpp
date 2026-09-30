#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {
    int fd = ::open("handle_demo.txt", O_CREAT|O_TRUNC|O_WRONLY, 0600);
    if (fd < 0) return 1;
    const char* text = "native handle\n";
    const auto n = ::write(fd, text, std::strlen(text));
    const int close_result = ::close(fd);
    return n > 0 && close_result == 0 ? 0 : 2;
}
