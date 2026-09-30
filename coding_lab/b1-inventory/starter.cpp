#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Record{int id;std::string name;int quantity;};
class Inventory{std::vector<Record> records_;public:bool add(Record r){records_.push_back(std::move(r));return true;}long long total()const{long long n=0;for(const auto& r:records_)n+=r.quantity;return n;}const std::vector<Record>& records()const{return records_;}};
