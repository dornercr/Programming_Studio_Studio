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
    void remember() { saved_.push_back(selection_.save()); }
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
int main() {
    Selection selection;
    History history(selection);
    check(!history.undo());
    selection.set(2, 5);
    const auto saved = selection.save();
    history.remember();
    selection.set(20, 40);
    check(history.undo());
    check(selection.low() == 2 && selection.high() == 5);
    std::cout << "restored: " << selection.low() << ':'
              << selection.high() << '\n';
    check(!history.undo());
    bool rejected = false;
    try { selection.set(9, 3); }
    catch (const std::out_of_range&) { rejected = true; }
    check(rejected && selection.low() == 2 && selection.high() == 5);
    selection.set(0, 100);
    selection.restore(saved);
    Selection other;
    other.restore(saved);
    check(other.low() == 2 && other.high() == 5);
    check(selection.low() == 2 && selection.high() == 5);
    std::cout << "invalid pair: rejected\n";
}
