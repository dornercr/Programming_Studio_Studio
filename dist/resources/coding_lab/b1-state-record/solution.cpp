#include <iostream>
#include <string>

enum class State{queued,running,done};
struct Reading{int id;std::string name;State state;};
std::string label(const Reading& reading){std::string suffix;switch(reading.state){case State::queued:suffix="queued";break;case State::running:suffix="running";break;case State::done:suffix="done";break;}return std::to_string(reading.id)+":"+reading.name+":"+suffix;}
