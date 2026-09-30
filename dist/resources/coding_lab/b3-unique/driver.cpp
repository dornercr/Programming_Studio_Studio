int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);for(int x:stableUnique(v))std::cout<<x<<" ";std::cout<<"\n";}
