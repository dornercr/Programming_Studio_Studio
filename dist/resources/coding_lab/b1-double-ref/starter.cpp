#include <iostream>

void doubleValue(int& value){ int copy=value*2; (void)copy; // BUG: caller still has the old value
}
