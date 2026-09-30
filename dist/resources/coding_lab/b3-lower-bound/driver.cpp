int main(){int target;if(!(std::cin>>target))return 2;std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);std::cout<<lowerBound(v,target)<<"\n";}
