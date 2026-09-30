#include <iostream>

int nextTicket(){ int count=0; return ++count; // BUG: local count starts over
}
