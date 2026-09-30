int main(){int id,code;std::string name;if(!(std::cin>>id>>name>>code))return 2;if(code<0||code>2)return 3;std::cout<<label({id,name,static_cast<State>(code)})<<"\n";}
