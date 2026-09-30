#include <iostream>
#include <utility>

class Ticket{int id_;public:explicit Ticket(int n):id_(n){}Ticket(const Ticket&)=delete;Ticket& operator=(const Ticket&)=delete;Ticket(Ticket&& other)noexcept:id_(other.id_){}Ticket& operator=(Ticket&& other)noexcept{id_=other.id_;return *this;}int id()const{return id_;}};
