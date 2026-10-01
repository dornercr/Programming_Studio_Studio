#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <string>
#include <utility>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
class Message {
public:
    Message() = default;
    Message(const Message&) = delete;
    Message& operator=(const Message&) = delete;
    Message(Message&&) = delete;
    Message& operator=(Message&&) = delete;
    virtual ~Message() = default;
    virtual std::string text() const = 0;
};
class Plain final : public Message {
    std::string value_;
public:
    explicit Plain(std::string s) : value_(std::move(s)) {}
    std::string text() const override { return value_; }
};
class Prefix final : public Message {
    std::unique_ptr<Message> inner_;
    std::string prefix_;
public:
    Prefix(std::unique_ptr<Message> inner, std::string prefix)
        : inner_(std::move(inner)), prefix_(std::move(prefix)) {
        if (!inner_) throw std::invalid_argument("inner");
    }
    std::string text() const override {
        // Ask inward before adding our text.
        const auto body = inner_->text();
        const auto result = prefix_ + body;
        return result;
    } // End prefix rendering.
};
class Limit final : public Message {
    std::unique_ptr<Message> inner_;
    std::size_t limit_;
public:
    Limit(std::unique_ptr<Message> inner, std::size_t n)
        : inner_(std::move(inner)), limit_(n) {
        if (!inner_) throw std::invalid_argument("inner");
    }
    std::string text() const override {
        // Limit the entire inner result.
        const auto body = inner_->text();
        auto cut = body.substr(0, limit_);
        return cut;
    } // End limit rendering.
};
class Suffix final : public Message {
    std::unique_ptr<Message> inner_;
    std::string suffix_;
public:
    Suffix(std::unique_ptr<Message> inner, std::string suffix)
        : inner_(std::move(inner)), suffix_(std::move(suffix)) {
        if (!inner_) throw std::invalid_argument("inner");
    }
    std::string text() const override { return inner_->text() + suffix_; }
};
// Move owning handles, not service objects.
static_assert(!std::is_move_constructible_v<Prefix>);
static_assert(!std::is_copy_constructible_v<Prefix>);
static_assert(!std::is_move_constructible_v<Limit>);
static_assert(!std::is_copy_constructible_v<Limit>);
static_assert(!std::is_move_constructible_v<Suffix>);
static_assert(!std::is_copy_constructible_v<Suffix>);
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  return Prefix(std::make_unique<Limit>(std::make_unique<Plain>("ready"),3),"!").text();
})() << '\n';
}
