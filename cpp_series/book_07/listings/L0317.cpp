#pragma once
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <limits>
#include <optional>
#include <random>
#include <stdexcept>
namespace harbor {
using Tick = std::uint64_t; // local monotonic milliseconds in the deterministic models
inline Tick saturating_add(Tick a, Tick b) {
    return b > std::numeric_limits<Tick>::max()-a ?
        std::numeric_limits<Tick>::max() : a+b;
}
struct Budget {
    Tick end;
    Tick remaining(Tick now) const { return now < end ? end-now : 0; }
    bool expired(Tick now) const { return now >= end; }
};
struct RetryPolicy {
    unsigned max_attempts = 4; // includes initial attempt
    Tick initial = 10, cap = 1000;
    Tick ceiling(unsigned retry_index) const {
        Tick d = std::min(initial, cap);
        for (unsigned i=0; i<retry_index && d<cap; ++i)
            d = d > cap/2 ? cap : std::min(cap, d*2);
        return d;
    }
    template<class Engine> Tick delay(unsigned retry_index, Engine& rng) const {
        return std::uniform_int_distribution<Tick>(0, ceiling(retry_index))(rng);
    }
};
// Single-threaded policy model. Serialize access in a real client.
class Breaker {
public:
    enum class State { closed, open, half_open };
private:
    State state_ = State::closed;
    unsigned failures_ = 0, limit_;
    Tick reopen_at_ = 0, pause_;
    bool probe_ = false;
    std::uint64_t epoch_ = 0;
public:
    Breaker(unsigned limit, Tick pause): limit_(limit), pause_(pause) {
        if (!limit) throw std::invalid_argument("zero threshold");
    }
    std::optional<std::uint64_t> acquire(Tick now) {
        if (state_ == State::open) {
            if (now < reopen_at_) return {};
            state_ = State::half_open; probe_ = false;
        }
        if (state_ == State::half_open) {
            if (probe_) return {};
            probe_ = true;
        }
        return epoch_;
    }
    void complete(std::uint64_t ticket, bool success, Tick now) {
        if (ticket != epoch_) return; // stale completion from an older generation
        if (state_ == State::half_open) {
            probe_ = false;
            ++epoch_;
            if (success) { state_ = State::closed; failures_ = 0; }
            else { state_ = State::open; reopen_at_ = saturating_add(now, pause_); }
        } else if (state_ == State::closed) {
            if (success) failures_ = 0;
            else if (++failures_ >= limit_) {
                state_ = State::open; reopen_at_ = saturating_add(now, pause_); ++epoch_;
            }
        }
    }
    State state() const { return state_; }
};
class TokenBucket {
    double tokens_, capacity_, per_ms_;
    Tick last_;
public:
    TokenBucket(double capacity, double per_second, Tick now=0)
      : tokens_(capacity), capacity_(capacity), per_ms_(per_second/1000.0), last_(now) {
        if (!(capacity>0) || !(per_second>=0) || !std::isfinite(capacity) || !std::isfinite(per_second)) throw std::invalid_argument("bucket config");
    }
    bool admit(double cost, Tick now) {
        if (!(cost>0) || now<last_) throw std::invalid_argument("cost or clock");
        tokens_ = std::min(capacity_, tokens_+static_cast<double>(now-last_)*per_ms_);
        last_ = now;
        if (cost>tokens_) return false;
        tokens_ -= cost; return true;
    }
};
}
