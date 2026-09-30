#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Record{int id;std::string name;int quantity;};
class Inventory{std::vector<Record> records_;public:bool add(Record r){if(r.id<=0||r.name.empty()||r.quantity<0||r.quantity>1000||records_.size()>=100)return false;for(const auto& item:records_)if(item.id==r.id)return false;records_.push_back(std::move(r));return true;}long long total()const{long long n=0;for(const auto& r:records_)n+=r.quantity;return n;}const std::vector<Record>& records()const{return records_;}};
