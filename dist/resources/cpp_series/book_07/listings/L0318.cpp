#pragma once
#include "harbor/policies.hpp"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
namespace harbor {
// Stable, non-cryptographic teaching hash; never use for credentials/signatures.
inline std::uint64_t stable_hash(std::string_view value) {
    std::uint64_t h = 14695981039346656037ULL;
    for (unsigned char c : value) { h ^= c; h *= 1099511628211ULL; }
    return h;
}
inline std::string placement(std::string_view key, const std::vector<std::string>& nodes) {
    if (nodes.empty()) throw std::invalid_argument("empty membership");
    std::string chosen; std::uint64_t best=0; bool first=true;
    for (const auto& node : nodes) {
        auto score = stable_hash(std::string(key)+"\x1f"+node);
        if (first || score>best || (score==best && node<chosen)) {
            first=false; best=score; chosen=node;
        }
    }
    return chosen;
}
class LamportClock {
    std::uint64_t value_ = 0;
public:
    std::uint64_t tick() {
        if (value_ == UINT64_MAX) throw std::overflow_error("logical clock");
        return ++value_;
    }
    std::uint64_t receive(std::uint64_t incoming) { value_=std::max(value_, incoming); return tick(); }
    auto value() const { return value_; }
};
struct CacheEntry { std::string value; std::uint64_t version; Tick expires; };
class VersionedCache {
    std::map<std::string, CacheEntry> entries_;
    std::map<std::string, std::uint64_t> floors_;
public:
    bool put(std::string key, std::string value, std::uint64_t version, Tick expires) {
        auto floor = floors_.find(key);
        if (floor != floors_.end() && version < floor->second) return false;
        auto old = entries_.find(key);
        if (old != entries_.end() && version < old->second.version) return false;
        entries_.insert_or_assign(std::move(key), CacheEntry{std::move(value),version,expires});
        return true;
    }
    void invalidate(const std::string& key, std::uint64_t version) {
        floors_[key]=std::max(floors_[key], version);
        auto old=entries_.find(key);
        if (old != entries_.end() && old->second.version<version) entries_.erase(old);
    }
    std::optional<std::string> get(const std::string& key, Tick now) const {
        auto it=entries_.find(key);
        if (it == entries_.end() || now >= it->second.expires) return {};
        return it->second.value;
    }
};
class FencedRegister {
    std::uint64_t epoch_ = 0;
    std::string value_;
public:
    bool write(std::uint64_t token, std::string value) {
        if (token<epoch_) return false;
        epoch_=token; value_=std::move(value); return true;
    }
    auto epoch() const { return epoch_; }
    const auto& value() const { return value_; }
};
struct Discovery {
    std::uint64_t generation=0;
    std::vector<std::string> endpoints;
    bool update(std::uint64_t g, std::vector<std::string> next) {
        if (g<=generation) return false;
        std::sort(next.begin(),next.end());
        next.erase(std::unique(next.begin(),next.end()),next.end());
        generation=g; endpoints=std::move(next); return true;
    }
};
class Saga {
public:
    enum class State { created, reserved, charged, completed, compensating, cancelled };
private:
    State state_=State::created;
    std::map<std::string,std::string> seen_;
public:
    bool event(const std::string& id, std::string_view kind) {
        if (auto old=seen_.find(id); old!=seen_.end()) {
            if (old->second!=kind) throw std::logic_error("event identity conflict");
            return false;
        }
        State next=state_;
        if (kind=="reserve" && state_==State::created) next=State::reserved;
        else if (kind=="charge" && state_==State::reserved) next=State::charged;
        else if (kind=="finish" && state_==State::charged) next=State::completed;
        else if (kind=="cancel" && (state_==State::reserved || state_==State::charged)) next=State::compensating;
        else if (kind=="compensated" && state_==State::compensating) next=State::cancelled;
        else throw std::logic_error("invalid saga transition");
        seen_.emplace(id,kind); state_=next; return true;
    }
    State state() const { return state_; }
};
}
