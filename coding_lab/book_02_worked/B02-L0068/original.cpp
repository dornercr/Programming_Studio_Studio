#include "course_test.hpp"
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::deque<int> jobs{2, 3};
    jobs.push_front(1); jobs.push_back(4);
    CHECK(jobs.front() == 1 && jobs.back() == 4);
    std::list<int> first{1, 2}, second{3, 4};
    auto stable = second.begin();
    first.splice(first.end(), second);
    CHECK(second.empty() && *stable == 3);
    CHECK(first.size() == 4);
    std::map<std::string, int> ordered{{"beta", 2}, {"alpha", 1}};
    std::unordered_map<std::string, int> hashed(ordered.begin(), ordered.end());
    CHECK(ordered.begin()->first == "alpha");
    CHECK(hashed.at("alpha") == ordered.at("alpha"));
    CHECK(!hashed.contains("missing"));
    const auto old_size = hashed.size();
    CHECK(hashed.find("missing") == hashed.end());
    CHECK(hashed.size() == old_size);
    const std::set<int> unique{3, 1, 3, 2};
    CHECK(unique.size() == 3);
    course::report();
}
