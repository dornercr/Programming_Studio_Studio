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
int main() {
    Marker marker;
    std::vector<std::unique_ptr<Command>> queue;
    queue.push_back(std::make_unique<Move>(marker, 3));
    queue.push_back(std::make_unique<Move>(marker, 7));
    check(marker.position() == 0);
    for (auto& command : queue) { command->execute(); }
    check(marker.position() == 7);
    std::cout << "after queue: " << marker.position() << '\n';
    bool repeated = false;
    try { queue.back()->execute(); }
    catch (const std::logic_error&) { repeated = true; }
    check(repeated && marker.position() == 7);
    for (auto it = queue.rbegin(); it != queue.rend(); ++it) {
        (*it)->undo();
    }
    check(marker.position() == 0);
    std::cout << "after undo: " << marker.position() << '\n';
    bool rejected = false;
    Move bad(marker, 11);
    try { bad.execute(); }
    catch (const std::out_of_range&) { rejected = true; }
    check(rejected && marker.position() == 0);
    bool undo_rejected = false;
    try { bad.undo(); }
    catch (const std::logic_error&) { undo_rejected = true; }
    check(undo_rejected);
    Move edge(marker, 10);
    edge.execute();
    check(marker.position() == 10);
    edge.undo();
    edge.execute();
    edge.undo();
    check(marker.position() == 0);
    std::cout << "invalid move: rejected\n";
}
