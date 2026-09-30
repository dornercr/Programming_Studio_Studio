int main(){Stack<int> s(2);int x;while(std::cin>>x)s.push(x);while(auto v=s.pop())std::cout<<*v<<" ";std::cout<<"\n";}
