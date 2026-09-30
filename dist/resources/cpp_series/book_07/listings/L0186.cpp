#include "sqlite_support.hpp"


int main(){try{
    Database db;db.exec("CREATE TABLE balance(value INTEGER);INSERT INTO balance VALUES(0);CREATE TABLE outbox(id TEXT PRIMARY KEY,delta INTEGER)");
    auto accept=[&](const char*id,int delta,bool fail){Transaction tx(db);
        Statement update(db,"UPDATE balance SET value=value+?1");update.integer(1,delta);update.row();
        Statement insert(db,"INSERT INTO outbox VALUES(?1,?2)");insert.text(1,id);insert.integer(2,delta);insert.row();
        if(fail)throw std::runtime_error("injected before commit");tx.commit();};
    try{accept("lost",5,true);}catch(const std::runtime_error&){}
    accept("kept",7,false);
    std::cout<<"balance="<<scalar(db,"SELECT value FROM balance")<<" pending="<<scalar(db,"SELECT count(*) FROM outbox")<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
