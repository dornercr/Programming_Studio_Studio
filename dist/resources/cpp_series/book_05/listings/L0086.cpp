#include <iostream>
#include <vector>

struct Particle{float x,y,z,mass;};
int main(){
 std::vector<Particle> aos(1000); for(auto& p:aos)p.x=1.0f;
 std::vector<float> x(1000,1.0f), y(1000), z(1000), mass(1000);
 float sa=0,ss=0; for(auto& p:aos)sa+=p.x; for(float v:x)ss+=v;
 std::cout<<sa<<' '<<ss<<"\n";
}
