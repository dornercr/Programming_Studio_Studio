int main(){ std::vector<int> v; int n; while(std::cin>>n)v.push_back(n); for(int x:descending(v))std::cout<<x<<" "; std::cout<<"\n"; }
