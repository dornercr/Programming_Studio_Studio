#include "harbor/check.hpp"
#include "harbor/domain.hpp"
#include "harbor/wire.hpp"
#include "harbor/policies.hpp"
#include "harbor/models.hpp"
#include "harbor/queue.hpp"
#include <atomic>
#include <iostream>
#include <numeric>
#include <thread>
using harbor::check;
using harbor::rejects;
void codec() {
    for (std::size_t size: {0U,1U,2U,3U,17U,63U,1024U,4096U}) {
        std::string value(size,'x'); if (size) value[size/2]='\0';
        auto bytes=harbor::frame(value);
        for (std::size_t split=0;split<=bytes.size();++split) {
            harbor::Decoder decoder;
            decoder.feed(std::string_view(bytes).substr(0,split));
            decoder.feed(std::string_view(bytes).substr(split));
            decoder.finish(); check(decoder.payload()==value,"roundtrip split");
        }
    }
    rejects([]{harbor::frame(std::string(4097,'x'));},"oversize encode");
    rejects([]{harbor::Decoder d; d.feed(std::string("\0\0\x10\x01",4));},"oversize declared length");
    rejects([]{harbor::Decoder d; d.feed(std::string("\0\0\0\3ab",6));d.finish();},"truncation");
    rejects([]{harbor::Decoder d; d.feed(harbor::frame("a")+"b");},"trailing bytes");
}
void policy() {
    harbor::RetryPolicy p;
    check(p.ceiling(0)==10 && p.ceiling(7)==1000 && p.ceiling(1000000)==1000,"bounded exponential");
    std::mt19937 rng(19);
    for (unsigned i=0;i<1000;++i) check(p.delay(i%12,rng)<=p.ceiling(i%12),"jitter bound");
    harbor::Budget b{100}; check(b.remaining(20)==80 && b.remaining(100)==0,"budget");
    check(harbor::saturating_add(UINT64_MAX-1,9)==UINT64_MAX,"saturation");
    harbor::TokenBucket bucket(2,100);
    check(bucket.admit(2,0) && !bucket.admit(1,0) && bucket.admit(1,10),"token refill");
    rejects([&]{bucket.admit(1,9);},"backward clock");
    rejects([]{harbor::TokenBucket b(0,1);},"invalid capacity");
}
void routing() {
    const std::vector<std::string> before{"one","two","three"},rev{"three","two","one"};
    auto after=before; after.push_back("four");
    for (int i=0;i<1000;++i) {
        auto key=std::to_string(i),old=harbor::placement(key,before),next=harbor::placement(key,after);
        check(old==harbor::placement(key,rev),"membership order");
        check(old==next || next=="four","minimal reassignment");
    }
    rejects([]{harbor::placement("key",{});},"no members");
    harbor::Discovery d; check(d.update(5,{"a","b","a"}),"revision");
    check(!d.update(4,{}) && d.endpoints.size()==2,"old snapshot");
}
void cache() {
    harbor::VersionedCache c;
    check(c.put("a","old",1,100),"first fill");
    c.invalidate("a",2);
    check(!c.put("a","old",1,200) && !c.get("a",1),"delayed stale fill");
    check(c.put("a","new",2,200),"new fill");
    c.invalidate("a",1); check(c.get("a",199)=="new","old invalidation");
    check(!c.get("a",200),"expiry equality");
}
void clock_test() {
    harbor::LamportClock a,b;
    for (int i=0;i<100;++i) { auto t=a.tick(); check(b.receive(t)>t,"receive advances"); a.receive(b.tick()); }
    rejects([]{harbor::LamportClock c; c.receive(UINT64_MAX);},"clock wrap");
    harbor::FencedRegister r; r.write(2,"new"); check(!r.write(1,"old") && r.value()=="new","fence");
}
void breaker() {
    harbor::Breaker b(2,10);
    auto old=*b.acquire(0);
    b.complete(old,false,0); b.complete(old,false,1);
    check(!b.acquire(10),"cooldown");
    auto probe=*b.acquire(11); check(!b.acquire(11),"one probe");
    b.complete(old,true,11); check(b.state()==harbor::Breaker::State::half_open,"stale success ignored");
    b.complete(probe,false,12); check(!b.acquire(21),"failed probe");
    auto probe2=*b.acquire(22); b.complete(probe2,true,23);
    check(b.state()==harbor::Breaker::State::closed,"successful recovery");
    b.complete(probe,false,24); check(b.state()==harbor::Breaker::State::closed,"late failure ignored");
}
void saga() {
    harbor::Saga s;
    s.event("r","reserve"); s.event("c","charge"); s.event("x","cancel"); s.event("u","compensated");
    check(!s.event("u","compensated"),"duplicate ack");
    rejects([&]{s.event("u","finish");},"same identity different payload");
    rejects([&]{s.event("f","finish");},"forward after compensation");
    check(s.state()==harbor::Saga::State::cancelled,"terminal preserved");
}
void queue() {
    harbor::BoundedQueue<int> q(8);
    std::atomic<int> count{0},sum{0};
    std::vector<std::jthread> consumers;
    for(int i=0;i<3;++i) consumers.emplace_back([&]{while(auto n=q.pop()){++count;sum+=*n;}});
    std::vector<std::jthread> producers;
    for (int p=0;p<4;++p) producers.emplace_back([&,p]{
        for(int i=0;i<100;++i) { int n=p*100+i; while(!q.try_push(n)) std::this_thread::yield(); }
    });
    producers.clear(); q.close(); consumers.clear();
    check(count==400 && sum==(399*400/2),"all admitted values accounted");
    check(!q.try_push(4) && !q.pop(),"closed exhausted");
}
void domain() {
    for (const auto& s: {"-1","1000001","+1","1x","","999999999999999999999"})
        check(std::holds_alternative<harbor::Error>(harbor::parse_request("k",s)),"input grammar");
    check(!harbor::valid_key("") && !harbor::valid_key("space key") && !harbor::valid_key(std::string(65,'a')),"key bounds");
    check(harbor::evaluate({"max",1000000})==1000000000000LL,"numeric max");
    rejects([]{harbor::evaluate({"max",1000001});},"domain guard");
}
int main(int argc,char** argv) {
    try {
        if(argc!=2) throw std::runtime_error("case required");
        const std::string name=argv[1];
        if(name=="codec") codec(); else if(name=="policy") policy(); else if(name=="routing") routing();
        else if(name=="cache") cache(); else if(name=="clock") clock_test(); else if(name=="breaker") breaker();
        else if(name=="saga") saga(); else if(name=="queue") queue(); else if(name=="domain") domain();
        else throw std::runtime_error("unknown test");
        std::cout<<name<<": passed\n";
    } catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
