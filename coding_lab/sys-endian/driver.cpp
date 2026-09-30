int main(){std::array<unsigned char,4> b{};for(auto& x:b){unsigned n;if(!(std::cin>>n)||n>255)return 2;x=static_cast<unsigned char>(n);}std::cout<<readLE32(b)<<"\n";}
