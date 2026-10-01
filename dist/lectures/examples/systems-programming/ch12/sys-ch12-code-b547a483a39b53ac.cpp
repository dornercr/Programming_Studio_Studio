#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>

enum class Op { Load, Add, Halt };
struct Instruction { Op op; int operand; };
int execute(const std::vector<Instruction>& program) {
    int accumulator = 0;
    for(const auto& instruction : program) {
        switch(instruction.op) {
            case Op::Load: accumulator = instruction.operand; break;
            case Op::Add: accumulator += instruction.operand; break;
            case Op::Halt: return accumulator;
        }
    }
    throw std::invalid_argument("missing halt");
}

int main() {
    const int result = execute({{Op::Load,4},{Op::Add,3},{Op::Halt,0}});
    assert(result == 7 && execute({{Op::Halt,0}}) == 0);
    bool rejected = false;
    try { execute({{Op::Load,4}}); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "acc=" << result << " missing-halt=rejected\n";
}
