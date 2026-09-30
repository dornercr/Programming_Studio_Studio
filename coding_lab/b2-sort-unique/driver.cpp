int main(){std::vector<int> values;for(int v;std::cin>>v;)values.push_back(v);for(int v:sortedUnique(values))std::cout<<v<<" ";std::cout<<"\n";}
