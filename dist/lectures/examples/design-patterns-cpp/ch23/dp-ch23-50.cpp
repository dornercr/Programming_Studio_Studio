#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

struct RawSample {
    int id;
    double value;
    char unit;
};

struct Sample {
    int id;
    double celsius;
};

class SampleAdapter {
public:
    static Sample convert(const RawSample& raw) {
        if (raw.id <= 0 || !std::isfinite(raw.value)) {
            throw std::invalid_argument("invalid sample");
        }
        // Normalize before judging a value.
        double c = raw.value;
        if (raw.unit == 'F') {
            c = (c - 32.0) * 5.0 / 9.0;
        } else if (raw.unit != 'C') {
            throw std::invalid_argument(
                "unknown unit");
        }
        if (!std::isfinite(c) || c < -40.0 || c > 125.0) {
            throw std::invalid_argument("outside sensor range");
        }
        return {raw.id, c};
    }
};

class AlertRule {
public:
    // Destruction follows the actual type.
    virtual ~AlertRule() = default;
    // Implementations cannot throw here.
    virtual bool alert(
        const Sample& s) const noexcept = 0;
};

class AtLeast final : public AlertRule {
    double limit_;
public:
    explicit AtLeast(double limit) : limit_(limit) {
        if (!std::isfinite(limit) || limit < -40.0 || limit > 125.0) {
            throw std::invalid_argument("invalid limit");
        }
    }
    bool alert(const Sample& s) const noexcept override {
        return s.celsius >= limit_;
    }
};

enum class State { idle, running, paused, stopped };

struct Record {
    Sample sample;
    bool alert;
};

class Monitor {
    // Own the rule and the record values.
    std::unique_ptr<AlertRule> rule_;
    std::vector<Record> records_;
    State state_ = State::idle;
    static constexpr std::size_t cap = 8;
public:
    // Moving handles is safe; moving this object is forbidden.
    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;
    Monitor(Monitor&&) = delete;
    Monitor& operator=(Monitor&&) = delete;
    explicit Monitor(std::unique_ptr<AlertRule> rule)
        : rule_(std::move(rule)) {
        if (!rule_) {
            throw std::invalid_argument("missing rule");
        }
    }
    void start() {
        // Check before changing state.
        if (state_ != State::idle) {
            throw std::logic_error(
                "start needs idle");
        }
        state_ = State::running;
    }
    void pause() {
        if (state_ != State::running) {
            throw std::logic_error("pause needs running");
        }
        state_ = State::paused;
    }
    void resume() {
        if (state_ != State::paused) {
            throw std::logic_error("resume needs paused");
        }
        state_ = State::running;
    }
    void stop() {
        if (state_ != State::running && state_ != State::paused) {
            throw std::logic_error("stop needs active session");
        }
        state_ = State::stopped;
    }
    void replace_rule(std::unique_ptr<AlertRule> rule) {
        if (state_ != State::idle && state_ != State::paused) {
            throw std::logic_error("replace needs idle or paused");
        }
        if (!rule) {
            throw std::invalid_argument("missing rule");
        }
        rule_ = std::move(rule);
    }
    void ingest(const std::vector<RawSample>& batch) {
        if (state_ != State::running) {
            throw std::logic_error("ingest needs running");
        }
        if (batch.size() > cap - records_.size()) {
            throw std::length_error("record capacity reached");
        }
        // Propose a new history privately.
        auto staged = records_;
        for (const auto& raw : batch) {
            const auto s =
                SampleAdapter::convert(raw);
            if (!staged.empty() &&
                s.id <=
                staged.back().sample.id) {
                throw std::invalid_argument(
                    "ids must increase");
            }
            // Keep the intake decision.
            const bool hot = rule_->alert(s);
            staged.push_back({s, hot});
        }
        // Commit after all items pass.
        records_.swap(staged);
    }
    State state() const noexcept { return state_; }
    const std::vector<Record>& records() const noexcept {
        return records_;
    }
};

static_assert(!std::is_copy_constructible_v<Monitor> &&
              !std::is_copy_assignable_v<Monitor>, "monitor cannot copy");
static_assert(!std::is_move_constructible_v<Monitor> &&
              !std::is_move_assignable_v<Monitor>, "monitor cannot move");

class ReportFacade {
public:
    static std::string summary(const Monitor& monitor) {
        std::ostringstream out;
        out.imbue(std::locale::classic());
        out << std::fixed << std::setprecision(1);
        std::size_t alerts = 0;
        // Read through a temporary borrow.
        for (const auto& r :
             monitor.records()) {
            alerts += r.alert ? 1U : 0U;
            out << r.sample.id << ": "
                << r.sample.celsius << " C "
                << (r.alert ? "ALERT" : "ok")
                << '\n';
        }
        out << "records=" << monitor.records().size()
            << " alerts=" << alerts << '\n';
        return out.str();
    }
};

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void rejects(const std::function<void()>& action) {
    bool rejected = false;
    try {
        action();
    } catch (const std::logic_error&) {
        rejected = true;
    }
    check(rejected, "expected rejection");
}

class LifetimeRule final : public AlertRule {
    bool& destroyed_;
public:
    explicit LifetimeRule(bool& destroyed) : destroyed_(destroyed) {}
    ~LifetimeRule() override { destroyed_ = true; }
    bool alert(const Sample&) const noexcept override { return false; }
};

