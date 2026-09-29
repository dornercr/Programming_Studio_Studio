#include "harbor/check.hpp"
#include "harbor/models.hpp"
#include <iostream>
int main() {
    harbor::VersionedCache cache;
    cache.put("job","pending",4,100);
    cache.invalidate("job",5);
    harbor::check(!cache.put("job","pending",4,200),"late stale fill");
    harbor::check(cache.put("job","done",5,200),"current fill");
    harbor::check(cache.get("job",199)=="done" && !cache.get("job",200),"expiry boundary");
    std::cout<<"An invalidation floor stopped an old in-flight read from resurrecting data.\n";
}
