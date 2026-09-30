#include "course_test.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Snapshot {
    std::string name_;
    std::vector<int> values_;
public:
    Snapshot(std::string name, std::vector<int> values)
        : name_(std::move(name)), values_(std::move(values)) {}
    const std::string& name() const noexcept { return name_; }
    const std::vector<int>& values() const noexcept { return values_; }
};

int main() {
    auto current = std::make_shared<const Snapshot>("revision-one", std::vector{1, 2, 3});
    auto first_reader = current;
    auto second_reader = current;
    const std::weak_ptr<const Snapshot> old_revision = current;
    current = std::make_shared<const Snapshot>("revision-two", std::vector{8, 9});
    CHECK(current->name() == "revision-two");
    CHECK(first_reader->name() == "revision-one");
    CHECK(second_reader->values() == std::vector<int>({1, 2, 3}));
    first_reader.reset();
    CHECK(!old_revision.expired());
    {
        auto secured = old_revision.lock();
        CHECK(secured && secured->values().size() == 3);
    }
    second_reader.reset();
    CHECK(old_revision.expired());
    CHECK(!old_revision.lock());
    CHECK(current->values() == std::vector<int>({8, 9}));
    // Aliasing ownership: expose a member while retaining its enclosing owner.
    std::shared_ptr<const std::vector<int>> member(current, &current->values());
    const std::weak_ptr<const Snapshot> current_observer = current;
    current.reset();
    CHECK(!current_observer.expired());
    CHECK(member->size() == 2 && member->front() == 8);
    member.reset();
    CHECK(current_observer.expired());
    course::report();
}
