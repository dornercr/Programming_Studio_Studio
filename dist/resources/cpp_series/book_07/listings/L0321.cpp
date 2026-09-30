#pragma once
#include "harbor/domain.hpp"
#include <sqlite3.h>
#include <optional>
#include <string>
#include <vector>
namespace harbor {
class Store {
    sqlite3* db_=nullptr;
    void exec(const char* sql);
public:
    enum class Submit { accepted, duplicate, conflict, full };
    struct Job { Request request; std::optional<std::int64_t> result; };
    explicit Store(const std::string& path);
    ~Store();
    Store(const Store&)=delete;
    Store& operator=(const Store&)=delete;
    Submit submit(const Request&, std::size_t max_pending=128);
    std::optional<Job> lookup(const std::string& key);
    std::vector<Request> pending(std::size_t limit=16);
    std::int64_t apply(const Request&); // worker inbox and effect in one transaction
    void complete(const Request&, std::int64_t result);
    std::size_t receipt_count();
};
}
