#include <iostream>
int main(){

int total{};
{
    int total{7}; // A distinct inner object.
    std::cout << total << '\n';
}
std::cout << total << '\n'; // The outer object remains zero.

}
