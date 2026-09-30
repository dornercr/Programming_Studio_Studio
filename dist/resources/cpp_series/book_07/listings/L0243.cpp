#include "harbor/check.hpp"
#include "harbor/metrics.hpp"
#include <iostream>
int main() {
    harbor::Histogram metric;
    metric.observe(0.003); metric.observe(0.05); metric.observe(2.0);
    auto text=metric.text();
    harbor::check(text.find("harbor_request_seconds_count 3")!=std::string::npos,"count");
    harbor::check(text.find("{le=\"+Inf\"} 3")!=std::string::npos,"cumulative terminal bucket");
    std::cout<<text;
}
