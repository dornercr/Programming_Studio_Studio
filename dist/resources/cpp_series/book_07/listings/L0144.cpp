#pragma once
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <iostream>
struct Database {
    sqlite3* p=nullptr;
    explicit Database(const char* path=":memory:") {
        if(sqlite3_open(path,&p)!=SQLITE_OK){std::string e=p?sqlite3_errmsg(p):"open";if(p)sqlite3_close(p);throw std::runtime_error(e);}
    }
    ~Database(){sqlite3_close(p);}
    Database(const Database&)=delete;Database&operator=(const Database&)=delete;
    void exec(const char* sql){char* e=nullptr;int r=sqlite3_exec(p,sql,nullptr,nullptr,&e);
        if(r!=SQLITE_OK){std::string s=e?e:"SQL error";sqlite3_free(e);throw std::runtime_error(s);}}
};
struct Statement {
    sqlite3_stmt* p=nullptr;
    Statement(Database& d,const char* sql){if(sqlite3_prepare_v2(d.p,sql,-1,&p,nullptr)!=SQLITE_OK)throw std::runtime_error(sqlite3_errmsg(d.p));}
    ~Statement(){sqlite3_finalize(p);}
    Statement(const Statement&)=delete;Statement&operator=(const Statement&)=delete;
    void text(int n,const std::string& s){if(sqlite3_bind_text(p,n,s.data(),int(s.size()),SQLITE_TRANSIENT)!=SQLITE_OK)throw std::runtime_error("bind");}
    void integer(int n,sqlite3_int64 v){if(sqlite3_bind_int64(p,n,v)!=SQLITE_OK)throw std::runtime_error("bind");}
    bool row(){int r=sqlite3_step(p);if(r==SQLITE_ROW)return true;if(r==SQLITE_DONE)return false;throw std::runtime_error(sqlite3_errstr(r));}
};
inline sqlite3_int64 scalar(Database& d,const char* sql){Statement s(d,sql);if(!s.row())throw std::runtime_error("missing scalar");return sqlite3_column_int64(s.p,0);}
struct Transaction {
    Database& d;bool committed=false;
    explicit Transaction(Database&db):d(db){d.exec("BEGIN IMMEDIATE");}
    void commit(){d.exec("COMMIT");committed=true;}
    ~Transaction(){if(!committed)(void)sqlite3_exec(d.p,"ROLLBACK",nullptr,nullptr,nullptr);}
};
