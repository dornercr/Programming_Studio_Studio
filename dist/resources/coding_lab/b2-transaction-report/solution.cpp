#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <string_view>
#include <sstream>
#include <charconv>
#include <system_error>
#include <cstddef>

struct Row{int id,quantity;};class Report{std::vector<Row> rows_;public:bool load(std::istream& in){std::vector<Row> next;std::set<int> ids;std::string line;while(std::getline(in,line)){auto p=line.find('|');if(p==std::string::npos||line.find('|',p+1)!=std::string::npos)return false;int id=0,qty=0;auto left=std::string_view(line).substr(0,p),right=std::string_view(line).substr(p+1);if(left.empty()||right.empty())return false;auto [a,ae]=std::from_chars(left.data(),left.data()+left.size(),id);auto [b,be]=std::from_chars(right.data(),right.data()+right.size(),qty);if(ae!=std::errc{}||be!=std::errc{}||a!=left.data()+left.size()||b!=right.data()+right.size()||id<=0||qty<0||qty>1000||next.size()>=100||!ids.insert(id).second)return false;next.push_back({id,qty});}if(in.bad())return false;rows_.swap(next);return true;}std::size_t size()const{return rows_.size();}long long total()const{long long n=0;for(const auto& r:rows_)n+=r.quantity;return n;}};
