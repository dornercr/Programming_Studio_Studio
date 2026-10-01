#include <iostream>
#include <map>
#include <memory>
#include <string>
struct Style { std::string name; int size; };
using Owner = std::shared_ptr<const Style>;
class BadPool {
    std::map<std::string, Owner> styles_;
public:
    Owner get(const std::string& name, int size) {
        // Intentional bug: size is absent from the lookup key.
        if (const auto found = styles_.find(name); found != styles_.end())
            return found->second;
        auto created = std::make_shared<const Style>(Style{name, size});
        styles_.emplace(name, created);
        return created;
    }
};
int main() {
    BadPool pool;
    const auto small = pool.get("stamp", 12);
    const auto large = pool.get("stamp", 20);
    std::cout << "requested=12 actual=" << small->size << '\n';
    std::cout << "requested=20 actual=" << large->size << '\n';
    std::cout << "same object=" << std::boolalpha << (small == large) << '\n';
}
