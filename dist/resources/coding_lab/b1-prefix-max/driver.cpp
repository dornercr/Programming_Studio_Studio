int main(){std::vector<int> v;int n;while(std::cin>>n)v.push_back(n);for(int x:prefixMax(v))std::cout<<x<<" ";std::cout<<"\n";}
