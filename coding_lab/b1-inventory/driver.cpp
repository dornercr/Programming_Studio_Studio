int main(){Inventory store;int id,quantity;std::string name;while(std::cin>>id>>name>>quantity)store.add({id,name,quantity});std::cout<<store.records().size()<<" "<<store.total()<<"\n";}
