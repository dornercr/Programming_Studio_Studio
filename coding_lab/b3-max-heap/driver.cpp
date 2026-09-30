int main(){MaxHeap q;int x;while(std::cin>>x)q.push(x);while(auto v=q.pop())std::cout<<*v<<" ";std::cout<<"\n";}
