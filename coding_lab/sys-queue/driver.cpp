int main(){std::size_t cap;if(!(std::cin>>cap))return 2;BoundedQueue q(cap);int x;while(std::cin>>x)q.push(x);while(auto value=q.pop())std::cout<<*value<<" ";std::cout<<"\n";}
