int main(){int v,l,u;if(!(std::cin>>v>>l>>u))return 2;try{clamp(v,l,u);std::cout<<v<<"\n";}catch(const std::invalid_argument&){std::cout<<"bounds\n";}}
