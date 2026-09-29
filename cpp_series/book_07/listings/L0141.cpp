#include "sqlite_support.hpp"


int main(){try{
    Database first("busy.db");first.exec("CREATE TABLE IF NOT EXISTS jobs(id TEXT PRIMARY KEY,value INTEGER)");
    {Statement insert(first,"INSERT OR REPLACE INTO jobs VALUES(?1,?2)");insert.text(1,"a'quoted");insert.integer(2,7);if(insert.row())return 1;}
    Database second("busy.db");Transaction locked(first);
    int result=sqlite3_exec(second.p,"BEGIN IMMEDIATE",nullptr,nullptr,nullptr);
    if(result!=SQLITE_BUSY)return 2;
    std::cout<<"bound rows="<<scalar(first,"SELECT count(*) FROM jobs")<<" second-writer=BUSY\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
