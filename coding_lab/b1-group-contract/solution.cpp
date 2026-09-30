#include <iostream>
#include <limits>

int groups_needed(int items,int capacity);
int groups_needed(int items,int capacity){if(items<0||capacity<=0)return -1;return items/capacity+(items%capacity!=0);}
