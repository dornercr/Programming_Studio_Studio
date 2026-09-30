#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

enum class ParcelState { pending, ready };
struct Parcel { int weight; ParcelState state; };
bool releaseParcel(Parcel& parcel){
 // BUG: changes the state before validating the weight.
 parcel.state=ParcelState::ready;return parcel.weight>=1&&parcel.weight<=100;
}

int main() {
    Parcel p{0,ParcelState::pending};
    bool ok=releaseParcel(p);
    std::cout<<std::boolalpha<<ok<<" "<<(p.state==ParcelState::pending)<<"\n";
    }
