#include <iostream>
#include <unistd.h>
int main() {
    int ends[2];
    if (::pipe(ends) != 0) return 1;
    const char sent = 'Q';
    const auto written = ::write(ends[1], &sent, 1);
    if (written != 1) {
        ::close(ends[0]); ::close(ends[1]);
        return 1;
    }
    char received = 0;
    const auto read_count = ::read(ends[0], &received, 1);
    const int closed_read = ::close(ends[0]);
    const int closed_write = ::close(ends[1]);
    if (read_count != 1 || closed_read != 0 || closed_write != 0) return 1;
    std::cout << "sent=" << sent << " received=" << received << '\n';
}
