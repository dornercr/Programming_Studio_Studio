#include <iostream>
#include <string>

enum class State{queued,running,done};
struct Reading{int id;std::string name;State state;};
std::string label(const Reading& reading){return std::to_string(reading.id)+":"+reading.name+":queued";}
