#include "sqlite_support.hpp"


int main(){try{
    Database db;db.exec("CREATE TABLE schema_migrations(version INTEGER PRIMARY KEY); CREATE TABLE jobs(id INTEGER PRIMARY KEY)");
    auto migrate=[&](){Transaction tx(db);if(scalar(db,"SELECT count(*) FROM schema_migrations WHERE version=2")){tx.commit();return false;}
        db.exec("ALTER TABLE jobs ADD COLUMN note TEXT; INSERT INTO schema_migrations VALUES(2)");tx.commit();return true;};
    bool first=migrate(),second=migrate();
    std::cout<<"first="<<std::boolalpha<<first<<" repeated="<<second<<" versions="<<scalar(db,"SELECT count(*) FROM schema_migrations")<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
