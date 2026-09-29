#include "sqlite_support.hpp"


int main(){try{
    Database gateway("gateway.db"),worker("worker.db");
    gateway.exec("CREATE TABLE jobs(id TEXT PRIMARY KEY,input INTEGER,result INTEGER);INSERT INTO jobs VALUES('lesson28',12,NULL)");
    worker.exec("CREATE TABLE receipts(id TEXT PRIMARY KEY,input INTEGER,result INTEGER);BEGIN IMMEDIATE;INSERT INTO receipts VALUES('lesson28',12,144);COMMIT");
    // The worker reply is lost here; gateway remains pending.
    if(scalar(gateway,"SELECT count(*) FROM jobs WHERE result IS NULL")!=1)return 1;
    auto result=scalar(worker,"SELECT result FROM receipts WHERE id='lesson28'");
    if(result!=144)return 2;
    {Transaction tx(gateway);Statement update(gateway,"UPDATE jobs SET result=?1 WHERE id='lesson28' AND input=12");update.integer(1,result);update.row();tx.commit();}
    std::cout<<"DONE lesson28 "<<scalar(gateway,"SELECT result FROM jobs")<<" worker-receipts="<<scalar(worker,"SELECT count(*) FROM receipts")<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
