#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class Marker {
    int position_ = 0;
public:
    int position() const { return position_; }
    void set(int position) {
        if (position < 0 || position > 10) {
            throw std::out_of_range("marker range");
        }
        position_ = position;
    }
};
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};
class Move final : public Command {
    Marker& marker_;
    int target_;
    int before_ = 0;
    bool applied_ = false;
public:
    Move(Marker& marker, int target)
        : marker_(marker), target_(target) {}
    void execute() override {
        // Reject a second execution.
        if (applied_) {
            throw std::logic_error("repeat");
        }
        before_ = marker_.position();
        marker_.set(target_);
        applied_ = true;
    } // execute
    void undo() override {
        // Undo a successful application.
        if (!applied_) {
            throw std::logic_error("unused");
        }
        marker_.set(before_);
        applied_ = false;
    } // undo
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("command check failed"); }
}
bool run_batch(std::vector<std::unique_ptr<Command>>& batch) {
    std::size_t applied = 0;
    try {
        for (auto& command : batch) {
            command->execute();
            ++applied;
        }
        return true;
    } catch (const std::out_of_range&) {
        while (applied > 0) {
            --applied;
            batch[applied]->undo();
        }
        return false;
    }
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
  std::vector<std::unique_ptr<Command>> b;
  return run_batch(b);
})() << '\n';
}
