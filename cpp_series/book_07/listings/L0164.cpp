#include <optional>
#include <iostream>

int main() {
    unsigned generation=5;std::optional<int> cached;
    unsigned fetch_generation=generation;int fetched_value=10;
    ++generation;cached.reset(); // invalidation arrives while fetch is in flight
    bool accepted=fetch_generation==generation;
    if(accepted)cached=fetched_value;
    std::cout<<"old-fill-accepted="<<std::boolalpha<<accepted<<" cache-empty="<<!cached<<" generation="<<generation<<'\n';
}
