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
        return "stored " +
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

// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  return dpx_rejects<std::invalid_argument>([]{
    TextJob{
    }.run("");
  });
})() << '\n';
}
