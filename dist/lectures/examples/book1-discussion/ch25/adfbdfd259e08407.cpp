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
bool releaseParcel(Parcel& parcel) {
    if (parcel.state != ParcelState::pending || parcel.weight < 1 || parcel.weight > 100) return false;
    parcel.state = ParcelState::ready; // Commit the permitted transition.
    return true;
}

int main() {
    Parcel p{0,ParcelState::pending};
    bool ok=releaseParcel(p);
    std::cout<<std::boolalpha<<ok<<" "<<(p.state==ParcelState::pending)<<"\n";
    }
