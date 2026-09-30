#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class Record {
public:
    virtual ~Record() = default;
    virtual std::string format(const std::string& text) const = 0;
};
class TextRecord final : public Record {
public:
    std::string format(const std::string& text) const override {
        return "text:" + text;
    }
};
class BracketRecord final : public Record {
public:
    std::string format(const std::string& text) const override {
        return "[" + text + "]";
    }
};
class ArchiveJob {
protected:
    virtual std::unique_ptr<Record> make() const = 0;
public:
    virtual ~ArchiveJob() = default;
    std::string run(const std::string& text) const {
        if (text.empty() || text.size() > 20)
            throw std::invalid_argument("text");
        // Choose the concrete product here.
        auto record = make();
        if (!record)
            throw std::logic_error(
                "null record");
        return "" +
            record->format(text);
    }
};
class TextJob final : public ArchiveJob {
    // Return an owning interface handle.
    std::unique_ptr<Record>
    make() const override {
        return
            std::make_unique<TextRecord>();
    }
};
class BracketJob final : public ArchiveJob {
    std::unique_ptr<Record>
    make() const override {
        return std::make_unique<BracketRecord>();
    }
};
class EmptyJob final : public ArchiveJob {
    std::unique_ptr<Record>
    make() const override { return nullptr; }
};

class LengthRecord final : public Record {
public:
    std::string format(const std::string& text) const override {
        return std::to_string(text.size()) + ":" + text;
    }
};
class LengthJob final : public ArchiveJob {
    std::unique_ptr<Record> make() const override {
        return std::make_unique<LengthRecord>();
    }
};

// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    TextJob text;
    BracketJob bracket;
    const ArchiveJob& selected = bracket;
    check(text.run("sample") == "stored text:sample");
    check(selected.run("sample") == "stored [sample]");
    std::cout << text.run("sample") << '\n';
    std::cout << selected.run("sample") << '\n';
    check(text.run(std::string(20, 'x')).size() == 32);
    bool rejected = false;
    try { text.run(""); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { text.run(std::string(21, 'x')); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    EmptyJob empty;
    rejected = false;
    try { empty.run("sample"); }
    catch (const std::logic_error&) { rejected = true; }
    check(rejected);
    std::cout << "invalid text and null product rejected\n";
    LengthJob length;
    const ArchiveJob& job = length;
    check(job.run("sample") == "stored 6:sample");
    check(job.run("x") == "stored 1:x");
    check(job.run(std::string(20, 'x')) ==
          "stored 20:" + std::string(20, 'x'));
    rejected = false;
    try { job.run(""); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    std::cout << job.run("sample") << '\n';
}
