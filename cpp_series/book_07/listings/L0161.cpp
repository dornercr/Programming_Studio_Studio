#include <optional>
#include <iostream>

int main() {
    struct Entry{int value;int expires_ms;};std::optional<Entry> cache=Entry{10,100};int authoritative=20;
    auto read=[&](int now){if(cache&&now<cache->expires_ms)return cache->value;cache=Entry{authoritative,now+50};return authoritative;};
    std::cout<<"before="<<read(99)<<" at-expiry="<<read(100)<<" refreshed-expiry="<<cache->expires_ms<<'\n';
}
