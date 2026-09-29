#include "sqlite_support.hpp"


int main(){try{
    Database db;db.exec("CREATE TABLE users(id INTEGER PRIMARY KEY,name TEXT NOT NULL); INSERT INTO users VALUES(1,'Ada')");
    db.exec("ALTER TABLE users ADD COLUMN display_name TEXT");
    {Transaction tx(db);db.exec("UPDATE users SET display_name=name WHERE display_name IS NULL");tx.commit();}
    Statement old(db,"SELECT name FROM users WHERE id=1"),newer(db,"SELECT COALESCE(display_name,name) FROM users WHERE id=1");
    if(!old.row()||!newer.row())return 1;
    std::string a=reinterpret_cast<const char*>(sqlite3_column_text(old.p,0));
    std::string b=reinterpret_cast<const char*>(sqlite3_column_text(newer.p,0));
    if(a!="Ada"||a!=b)return 2;std::cout<<"old=Ada new=Ada old-column-retained\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
