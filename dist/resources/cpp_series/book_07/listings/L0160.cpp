#include "harbor/check.hpp"
#include <iostream>
#include <sqlite3.h>
int main() {
    sqlite3* raw=nullptr;
    harbor::check(sqlite3_open(":memory:",&raw)==SQLITE_OK,"open");
    struct Owner { sqlite3* db; ~Owner(){sqlite3_close(db);} } owner{raw};
    auto sql=[&](const char* text) { return sqlite3_exec(raw,text,nullptr,nullptr,nullptr); };
    harbor::check(sql("CREATE TABLE jobs(id TEXT PRIMARY KEY); PRAGMA user_version=1;")==SQLITE_OK,"version one");
    harbor::check(sql("BEGIN; ALTER TABLE jobs ADD COLUMN priority INTEGER NOT NULL DEFAULT 0; PRAGMA user_version=2; COMMIT;")==SQLITE_OK,"expand");
    harbor::check(sql("INSERT INTO jobs(id) VALUES('old-client');")==SQLITE_OK,"old writer remains valid");
    std::cout<<"Additive migration preserves this old INSERT contract.\n";
}
