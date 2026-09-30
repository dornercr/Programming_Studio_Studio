#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
struct Key{int id;std::string region;};
std::size_t hash_key(const Key&k){auto h1=std::hash<int>{}(k.id);auto h2=std::hash<std::string>{}(k.region);return h1^(h2+0x9e3779b9+(h1<<6)+(h1>>2));}
int main(){std::cout<<std::boolalpha<<(hash_key({7,"east"})==hash_key({7,"east"}))<<'\n';}
