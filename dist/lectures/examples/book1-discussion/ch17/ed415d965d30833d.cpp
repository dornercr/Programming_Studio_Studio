#include <iostream>
class Account{
    double balance_{};
public:
    bool withdraw(double amount){ if(amount < 0 || amount > balance_) return false; balance_ -= amount; return true; }
    void deposit(double amount){ if(amount > 0) balance_ += amount; }
    double balance() const { return balance_; }
};
int main(){ Account a; a.deposit(50); std::cout << a.withdraw(70) << ' ' << a.balance() << '\n'; }