void run_checks() {
    rejects([] { Monitor absent(nullptr); });
    rejects([] { AtLeast invalid(126.0); });
    Monitor m(std::make_unique<AtLeast>(80.0));
    rejects([&] { m.ingest({{1, 20.0, 'C'}}); });
    rejects([&] { m.resume(); });
    rejects([&] { m.stop(); });
    check(m.state() == State::idle, "invalid transition changed state");
    m.start();
    rejects([&] { m.start(); });
    m.ingest({});
    check(m.records().empty(), "empty batch changed records");
    m.ingest({{1, -40.0, 'C'}, {2, 80.0, 'C'}, {3, 212.0, 'F'}});
    check(!m.records()[0].alert && m.records()[1].alert,
          "threshold boundary wrong");
    check(m.records()[2].sample.celsius == 100.0, "unit conversion wrong");
    const auto before = ReportFacade::summary(m);
    rejects([&] { m.ingest({{4, 21.0, 'C'}, {5, 22.0, 'K'}}); });
    check(ReportFacade::summary(m) == before, "partial commit");
    rejects([&] { m.ingest({{4, 21.0, 'C'}, {4, 22.0, 'C'}}); });
    check(ReportFacade::summary(m) == before, "duplicate partly committed");
    rejects([&] { m.ingest({{3, 22.0, 'C'}}); });
    rejects([&] { m.ingest({{0, 22.0, 'C'}}); });
    rejects([&] { m.ingest({{4, std::nan(""), 'C'}}); });
    rejects([&] { m.ingest({{4, 126.0, 'C'}}); });
    rejects([&] { m.replace_rule(std::make_unique<AtLeast>(90.0)); });
    m.pause();
    rejects([&] { m.ingest({{4, 10.0, 'C'}}); });
    rejects([&] { m.pause(); });
    rejects([&] { m.replace_rule(nullptr); });
    m.replace_rule(std::make_unique<AtLeast>(110.0));
    m.resume();
    m.ingest({{4, 100.0, 'C'}, {5, 125.0, 'C'}});
    check(m.records()[2].alert && !m.records()[3].alert,
          "replacement changed history or ignored new rule");
    m.ingest({{6, 0.0, 'C'}, {7, 1.0, 'C'}, {8, 2.0, 'C'}});
    rejects([&] { m.ingest({{9, 3.0, 'C'}}); });
    check(m.records().size() == 8, "capacity rejection changed size");
    m.stop();
    rejects([&] { m.resume(); });
    rejects([&] { m.start(); });
    rejects([&] { m.ingest({}); });
    check(m.state() == State::stopped, "stop was not final");
    Monitor paused(std::make_unique<AtLeast>(80.0));
    paused.start();
    paused.pause();
    paused.stop();
    check(paused.state() == State::stopped, "paused stop failed");
    bool destroyed = false;
    {
        Monitor owned(std::make_unique<LifetimeRule>(destroyed));
        check(!destroyed, "rule destroyed too early");
    }
    check(destroyed, "owned rule was not destroyed");
    bool replaced = false;
    Monitor owner(std::make_unique<LifetimeRule>(replaced));
    owner.replace_rule(std::make_unique<AtLeast>(80.0));
    check(replaced, "old rule survived replacement");
}

class OutsideBand final : public AlertRule {
    double low_;
    double high_;
public:
    OutsideBand(double low, double high) : low_(low), high_(high) {
        if (!std::isfinite(low) || !std::isfinite(high) ||
            low < -40.0 || high > 125.0 || low > high) {
            throw std::invalid_argument("invalid band");
        }
    }
    bool alert(const Sample& s) const noexcept override {
        return s.celsius < low_ || s.celsius > high_;
    }
};

void check_extension() {
    rejects([] { OutsideBand bad(60.0, 20.0); });
    rejects([] { OutsideBand bad(-41.0, 20.0); });
    rejects([] { OutsideBand bad(0.0, 126.0); });
    rejects([] { OutsideBand bad(std::nan(""), 20.0); });
    Monitor extended(std::make_unique<OutsideBand>(20.0, 60.0));
    extended.start();
    extended.ingest({{1, 19.0, 'C'}, {2, 20.0, 'C'},
                     {3, 60.0, 'C'}, {4, 61.0, 'C'}});
    const auto& rows = extended.records();
    check(rows[0].alert && !rows[1].alert &&
          !rows[2].alert && rows[3].alert, "band boundaries wrong");
    const auto before = ReportFacade::summary(extended);
    rejects([&] { extended.ingest({{5, 21.0, 'C'}, {6, 9.0, 'K'}}); });
    check(ReportFacade::summary(extended) == before, "band rollback broke");
    extended.pause();
    extended.replace_rule(std::make_unique<AtLeast>(100.0));
    extended.resume();
    extended.ingest({{5, 61.0, 'C'}});
    check(rows[3].alert && !rows[4].alert, "old band history changed");
    // rows refers to the vector object, not one of its elements.
    // Do not retain an element reference across ingest().
    extended.stop();
    OutsideBand single(20.0, 20.0);
    check(!single.alert({1, 20.0}) && single.alert({2, 20.1}),
          "single-point band wrong");
}

// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  Monitor m(std::make_unique<AtLeast>(80));
  m.start();
  m.ingest({
    {
      1,20,'C'
    }
  });
  const bool bad=dpx_rejects<std::invalid_argument>([&]{
    m.ingest({
      {
        2,30,'C'
      },{
        3,40,'K'
      }
    });
  });
  return bad && m.records().size()==1;
})() << '\n';
}
