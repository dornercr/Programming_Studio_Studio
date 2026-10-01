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
    int refresh() {
        if (!allowed_) throw std::runtime_error(
                "denied");
        invalidate();
        return opening();
    }
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
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  Source s;
  HoursProxy p(s);
  return p.loaded();
})() << '\n';
}
