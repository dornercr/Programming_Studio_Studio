#include "check.hpp"
#include <array>
#include <cerrno>
#include <numeric>
#include <string_view>
#include <system_error>
#include <unistd.h>

void write_all(int descriptor, std::string_view text) {
    while (!text.empty()) {
        const auto count = ::write(descriptor, text.data(), text.size());
        if (count < 0) {
            if (errno == EINTR) continue;
            throw std::system_error(errno, std::generic_category(), "write");
        }
        if (count == 0) throw std::runtime_error("write made no progress");
        text.remove_prefix(static_cast<std::size_t>(count));
    }
}
int main() {
    const std::array<int, 4> values{2, 4, 6, 8};
    CHECK(std::accumulate(values.begin(), values.end(), 0) == 20);
    const auto process = ::getpid();
    const long page_size = ::sysconf(_SC_PAGESIZE);
    CHECK(process > 0 && page_size > 0);
    write_all(STDOUT_FILENO, "PASS\n");
}
