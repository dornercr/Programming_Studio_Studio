#include <iostream>
int main(){

int devices = 4;   // Initialization of a new object.
devices = 5;       // Assignment to an existing object.
int racks(2);      // Direct initialization.
int spares{3};     // List initialization.
std::cout<<devices<<" "<<racks<<" "<<spares<<"\n";
}
