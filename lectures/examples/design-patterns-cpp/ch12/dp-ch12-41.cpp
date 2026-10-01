// LAB STARTER: see the chapter for extension requirements.
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
struct Source { int hour = 8; int reads = 0; };
class Hours {
public:
    Hours() = default;
    Hours(const Hours&) = delete;
    Hours& operator=(const Hours&) = delete;
    Hours(Hours&&) = delete;
    Hours& operator=(Hours&&) = delete;
    virtual ~Hours() = default;
    virtual int opening() = 0;
};
class RealHours final : public Hours {
    Source& source_;
public:
    explicit RealHours(Source& s) : source_(s) {}
    int opening() override {
        ++source_.reads;
        return source_.hour;
    }
};
class HoursProxy final : public Hours {
    Source& source_;
    bool allowed_ = true;
    std::unique_ptr<RealHours> real_;
    std::optional<int> cached_;
public:
    explicit HoursProxy(Source& s) : source_(s) {}
    void allow(bool value) { allowed_ = value; }
    void invalidate() { cached_.reset(); }
    bool loaded() const { return static_cast<bool>(real_); }
    int opening() override {
        // Check access before any result.
        if (!allowed_) {
            throw std::runtime_error(
                "denied");
        }
        if (cached_) return *cached_;
        // Construct the service on demand.
        if (!real_) {
            real_ =
                std::make_unique<RealHours>(
                    source_);
        }
        cached_ = real_->opening();
        return *cached_;
    } // End miss path.
};
// Move owning handles, not service objects.
static_assert(!std::is_move_constructible_v<HoursProxy>);
static_assert(!std::is_copy_constructible_v<HoursProxy>);
int main() {
    Source source;
    HoursProxy proxy(source);
    check(!proxy.loaded() && source.reads == 0);
    check(proxy.opening() == 8);
    check(proxy.loaded() && source.reads == 1);
    source.hour = 9;
    check(proxy.opening() == 8 && source.reads == 1);
    std::cout << "cached=" << proxy.opening() << '\n';
    proxy.invalidate();
    check(proxy.opening() == 9 && source.reads == 2);
    std::cout << "fresh=" << proxy.opening() << '\n';
    proxy.allow(false);
    bool denied = false;
    try { static_cast<void>(proxy.opening()); }
    catch (const std::runtime_error&) { denied = true; }
    check(denied && source.reads == 2);
    proxy.allow(true);
    source.hour = 0;
    proxy.invalidate();
    check(proxy.opening() == 0);
    check(proxy.opening() == 0 && source.reads == 3);
    std::cout << "midnight=" << proxy.opening() << '\n';
    std::cout << "reads=" << source.reads << '\n';
    std::cout << "cached access denied after revocation\n";
}
