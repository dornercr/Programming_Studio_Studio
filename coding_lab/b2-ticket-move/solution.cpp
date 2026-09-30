#include <iostream>
#include <utility>

class Ticket{int id_;public:explicit Ticket(int n):id_(n){}Ticket(const Ticket&)=delete;Ticket& operator=(const Ticket&)=delete;Ticket(Ticket&& other)noexcept:id_(std::exchange(other.id_,0)){}Ticket& operator=(Ticket&& other)noexcept{if(this!=&other)id_=std::exchange(other.id_,0);return *this;}int id()const{return id_;}};
