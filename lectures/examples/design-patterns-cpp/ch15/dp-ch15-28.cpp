#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

class Readings {
    std::vector<int> values_;
    std::size_t stamp_ = 0;
public:
    Readings() = default;
    Readings(const Readings&) = delete;
    Readings& operator=(const Readings&) = delete;
    void append(int value) {
        if (stamp_ == std::numeric_limits<std::size_t>::max()) {
            throw std::overflow_error("version exhausted");
        }
        values_.push_back(value);
        ++stamp_;
    }
    class Cursor {
        const Readings& owner_;
        std::size_t stamp_;
        std::size_t index_ = 0;
        void validate() const {
            // A change ends this view.
            if (stamp_ != owner_.stamp_) {
                throw std::logic_error(
                    "stale cursor");
            }
        } // validate
    public:
        explicit Cursor(const Readings& owner)
            : owner_(owner), stamp_(owner.stamp_) {}
        bool done() const {
            validate();
            return index_ == owner_.values_.size();
        }
        int read() const {
            // Copy the element value.
            validate();
            return owner_.values_.at(index_);
        } // read
        void next() {
            if (done()) {
                throw std::out_of_range("past end");
            }
            ++index_;
        }
    };
    class ReverseCursor {
        const Readings& owner_;
        std::size_t stamp_;
        std::size_t remaining_;
        void validate() const {
            if (stamp_ != owner_.stamp_) {
                throw std::logic_error("stale reverse cursor");
            }
        }
    public:
        explicit ReverseCursor(const Readings& owner)
            : owner_(owner), stamp_(owner.stamp_),
              remaining_(owner.values_.size()) {}
        bool done() const { validate(); return remaining_ == 0; }
        int read() const {
            if (done()) { throw std::out_of_range("reverse end"); }
            return owner_.values_.at(remaining_ - 1);
        }
        void next() {
            if (done()) { throw std::out_of_range("reverse end"); }
            --remaining_;
        }
    };
    ReverseCursor reverse_cursor() const { return ReverseCursor(*this); }
    Cursor cursor() const { return Cursor(*this); }
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("cursor check failed"); }
}
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  Readings r;
  r.append(3);
  r.append(8);
  auto a=r.cursor(),b=a;
  a.next();
  return b.read();
})() << '\n';
}
