#include <iostream>
#include <string>
#include <vector>
struct Expense{ std::string name; double amount; };
int main(){
    std::vector<Expense> expenses{{"food",12.50},{"fuel",30.00}};
    double total=0;
    for(const auto& e:expenses){ total += e.amount; std::cout << e.name << ": " << e.amount << '\n'; }
    std::cout << "total=" << total << '\n';
}
