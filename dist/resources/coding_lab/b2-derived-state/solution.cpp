#include <iostream>

class Device{int id_;public:explicit Device(int id):id_(id){}int id()const{return id_;}};class Meter:public Device{int reading_;public:Meter(int id,int reading):Device(id),reading_(reading){}int reading()const{return reading_;}};
