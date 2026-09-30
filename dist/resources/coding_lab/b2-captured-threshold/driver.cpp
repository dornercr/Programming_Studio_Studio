int main(){int threshold;if(!(std::cin>>threshold))return 2;std::vector<int> input;for(int x;std::cin>>x;)input.push_back(x);for(int v:above(input,threshold))std::cout<<v<<" ";std::cout<<"\n";}
