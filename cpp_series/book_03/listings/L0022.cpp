#include <iostream>
#include <stack>
#include <string>
bool balanced(const std::string&s){std::stack<char>st;for(char c:s){if(c=='('||c=='['||c=='{')st.push(c);else if(c==')'||c==']'||c=='}'){if(st.empty())return false;char o=st.top();st.pop();if((o=='('&&c!=')')||(o=='['&&c!=']')||(o=='{'&&c!='}'))return false;}}return st.empty();}
int main(){std::cout<<std::boolalpha<<balanced("{[()]}")<<' '<<balanced("{[(])}")<<'\n';}
