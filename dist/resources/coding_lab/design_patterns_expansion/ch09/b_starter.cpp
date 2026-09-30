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
    std::string text() const override { return suffix_ + inner_->text(); }
};
// Move owning handles, not service objects.
static_assert(!std::is_move_constructible_v<Prefix>);
static_assert(!std::is_copy_constructible_v<Prefix>);
static_assert(!std::is_move_constructible_v<Limit>);
static_assert(!std::is_copy_constructible_v<Limit>);
static_assert(!std::is_move_constructible_v<Suffix>);
static_assert(!std::is_copy_constructible_v<Suffix>);
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    auto first = std::make_unique<Prefix>(
        std::make_unique<Limit>(std::make_unique<Plain>("ready now"), 5),
        "WARN ");
    auto second = std::make_unique<Limit>(
        std::make_unique<Prefix>(std::make_unique<Plain>("ready now"),
                                 "WARN "), 5);
    check(first->text() == "WARN ready");
    check(second->text() == "WARN ");
    check(Plain("").text().empty());
    Limit zero(std::make_unique<Plain>("ready"), 0);
    check(zero.text().empty());
    Limit wide(std::make_unique<Plain>("ready"), 99);
    check(wide.text() == "ready");
    bool rejected = false;
    try { Prefix bad(nullptr, "WARN "); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    Suffix tail(std::make_unique<Plain>("ready"), "!");
    check(tail.text() == "ready!");
    Limit cut(std::make_unique<Suffix>(
        std::make_unique<Plain>("ready"), "!"), 5);
    check(cut.text() == "ready");
    Suffix after(std::make_unique<Limit>(
        std::make_unique<Plain>("ready now"), 5), "!");
    check(after.text() == "ready!");
    Suffix blank(std::make_unique<Plain>("ready"), "");
    check(blank.text() == "ready");
    bool missing = false;
    try { Suffix bad(nullptr, "!"); }
    catch (const std::invalid_argument&) { missing = true; }
    check(missing);
    std::cout << "suffix=" << tail.text() << '\n';
    std::cout << "prefix outside: '" << first->text() << "'\n";
    std::cout << "limit outside: '" << second->text() << "'\n";
    std::cout << "empty and missing-inner checks passed\n";
}
