#include <iostream>

int nextTicket(){ static int count=0; return ++count; }
