#include "sqlite_support.hpp"


int main(){try{
    {Database db("reopen.db");db.exec("PRAGMA journal_mode=WAL; PRAGMA synchronous=FULL; CREATE TABLE state(value INTEGER)");
        {Transaction tx(db);db.exec("INSERT INTO state VALUES(11)");tx.commit();}
        {Transaction tx(db);db.exec("UPDATE state SET value=99");}
    }
    Database reopened("reopen.db");auto value=scalar(reopened,"SELECT value FROM state");
    if(value!=11)return 1;std::cout<<"reopened committed value="<<value<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
