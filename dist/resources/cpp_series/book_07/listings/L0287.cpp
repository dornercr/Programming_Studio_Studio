#include "sqlite_support.hpp"


int main(){try{
    Database source;source.exec("CREATE TABLE jobs(id INTEGER PRIMARY KEY,result INTEGER);INSERT INTO jobs VALUES(1,144),(2,169)");
    Database backup("backup.db");sqlite3_backup* handle=sqlite3_backup_init(backup.p,"main",source.p,"main");
    if(!handle)throw std::runtime_error("backup init");int step=sqlite3_backup_step(handle,-1);int finish=sqlite3_backup_finish(handle);
    if(step!=SQLITE_DONE||finish!=SQLITE_OK)throw std::runtime_error("backup copy");
    Database restored("backup.db");auto count=scalar(restored,"SELECT count(*) FROM jobs"),sum=scalar(restored,"SELECT sum(result) FROM jobs");
    if(count!=2||sum!=313)return 1;std::cout<<"restored rows=2 result-sum=313\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
