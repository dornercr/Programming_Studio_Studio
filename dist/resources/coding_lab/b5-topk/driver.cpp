int main(){std::size_t k;if(!(std::cin>>k))return 2;std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);for(int x:topK(v,k))std::cout<<x<<" ";std::cout<<"\n";}
