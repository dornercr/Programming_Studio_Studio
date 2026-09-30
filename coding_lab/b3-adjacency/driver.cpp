int main(){auto g=adjacency(3,{{0,1},{0,2},{2,1}});for(std::size_t i=0;i<g.size();++i){std::cout<<i<<":";for(auto j:g[i])std::cout<<j<<" ";std::cout<<"\n";}}
