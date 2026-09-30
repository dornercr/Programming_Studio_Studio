// Shared test support from Book II, B02-L0110.
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

#include <charconv>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

template<class T> class Stack {
    std::vector<T> data_;
public:
    bool empty() const noexcept { return data_.empty(); }
    std::size_t size() const noexcept { return data_.size(); }
    void push(T value) { data_.push_back(std::move(value)); }
    std::optional<T> pop() {
        if (data_.empty()) return std::nullopt;
        T result = std::move(data_.back());
        data_.pop_back();
        return result;
    }
    const T& top() const {
        if (data_.empty()) throw std::out_of_range("empty stack");
        return data_.back();
    }
};
bool balanced(std::string_view text) {
    Stack<char> openings;
    for (char c : text) {
        if (c == '(' || c == '[') openings.push(c);
        else if (c == ')' || c == ']') {
            const auto open = openings.pop();
            if (!open || (*open == '(' && c != ')') || (*open == '[' && c != ']')) return false;
        }
    }
    return openings.empty();
}
std::optional<int> postfix(const std::vector<std::string>& tokens) {
    Stack<int> values;
    for (const std::string& token : tokens) {
        if (token == "+" || token == "-" || token == "*") {
            const auto right = values.pop(), left = values.pop();
            if (!left || !right) return std::nullopt;
            long long value{};
            if (token == "+") value = static_cast<long long>(*left) + *right;
            if (token == "-") value = static_cast<long long>(*left) - *right;
            if (token == "*") value = static_cast<long long>(*left) * *right;
            if (value < -1000000 || value > 1000000) return std::nullopt;
            values.push(static_cast<int>(value));
        } else {
            if (token.empty()) return std::nullopt;
            int value{};
            const auto p = std::from_chars(token.data(), token.data() + token.size(), value);
            if (p.ec != std::errc{} || p.ptr != token.data() + token.size() ||
                value < -1000000 || value > 1000000) return std::nullopt;
            values.push(value);
        }
    }
    if (values.size() != 1) return std::nullopt;
    return values.pop();
}

int main() {
    CHECK(balanced("([])[]") && balanced(""));
    CHECK(!balanced("([)]") && !balanced("("));
    CHECK(postfix({"2", "3", "+", "4", "*"}) == 20);
    CHECK(postfix({"9", "2", "-"}) == 7);
    CHECK(!postfix({"+"}) && !postfix({"2", "3"}));
    CHECK(!postfix({"1000000", "1000000", "*"}));
    Stack<int> stack;
    CHECK(!stack.pop());
    CHECK(course::throws<std::out_of_range>([&] { (void)stack.top(); }));
    course::report();
}
