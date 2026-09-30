#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
struct Style {
    std::string name;
    int size;
};
using StylePtr = std::shared_ptr<const Style>;
using Key = std::pair<std::string, int>;
class StylePool {
    std::map<Key, StylePtr> styles_;
public:
    StylePtr get(const std::string& name, int size) {
        if (name.empty() || size < 1 || size > 100) {
            throw std::invalid_argument("style");
        }
        // The key covers all shared fields.
        const Key key{name, 1};
        const auto found = styles_.find(key);
        if (found != styles_.end()) {
            return found->second;
        } // End reuse path.
        auto style = std::make_shared<const Style>(Style{name, size});
        styles_.emplace(key, style);
        return style;
    }
    void clear() { styles_.clear(); }
    std::size_t count() const { return styles_.size(); }
};
class Mark {
    StylePtr style_;
    int x_;
public:
    Mark(StylePtr style, int x) : style_(std::move(style)), x_(x) {
        if (!style_) throw std::invalid_argument("style");
    }
    // Copying shares style and preserves both usable values.
    Mark(const Mark&) = default;
    Mark& operator=(const Mark&) = default;
    std::string draw() const {
        // Position belongs to this use.
        const auto& name = style_->name;
        const auto size = style_->size;
        return name + "/"
             + std::to_string(size)
             + "@" + std::to_string(x_);
    } // End drawing.
};
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    StylePool pool;
    const auto first = pool.get("dot", 12);
    const auto same = pool.get("dot", 12);
    const auto large = pool.get("dot", 20);
    check(first == same && first != large);
    check(pool.count() == 2);
    Mark left(first, 3);
    Mark right(same, 8);
    check(left.draw() == "dot/12@3");
    check(right.draw() == "dot/12@8");
    Mark copied(std::move(left));
    check(copied.draw() == left.draw());
    check(pool.get("dot", 1)->size == 1);
    check(pool.get("dot", 100)->size == 100);
    bool rejected = false;
    try { static_cast<void>(pool.get("dot", 0)); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    StylePtr survivor;
    {
        StylePool short_lived;
        survivor = short_lived.get("dash", 10);
    }
    check(survivor->name == "dash");
    bool missing = false;
    try { Mark bad(nullptr, 0); }
    catch (const std::invalid_argument&) { missing = true; }
    check(missing);
    pool.clear();
    check(pool.count() == 0);
    check(left.draw() == "dot/12@3");
    const auto replacement = pool.get("dot", 12);
    check(replacement != first);
    check(replacement->name == first->name);
    check(replacement->size == first->size);
    check(pool.get("dot", 12) == replacement);
    check(pool.count() == 1);
    std::cout << "pool reset; old marks remain valid\n";
    std::cout << left.draw() << '\n' << right.draw() << '\n';
    std::cout << "shared=" << (first == same) << '\n';
    std::cout << "lifetime and rejected-input checks passed\n";
}
