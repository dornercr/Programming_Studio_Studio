#include "check.hpp"
#include "linux_support.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <span>
#include <string_view>
#include <sys/mman.h>

class ReadMapping {
    void* address_{MAP_FAILED};
    std::size_t size_{};
public:
    ReadMapping(int fd, std::size_t size) : size_(size) {
        if (size == 0) throw std::invalid_argument("empty mapping");
        address_ = ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (address_ == MAP_FAILED) harbor_linux::fail("mmap");
    }
    ReadMapping(const ReadMapping&) = delete;
    ReadMapping& operator=(const ReadMapping&) = delete;
    ~ReadMapping() { if (address_ != MAP_FAILED) (void)::munmap(address_, size_); }
    std::span<const std::byte> bytes() const {
        return {static_cast<const std::byte*>(address_), size_};
    }
};
int main() {
    using namespace harbor_linux;
    auto file = checked_fd(::open("chapter11.bin", O_CREAT | O_EXCL | O_RDWR | O_CLOEXEC, 0600), "open");
    if (::unlink("chapter11.bin") < 0) fail("unlink fixture");
    const std::string_view message = "Harbor file bytes";
    write_all(file.get(), std::as_bytes(std::span{message.data(), message.size()}));
    if (::lseek(file.get(), 0, SEEK_SET) < 0) fail("lseek");
    std::array<std::byte, 17> readback{};
    CHECK(message.size() == readback.size());
    CHECK(read_exact(file.get(), readback));
    const auto original = std::as_bytes(std::span{message.data(), message.size()});
    CHECK(std::equal(readback.begin(), readback.end(), original.begin(), original.end()));
    ReadMapping mapping{file.get(), message.size()};
    CHECK(std::equal(mapping.bytes().begin(), mapping.bytes().end(), original.begin(), original.end()));
    std::cout << "PASS\n";
}
