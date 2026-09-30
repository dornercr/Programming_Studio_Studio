#include <cassert>
#include <vector>
int sum(const std::vector<int>& v){ int total=0; for(std::size_t i=0;i<v.size();++i){ total+=v[i]; /* invariant: total equals sum of v[0..i] */ } return total; }
int main(){ assert(sum({})==0); assert(sum({1,2,3})==6); }
