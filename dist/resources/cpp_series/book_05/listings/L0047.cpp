#include <chrono>
#include <cassert>

struct Clock{virtual ~Clock()=default;virtual long long now_ms()const=0;};
struct FakeClock:Clock{long long value{};long long now_ms()const override{return value;}};

bool expired(const Clock& c,long long deadline){return c.now_ms()>=deadline;}
int main(){FakeClock c;c.value=999;assert(!expired(c,1000));c.value=1000;assert(expired(c,1000));}
