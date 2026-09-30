#include "check.hpp"
#include "linux_support.hpp"
#include <array>
#include <cerrno>
#include <memory>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

struct Shared { sem_t ready; int value; };
int main() {
    using namespace harbor_linux;
    int raw[2];
    if (::pipe2(raw, O_CLOEXEC) < 0) fail("pipe2");
    Fd read_end{raw[0]}, write_end{raw[1]};
    void* memory = ::mmap(nullptr, sizeof(Shared), PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) fail("mmap");
    auto* shared = std::construct_at(static_cast<Shared*>(memory));
    if (::sem_init(&shared->ready, 1, 0) < 0) {
        const int error = errno; (void)::munmap(memory, sizeof(Shared)); errno = error; fail("sem_init");
    }
    const pid_t child = ::fork();
    if (child < 0) {
        const int error = errno; (void)::sem_destroy(&shared->ready);
        (void)::munmap(memory, sizeof(Shared)); errno = error; fail("fork");
    }
    if (child == 0) {
        (void)::close(read_end.get());
        shared->value = 42;
        if (::sem_post(&shared->ready) < 0) ::_exit(3);
        const char message[4]{'d', 'o', 'n', 'e'};
        std::size_t offset = 0;
        while (offset < sizeof(message)) {
            const auto count = ::write(write_end.get(), message + offset, sizeof(message) - offset);
            if (count < 0 && errno == EINTR) continue;
            if (count <= 0) ::_exit(4);
            offset += static_cast<std::size_t>(count);
        }
        (void)::close(write_end.get());
        ::_exit(0); // Do not run duplicated parent stream/destructor state.
    }
    write_end.reset();
    std::array<std::byte, 4> message{};
    bool received = false;
    try { wait_readable(read_end.get()); received = read_exact(read_end.get(), message); }
    catch (...) { received = false; }
    int status{};
    pid_t waited;
    do { waited = ::waitpid(child, &status, 0); } while (waited < 0 && errno == EINTR);
    const bool signaled = ::sem_trywait(&shared->ready) == 0;
    const int value = signaled ? shared->value : 0;
    const int destroyed = ::sem_destroy(&shared->ready);
    std::destroy_at(shared);
    const int unmapped = ::munmap(memory, sizeof(Shared));
    CHECK(waited == child && WIFEXITED(status) && WEXITSTATUS(status) == 0);
    CHECK(received && signaled && value == 42 && destroyed == 0 && unmapped == 0);
    CHECK(std::to_integer<char>(message[0]) == 'd');
    std::cout << "PASS\n";
}
