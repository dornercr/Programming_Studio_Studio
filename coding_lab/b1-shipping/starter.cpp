#include <iostream>
#include <string>

std::string shippingStatus(bool paid,bool addressValid,int stock){ if(paid&&addressValid&&stock>0)return "ready"; return "stock"; }
