#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <string_view>
#include <sstream>
#include <charconv>
#include <system_error>
#include <cstddef>

struct Row{int id,quantity;};class Report{std::vector<Row> rows_;public:bool load(std::istream& in){std::string line;while(std::getline(in,line)){auto p=line.find('|');if(p==std::string::npos)return false;int id=std::stoi(line.substr(0,p)),qty=std::stoi(line.substr(p+1));rows_.push_back({id,qty});}return true;}std::size_t size()const{return rows_.size();}long long total()const{long long n=0;for(const auto& r:rows_)n+=r.quantity;return n;}};
