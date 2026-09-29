#include "sqlite_support.hpp"


int main(){try{
    Database db;db.exec("CREATE TABLE receipts(id TEXT PRIMARY KEY,input INTEGER NOT NULL,result INTEGER NOT NULL)");
    auto execute=[&](const std::string&id,int x){Transaction tx(db);Statement existing(db,"SELECT input,result FROM receipts WHERE id=?1");existing.text(1,id);
        if(existing.row()){if(sqlite3_column_int(existing.p,0)!=x)throw std::runtime_error("conflict");int result=sqlite3_column_int(existing.p,1);tx.commit();return result;}
        Statement put(db,"INSERT INTO receipts VALUES(?1,?2,?3)");put.text(1,id);put.integer(2,x);put.integer(3,x*x);put.row();tx.commit();return x*x;};
    int first=execute("a",12),again=execute("a",12);bool conflict=false;
    try{(void)execute("a",13);}catch(const std::runtime_error&){conflict=true;}
    std::cout<<first<<' '<<again<<" conflict="<<std::boolalpha<<conflict<<" receipts="<<scalar(db,"SELECT count(*) FROM receipts")<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
