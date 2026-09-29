#include "harbor/check.hpp"
#include "harbor/store.hpp"
#include <filesystem>
#include <iostream>
#include <thread>
#include <atomic>
#include <cstdlib>
int main() {
    try {
        char pattern[]="/tmp/harbor-store-XXXXXX";
        auto* name=::mkdtemp(pattern); harbor::check(name!=nullptr,"temp directory");
        std::filesystem::path dir(name);
        struct Cleanup { std::filesystem::path path; ~Cleanup(){std::error_code e;std::filesystem::remove_all(path,e);} } cleanup{dir};
        auto path=(dir/"test.db").string();
        {
            harbor::Store s(path);
            harbor::Request first{"a",7};
            harbor::check(s.submit(first,1)==harbor::Store::Submit::accepted,"first");
            harbor::check(s.submit({"b",8},1)==harbor::Store::Submit::full,"admission limit");
            harbor::check(s.submit(first,1)==harbor::Store::Submit::duplicate,"duplicate when full");
            harbor::check(s.submit({"a",8})==harbor::Store::Submit::conflict,"identity conflict");
            auto result=s.apply(first);
            harbor::check(result==49 && s.apply(first)==49 && s.receipt_count()==1,"durable inbox");
            harbor::rejects([&]{s.apply({"a",8});},"worker conflict");
            harbor::rejects([&]{s.complete(first,50);},"wrong result");
            harbor::check(!s.lookup("a")->result,"failed completion unchanged");
            s.complete(first,49); s.complete(first,49);
            harbor::check(s.pending().empty(),"outbox drained");
        }
        {
            harbor::Store reopened(path);
            harbor::check(reopened.lookup("a")->result==49 && reopened.receipt_count()==1,"restart durability");
        }
        {
            harbor::Store a(path),b(path); std::atomic<int> ok{0};
            auto operation=[&](harbor::Store& store){
                try { if(store.apply({"parallel",21})==441) ++ok; } catch(...) {}
            };
            std::jthread one(operation,std::ref(a)),two(operation,std::ref(b));
            one.join(); two.join(); harbor::check(ok==2 && a.receipt_count()==2,"concurrent deduplication");
        }
        std::cout<<"SQLite transaction, conflict, quota, restart, and concurrent dedup tests passed.\n";
    } catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
