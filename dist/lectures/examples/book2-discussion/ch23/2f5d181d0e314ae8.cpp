#include "course_test.hpp"
#include <array>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>

struct AcquisitionFailure final {};
struct PlannedFailure final {};

// A fake device registry, not an operating-system handle implementation.
// This owner must outlive every Lease that refers to it.
class Registry {
    std::array<bool, 8> occupied_{};
    unsigned acquired_{};
    unsigned released_{};
    unsigned invalid_releases_{};
public:
    int acquire(bool reject = false) {
        if (reject) throw AcquisitionFailure{};
        for (std::size_t i = 0; i < occupied_.size(); ++i) {
            if (!occupied_[i]) {
                occupied_[i] = true;
                ++acquired_;
                return static_cast<int>(i);
            }
        }
        throw AcquisitionFailure{};
    }
    bool release(int handle) noexcept {
        if (handle < 0 || static_cast<std::size_t>(handle) >= occupied_.size()
            || !occupied_[static_cast<std::size_t>(handle)]) {
            ++invalid_releases_;
            return false;
        }
        occupied_[static_cast<std::size_t>(handle)] = false;
        ++released_;
        return true;
    }
    unsigned live() const noexcept { return acquired_ - released_; }
    unsigned acquired() const noexcept { return acquired_; }
    unsigned released() const noexcept { return released_; }
    unsigned invalid_releases() const noexcept { return invalid_releases_; }
};

class Lease {
    Registry* registry_{}; // Borrowed, with a documented enclosing lifetime.
    int handle_{-1};
public:
    explicit Lease(Registry& registry, bool reject = false)
        : registry_(&registry), handle_(registry.acquire(reject)) {}
    ~Lease() noexcept { (void)close(); }
    Lease(const Lease&) = delete;
    Lease& operator=(const Lease&) = delete;
    Lease(Lease&& other) noexcept
        : registry_(std::exchange(other.registry_, nullptr)),
          handle_(std::exchange(other.handle_, -1)) {}
    Lease& operator=(Lease&& other) noexcept {
        if (this != &other) {
            (void)close();
            registry_ = std::exchange(other.registry_, nullptr);
            handle_ = std::exchange(other.handle_, -1);
        }
        return *this;
    }
    bool close() noexcept {
        if (handle_ == -1) return true;
        const int previous = std::exchange(handle_, -1);
        return registry_->release(previous);
    }
    bool owns_resource() const noexcept { return handle_ != -1; }
};

int main() {
    static_assert(!std::is_copy_constructible_v<Lease>);
    static_assert(std::is_nothrow_move_constructible_v<Lease>);
    Registry registry;
    {
        Lease resource{registry};
        CHECK(registry.live() == 1 && resource.owns_resource());
    }
    CHECK(registry.live() == 0 && registry.released() == 1);
    try {
        Lease resource{registry};
        CHECK(registry.live() == 1);
        throw PlannedFailure{};
    } catch (const PlannedFailure&) {}
    CHECK(registry.live() == 0 && registry.released() == 2);
    CHECK(course::throws<AcquisitionFailure>([&] { Lease rejected{registry, true}; }));
    CHECK(registry.acquired() == 2 && registry.released() == 2);
    {
        Lease first{registry};
        Lease second{std::move(first)};
        CHECK(!first.owns_resource() && second.owns_resource());
        CHECK(registry.live() == 1);
        Lease destination{registry};
        CHECK(registry.live() == 2);
        destination = std::move(second);
        CHECK(registry.live() == 1 && !second.owns_resource());
        Lease& alias = destination;
        destination = std::move(alias); // This class preserves self-move.
        CHECK(destination.owns_resource() && registry.live() == 1);
        CHECK(destination.close());
        const auto releases = registry.released();
        CHECK(destination.close());
        CHECK(registry.released() == releases && registry.live() == 0);
    }
    CHECK(registry.acquired() == registry.released());
    CHECK(registry.invalid_releases() == 0);
    course::report();
}
