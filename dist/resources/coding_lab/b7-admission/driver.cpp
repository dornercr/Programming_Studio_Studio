int main(){std::size_t cap;if(!(std::cin>>cap))return 2;Admission a(cap);int x;while(std::cin>>x)std::cout<<std::boolalpha<<a.submit(x)<<" ";std::cout<<"count="<<a.size()<<"\n";}
