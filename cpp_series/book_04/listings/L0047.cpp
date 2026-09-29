#ifndef HARBOR_CHAPTER_FD_HPP
#define HARBOR_CHAPTER_FD_HPP
#include <cerrno>
#include <system_error>
#include <utility>
#include <unistd.h>

class FileDescriptor {
    int value_{-1};
public:
    explicit FileDescriptor(int value = -1) noexcept : value_(value) {}
    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;
    FileDescriptor(FileDescriptor&& other) noexcept : value_(other.release()) {}
    FileDescriptor& operator=(FileDescriptor&& other) noexcept {
        if (this != &other) { reset(); value_ = other.release(); }
        return *this;
    }
    ~FileDescriptor() { reset(); }
    int get() const noexcept { return value_; }
    explicit operator bool() const noexcept { return value_ >= 0; }
    int release() noexcept { return std::exchange(value_, -1); }
    void reset() noexcept {
        const int saved_errno = errno;
        if (value_ >= 0) (void)::close(release()); // Linux: do not retry close.
        errno = saved_errno;
    }
    void close_checked() {
        if (value_ < 0) return;
        const int descriptor = release();
        if (::close(descriptor) < 0)
            throw std::system_error(errno, std::generic_category(), "close");
    }
};
#endif
