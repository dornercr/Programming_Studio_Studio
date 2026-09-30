int main(){std::string file;std::getline(std::cin,file);auto p=reportPath("reports",file);std::cout<<(p?p->generic_string():"invalid")<<"\n";}
