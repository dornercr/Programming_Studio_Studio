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
        const Key key{name, size};
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
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  StylePool p;
  auto a=p.get("dot",8);
  p.clear();
  return a==p.get("dot",8);
})() << '\n';
}
