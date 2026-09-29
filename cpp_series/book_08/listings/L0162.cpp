#include <iostream>

int main() {
    unsigned app_attempts=3,mesh_attempts_per_call=3;
    std::cout<<"uncoordinated maximum="<<app_attempts*mesh_attempts_per_call<<'\n';
    app_attempts=1;std::cout<<"single retry owner maximum="<<app_attempts*mesh_attempts_per_call<<'\n';
}
