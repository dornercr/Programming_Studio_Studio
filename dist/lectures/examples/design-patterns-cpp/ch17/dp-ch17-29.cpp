#include <iostream>
#include <stdexcept>
#include <vector>

class Selection {
    int low_ = 0;
    int high_ = 0;
public:
    class Snapshot {
        friend class Selection;
        int low_;
        int high_;
        Snapshot(int low, int high)
            : low_(low), high_(high) {}
    public:
        Snapshot(const Snapshot&) = default;
        Snapshot& operator=(const Snapshot&) = default;
    };
    void set(int low, int high) {
        if (low < 0 || high > 100 || low > high) {
            throw std::out_of_range("selection range");
        }
        low_ = low;
        high_ = high;
    }
    Snapshot save() const {
        // Copy the valid pair.
        return Snapshot(low_, high_);
    }
    void restore(const Snapshot& saved) {
        // Copy the pair back.
        low_ = saved.low_;
        high_ = saved.high_;
    } // restore
    int low() const { return low_; }
    int high() const { return high_; }
};
class History {
    Selection& selection_;
    std::vector<Selection::Snapshot> saved_;
public:
    explicit History(Selection& selection) : selection_(selection) {}
    void remember() {
        auto proposed = saved_;
        if (proposed.size() == 3) { proposed.erase(proposed.begin()); }
        proposed.push_back(selection_.save());
        saved_.swap(proposed);
    }
    bool undo() {
        // An empty history is normal.
        if (saved_.empty()) { return false; }
        selection_.restore(saved_.back());
        saved_.pop_back();
        return true;
    } // undo
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("snapshot check failed"); }
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
  Selection a,b;
  a.set(4,9);
  b.restore(a.save());
  return b.low()==4 && b.high()==9;
})() << '\n';
}
