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
            remaining_ = 0;
        }
    };
    ReverseCursor reverse_cursor() const { return ReverseCursor(*this); }
    Cursor cursor() const { return Cursor(*this); }
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("cursor check failed"); }
}
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    Readings readings;
    auto empty = readings.cursor();
    check(empty.done());
    bool end_rejected = false;
    try { empty.read(); }
    catch (const std::out_of_range&) { end_rejected = true; }
    check(end_rejected);
    readings.append(4);
    readings.append(9);
    auto cursor = readings.cursor();
    auto independent = cursor;
    check(cursor.read() == 4);
    cursor.next();
    check(cursor.read() == 9 && independent.read() == 4);
    int total = 0;
    for (auto scan = readings.cursor(); !scan.done(); scan.next()) {
        total += scan.read();
    }
    check(total == 13);
    std::cout << "total: " << total << '\n';
    cursor.next();
    check(cursor.done());
    bool next_rejected = false;
    try { cursor.next(); }
    catch (const std::out_of_range&) { next_rejected = true; }
    check(next_rejected);
    readings.append(12);
    bool stale_rejected = false;
    try { independent.read(); }
    catch (const std::logic_error&) { stale_rejected = true; }
    check(stale_rejected);
    std::cout << "stale cursor: rejected\n";

    auto reverse = readings.reverse_cursor();
    check(reverse.read() == 12);
    reverse.next();
    check(reverse.read() == 9);
    reverse.next();
    check(reverse.read() == 4);
    reverse.next();
    check(reverse.done());
    bool reverse_end = false;
    try { reverse.read(); }
    catch (const std::out_of_range&) { reverse_end = true; }
    check(reverse_end);
    bool reverse_next_end = false;
    try { reverse.next(); }
    catch (const std::out_of_range&) { reverse_next_end = true; }
    check(reverse_next_end);
    readings.append(20);
    bool reverse_stale = false;
    try { reverse.done(); }
    catch (const std::logic_error&) { reverse_stale = true; }
    check(reverse_stale);
    Readings none;
    check(none.reverse_cursor().done());
    std::cout << "reverse: 12 9 4\n";
}
