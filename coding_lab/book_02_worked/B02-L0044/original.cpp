#include "course_test.hpp"
#include <string>
#include <vector>
#include <utility>

struct Sink {
    virtual ~Sink() = default;
    virtual void write(const std::string& text) = 0;
};
class MemorySink final : public Sink {
public:
    std::vector<std::string> messages;
    void write(const std::string& text) override { messages.push_back(text); }
};
class Formatter {
    std::string prefix_;
public:
    explicit Formatter(std::string prefix) : prefix_(std::move(prefix)) {}
    std::string format(int value) const { return prefix_ + std::to_string(value); }
};
class Reporter {
    Formatter formatter_; // Owned value.
    Sink& sink_;           // Borrowed; the caller must outlive this reporter.
public:
    Reporter(Formatter formatter, Sink& sink)
        : formatter_(std::move(formatter)), sink_(sink) {}
    void sample(int value) { sink_.write(formatter_.format(value)); }
};

int main() {
    MemorySink sink;
    Reporter reporter{Formatter{"sample="}, sink};
    reporter.sample(12);
    reporter.sample(14);
    CHECK(sink.messages == std::vector<std::string>({"sample=12", "sample=14"}));
    course::report();
}
