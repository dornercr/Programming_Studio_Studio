#include "harbor/store.hpp"
#include <limits>
#include <stdexcept>
namespace harbor {
namespace {
class Statement {
    sqlite3_stmt* stmt_=nullptr;
public:
    Statement(sqlite3* db,const char* sql) {
        if (sqlite3_prepare_v2(db,sql,-1,&stmt_,nullptr)!=SQLITE_OK)
            throw std::runtime_error(sqlite3_errmsg(db));
    }
    ~Statement() { sqlite3_finalize(stmt_); }
    Statement(const Statement&)=delete;
    Statement& operator=(const Statement&)=delete;
    void text(int i,const std::string& s) {
        if (s.size()>static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
            sqlite3_bind_text(stmt_,i,s.data(),static_cast<int>(s.size()),SQLITE_TRANSIENT)!=SQLITE_OK)
            throw std::runtime_error("bind text");
    }
    void number(int i,std::int64_t n) {
        if (sqlite3_bind_int64(stmt_,i,n)!=SQLITE_OK) throw std::runtime_error("bind integer");
    }
    bool step() {
        int rc=sqlite3_step(stmt_);
        if (rc==SQLITE_ROW) return true;
        if (rc==SQLITE_DONE) return false;
        throw std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt_)));
    }
    std::int64_t number(int i) const { return sqlite3_column_int64(stmt_,i); }
    std::string text(int i) const {
        const auto* p=sqlite3_column_text(stmt_,i);
        if (!p) return {};
        return {reinterpret_cast<const char*>(p),static_cast<std::size_t>(sqlite3_column_bytes(stmt_,i))};
    }
    bool is_null(int i) const { return sqlite3_column_type(stmt_,i)==SQLITE_NULL; }
};
class Transaction {
    sqlite3* db_; bool committed_=false;
public:
    explicit Transaction(sqlite3* db): db_(db) {
        if (sqlite3_exec(db_,"BEGIN IMMEDIATE",nullptr,nullptr,nullptr)!=SQLITE_OK)
            throw std::runtime_error(sqlite3_errmsg(db_));
    }
    ~Transaction() { if (!committed_) sqlite3_exec(db_,"ROLLBACK",nullptr,nullptr,nullptr); }
    void commit() {
        if (sqlite3_exec(db_,"COMMIT",nullptr,nullptr,nullptr)!=SQLITE_OK)
            throw std::runtime_error(sqlite3_errmsg(db_));
        committed_=true;
    }
};
}
void Store::exec(const char* sql) {
    if (sqlite3_exec(db_,sql,nullptr,nullptr,nullptr)!=SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db_));
}
Store::Store(const std::string& path) {
    if (sqlite3_open_v2(path.c_str(),&db_,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_FULLMUTEX,nullptr)!=SQLITE_OK) {
        std::string message=db_ ? sqlite3_errmsg(db_) : "sqlite open";
        if (db_) sqlite3_close_v2(db_);
        db_=nullptr;
        throw std::runtime_error(message);
    }
    try {
        if (sqlite3_busy_timeout(db_,100)!=SQLITE_OK) throw std::runtime_error("busy timeout");
        exec("PRAGMA foreign_keys=ON; PRAGMA journal_mode=WAL; PRAGMA synchronous=FULL;");
        Transaction tx(db_);
        int version=0;
        { Statement s(db_,"PRAGMA user_version"); if (s.step()) version=static_cast<int>(s.number(0)); }
        if (version>1) throw std::runtime_error("database schema is newer than this binary");
        if (version==0) {
            exec("CREATE TABLE jobs("
                 "id TEXT PRIMARY KEY, input INTEGER NOT NULL CHECK(input BETWEEN 0 AND 1000000),"
                 "result INTEGER CHECK(result IS NULL OR result>=0));"
                 "CREATE TABLE receipts("
                 "id TEXT PRIMARY KEY, input INTEGER NOT NULL, result INTEGER NOT NULL);"
                 "PRAGMA user_version=1;");
        }
        tx.commit();
    } catch (...) { sqlite3_close_v2(db_); db_=nullptr; throw; }
}
Store::~Store() { if (db_) sqlite3_close_v2(db_); }
Store::Submit Store::submit(const Request& r,std::size_t max_pending) {
    (void)evaluate(r);
    Transaction tx(db_);
    auto old=lookup(r.key);
    if (old) {
        tx.commit(); return old->request.input==r.input ? Submit::duplicate : Submit::conflict;
    }
    { Statement count(db_,"SELECT count(*) FROM jobs WHERE result IS NULL");
      if (!count.step()) throw std::runtime_error("count missing");
      if (static_cast<std::uint64_t>(count.number(0))>=max_pending) { tx.commit(); return Submit::full; }
    }
    { Statement s(db_,"INSERT INTO jobs(id,input) VALUES(?,?)");
      s.text(1,r.key); s.number(2,r.input); s.step(); }
    tx.commit(); return Submit::accepted;
}
std::optional<Store::Job> Store::lookup(const std::string& key) {
    Statement s(db_,"SELECT input,result FROM jobs WHERE id=?"); s.text(1,key);
    if (!s.step()) return {};
    Job j{Request{key,s.number(0)}, {}};
    if (!s.is_null(1)) j.result=s.number(1);
    return j;
}
std::vector<Request> Store::pending(std::size_t limit) {
    if (limit>128) throw std::invalid_argument("pending batch limit");
    Statement s(db_,"SELECT id,input FROM jobs WHERE result IS NULL ORDER BY rowid LIMIT ?");
    s.number(1,static_cast<std::int64_t>(limit)); std::vector<Request> out;
    while (s.step()) out.push_back(Request{s.text(0),s.number(1)});
    return out;
}
std::int64_t Store::apply(const Request& r) {
    auto answer=evaluate(r);
    Transaction tx(db_);
    { Statement old(db_,"SELECT input,result FROM receipts WHERE id=?"); old.text(1,r.key);
      if (old.step()) {
          if (old.number(0)!=r.input) throw std::runtime_error("CONFLICT");
          answer=old.number(1); tx.commit(); return answer;
      }
    }
    { Statement s(db_,"INSERT INTO receipts(id,input,result) VALUES(?,?,?)");
      s.text(1,r.key); s.number(2,r.input); s.number(3,answer); s.step(); }
    tx.commit(); return answer;
}
void Store::complete(const Request& r,std::int64_t result) {
    if (result!=evaluate(r)) throw std::runtime_error("incorrect worker result");
    Transaction tx(db_); auto job=lookup(r.key);
    if (!job || job->request.input!=r.input) throw std::runtime_error("completion identity mismatch");
    if (job->result && *job->result!=result) throw std::runtime_error("completion conflict");
    { Statement s(db_,"UPDATE jobs SET result=? WHERE id=? AND input=?");
      s.number(1,result); s.text(2,r.key); s.number(3,r.input); s.step(); }
    tx.commit();
}
std::size_t Store::receipt_count() {
    Statement s(db_,"SELECT count(*) FROM receipts");
    if (!s.step()) throw std::runtime_error("count missing");
    return static_cast<std::size_t>(s.number(0));
}
}
