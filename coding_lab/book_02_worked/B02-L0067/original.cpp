#include <iostream>
#include <map>
#include <set>
#include <string>
int main(){ std::map<std::string,int> score{{"Ada",10},{"Grace",12}}; std::set<std::string> active{"Ada"}; std::cout<<score.at("Grace")<<' '<<std::boolalpha<<active.contains("Ada")<<'\n'; }
