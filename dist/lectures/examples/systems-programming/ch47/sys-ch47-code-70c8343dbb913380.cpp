#include <iostream>
#include <mutex>
#include <string>
#include <utility>
class Document {
    mutable std::mutex mutex_;
    std::string text_ = "draft";
    unsigned revision_ = 1;
public:
    void replace(std::string next) {
        std::lock_guard lock(mutex_);
        text_.swap(next); // Prepared storage is committed without allocation.
        ++revision_; // This bounded demonstration performs one update.
    }
    std::pair<unsigned,std::string> snapshot() const {
        std::lock_guard lock(mutex_);
        return {revision_, text_};
    }
};
int main() {
    Document document;
    const auto before = document.snapshot();
    document.replace("ready");
    const auto after = document.snapshot();
    std::cout << before.first << ':' << before.second << '\n';
    std::cout << after.first << ':' << after.second << '\n';
}
