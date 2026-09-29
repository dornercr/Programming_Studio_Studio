#include <iostream>
#include <string>
void gen(std::string&cur,int n){if((int)cur.size()==n){std::cout<<cur<<'\n';return;}for(char c:{'0','1'}){cur.push_back(c);gen(cur,n);cur.pop_back();}}
int main(){std::string s;gen(s,3);}
