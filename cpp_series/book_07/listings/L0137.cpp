#include "sqlite_support.hpp"


int main(){try{
    Database db;
    db.exec("CREATE TABLE jobs(id TEXT PRIMARY KEY,input INTEGER NOT NULL CHECK(input BETWEEN 0 AND 1000000),result INTEGER)");
    {Transaction tx(db);db.exec("INSERT INTO jobs VALUES('good',12,144)");tx.commit();}
    try{Transaction tx(db);db.exec("INSERT INTO jobs VALUES('temporary',3,9)");db.exec("INSERT INTO jobs VALUES('bad',-1,NULL)");tx.commit();return 1;}
    catch(const std::runtime_error&){}
    auto count=scalar(db,"SELECT count(*) FROM jobs");
    if(count!=1)return 2;std::cout<<"committed rows="<<count<<" result="<<scalar(db,"SELECT result FROM jobs WHERE id='good'")<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
