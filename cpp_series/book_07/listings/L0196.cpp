#include <iostream>

int main() {
    enum class Mode{closed,open,half_open};Mode mode=Mode::closed;unsigned epoch=1;
    unsigned old_request_epoch=epoch;
    mode=Mode::open;++epoch;
    auto success=[&](unsigned request_epoch){if(request_epoch!=epoch)return false;mode=Mode::closed;return true;};
    bool stale=success(old_request_epoch);mode=Mode::half_open;bool probe=success(epoch);
    std::cout<<"stale-closed="<<std::boolalpha<<stale<<" probe-closed="<<probe<<" final-closed="<<(mode==Mode::closed)<<'\n';
}
