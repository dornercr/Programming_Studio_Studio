#include "check.hpp"
#include <cstdint>
#include <optional>
#include <stdexcept>

struct Clock { virtual ~Clock() = default; virtual std::int64_t now() const = 0; };
struct Source { virtual ~Source() = default; virtual int fetch() = 0; };
class Cache {
    Clock& clock_;
    Source& source_;
    std::optional<int> value_;
    std::int64_t stored_at_{};
public:
    Cache(Clock& clock, Source& source) : clock_(clock), source_(source) {}
    int get() {
        const auto now = clock_.now();
        if (now < 0 || now > 1000000000) throw std::runtime_error("clock domain");
        if (value_ && now >= stored_at_ && now - stored_at_ < 10) return *value_;
        const int candidate = source_.fetch(); // Failure leaves prior state unchanged.
        value_ = candidate; stored_at_ = now;
        return candidate;
    }
};
struct FakeClock final : Clock {
    std::int64_t tick{};
    std::int64_t now() const override { return tick; }
};
struct CountingSource final : Source {
    int calls{}, value{7}; bool fail{};
    int fetch() override { ++calls; if (fail) throw std::runtime_error("source failure"); return value; }
};
int main() {
    FakeClock clock; CountingSource source; Cache cache{clock, source};
    CHECK(cache.get() == 7 && source.calls == 1);
    clock.tick = 9; source.value = 8;
    CHECK(cache.get() == 7 && source.calls == 1);
    clock.tick = 10;
    CHECK(cache.get() == 8 && source.calls == 2);
    clock.tick = 20; source.fail = true;
    bool failed = false;
    try { (void)cache.get(); } catch (const std::runtime_error&) { failed = true; }
    CHECK(failed && source.calls == 3);
    source.fail = false; source.value = 9;
    CHECK(cache.get() == 9 && source.calls == 4);
    std::cout << "PASS\n";
}
