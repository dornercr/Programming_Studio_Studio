#include <iostream>
#include <string>

std::string shippingStatus(bool paid,bool addressValid,int stock){ if(!paid)return "pay"; if(!addressValid)return "address"; if(stock<=0)return "stock"; return "ready"; }
