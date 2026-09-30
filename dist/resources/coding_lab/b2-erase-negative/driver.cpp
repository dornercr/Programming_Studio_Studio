int main(){std::vector<int> v;for(int x;std::cin>>x;)v.push_back(x);eraseNegative(v);for(int x:v)std::cout<<x<<" ";std::cout<<"\n";}
