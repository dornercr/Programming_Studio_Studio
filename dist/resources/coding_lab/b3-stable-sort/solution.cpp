#include <iostream>
#include <vector>
#include <cstddef>

struct Record{int key;char identity;bool operator==(const Record&)const=default;};
std::vector<Record> stableSort(std::vector<Record> v){
    for(std::size_t i=1;i<v.size();++i){
        auto item=v[i];auto j=i;
        while(j>0&&v[j-1].key>item.key){v[j]=v[j-1];--j;}
        v[j]=item;
    }
    return v;
}
