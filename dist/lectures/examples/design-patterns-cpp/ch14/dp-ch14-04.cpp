#include <iostream>
int main() {
    int position = 0;
    // Wrong: both history records are captured before playback.
    const int before_first = position;
    const int before_second = position;
    position = 3;
    position = 7;
    position = before_second;
    std::cout << "wrong second undo: " << position << '\n';
    std::cout << "required second undo: 3\n";
    position = before_first;
    std::cout << "after both wrong undos: " << position << '\n';
}
